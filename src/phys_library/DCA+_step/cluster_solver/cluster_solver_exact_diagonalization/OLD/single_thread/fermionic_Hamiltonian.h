//-*-C++-*-

#ifndef FERMIONIC_HAMILTONIAN_H
#define FERMIONIC_HAMILTONIAN_H

namespace EXACT_DIAGONALIZATION_SOLVER
{
  /*!
   *   \author Peter Staar
   */
  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  class fermionic_Hamiltonian
  {
#include "type_definitions.h"
    
  public:

    typedef typename parameter_type::profiler_type    profiler_t;
    typedef typename parameter_type::Concurrency_Type concurrency_type;
    
    typedef double                                      scalar_type;
    typedef std::complex<scalar_type>                   complex_type;

    typedef LIN_ALG::vector<scalar_type , LIN_ALG::CPU> vector_type;
    typedef LIN_ALG::matrix<complex_type, LIN_ALG::CPU> matrix_type;

    typedef LIN_ALG::matrix<int         , LIN_ALG::CPU> int_matrix_type;
    
    typedef dmn_3<b_dmn, s_dmn, r_dmn> b_s_r;

    typedef dmn_0<occupation_number_domain<b_dmn, s_dmn, r_dmn> > occ_dmn;
    typedef dmn_0<magnetization_domain    <b_dmn, s_dmn, r_dmn> > mag_dmn;
    
    typedef dmn_2<occ_dmn, mag_dmn> occ_mag_dmn;
    
    typedef fermionic_Fock_space<parameter_type, b_dmn, s_dmn, r_dmn> fermionic_Fock_space_type;
    
  public:
    
    fermionic_Hamiltonian(parameter_type&            parameters_ref,
			  fermionic_Fock_space_type& Fock_space_ref);

    ~fermionic_Hamiltonian();
  
    template<typename MOMS_type>
    void set_functions(MOMS_type& MOMS);

    void initialize(FUNC_LIB::function<std::complex<double>, dmn_3<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_dmn> >& H_0,
		    FUNC_LIB::function<double              , dmn_3<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_dmn> >& H_i);

    void construct_Hamiltonians(bool interacting);

    void diagonolize_Hamiltonians();

    void compute_G_k_w(bool interacting);
    void compute_G_k_t(bool interacting);

    void compute_selfenergy();
    
  private:
    
    void initialize_t_ij_and_U_ij(FUNC_LIB::function<std::complex<double>, dmn_3<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_dmn> >& H_0,
				  FUNC_LIB::function<double              , dmn_3<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_dmn> >& H_i);


    void check_overlap_ac(int n_0, int Sz_0, int n_1, int Sz_1, overlap_indices& overlap_i, overlap_indices& overlap_j);
    void check_overlap_ca(int n_0, int Sz_0, int n_1, int Sz_1, overlap_indices& overlap_i, overlap_indices& overlap_j);

  private:

    struct V_struct
    {
    public:

      long long index;

      scalar_type value;
    };

    struct t_struct
    {
    public:

      long long lhs;
      long long rhs;

      std::complex<scalar_type> value;
    };

    struct U_struct
    {
    public:

      long long lhs;

      long long rhs;

      std::complex<scalar_type> value;
    };

  private:

    parameter_type&   parameters;
    concurrency_type& concurrency;

    fermionic_Fock_space_type& Fock_space;

    FUNC_LIB::function<int            , dmn_2<occ_dmn, mag_dmn> >& n_occupation_states;
    FUNC_LIB::function<int_matrix_type, dmn_2<occ_dmn, mag_dmn> >&   occupation_states;

    FUNC_LIB::function<std::vector<overlap_indices>, dmn_2<occ_mag_dmn, occ_mag_dmn> >& creation_overlap_of_states;
    FUNC_LIB::function<std::vector<overlap_indices>, dmn_2<occ_mag_dmn, occ_mag_dmn> >& annihilation_overlap_of_states;

    std::vector<V_struct> V_i;
    std::vector<t_struct> t_ij;
    std::vector<U_struct> U_ij;
		  
    FUNC_LIB::function<matrix_type, dmn_2<occ_dmn, mag_dmn> > Hamiltonians;	  

    FUNC_LIB::function<vector_type, dmn_2<occ_dmn, mag_dmn> > eigen_energies;	  
    FUNC_LIB::function<matrix_type, dmn_2<occ_dmn, mag_dmn> > eigen_states;

    FUNC_LIB::function<std::complex<double>, dmn_3<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_dmn> > overlap;
    
    FUNC_LIB::function<std::complex<double>, dmn_4<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, k_DCA, w> > G_k_w;
    FUNC_LIB::function<std::complex<double>, dmn_4<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_DCA, w> > G_r_w;
    FUNC_LIB::function<std::complex<double>, dmn_4<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, k_DCA, t> > G_k_t;
    FUNC_LIB::function<std::complex<double>, dmn_4<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_DCA, t> > G_r_t;

    FUNC_LIB::function<std::complex<double>, dmn_4<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, k_DCA, w> > G0_k_w;
    FUNC_LIB::function<std::complex<double>, dmn_4<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_DCA, w> > G0_r_w;
    FUNC_LIB::function<std::complex<double>, dmn_4<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, k_DCA, t> > G0_k_t;
    FUNC_LIB::function<std::complex<double>, dmn_4<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_DCA, t> > G0_r_t;

    FUNC_LIB::function<std::complex<double>, dmn_4<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, k_DCA, w> > S_k_w;
  };

  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  fermionic_Hamiltonian<parameter_type, b_dmn, s_dmn, r_dmn>::fermionic_Hamiltonian(parameter_type&            parameters_ref,
										    fermionic_Fock_space_type& Fock_space_ref):
    parameters(parameters_ref),
    concurrency(parameters.get_concurrency()),

