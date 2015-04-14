//-*-C++-*-

/*
 *      Author: peter staar
 */

#ifndef BIPARTITE_SQUARE_MODEL_H
#define BIPARTITE_SQUARE_MODEL_H

template<typename DCA_point_group_type>
class bipartite_square_model
{
public:

  //typedef Null_symmetry_2D     LDA_point_group;

  typedef no_symmetry<2>       LDA_point_group;
  typedef DCA_point_group_type DCA_point_group; 

  const static cluster_shape_type DCA_cluster_shape = BETT_CLUSTER;
  const static cluster_shape_type LDA_cluster_shape = PARALLELEPIPED;

  const static int DIMENSION = 2;
  const static int BANDS     = 2;

  static double* initialize_r_DCA_basis();
  static double* initialize_k_DCA_basis();

  static double* initialize_r_LDA_basis();
  static double* initialize_k_LDA_basis();

  static std::vector<std::pair<std::pair<int,int>, std::pair<int,int> > > get_orbital_permutations();

  template<class domain, class parameters_type>
  static void initialize_H_interaction(FUNC_LIB::function<double , domain >& H_interaction,
				       parameters_type&            parameters);

  template<class domain>
  static void initialize_H_symmetry(FUNC_LIB::function<int , domain>& H_symmetry);

  template<class parameters_type>
  static std::complex<double> get_LDA_Hamiltonians(parameters_type& parameters, 
						   std::vector<double> k, int b1, int s1, int b2, int s2);
};


template<typename DCA_point_group_type>
double* bipartite_square_model<DCA_point_group_type>::initialize_r_DCA_basis()
{
  static double* r_DCA = new double[4];
  
  r_DCA[0] = 1.0;  r_DCA[1] = 1.0;
  r_DCA[2] = 1.0;  r_DCA[3] = -1.0;

  return r_DCA;
}

template<typename DCA_point_group_type>
double* bipartite_square_model<DCA_point_group_type>::initialize_k_DCA_basis()
{
  static double* k_DCA = new double[4];

  k_DCA[0] = 3.141592;  k_DCA[1] = 3.141592;
  k_DCA[2] = 3.141592;  k_DCA[3] = -3.141592;

  return k_DCA;
}

template<typename DCA_point_group_type>
double* bipartite_square_model<DCA_point_group_type>::initialize_r_LDA_basis()
{
  static double* r_LDA = new double[4];

  r_LDA[0] = 1.0;  r_LDA[1] = 0.0;
  r_LDA[2] = 0.0;  r_LDA[3] = 1.0;

  return r_LDA;
}

template<typename DCA_point_group_type>
double* bipartite_square_model<DCA_point_group_type>::initialize_k_LDA_basis()
{
  static double* k_LDA = new double[4];
  
  k_LDA[0] = 2*M_PI;  k_LDA[1] = 0.0;
  k_LDA[2] = 0.0   ;  k_LDA[3] = 2*M_PI;

  return k_LDA;
}

template<typename DCA_point_group_type>
template<class domain, class parameters_type>
void bipartite_square_model<DCA_point_group_type>::initialize_H_interaction(FUNC_LIB::function<double , domain >& H_interaction,
									    parameters_type&            parameters)
{
  std::vector<std::vector<double> >& U_ij = parameters.get_U_ij();

  for(size_t i=0; i<U_ij.size(); i++)
    H_interaction(U_ij[i][0], U_ij[i][1], U_ij[i][2], U_ij[i][3], U_ij[i][4]) = U_ij[i][5];
}

template<typename DCA_point_group_type>
std::vector<std::pair<std::pair<int,int>, std::pair<int,int> > > bipartite_square_model<DCA_point_group_type>::get_orbital_permutations()
{
  std::vector<std::pair<std::pair<int,int>, std::pair<int,int> > > permutations(0);

  {// permutation 0
    std::pair<int,int> initial_state(0,1);
    std::pair<int,int> final_state  (1,0);
    std::pair<std::pair<int,int>, std::pair<int,int> > p(initial_state, final_state);

    permutations.push_back(p);
  }

  return permutations;
}

template<typename DCA_point_group_type>
template<class domain>
void bipartite_square_model<DCA_point_group_type>::initialize_H_symmetry(FUNC_LIB::function<int , domain>& H_symmetries)
{
  // e_up <==> e_dn symmetry
//   for(int i=0; i<BANDS; i++){
//     for(int j=0; j<BANDS; j++){
//       int l = j+i*BANDS;
//       H_symmetries(i,0,j,0) = l;
//       H_symmetries(i,1,j,1) = l;
//     }
//   }

  for(int i=0; i<BANDS; i++){
    H_symmetries(i,0,i,0) = 0;
    H_symmetries(i,1,i,1) = 0;
  }

  H_symmetries(0,0,1,0) = 1;
  H_symmetries(0,1,1,1) = 1;
  H_symmetries(1,0,0,0) = 1;
  H_symmetries(1,1,0,1) = 1;

//   cout << __FUNCTION__ << endl;
//   for(int i=0; i<H_symmetries.size(); i++)
//     cout << H_symmetries(i) << endl;
//   cout << endl;
}

template<typename DCA_point_group_type>
template<class parameters_type>
std::complex<double> bipartite_square_model<DCA_point_group_type>::get_LDA_Hamiltonians(parameters_type& parameters,
						      		      std::vector<double> k, 
							      	      int b1, int s1, int b2, int s2)
{
  std::vector<std::vector<double> >& t_ij = parameters.get_t_ij();

  double t=0;
  for(size_t i=0; i<t_ij.size(); i++)
    if(t_ij[i][0]==b1 && t_ij[i][1]==b2 && t_ij[i][2]==0)
      t = t_ij[i][3];

  std::complex<double> i(0.,1.);
  std::complex<double> H_LDA = 0;

  if(s1==s2)
  {
	if(b1==0 && b2==0)
	  H_LDA = 0.;//t*abs(std::exp(i*(k[0]*(-1.0)+k[1]*(0.0)))+std::exp(i*(k[0]*(0.0)+k[1]*(-1.0)))+std::exp(i*(k[0]*(0.0)+k[1]*(1.0)))+std::exp(i*(k[0]*(1.0)+k[1]*(0.0))));

	if(b1==0 && b2==1)
	  H_LDA = -t*(0.+std::exp(i*(k[0]*(-1.0)+k[1]*(0.0)))+std::exp(i*(k[0]*(0.0)+k[1]*(-1.0)))+std::exp(i*(k[0]*(0.0)+k[1]*(1.0)))+std::exp(i*(k[0]*(1.0)+k[1]*(0.0))));

	if(b1==1 && b2==0)
	  H_LDA = -t*(0.+std::exp(i*(k[0]*(-1.0)+k[1]*(0.0)))+std::exp(i*(k[0]*(0.0)+k[1]*(-1.0)))+std::exp(i*(k[0]*(0.0)+k[1]*(1.0)))+std::exp(i*(k[0]*(1.0)+k[1]*(0.0))));

	if(b1==1 && b2==1)
	  H_LDA = 0;//-t*abs(std::exp(i*(k[0]*(-1.0)+k[1]*(0.0)))+std::exp(i*(k[0]*(0.0)+k[1]*(-1.0)))+std::exp(i*(k[0]*(0.0)+k[1]*(1.0)))+std::exp(i*(k[0]*(1.0)+k[1]*(0.0))));
  }

  return H_LDA;
}

#endif
