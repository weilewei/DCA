// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Giovanni Balduzzi (gbalduzz@itp.phys.ethz.ch)
//         Peter Staar (taa@zurich.ibm.com)
//         Raffaele Solca' (rasolca@itp.phys.ethz.ch)
//
// This file implements kernels_gpu.hpp

#include "dca/linalg/blas/kernels_gpu.hpp"
#include <cassert>
#include <cuComplex.h>
#include <cuda_runtime.h>
#include "dca/linalg/util/complex_operators_cuda.cu.hpp"
#include "dca/linalg/util/error_cuda.hpp"
#include "dca/linalg/util/stream_functions.hpp"
#include "dca/util/integer_division.hpp"

namespace dca {
namespace linalg {
namespace blas {
namespace kernels {
// dca::linalg::blas::kernels::

constexpr int copy_block_size_x = 32;
constexpr int copy_block_size_y = 8;
constexpr int move_block_size_x = 32;
constexpr int move_block_size_y = 8;
constexpr int scale_block_size_x = 32;
constexpr int scale_block_size_y = 8;
constexpr int swap_block_size_x = 32;
constexpr int swap_block_size_y = 32;

template <typename Type>
__global__ void copyRows(int row_size, int n_rows, const int* i_x, const Type* x, int ldx,
                         const int* i_y, Type* y, int ldy) {
  assert(blockDim.y == 1);
  assert(blockDim.z == 1);
  assert(blockIdx.z == 0);

  // Work on BlockDim.x rows and copyrows_block_size_y cols.
  int ind_i = threadIdx.x + blockIdx.x * blockDim.x;

  int js = blockIdx.y * copy_block_size_y;
  int je = min(row_size, (blockIdx.y + 1) * copy_block_size_y);

  if (ind_i < n_rows) {
    int iy = i_y[ind_i];
    int ix = i_x[ind_i];

    for (int j = js; j < je; ++j)
      y[iy + j * ldy] = x[ix + j * ldx];
  }
}

template <typename Type>
__global__ void copyCols(int col_size, int n_cols, const int* j_x, const Type* x, int ldx,
                         const int* j_y, Type* y, int ldy) {
  assert(blockDim.y == 1);
  assert(blockDim.z == 1);
  assert(blockIdx.z == 0);

  // Work on BlockDim.x rows and copyrows_block_size_y cols.
  int i = threadIdx.x + blockIdx.x * blockDim.x;

  int ind_js = blockIdx.y * copy_block_size_y;
  int ind_je = min(n_cols, (blockIdx.y + 1) * copy_block_size_y);

  if (i < col_size) {
    for (int ind_j = ind_js; ind_j < ind_je; ++ind_j)
      y[i + j_y[ind_j] * ldy] = x[i + j_x[ind_j] * ldx];
  }
}

template <typename Type>
__global__ void moveLeft(int m, int n, Type* a, int lda) {
  assert(blockDim.y == 1);
  assert(blockDim.z == 1);
  assert(blockIdx.y == 0);
  assert(blockIdx.z == 0);

  int i = threadIdx.x + blockIdx.x * blockDim.x;

  if (i < m) {
    for (int j = 0; j < n - 1; ++j)
      a[i + j * lda] = a[i + (j + 1) * lda];
  }
}

template <typename Type>
__global__ void moveUp(int m, int n, Type* a, int lda) {
  assert(blockDim.x == move_block_size_x);
  assert(blockDim.y == 1);
  assert(blockDim.z == 1);
  assert(blockIdx.x == 0);
  assert(blockIdx.z == 0);

  __shared__ Type work[move_block_size_x * move_block_size_y];
  int ldw = move_block_size_x;

  int idx = threadIdx.x;
  int js = blockIdx.y * move_block_size_y;
  int je = min(n, (blockIdx.y + 1) * move_block_size_y);

  int jd = je - js;
  a += lda * js;

  int m_div = (m - 1) / blockDim.x * blockDim.x;

  for (int i = 0; i < m_div; i += blockDim.x) {
    for (int j = 0; j < jd; ++j)
      work[idx + ldw * j] = a[i + 1 + idx + lda * j];
    __syncthreads();
    for (int j = 0; j < jd; ++j)
      a[i + idx + lda * j] = work[idx + ldw * j];
    __syncthreads();
  }
  int i = m_div;
  if (i + idx < m - 1) {
    for (int j = 0; j < jd; ++j)
      work[idx + ldw * j] = a[i + 1 + idx + lda * j];
    __syncthreads();
    for (int j = 0; j < jd; ++j)
      a[i + idx + lda * j] = work[idx + ldw * j];
  }
}

template <typename Type>
__global__ void scaleRows(int row_size, int n_rows, const int* i, const Type* alpha, Type* a,
                          int lda) {
  assert(blockDim.y == 1);
  assert(blockDim.z == 1);
  assert(blockIdx.z == 0);

  // Work on BlockDim.x rows and copyrows_block_size_y cols.
  int ind_i = threadIdx.x + blockIdx.x * blockDim.x;

  int js = blockIdx.y * scale_block_size_y;
  int je = min(row_size, (blockIdx.y + 1) * scale_block_size_y);

  if (ind_i < n_rows) {
    int ia = i[ind_i];

    for (int j = js; j < je; ++j)
      a[ia + j * lda] = a[ia + j * lda] * alpha[ind_i];
  }
}

template <typename Type>
__global__ void swapRows(int row_size, int n_rows, const int* i1, const int* i2, Type* a, int lda) {
  const int ind_i = threadIdx.x + blockIdx.x * blockDim.x;
  const int ind_j = threadIdx.y + blockIdx.y * blockDim.y;

  if (ind_i < n_rows && ind_j < row_size) {
    const Type tmp = a[i1[ind_i] + ind_j * lda];
    a[i1[ind_i] + ind_j * lda] = a[i2[ind_i] + ind_j * lda];
    a[i2[ind_i] + ind_j * lda] = tmp;
  }
}

template <typename Type>
__global__ void swapCols(int col_size, int n_cols, const int* j1, const int* j2, Type* a, int lda) {
  const int ind_i = threadIdx.x + blockIdx.x * blockDim.x;
  const int ind_j = threadIdx.y + blockIdx.y * blockDim.y;

  if (ind_i < col_size && ind_j < n_cols) {
    const Type tmp = a[ind_i + j1[ind_j] * lda];
    a[ind_i + j1[ind_j] * lda] = a[ind_i + j2[ind_j] * lda];
    a[ind_i + j2[ind_j] * lda] = tmp;
  }
}

}  // kernels
// dca::linalg::blas::

template <typename Type>
void copyRows(int row_size, int n_rows, const int* i_x, const Type* x, int ldx, const int* i_y,
              Type* y, int ldy, int thread_id, int stream_id) {
  if (row_size > 0 && n_rows > 0) {
    checkErrorsCudaDebug();
    int bl_x = dca::util::ceilDiv(n_rows, kernels::copy_block_size_x);
    int bl_y = dca::util::ceilDiv(row_size, kernels::copy_block_size_y);

    dim3 threads(kernels::copy_block_size_x);
    dim3 blocks(bl_x, bl_y);

    cudaStream_t stream = dca::linalg::util::getStream(thread_id, stream_id);

    kernels::copyRows<<<blocks, threads, 0, stream>>>(row_size, n_rows, i_x, x, ldx, i_y, y, ldy);
    checkErrorsCudaDebug();
  }
}
template void copyRows(int row_size, int n_rows, const int* i_x, const float* x, int ldx,
                       const int* i_y, float* y, int ldy, int thread_id, int stream_id);
template void copyRows(int row_size, int n_rows, const int* i_x, const double* x, int ldx,
                       const int* i_y, double* y, int ldy, int thread_id, int stream_id);
template void copyRows(int row_size, int n_rows, const int* i_x, const cuComplex* x, int ldx,
                       const int* i_y, cuComplex* y, int ldy, int thread_id, int stream_id);
template void copyRows(int row_size, int n_rows, const int* i_x, const cuDoubleComplex* x, int ldx,
                       const int* i_y, cuDoubleComplex* y, int ldy, int thread_id, int stream_id);

template <typename Type>
void copyCols(int col_size, int n_cols, const int* j_x, const Type* x, int ldx, const int* j_y,
              Type* y, int ldy, int thread_id, int stream_id) {
  if (col_size > 0 && n_cols > 0) {
    checkErrorsCudaDebug();
    int bl_x = dca::util::ceilDiv(col_size, kernels::copy_block_size_x);
    int bl_y = dca::util::ceilDiv(n_cols, kernels::copy_block_size_y);

    dim3 threads(kernels::copy_block_size_x);
    dim3 blocks(bl_x, bl_y);

    cudaStream_t stream = dca::linalg::util::getStream(thread_id, stream_id);

    kernels::copyCols<<<blocks, threads, 0, stream>>>(col_size, n_cols, j_x, x, ldx, j_y, y, ldy);
    checkErrorsCudaDebug();
  }
}
template void copyCols(int col_size, int n_cols, const int* j_x, const float* x, int ldx,
                       const int* j_y, float* y, int ldy, int thread_id, int stream_id);
template void copyCols(int col_size, int n_cols, const int* j_x, const double* x, int ldx,
                       const int* j_y, double* y, int ldy, int thread_id, int stream_id);
template void copyCols(int col_size, int n_cols, const int* j_x, const cuComplex* x, int ldx,
                       const int* j_y, cuComplex* y, int ldy, int thread_id, int stream_id);
template void copyCols(int col_size, int n_cols, const int* j_x, const cuDoubleComplex* x, int ldx,
                       const int* j_y, cuDoubleComplex* y, int ldy, int thread_id, int stream_id);

template <typename Type>
void moveLeft(int m, int n, Type* a, int lda) {
  assert(lda >= m);

  if (m > 0 && n > 1) {
    checkErrorsCudaDebug();
    int bl_x = dca::util::ceilDiv(m, kernels::move_block_size_x);

    dim3 threads(kernels::move_block_size_x);
    dim3 blocks(bl_x);

    kernels::moveLeft<<<blocks, threads>>>(m, n, a, lda);
    checkErrorsCudaDebug();
  }
}
template void moveLeft(int m, int n, float* a, int lda);
template void moveLeft(int m, int n, double* a, int lda);
template void moveLeft(int m, int n, cuComplex* a, int lda);
template void moveLeft(int m, int n, cuDoubleComplex* a, int lda);

template <typename Type>
void moveUp(int m, int n, Type* a, int lda) {
  assert(lda >= m);

  if (m > 1 && n > 0) {
    checkErrorsCudaDebug();
    int bl_y = dca::util::ceilDiv(n, kernels::move_block_size_y);

    dim3 threads(kernels::move_block_size_x);
    dim3 blocks(1, bl_y);

    kernels::moveUp<<<blocks, threads>>>(m, n, a, lda);
    checkErrorsCudaDebug();
  }
}
template void moveUp(int m, int n, float* a, int lda);
template void moveUp(int m, int n, double* a, int lda);
template void moveUp(int m, int n, cuComplex* a, int lda);
template void moveUp(int m, int n, cuDoubleComplex* a, int lda);

template <typename Type>
void scaleRows(int row_size, int n_rows, const int* i, const Type* alpha, Type* a, int lda,
               int thread_id, int stream_id) {
  if (row_size > 0 && n_rows > 0) {
    checkErrorsCudaDebug();
    int bl_x = dca::util::ceilDiv(n_rows, kernels::scale_block_size_x);
    int bl_y = dca::util::ceilDiv(row_size, kernels::scale_block_size_y);

    dim3 threads(kernels::scale_block_size_x);
    dim3 blocks(bl_x, bl_y);

    cudaStream_t stream = dca::linalg::util::getStream(thread_id, stream_id);

    kernels::scaleRows<<<blocks, threads, 0, stream>>>(row_size, n_rows, i, alpha, a, lda);
    checkErrorsCudaDebug();
  }
}
template void scaleRows(int row_size, int n_rows, const int* i, const float* alpha, float* a,
                        int lda, int thread_id, int stream_id);
template void scaleRows(int row_size, int n_rows, const int* i, const double* alpha, double* a,
                        int lda, int thread_id, int stream_id);
template void scaleRows(int row_size, int n_rows, const int* i, const cuComplex* alpha,
                        cuComplex* a, int lda, int thread_id, int stream_id);
template void scaleRows(int row_size, int n_rows, const int* i, const cuDoubleComplex* alpha,
                        cuDoubleComplex* a, int lda, int thread_id, int stream_id);

template <typename Type>
void swapRows(int row_size, int n_rows, const int* i1, const int* i2, Type* a, int lda,
              int thread_id, int stream_id) {
  if (row_size > 0 && n_rows > 0) {
    checkErrorsCudaDebug();
    const int bl_x = dca::util::ceilDiv(n_rows, kernels::swap_block_size_x);
    const int bl_y = dca::util::ceilDiv(row_size, kernels::swap_block_size_y);

    dim3 threads(kernels::swap_block_size_x, kernels::swap_block_size_y);
    dim3 blocks(bl_x, bl_y);

    cudaStream_t stream = dca::linalg::util::getStream(thread_id, stream_id);

    kernels::swapRows<<<blocks, threads, 0, stream>>>(row_size, n_rows, i1, i2, a, lda);
    checkErrorsCudaDebug();
  }
}
template void swapRows(int row_size, int n_rows, const int* i1, const int* i2, float* a, int lda,
                       int thread_id, int stream_id);
template void swapRows(int row_size, int n_rows, const int* i1, const int* i2, double* a, int lda,
                       int thread_id, int stream_id);
template void swapRows(int row_size, int n_rows, const int* i1, const int* i2, cuComplex* a,
                       int lda, int thread_id, int stream_id);
template void swapRows(int row_size, int n_rows, const int* i1, const int* i2, cuDoubleComplex* a,
                       int lda, int thread_id, int stream_id);

template <typename Type>
void swapCols(int col_size, int n_cols, const int* j1, const int* j2, Type* a, int lda,
              int thread_id, int stream_id) {
  if (col_size > 0 && n_cols > 0) {
    checkErrorsCudaDebug();
    const int bl_x = dca::util::ceilDiv(col_size, kernels::swap_block_size_x);
    const int bl_y = dca::util::ceilDiv(n_cols, kernels::swap_block_size_y);

    dim3 threads(kernels::swap_block_size_x, kernels::swap_block_size_y);
    dim3 blocks(bl_x, bl_y);

    cudaStream_t stream = dca::linalg::util::getStream(thread_id, stream_id);

    kernels::swapCols<<<blocks, threads, 0, stream>>>(col_size, n_cols, j1, j2, a, lda);
    checkErrorsCudaDebug();
  }
}
template void swapCols(int col_size, int n_cols, const int* j1, const int* j2, float* a, int lda,
                       int thread_id, int stream_id);
template void swapCols(int col_size, int n_cols, const int* j1, const int* j2, double* a, int lda,
                       int thread_id, int stream_id);
template void swapCols(int col_size, int n_cols, const int* j1, const int* j2, cuComplex* a,
                       int lda, int thread_id, int stream_id);
template void swapCols(int col_size, int n_cols, const int* j1, const int* j2, cuDoubleComplex* a,
                       int lda, int thread_id, int stream_id);
}  // blas
}  // linalg
}  // dca