    Fock_space(Fock_space_ref),

    n_occupation_states(Fock_space.n_occupation_states),
    occupation_states  (Fock_space.  occupation_states),
  
    creation_overlap_of_states    (Fock_space.creation_overlap_of_states),
    annihilation_overlap_of_states(Fock_space.annihilation_overlap_of_states),

    V_i (0),
    t_ij(0),
    U_ij(0),

    Hamiltonians  ("Hamiltonians"),

    eigen_energies("eigen_energies"),
    eigen_states  ("eigen_states")
  {}

  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  fermionic_Hamiltonian<parameter_type, b_dmn, s_dmn, r_dmn>::~fermionic_Hamiltonian()
  {}

  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  template<typename MOMS_type>
  void fermionic_Hamiltonian<parameter_type, b_dmn, s_dmn, r_dmn>::set_functions(MOMS_type& MOMS)
  {
    {
      for(int l=0; l<MOMS.G_k_w.size(); l++)
	MOMS.G_k_w(l) = G_k_w(l);
      
      for(int l=0; l<MOMS.G_r_w.size(); l++)
	MOMS.G_r_w(l) = G_r_w(l);

      for(int l=0; l<MOMS.G_k_w.size(); l++)
	MOMS.G0_k_w(l) = G0_k_w(l);
      
      for(int l=0; l<MOMS.G_r_w.size(); l++)
	MOMS.G0_r_w(l) = G0_r_w(l);
      
      for(int l=0; l<MOMS.G_k_w.size(); l++)
	MOMS.G0_k_w_cluster_excluded(l) = G0_k_w(l);
      
      for(int l=0; l<MOMS.G_r_w.size(); l++)
	MOMS.G0_r_w_cluster_excluded(l) = G0_r_w(l);
    }

    {
      for(int l=0; l<MOMS.G_k_t.size(); l++)
	MOMS.G_k_t(l) = real(G_k_t(l));
      
      for(int l=0; l<MOMS.G_r_t.size(); l++)
	MOMS.G_r_t(l) = real(G_r_t(l));
      
      for(int l=0; l<MOMS.G_k_t.size(); l++)
	MOMS.G0_k_t(l) = real(G0_k_t(l));
      
      for(int l=0; l<MOMS.G_r_t.size(); l++)
	MOMS.G0_r_t(l) = real(G0_r_t(l));
      
      for(int l=0; l<MOMS.G_k_t.size(); l++)
	MOMS.G0_k_t_cluster_excluded(l) = real(G0_k_t(l));

      for(int l=0; l<MOMS.G_r_t.size(); l++)
	MOMS.G0_r_t_cluster_excluded(l) = real(G0_r_t(l));
    }

    for(int l=0; l<MOMS.G_r_w.size(); l++)
      MOMS.Sigma(l)         = S_k_w(l);

    for(int l=0; l<MOMS.G_r_w.size(); l++)
      MOMS.Sigma_cluster(l) = S_k_w(l);

  }

  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  void fermionic_Hamiltonian<parameter_type, b_dmn, s_dmn, r_dmn>::initialize(FUNC_LIB::function<std::complex<double>, dmn_3<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_dmn> >& H_0,
									      FUNC_LIB::function<double              , dmn_3<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_dmn> >& H_i)
  {
    initialize_t_ij_and_U_ij(H_0, H_i);
  }

  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  void fermionic_Hamiltonian<parameter_type, b_dmn, s_dmn, r_dmn>::initialize_t_ij_and_U_ij(FUNC_LIB::function<std::complex<double>, dmn_3<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_dmn> >& H_0,
											    FUNC_LIB::function<double              , dmn_3<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_dmn> >& H_i)
  {
    {
      for(int r_j=0; r_j<r_dmn::dmn_size(); r_j++){
	for(int s_j=0; s_j<s_dmn::dmn_size(); s_j++){
	  for(int b_j=0; b_j<b_dmn::dmn_size(); b_j++){	
	    
	    V_struct V_obj;

	    V_obj.index = b_j+b_dmn::dmn_size()*(s_j+s_dmn::dmn_size()*r_j);

	    V_obj.value = -parameters.get_chemical_potential();

	    V_i.push_back(V_obj);
	  }
	}
      }
    }
 
    for(int r_j=0; r_j<r_dmn::dmn_size(); r_j++){
      for(int r_i=0; r_i<r_dmn::dmn_size(); r_i++){

	int delta_r = r_dmn::parameter_type::subtract(r_j, r_i);

	for(int s_j=0; s_j<s_dmn::dmn_size(); s_j++){
	  for(int b_j=0; b_j<b_dmn::dmn_size(); b_j++){	

	    for(int s_i=0; s_i<s_dmn::dmn_size(); s_i++){
	      for(int b_i=0; b_i<b_dmn::dmn_size(); b_i++){	
	      
		if(abs(H_0(b_i,s_i, b_j,s_j, delta_r))>1.e-3)
		  {
		    t_struct t_obj;
		  
		    t_obj.lhs = b_i+b_dmn::dmn_size()*(s_i+s_dmn::dmn_size()*r_i);
		    t_obj.rhs = b_j+b_dmn::dmn_size()*(s_j+s_dmn::dmn_size()*r_j);
		  
		    t_obj.value = H_0(b_i,s_i, b_j,s_j, delta_r)/2.;
		  
		    t_ij.push_back(t_obj);
		  }
		
		if(abs(H_i(b_i,s_i, b_j,s_j, delta_r))>1.e-3)
		  {
		    U_struct U_obj;
		  
		    U_obj.lhs = b_i+b_dmn::dmn_size()*(s_i+s_dmn::dmn_size()*r_i);
		    U_obj.rhs = b_j+b_dmn::dmn_size()*(s_j+s_dmn::dmn_size()*r_j);

		    U_obj.value = H_i(b_i,s_i, b_j,s_j, delta_r)/2.;
		  
		    U_ij.push_back(U_obj);
		  }
	      }
	    }
	  }
	}
      }
    }
  }

  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  void fermionic_Hamiltonian<parameter_type, b_dmn, s_dmn, r_dmn>::construct_Hamiltonians(bool interacting)
  {
    if(concurrency.id()==0)
      cout << __FUNCTION__ << endl;

    int Ns = b_dmn::dmn_size()*s_dmn::dmn_size()*r_dmn::dmn_size();

    int* Psi_lhs = new int[Ns];
    int* Psi_rhs = new int[Ns];
    int* Psi_new = new int[Ns];

    for(int i=0; i<occ_dmn::dmn_size(); i++){
      for(int j=0; j<mag_dmn::dmn_size(); j++){
      
	int N = n_occupation_states(i,j);
      
	if(N==0)
	  {
	    Hamiltonians(i,j).resize_no_copy(0);
	  }
	else
	  {
	    Hamiltonians(i,j).resize_no_copy(N);

	    matrix_type& H = Hamiltonians(i,j);

	    int_matrix_type& Psi = occupation_states(i,j);
	  
	    for(int l0=0; l0<N; l0++)
	      {
		for(int l=0; l<Ns; l++)
		  Psi_lhs[l] = Psi(l,l0);
	      
		for(int l1=0; l1<N; l1++)
		  {
		    for(int l=0; l<Ns; l++)
		      Psi_rhs[l] = Psi(l,l1);
		  
		    //H_ptr[l0+l1*N] = 0.;
		    H(l0, l1) = 0.;
		  
		    int diff=0;
		    for(int d=0; d<Ns; d++)
		      diff += square(Psi_lhs[d]-Psi_rhs[d]);

		    if(diff==0)
		      {
			for(size_t l=0; l<V_i.size(); l++)
			  H(l0,l1) += V_i[l].value*Psi_rhs[V_i[l].index];
		      }

		    if(diff==2)
		      {
			for(size_t l=0; l<t_ij.size(); l++)
			  {
			    if(Psi_lhs[t_ij[l].lhs] == 1 and Psi_lhs[t_ij[l].rhs] == 0 and
			       Psi_rhs[t_ij[l].lhs] == 0 and Psi_rhs[t_ij[l].rhs] == 1)
			      {
				scalar_type phase = 1.;
				
				for(int d=0; d<Ns; d++)
				  Psi_new[d] = Psi_rhs[d];
				
				{// apply annihilation operator
				  for(int d=0; d<t_ij[l].rhs; d++)
				    if(Psi_new[d] == 1)
				      phase *= scalar_type(-1.);
			    
				  Psi_new[t_ij[l].rhs] = 0;
				}
			  
				{// apply creation operator
				  for(int d=0; d<t_ij[l].lhs; d++)
				    if(Psi_new[d] == 1)
				      phase *= scalar_type(-1.);
				  
				  Psi_new[t_ij[l].lhs] = 1;
				}
		  
				{
				  int res=0;
				  for(int d=0; d<Ns; d++)
				    res += square(Psi_lhs[d]-Psi_new[d]);
				  
				  if(res != 0)
				    throw std::logic_error(__FUNCTION__);
				}

				H(l0,l1) += phase*t_ij[l].value;
			      }
			  }
		      }

		    if(interacting and diff==0)
		      {
			for(size_t l=0; l<U_ij.size(); l++)
			  {
			    H(l0,l1) += U_ij[l].value/scalar_type(4.);
			  
			    if(abs(Psi_rhs[U_ij[l].lhs]-scalar_type(1.))<1.e-3)
			      H(l0,l1) -= U_ij[l].value/scalar_type(2.);
			    
			    if(abs(Psi_rhs[U_ij[l].rhs]-scalar_type(1.))<1.e-3)
			      H(l0,l1) -= U_ij[l].value/scalar_type(2.);

			    if(abs(Psi_rhs[U_ij[l].lhs]-scalar_type(1.))<1.e-3 and 
			       abs(Psi_rhs[U_ij[l].rhs]-scalar_type(1.))<1.e-3)
			      H(l0,l1) += U_ij[l].value;
			  }
		      }

		  }
	      }

	    for(int l0=0; l0<N; l0++)
	      for(int l1=0; l1<N; l1++)
		if(abs(Hamiltonians(i,j)(l0,l1)-conj(Hamiltonians(i,j)(l1,l0)))>1.e-6)
		  throw std::logic_error(__FUNCTION__);
	  }
      }
    }

    delete [] Psi_lhs;
    delete [] Psi_rhs;
    delete [] Psi_new;
  }

  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  void fermionic_Hamiltonian<parameter_type, b_dmn, s_dmn, r_dmn>::diagonolize_Hamiltonians()
  {
    if(concurrency.id()==0)
      cout << "\n\t" << __FUNCTION__ << "\n\n";

    for(int i=0; i<occ_dmn::dmn_size(); i++){
      for(int j=0; j<mag_dmn::dmn_size(); j++){
      
	int N = n_occupation_states(i,j);
      
	if(N==0)
	  {
	    eigen_energies(i,j).resize        (0);
	    eigen_states  (i,j).resize_no_copy(0);
	  }
	else
	  {
	    eigen_energies(i,j).resize        (N);// = new              double [      N ];
	    eigen_states  (i,j).resize_no_copy(N);// = new std::complex<scalar_type>[square(N)];

	    {
	      if(concurrency.id()==0)
		cout << "\t N : " << i << ", Sz : " << mag_dmn::parameter_type::Sz(j) << ", \t time : ";
	      
	      int start = clock();
	      LIN_ALG::GEEV<LIN_ALG::CPU>::execute('V', 'U', Hamiltonians(i,j), eigen_energies(i,j), eigen_states(i,j));
	      int end = clock();

	      if(concurrency.id()==0)
		cout << double(end-start)/double(CLOCKS_PER_SEC) << "\n";
	    }

	    /*	  
	    for(int lj=0; lj<N; lj++)
	      {
		std::complex<scalar_type> norm = 0;
		for(int li=0; li<N; li++)	  
		  norm += conj(eigen_states(i,j)[li+lj*N])*eigen_states(i,j)[li+lj*N];
	      
		norm = scalar_type(1.)/std::sqrt(norm);

		for(int li=0; li<N; li++)
		  eigen_states(i,j)[li+lj*N] *= norm;
	      }
	    */
	  }
      }
    }
  }

