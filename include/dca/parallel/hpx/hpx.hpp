// Copyright (C) 2009-2016 ETH Zurich
// Copyright (C) 2007?-2016 Center for Nanophase Materials Sciences, ORNL
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Peter Staar (taa@zurich.ibm.com)
//         Urs R. Haehner (haehneru@itp.phys.ethz.ch)
//
// This class provides an interface for parallelizing with HPX.
//
// TODO: Finish sum methods.

#ifndef DCA_PARALLEL_HPX_HPX_HPP
#define DCA_PARALLEL_HPX_HPX_HPP

#include <hpx/hpx.hpp>
#include <hpx/lcos/local/spinlock.hpp>
#include <hpx/lcos/local/condition_variable.hpp>
#include <hpx/lcos/future.hpp>
#include <hpx/include/threads.hpp>
#include <hpx/runtime/threads/thread.hpp>
#include <hpx/runtime/threads/executors/signalling_executor.hpp>
#include <hpx/util/debug/demangle_helper.hpp>
//
#include <vector>
#include <thread>

// #define DCA_HPX_THREAD_POOL_DEBUG

namespace dca {
namespace parallel {
struct thread_traits {
    template <typename T>
    using promise_type              = hpx::lcos::promise<T>;
    template <typename T>
    using future_type               = hpx::lcos::future<T>;
    using mutex_type                = hpx::lcos::local::mutex;
    using condition_variable_type   = hpx::lcos::local::condition_variable;
    using scoped_lock               = std::lock_guard<mutex_type>;
    using unique_lock               = std::unique_lock<mutex_type>;
    //
    static void sleep_for(hpx::util::steady_duration const& rel_time) {
        hpx::this_thread::sleep_for(rel_time);
    }
    //
    static void yield() {
        hpx::this_thread::yield();
    }
    //
    static std::uint64_t default_threadcount() {
        return hpx::get_num_worker_threads();
    }
};

class ThreadPool {
public:
    // Creates a pool with n_threads.
    // Actually does nothing, HPX does not need to allocate threads
    ThreadPool(size_t n_threads = 1) : exec(100) {
        set_task_count_threshold(n_threads-1);
    }

    ThreadPool(const ThreadPool& /*other*/) = delete;
    ThreadPool(ThreadPool&& /*other*/) = default;

    // Conclude all the pending work and destroy the threads spawned by this class.
    ~ThreadPool() {
        exec.set_and_wait(0);
    }

    void set_task_count_threshold(std::int64_t count)
    {
        exec.set_threshold(count);
    }

    void wait_for_tasks()
    {
        exec.wait();
    }

    // we don't do anything here
    void enlarge(std::size_t n_threads) {
        std::cout << "HPX threadpool enlarge: " << n_threads << std::endl;
    }

    // Call asynchronously the function f with arguments args. This method is thread safe.
    // Returns: a future to the result of f(args...).
    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
    {
#ifdef DCA_HPX_THREAD_POOL_DEBUG
                std::cout << "HPX threadpool enqueue\n";
    std::cout << "\n-------------------------------\n";
    std::cout << "enqueue: Function    : "
              << hpx::util::debug::print_type<F>() << "\n";
    std::cout << "enqueue: Arguments   : "
              << hpx::util::debug::print_type<Args...>(" | ") << std::endl;
#endif
//    typedef decltype(hpx::async(exec, std::forward<F>(f), std::forward<Args>(args)...)) return_type;
        return hpx::async(exec, std::forward<F>(f), std::forward<Args>(args)...);
    }

    // We will not be using the pool for a while - put threads to sleep
    void suspend() {
        hpx::suspend();
    }

    // Returns the number of threads used by this class.
    std::size_t size() const {
        std::cout << "HPX threadpool size" << std::endl;
        return hpx::get_num_worker_threads();
    }

    // Returns a static instance.
    static ThreadPool& get_instance() {
        static ThreadPool global_pool;
        return global_pool;
    }

    hpx::threads::executors::signalling_executor
    <hpx::threads::executors::default_executor> exec;
};


struct hpxthread
{
    hpxthread() {};

    // Execute the function f(id, num_threads, args...) as num_threads asynchronous tasks with id in
    // [0, num_threads - 1]. Then wait for the completion of the tasks.
    template <class F, class... Args>
    void execute(int num_threads, F&& f, Args&&... args)
    {
        std::vector<thread_traits::future_type<void>> futures;
        //
        auto& pool = ThreadPool::get_instance();
        pool.enlarge(num_threads);

        // Fork.
        for (int id = 0; id < num_threads; ++id)
            futures.emplace_back(
                    pool.enqueue(std::forward<F>(f),
                                 id, num_threads,std::forward<Args>(args)...));

        // Join.
        for (auto& future : futures)
            future.wait();
    }
};  // hpxthread

}  // parallel
}  // dca

#endif  // DCA_PARALLEL_hpxthread_hpxthread_HPP