  /*! p 134
   *
   *   G(\nu, \mu, z) = \frac{1}{Z} \sum_{n_0, n_1} \frac{\langle n_0 | c_{\nu} | n_1 \rangle \langle n_1 | c^{\dagger}_{\nu} | n_0 \rangle}{z+(E_n-E_{n'})}
   *
   *
   *   single site : G[\omega ] := 1/2 (1/(I*\omega - U/2) + 1/(I*\omega + U/2));
   */
  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  void fermionic_Hamiltonian<parameter_type, b_dmn, s_dmn, r_dmn>::compute_G_k_w(bool interacting)
  {
    if(concurrency.id()==0)
      cout << "\n\t" << __FUNCTION__ << endl;

    double beta = parameters.get_beta();

    double E_0 = 0.;
    for(int i=0; i<occ_dmn::dmn_size(); i++)
      for(int j=0; j<mag_dmn::dmn_size(); j++)
	for(int n=0; n<n_occupation_states(i,j); n++)
	  E_0 = E_0 > eigen_energies(i,j)[n]? eigen_energies(i,j)[n] : E_0;

    for(int i=0; i<occ_dmn::dmn_size(); i++)
      for(int j=0; j<mag_dmn::dmn_size(); j++)
	for(int n=0; n<n_occupation_states(i,j); n++)
	  eigen_energies(i,j)[n] -= E_0;

    FUNC_LIB::function<std::complex<double>, dmn_4<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_dmn, w> > tmp;

    double Z = 0;
    for(int i=0; i<occ_dmn::dmn_size(); i++)
      for(int j=0; j<mag_dmn::dmn_size(); j++)
	for(int n=0; n<n_occupation_states(i,j); n++)
	  Z += std::exp(-beta*eigen_energies(i,j)[n]);

    std::complex<double> I(0,1);

    //FUNC_LIB::function<std::complex<double>, dmn_3<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_dmn> > overlap;

    for(int n_0=0; n_0<occ_dmn::dmn_size(); n_0++){
      for(int Sz_0=0; Sz_0<mag_dmn::dmn_size(); Sz_0++){
	for(int n_1=0; n_1<occ_dmn::dmn_size(); n_1++){
	  for(int Sz_1=0; Sz_1<mag_dmn::dmn_size(); Sz_1++){
	  
	    if(annihilation_overlap_of_states(n_0, Sz_0, n_1, Sz_1).size()>0 and
	       creation_overlap_of_states    (n_1, Sz_1, n_0, Sz_0).size()>0 )
	      {
		int N_0 = n_occupation_states(n_0, Sz_0);
		int N_1 = n_occupation_states(n_1, Sz_1);

		for(int l0=0; l0<N_0; l0++){
		  for(int l1=0; l1<N_1; l1++){
		  
		    overlap = 0.;

		    std::complex<scalar_type>* psi_0 = &(eigen_states(n_0, Sz_0)(0, l0));
		    std::complex<scalar_type>* psi_1 = &(eigen_states(n_1, Sz_1)(0, l1));

		    for(size_t i=0; i<annihilation_overlap_of_states(n_0, Sz_0, n_1, Sz_1).size(); i++){
		      for(size_t j=0; j<creation_overlap_of_states(n_1, Sz_1, n_0, Sz_0).size(); j++){
		      
			overlap_indices& overlap_i = annihilation_overlap_of_states(n_0, Sz_0, n_1, Sz_1)[i];
			overlap_indices& overlap_j = creation_overlap_of_states    (n_1, Sz_1, n_0, Sz_0)[j];

			assert(overlap_i.lhs>-1 and overlap_i.lhs<N_0);
			assert(overlap_i.rhs>-1 and overlap_i.rhs<N_1);
			assert(overlap_j.lhs>-1 and overlap_j.lhs<N_1);
			assert(overlap_j.rhs>-1 and overlap_j.rhs<N_0);

			scalar_type phase = overlap_i.sign*overlap_j.sign;

			check_overlap_ac(n_0, Sz_0, n_1, Sz_1, overlap_i, overlap_j);

			int delta_r = r_dmn::parameter_type::subtract(overlap_i.r_i, overlap_j.r_i);

			overlap(overlap_i.b_i, overlap_i.s_i, 
				overlap_j.b_i, overlap_j.s_i,
				delta_r) += phase*(conj(psi_0[overlap_i.lhs])*psi_1[overlap_i.rhs]
						   *conj(psi_1[overlap_j.lhs])*psi_0[overlap_j.rhs]);
		      }
		    }

		    for(int w_i=0; w_i<w::dmn_size(); w_i++)
		      {
			std::complex<double> iw = I*w::get_elements()[w_i];

			double E_n0 = eigen_energies(n_0, Sz_0)[l0];
			double E_n1 = eigen_energies(n_1, Sz_1)[l1];

			std::complex<double> denum = 1./(iw + E_n0 - E_n1);
			std::complex<double>   num = std::exp(-beta*E_n0);

			for(int r_i=0; r_i<r_dmn::dmn_size(); r_i++)
			  for(int nu_j=0; nu_j<2*b_dmn::dmn_size(); nu_j++)
			    for(int nu_i=0; nu_i<2*b_dmn::dmn_size(); nu_i++)
			      tmp(nu_i, nu_j, r_i, w_i) += num*denum*overlap(nu_i, nu_j, r_i);
		      }
		  }
		
		}
	      }

	    if(creation_overlap_of_states    (n_0, Sz_0, n_1, Sz_1).size()>0 and
	       annihilation_overlap_of_states(n_1, Sz_1, n_0, Sz_0).size()>0 )
	      {
		int N_0 = n_occupation_states(n_0, Sz_0);
		int N_1 = n_occupation_states(n_1, Sz_1);

		for(int l0=0; l0<N_0; l0++){
		  for(int l1=0; l1<N_1; l1++){

		    overlap  = 0.;

		    std::complex<scalar_type>* psi_0 = &(eigen_states(n_0, Sz_0)(0,l0));
		    std::complex<scalar_type>* psi_1 = &(eigen_states(n_1, Sz_1)(0,l1));

		    for(size_t i=0; i<creation_overlap_of_states(n_0, Sz_0, n_1, Sz_1).size(); i++){
		      for(size_t j=0; j<annihilation_overlap_of_states(n_1, Sz_1, n_0, Sz_0).size(); j++){
		      
			overlap_indices& overlap_i = creation_overlap_of_states    (n_0, Sz_0, n_1, Sz_1)[i];
			overlap_indices& overlap_j = annihilation_overlap_of_states(n_1, Sz_1, n_0, Sz_0)[j];

			assert(overlap_i.lhs>-1 and overlap_i.lhs<N_0);
			assert(overlap_i.rhs>-1 and overlap_i.rhs<N_1);
			assert(overlap_j.lhs>-1 and overlap_j.lhs<N_1);
			assert(overlap_j.rhs>-1 and overlap_j.rhs<N_0);

			scalar_type phase = overlap_i.sign*overlap_j.sign;

			check_overlap_ca(n_0, Sz_0, n_1, Sz_1, overlap_i, overlap_j);

			int delta_r = r_dmn::parameter_type::subtract(overlap_i.r_i, overlap_j.r_i);

			overlap(overlap_i.b_i, overlap_i.s_i, 
				overlap_j.b_i, overlap_j.s_i,
				delta_r) += phase*( conj(psi_0[overlap_i.lhs])*psi_1[overlap_i.rhs]
						    *conj(psi_1[overlap_j.lhs])*psi_0[overlap_j.rhs]);
		      }
		    }

		    for(int w_i=0; w_i<w::dmn_size(); w_i++)
		      {
			std::complex<double> iw = I*w::get_elements()[w_i];

			double E_n0 = eigen_energies(n_0, Sz_0)[l0];
			double E_n1 = eigen_energies(n_1, Sz_1)[l1];

			std::complex<double> denum = 1./(iw - E_n0 + E_n1);
			std::complex<double>   num = std::exp(-beta*E_n0);

			for(int r_i=0; r_i<r_dmn::dmn_size(); r_i++)
			  for(int nu_j=0; nu_j<2*b_dmn::dmn_size(); nu_j++)
			    for(int nu_i=0; nu_i<2*b_dmn::dmn_size(); nu_i++)
			      tmp(nu_i, nu_j, r_i, w_i) += num*denum*overlap(nu_i, nu_j, r_i);
		      }
		  }
		}
	      }
	  }
	}
      }
    }

    double factor = 1./(Z*r_dmn::dmn_size());
    tmp          *= factor;

    {
      for(int i=0; i<occ_dmn::dmn_size(); i++)
	for(int j=0; j<mag_dmn::dmn_size(); j++)
	  for(int n=0; n<n_occupation_states(i,j); n++)
	    eigen_energies(i,j)[n] += E_0;
    }

    if(interacting)
      {
	G_r_w = tmp;
	FT<r_dmn, k_DCA>::execute(G_r_w, G_k_w);
      }
    else
      {
	G0_r_w = tmp;
	FT<r_dmn, k_DCA>::execute(G0_r_w, G0_k_w);
      }
  }

  /*! p 134
   *
   *   G(\tau, \epsilon) = \frac{1}{\beta} \sum_{m} \frac{1}{i \varpi_m + \epsilon} e^{i \varpi \tau}
   *                     = \frac{1}{\beta} \sum_{m} (\frac{1}{i \varpi_m + \epsilon} -\frac{1}{i \varpi_m }) e^{i \varpi \tau} + 0.5
   *                     = (1 - \frac{1}{e^{-\beta \epsilon}+1} ) * e^{(\beta-\tau) \epsilon}
   *                     = \frac{1}{e^{-(\beta-\tau) \epsilon} + e^{\tau \epsilon} }
   *
   */
  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  void fermionic_Hamiltonian<parameter_type, b_dmn, s_dmn, r_dmn>::compute_G_k_t(bool interacting)
  {
    if(concurrency.id()==0)
      cout << "\n\t" << __FUNCTION__ << endl;

    double beta = parameters.get_beta();

    double E_0 = 0.;
    for(int i=0; i<occ_dmn::dmn_size(); i++)
      for(int j=0; j<mag_dmn::dmn_size(); j++)
	for(int n=0; n<n_occupation_states(i,j); n++)
	  E_0 = E_0 > eigen_energies(i,j)[n]? eigen_energies(i,j)[n] : E_0;

    for(int i=0; i<occ_dmn::dmn_size(); i++)
      for(int j=0; j<mag_dmn::dmn_size(); j++)
	for(int n=0; n<n_occupation_states(i,j); n++)
	  eigen_energies(i,j)[n] -= E_0;

    FUNC_LIB::function<std::complex<double>, dmn_4<dmn_2<b_dmn, s_dmn>, dmn_2<b_dmn, s_dmn>, r_dmn, t> > tmp;

    double Z = 0;
    for(int i=0; i<occ_dmn::dmn_size(); i++)
      for(int j=0; j<mag_dmn::dmn_size(); j++)
	for(int n=0; n<n_occupation_states(i,j); n++)
	  Z += std::exp(-beta*eigen_energies(i,j)[n]);

    cout << "\t Z : " << Z << endl;

    for(int n_0=0; n_0<occ_dmn::dmn_size(); n_0++){
      for(int Sz_0=0; Sz_0<mag_dmn::dmn_size(); Sz_0++){
	for(int n_1=0; n_1<occ_dmn::dmn_size(); n_1++){
	  for(int Sz_1=0; Sz_1<mag_dmn::dmn_size(); Sz_1++){
	  
	    if(annihilation_overlap_of_states(n_0, Sz_0, n_1, Sz_1).size()>0 and
	       creation_overlap_of_states    (n_1, Sz_1, n_0, Sz_0).size()>0 )
	      {
		int N_0 = n_occupation_states(n_0, Sz_0);
		int N_1 = n_occupation_states(n_1, Sz_1);

		for(int l0=0; l0<N_0; l0++){
		  for(int l1=0; l1<N_1; l1++){
		  
		    overlap = 0.;

		    std::complex<scalar_type>* psi_0 = &(eigen_states(n_0, Sz_0)(0,l0));
		    std::complex<scalar_type>* psi_1 = &(eigen_states(n_1, Sz_1)(0,l1));

		    for(size_t i=0; i<annihilation_overlap_of_states(n_0, Sz_0, n_1, Sz_1).size(); i++){
		      for(size_t j=0; j<creation_overlap_of_states(n_1, Sz_1, n_0, Sz_0).size(); j++){
		      
			overlap_indices& overlap_i = annihilation_overlap_of_states(n_0, Sz_0, n_1, Sz_1)[i];
			overlap_indices& overlap_j = creation_overlap_of_states    (n_1, Sz_1, n_0, Sz_0)[j];

			assert(overlap_i.lhs>-1 and overlap_i.lhs<N_0);
			assert(overlap_i.rhs>-1 and overlap_i.rhs<N_1);
			assert(overlap_j.lhs>-1 and overlap_j.lhs<N_1);
			assert(overlap_j.rhs>-1 and overlap_j.rhs<N_0);

			scalar_type phase = overlap_i.sign*overlap_j.sign;

			check_overlap_ac(n_0, Sz_0, n_1, Sz_1, overlap_i, overlap_j);

			int delta_r = r_dmn::parameter_type::subtract(overlap_i.r_i, overlap_j.r_i);

			overlap(overlap_i.b_i, overlap_i.s_i, 
				overlap_j.b_i, overlap_j.s_i,
				delta_r) += phase*(conj(psi_0[overlap_i.lhs])*psi_1[overlap_i.rhs]
						   *conj(psi_1[overlap_j.lhs])*psi_0[overlap_j.rhs]);
		      }
		    }


		    double E_n0 = eigen_energies(n_0, Sz_0)[l0];
		    double E_n1 = eigen_energies(n_1, Sz_1)[l1];

		    double w_e   = std::exp(-beta*E_n0);
			
		    //double FD = (1. - 1./(std::exp(-beta*(E_n0-E_n1))+1.) );

		    for(int t_i=t::dmn_size()/2; t_i<t::dmn_size(); t_i++)
		      {
			double tau = t::get_elements()[t_i];

			//double G_tau = FD*std::exp((beta-tau)*(E_n0 - E_n1));

			//double G_tau = 1./(std::exp(-(beta-tau)*(E_n0 - E_n1)) + std::exp(tau*(E_n0 - E_n1)));
			double G_tau = 1./(std::exp((beta-tau)*(E_n0 - E_n1)) + std::exp(-tau*(E_n0 - E_n1)));

			for(int r_i=0; r_i<r_dmn::dmn_size(); r_i++)
			  for(int nu_j=0; nu_j<2*b_dmn::dmn_size(); nu_j++)
			    for(int nu_i=0; nu_i<2*b_dmn::dmn_size(); nu_i++)
			      tmp(nu_i, nu_j, r_i, t_i) += w_e*G_tau*overlap(nu_i, nu_j, r_i);
		      }
		  }
		}
	      }

	    if(creation_overlap_of_states    (n_0, Sz_0, n_1, Sz_1).size()>0 and
	       annihilation_overlap_of_states(n_1, Sz_1, n_0, Sz_0).size()>0 )
	      {
		int N_0 = n_occupation_states(n_0, Sz_0);
		int N_1 = n_occupation_states(n_1, Sz_1);

		for(int l0=0; l0<N_0; l0++){
		  for(int l1=0; l1<N_1; l1++){

		    overlap  = 0.;

		    std::complex<scalar_type>* psi_0 = &(eigen_states(n_0, Sz_0)(0,l0));
		    std::complex<scalar_type>* psi_1 = &(eigen_states(n_1, Sz_1)(0,l1));

		    for(size_t i=0; i<creation_overlap_of_states(n_0, Sz_0, n_1, Sz_1).size(); i++){
		      for(size_t j=0; j<annihilation_overlap_of_states(n_1, Sz_1, n_0, Sz_0).size(); j++){
		      
			overlap_indices& overlap_i = creation_overlap_of_states    (n_0, Sz_0, n_1, Sz_1)[i];
			overlap_indices& overlap_j = annihilation_overlap_of_states(n_1, Sz_1, n_0, Sz_0)[j];

			assert(overlap_i.lhs>-1 and overlap_i.lhs<N_0);
			assert(overlap_i.rhs>-1 and overlap_i.rhs<N_1);
			assert(overlap_j.lhs>-1 and overlap_j.lhs<N_1);
			assert(overlap_j.rhs>-1 and overlap_j.rhs<N_0);

			scalar_type phase = overlap_i.sign*overlap_j.sign;

			check_overlap_ca(n_0, Sz_0, n_1, Sz_1, overlap_i, overlap_j);

			int delta_r = r_dmn::parameter_type::subtract(overlap_i.r_i, overlap_j.r_i);

			overlap(overlap_i.b_i, overlap_i.s_i, 
				overlap_j.b_i, overlap_j.s_i,
				delta_r) += phase*( conj(psi_0[overlap_i.lhs])*psi_1[overlap_i.rhs]
						    *conj(psi_1[overlap_j.lhs])*psi_0[overlap_j.rhs]);
		      }
		    }

		    double E_n0 = eigen_energies(n_0, Sz_0)[l0];
		    double E_n1 = eigen_energies(n_1, Sz_1)[l1];

		    double w_e   = std::exp(-beta*E_n0);

		    //double FD = (1. - 1./(std::exp(-beta*(E_n1-E_n0))+1.) );

		    for(int t_i=t::dmn_size()/2; t_i<t::dmn_size(); t_i++)
		      {
			double tau = t::get_elements()[t_i];

			//double G_tau = FD*std::exp((beta-tau)*(E_n1 - E_n0));

			//double G_tau = 1./(std::exp(-(beta-tau)*(E_n1 - E_n0)) + std::exp(tau*(E_n1 - E_n0)));
			double G_tau = 1./(std::exp((beta-tau)*(E_n1 - E_n0)) + std::exp(-tau*(E_n1 - E_n0)));

			for(int r_i=0; r_i<r_dmn::dmn_size(); r_i++)
			  for(int nu_j=0; nu_j<2*b_dmn::dmn_size(); nu_j++)
			    for(int nu_i=0; nu_i<2*b_dmn::dmn_size(); nu_i++)
			      tmp(nu_i, nu_j, r_i, t_i) += w_e*G_tau*overlap(nu_i, nu_j, r_i);
		      }
		  }
		}
	      }
	  }
	}
      }
    }

    for(int t_i=t::dmn_size()/2; t_i<t::dmn_size(); t_i++)
      for(int r_i=0; r_i<r_dmn::dmn_size(); r_i++)
	for(int nu_j=0; nu_j<2*b_dmn::dmn_size(); nu_j++)
	  for(int nu_i=0; nu_i<2*b_dmn::dmn_size(); nu_i++)
	    tmp(nu_i, nu_j, r_i, t_i-t::dmn_size()/2) = -tmp(nu_i, nu_j, r_i, t_i);
    
    double factor = -1./(Z*r_dmn::dmn_size());
    tmp        *= factor;
            
    {
      for(int i=0; i<occ_dmn::dmn_size(); i++)
	for(int j=0; j<mag_dmn::dmn_size(); j++)
	  for(int n=0; n<n_occupation_states(i,j); n++)
	    eigen_energies(i,j)[n] += E_0;
    }

    if(interacting)
      {
	G_r_t = tmp;
	FT<r_dmn, k_DCA>::execute(G_r_t, G_k_t);
      }
    else
      {
	G0_r_t = tmp;
	FT<r_dmn, k_DCA>::execute(G0_r_t, G0_k_t);
      }
  }


  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  void fermionic_Hamiltonian<parameter_type, b_dmn, s_dmn, r_dmn>::check_overlap_ac(int n_0, int Sz_0, 
										    int n_1, int Sz_1, 
										    overlap_indices& overlap_i, 
										    overlap_indices& overlap_j)
  {
    const static int N = b_dmn::dmn_size()*s_dmn::dmn_size()*r_dmn::dmn_size();

    assert(overlap_i.b_i+b_dmn::dmn_size()*overlap_i.s_i+b_dmn::dmn_size()*s_dmn::dmn_size()*overlap_i.r_i == overlap_i.index);
    assert(overlap_j.b_i+b_dmn::dmn_size()*overlap_j.s_i+b_dmn::dmn_size()*s_dmn::dmn_size()*overlap_j.r_i == overlap_j.index);

    int lhs_i = overlap_i.lhs;
    int rhs_i = overlap_i.rhs;
    int lhs_j = overlap_j.lhs;
    int rhs_j = overlap_j.rhs;

    int* psi_lhs_i = &(occupation_states(n_0, Sz_0)(0,lhs_i));
    int* psi_rhs_i = &(occupation_states(n_1, Sz_1)(0,rhs_i));
    int* psi_lhs_j = &(occupation_states(n_1, Sz_1)(0,lhs_j));
    int* psi_rhs_j = &(occupation_states(n_0, Sz_0)(0,rhs_j));

    assert(psi_lhs_i[overlap_i.index]==0);
    assert(psi_rhs_i[overlap_i.index]==1);
    assert(psi_lhs_j[overlap_j.index]==1);
    assert(psi_rhs_j[overlap_j.index]==0);
  
    {
      for(int d=0; d<N; d++)
	if(not (psi_lhs_i[d] == psi_rhs_i[d] or overlap_i.index==d) )
	  throw std::logic_error(__FUNCTION__);
    }

    {
      int sign = 1;
      for(int d=0; d<overlap_i.index; d++)
	if(psi_lhs_i[d]==1)
	  sign *= -1;
      assert(sign == overlap_i.sign);
    }

    {
      for(int d=0; d<N; d++)
	if(not (psi_lhs_j[d] == psi_rhs_j[d] or overlap_j.index==d) )
	  throw std::logic_error(__FUNCTION__);
    }

    {
      int sign = 1;
      for(int d=0; d<overlap_j.index; d++)
	if(psi_rhs_j[d]==1)
	  sign *= -1;
      assert(sign == overlap_j.sign);
    }
  }

  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  void fermionic_Hamiltonian<parameter_type, b_dmn, s_dmn, r_dmn>::check_overlap_ca(int n_0, int Sz_0, 
										    int n_1, int Sz_1, 
										    overlap_indices& overlap_i, 
										    overlap_indices& overlap_j)
  {
    const static int N = b_dmn::dmn_size()*s_dmn::dmn_size()*r_dmn::dmn_size();

    assert(overlap_i.b_i+b_dmn::dmn_size()*overlap_i.s_i+b_dmn::dmn_size()*s_dmn::dmn_size()*overlap_i.r_i == overlap_i.index);
    assert(overlap_j.b_i+b_dmn::dmn_size()*overlap_j.s_i+b_dmn::dmn_size()*s_dmn::dmn_size()*overlap_j.r_i == overlap_j.index);

    int lhs_i = overlap_i.lhs;
    int rhs_i = overlap_i.rhs;
    int lhs_j = overlap_j.lhs;
    int rhs_j = overlap_j.rhs;

    int* psi_lhs_i = &(occupation_states(n_0, Sz_0)(0,lhs_i));
    int* psi_rhs_i = &(occupation_states(n_1, Sz_1)(0,rhs_i));
    int* psi_lhs_j = &(occupation_states(n_1, Sz_1)(0,lhs_j));
    int* psi_rhs_j = &(occupation_states(n_0, Sz_0)(0,rhs_j));

    assert(psi_lhs_i[overlap_i.index]==1);
    assert(psi_rhs_i[overlap_i.index]==0);
    assert(psi_lhs_j[overlap_j.index]==0);
    assert(psi_rhs_j[overlap_j.index]==1);
  
    {
      for(int d=0; d<N; d++)
	if(not (psi_lhs_i[d] == psi_rhs_i[d] or overlap_i.index==d) )
	  throw std::logic_error(__FUNCTION__);
    }

    {
      for(int d=0; d<N; d++)
	if(not (psi_lhs_j[d] == psi_rhs_j[d] or overlap_j.index==d) )
	  throw std::logic_error(__FUNCTION__);
    }

    {
      int sign = 1;
      for(int d=0; d<overlap_i.index; d++)
	if(psi_rhs_i[d]==1)
	  sign *= -1;
      assert(sign == overlap_i.sign);
    }

    {
      int sign = 1;
      for(int d=0; d<overlap_j.index; d++)
	if(psi_lhs_j[d]==1)
	  sign *= -1;
      assert(sign == overlap_j.sign);
    }
  }

  template<typename parameter_type, typename b_dmn, typename s_dmn, typename r_dmn>
  void fermionic_Hamiltonian<parameter_type, b_dmn, s_dmn, r_dmn>::compute_selfenergy()
  {    
    if(concurrency.id()==0)
      cout << "\n\t" << __FUNCTION__ << endl;

    int matrix_size = b::dmn_size()*s::dmn_size()*b::dmn_size()*s::dmn_size();
    int matrix_dim  = b::dmn_size()*s::dmn_size();
  
    std::complex<double>* G_inverted_matrix                   = new std::complex<double>[matrix_size];
    std::complex<double>* G0_cluster_excluded_inverted_matrix = new std::complex<double>[matrix_size];
    std::complex<double>* Sigma_matrix                        = new std::complex<double>[matrix_size];
  
    // Sigma = 1/G0 - 1/G
  
    for(int k_ind=0; k_ind<k_DCA::dmn_size(); k_ind++){
      for(int w_ind=0; w_ind<w::dmn_size(); w_ind++){
      
	{
	  invert_plan<std::complex<double> > invert_pln(matrix_dim);
	  memcpy(invert_pln.Matrix, &G_k_w(0,0,0,0,k_ind,w_ind), sizeof(std::complex<double>)*matrix_size);
	  invert_pln.execute_plan();
	  memcpy(G_inverted_matrix, invert_pln.inverted_matrix, sizeof(std::complex<double>)*matrix_size);
	}
      
	{
	  invert_plan<std::complex<double> > invert_pln(matrix_dim);
	  memcpy(invert_pln.Matrix, &G0_k_w(0,0,0,0,k_ind,w_ind), sizeof(std::complex<double>)*matrix_size);
	  invert_pln.execute_plan();
	  memcpy(G0_cluster_excluded_inverted_matrix, invert_pln.inverted_matrix, sizeof(std::complex<double>)*matrix_size);
	}
      
	for(int l=0; l<matrix_size; ++l)
	  Sigma_matrix[l] = (G0_cluster_excluded_inverted_matrix[l] - G_inverted_matrix[l]);
      
	memcpy(&S_k_w(0,0,0,0,k_ind,w_ind), Sigma_matrix, sizeof(std::complex<double>)*matrix_size);
      }
    }
  
    delete [] G_inverted_matrix;
    delete [] G0_cluster_excluded_inverted_matrix;
    delete [] Sigma_matrix;
  
    if(concurrency.id()==0)
      {
	cout << "\n";
	for(int w_i=w::dmn_size()/2-36; w_i<w::dmn_size()/2+36; w_i++){
	  cout << w::get_elements()[w_i] << "\t";
	  for(int k_i=0; k_i<k_DCA::dmn_size(); k_i++)
	    cout << real(S_k_w(0,0,0,0,k_i,w_i)) << "\t" << imag(S_k_w(0,0,0,0,k_i,w_i)) << "\t";
	  cout << "\n";
	}
	cout << "\n";
      }
  }
  
}

#endif
