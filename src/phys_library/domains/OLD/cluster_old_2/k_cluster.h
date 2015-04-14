//-*-C++-*-

#ifndef K_CLUSTER_H
#define K_CLUSTER_H

/*!
 * \author Peter Staar
 */
template<class base_cluster_type>
class k_cluster<FULL, base_cluster_type>
{

public:

  typedef k_cluster<FULL, base_cluster_type > this_type;
  typedef r_cluster<FULL, base_cluster_type > dual_type;

  //typedef base_cluster_type                 base_cluster;


//   typedef typename base_cluster_type::parameter_type         parameter_type;
//   typedef typename parameter_type::point_group               point_group_type;

//   typedef dmn_0<electron_band_domain>                                   b_dmn_t;
//   typedef dmn_0<this_type>                                              k_dmn_t;

//   typedef dmn_0<point_group_symmetry_domain<UNIT_CELL, base_cluster> >  sym_unit_cell_dmn_t;
//   typedef dmn_0<point_group_symmetry_domain<SUPER_CELL, base_cluster> > sym_super_cell_dmn_t;

  typedef std::vector<double>                                element_type;

  const static cluster_representation_type cluster_representation = FULL;

//   const static int DIMENSION = parameter_type::DIMENSION;
  const static int DIMENSION = base_cluster_type::DIMENSION;
  
  typedef MATH_ALGORITHMS::discrete_periodic_dmn_nD_type dmn_specifications_type;

public:

  static int                                get_size(); 

//   static std::vector<std::vector<double> >& get_basis();
//   static std::vector<std::vector<double> >& get_super_basis();

  static std::vector<std::vector<double> >& get_basis_vectors();
  static std::vector<std::vector<double> >& get_super_basis_vectors();

  static std::vector<std::vector<double> >& get_elements();
  
  static int origin_index();
  
  //static std::vector<int> get_super_cluster_grid();

  static double get_volume();
  //static int    get_k_0_index();

  //static int    get_K_index(std::vector<double> k);

  //static double find_minimal_distance(std::vector<double> vec_0, std::vector<double> vec_1);

  //static std::pair<bool, std::vector<std::vector<double> > > find_equivalent_vectors_with_minimal_distance_to_origin(std::vector<double> K_vec);

  static int add     (int k_i, int k_j);
  static int subtract(int k_i, int k_j);

  //static std::vector<double> back_inside_cluster(std::vector<double>& k);

//   static FUNC_LIB::function<std::pair<int,int>, dmn_2< dmn_2<k_dmn_t,b_dmn_t>, sym_super_cell_dmn_t > >& get_symmetry_matrix(){
//     //static FUNC_LIB::function<std::pair<int,int>, dmn_2< dmn_2<k_dmn_t,b_dmn_t>, sym_super_cell_dmn_t> > symmetry_matrix("k_symmetry_matrix_super_cell");
//     //return symmetry_matrix;

//     return cluster_symmetry<this_type>::get_symmetry_matrix();
//   }

  //static bool is_high_symmetry_point(int k_ind);

  //static std::vector<double> get_affine_coordinate(std::vector<double>& k);

  static void print();

  template<class stream_type>
  static void to_JSON(stream_type& ss);

  //static bool test_back_inside_cluster();

private:

  //static std::vector<std::vector<double> >& get_sorted_elements();
  
  /*
  static std::vector<double> back_inside_cluster_1D(std::vector<double>& k);
  static std::vector<double> back_inside_cluster_2D(std::vector<double>& k);
  static std::vector<double> back_inside_cluster_3D(std::vector<double>& k);

  static std::vector<double> get_affine_coordinate_1D(std::vector<double>& k);
  static std::vector<double> get_affine_coordinate_2D(std::vector<double>& k);
  static std::vector<double> get_affine_coordinate_3D(std::vector<double>& k);
  */

//   static std::vector<int> find_super_cluster_grid();

  //static std::vector<element_type> initialize_sorted_elements();

  static std::vector<int> initialize_subtract_matrix();
  static std::vector<int> initialize_addition_matrix();

  static bool test_add_matrix();
  static bool test_subtract_matrix();
};

// template<class base_cluster_type >
// std::vector<std::vector<double> >& k_cluster<representation_type, base_cluster_type >::get_basis()
// {
//   return base_cluster_type::get_k_basis();
// }

// template<class base_cluster_type >
// std::vector<std::vector<double> >& k_cluster<representation_type, base_cluster_type >::get_super_basis()
// {
//   return base_cluster_type::get_k_super_basis();
// }


template<class base_cluster_type >
std::vector<std::vector<double> >& k_cluster<FULL, base_cluster_type >::get_basis_vectors()
{
  return base_cluster_type::get_k_basis_new();
}

template<class base_cluster_type >
std::vector<std::vector<double> >& k_cluster<FULL, base_cluster_type >::get_super_basis_vectors()
{
  return base_cluster_type::get_k_super_basis_new();
}

// template<class base_cluster_type >
// std::vector<int> k_cluster<FULL, base_cluster_type >::get_super_cluster_grid()
// {
//   static std::vector<int> grid = find_super_cluster_grid();
//   return grid;
// }

// template<class base_cluster_type >
// std::vector<int> k_cluster<FULL, base_cluster_type >::find_super_cluster_grid()
// {
//   std::vector<int> grid(DIMENSION,0);

//   for(int d=0; d<DIMENSION; ++d){
    
//     bool check=true;
//     double index=0.;
//     std::vector<double> tmp(DIMENSION, 0.);
    
//     do 
//       {
// 	index += 1.;
// 	tmp = VECTOR_OPERATIONS::SCALE(index, get_super_basis()[d]);
// 	tmp = back_inside_cluster(tmp);

// 	check=true;
// 	for(int l=0; l<DIMENSION; ++l)
// 	if(abs(get_super_basis()[d][l])>1.e-6 && abs(tmp[l])<1.e-6)
// 	  check=false;
//       }
//     while(check);
      
//     grid[d] = index;
//   }

//   int size=1;
//   for(int d=0; d<DIMENSION; ++d)
//     size *= grid[d];
  
//   if(size != get_size())
//     throw std::logic_error(__PRETTY_FUNCTION__);

//   return grid;
// }


// template<class base_cluster_type >
// int k_cluster<FULL, base_cluster_type >::get_k_0_index()
// {
//   //static int index = find(get_elements().begin(), get_elements().end(), std::vector<double>(DIMENSION, 0.))-get_elements().begin();

//   static int index = cluster_operations::origin_index(get_elements());
//   assert(VECTOR_OPERATIONS::L2_NORM(get_elements()[index])<1.e-6);

//   return index;
// }

template<class base_cluster_type >
int k_cluster<FULL, base_cluster_type >::origin_index()
{
  static int index = cluster_operations::origin_index(get_elements());
  assert(VECTOR_OPERATIONS::L2_NORM(get_elements()[index])<1.e-6);

  return index;
}

template<class base_cluster_type >
double k_cluster<FULL, base_cluster_type >::get_volume()
{
  assert(base_cluster_type::get_k_volume()>-1.e-16);
  return base_cluster_type::get_k_volume();
}

/*
template<class base_cluster_type >
double k_cluster<FULL, base_cluster_type >::find_minimal_distance(std::vector<double> vec_0,
										 std::vector<double> vec_1)
{
  vec_0 = back_inside_cluster(vec_0);
  vec_1 = back_inside_cluster(vec_1);

  double MIN_DISTANCE = VECTOR_OPERATIONS::L2_NORM(vec_0, vec_1);
  
  switch(DIMENSION)
    {
    case 2:
      
      for(int l0=-2; l0<=2; l0++){
	for(int l1=-2; l1<=2; l1++){
	  
	  std::vector<double> vec = vec_0;
	  
	  for(int d=0; d<DIMENSION; d++)
	    vec[d] += (l0*get_basis()[0][d] + l1*get_basis()[1][d]); 
	  
	  double distance = VECTOR_OPERATIONS::L2_NORM(vec, vec_1);
	  
	  if(distance < MIN_DISTANCE)
	    MIN_DISTANCE = distance;
	}
      }
      break;
      
    case 3:
      
      for(int l0=-2; l0<=2; l0++){
	for(int l1=-2; l1<=2; l1++){
	  for(int l2=-2; l2<=2; l2++){
	    
	    std::vector<double> vec = vec_0;
	    
	    for(int d=0; d<DIMENSION; d++)
	      vec[d] += (l0*get_basis()[0][d] + l1*get_basis()[1][d] + l2*get_basis()[2][d]); 
	    
	    double distance = VECTOR_OPERATIONS::L2_NORM(vec, vec_1);
	    
	    if(distance < MIN_DISTANCE)
	      MIN_DISTANCE = distance;
	  }
	}
      }
      break;
      
    default:
      throw std::logic_error(__FUNCTION__);
    }

    return MIN_DISTANCE;
}
*/

/*
template<class base_cluster_type >
std::pair<bool, std::vector<std::vector<double> > > k_cluster<FULL, base_cluster_type >::find_equivalent_vectors_with_minimal_distance_to_origin(std::vector<double> K_vec)
{
  K_vec = back_inside_cluster(K_vec);

  std::vector<double> origin(DIMENSION,0.);
  double MIN_DISTANCE = find_minimal_distance(origin, K_vec);

  bool IS_MINIMAL = VECTOR_OPERATIONS::L2_NORM(K_vec)>(MIN_DISTANCE+1.e-6)? false : true;

  std::vector<std::vector<double> > k_min;
 
  switch(DIMENSION)
    {
    case 2:

      for(int l0=-2; l0<=2; l0++){
	for(int l1=-2; l1<=2; l1++){
	    	    
	  std::vector<double> vec = K_vec;

	  for(int d=0; d<DIMENSION; d++)
	    vec[d] += (l0*get_basis()[0][d] + l1*get_basis()[1][d]); 

	  double distance = VECTOR_OPERATIONS::L2_NORM(vec);

	  if(distance > MIN_DISTANCE-1.e-6 && distance < MIN_DISTANCE+1.e-6)
	    k_min.push_back(vec);
	}
      }
	      
      break;
	
    case 3:
	
      for(int l0=-2; l0<=2; l0++){
	for(int l1=-2; l1<=2; l1++){
	  for(int l2=-2; l2<=2; l2++){
	    	    
	    std::vector<double> vec = K_vec;
	      
	    for(int d=0; d<DIMENSION; d++)
	      vec[d] += (l0*get_basis()[0][d] + l1*get_basis()[1][d] + l2*get_basis()[2][d]); 

	    double distance = VECTOR_OPERATIONS::L2_NORM(vec);

	    if(distance > MIN_DISTANCE-1.e-6 && distance < MIN_DISTANCE+1.e-6)
	      k_min.push_back(vec);
	  }
	}
      }
      break;
	
    default:
      throw std::logic_error(__FUNCTION__);
    }

  sort(k_min.begin(), k_min.end(), &VECTOR_OPERATIONS::SAME_VECTOR);
  int vec_size = unique(k_min.begin(), k_min.end(), &VECTOR_OPERATIONS::SAME_VECTOR)-k_min.begin();

  k_min.resize(vec_size);

  return std::pair<bool,  std::vector<std::vector<double> > >(IS_MINIMAL, k_min);
}
*/

template<class base_cluster_type >
int k_cluster<FULL, base_cluster_type >::get_size()
{
  switch(FULL)
    {
//     case IRREDUCIBLE:
//       return base_cluster_type::get_irreducible_k_cluster().size();
    case FULL:
      return base_cluster_type::get_cluster_size();
    default:
      cout << "ERROR in " << __PRETTY_FUNCTION__ << endl;
    }

  return -1;
}

template<class base_cluster_type >
std::vector<std::vector<double> >& k_cluster<FULL, base_cluster_type >::get_elements()
{
  switch(FULL)
    {
//     case IRREDUCIBLE:
//       return base_cluster_type::get_irreducible_k_cluster();
    case FULL:
      return base_cluster_type::get_k_cluster();
    default:
      {
	throw std::logic_error(__FUNCTION__);
	return base_cluster_type::get_k_cluster();
      }
    }
}

// template<class base_cluster_type >
// std::vector<std::vector<double> >& k_cluster<FULL, base_cluster_type >::get_sorted_elements()
// {
//   static std::vector<std::vector<double> > sorted_elements = initialize_sorted_elements();
//   return sorted_elements;
// }

template<class base_cluster_type >
int k_cluster<FULL, base_cluster_type >::add(int k_i, int k_j)
{
//   if(IS_EQUAL_TYPE<PsiMag_symmetry_2D, typename base_cluster_type::point_group_type>::check)
//     {
//       throw std::logic_error(__FUNCTION__);
//     }
//   else
  {
    static int              cluster_size = base_cluster_type::get_cluster_size();
    static std::vector<int> diff_indices = initialize_addition_matrix();
    
    assert(k_i >= 0 && k_i < cluster_size && k_j >= 0 && k_j < cluster_size);
    return diff_indices[k_i + cluster_size*k_j];
  }
}

// template<class base_cluster_type >
// std::vector<std::vector<double> > k_cluster<FULL, base_cluster_type >::initialize_sorted_elements()
// {
//   std::vector<std::vector<double> > sorted_elements = get_elements();

//   for(size_t l=0; l<sorted_elements.size(); l++)
//     sorted_elements[l].push_back(l);

//   sort(sorted_elements.begin(), sorted_elements.end(), VECTOR_OPERATIONS::IS_LARGER_VECTOR);

//   return sorted_elements;
// }

template<class base_cluster_type >
int k_cluster<FULL, base_cluster_type >::subtract(int k_i, int k_j)
{
  if(IS_EQUAL_TYPE<PsiMag_symmetry_2D, typename base_cluster_type::point_group_type>::check)
    {
      throw std::logic_error(__FUNCTION__);
    }
  else
    {
      static int              cluster_size = base_cluster_type::get_cluster_size();
      static std::vector<int> diff_indices = initialize_subtract_matrix();
    
      assert(k_i >= 0 && k_i < cluster_size && k_j >= 0 && k_j < cluster_size);
      return diff_indices[k_i + cluster_size*k_j];
    }
}

template<class base_cluster_type >
std::vector<int> k_cluster<FULL, base_cluster_type >::initialize_subtract_matrix()
{
  //assert(test_back_inside_cluster());

  int cluster_size = base_cluster_type::get_cluster_size();
  std::vector<int> diff_indices(cluster_size*cluster_size);

  for(int i=0; i<cluster_size; i++)
    {
      for(int j=0; j<cluster_size; j++)
	{
	  std::vector<double> k_j_minus_k_i = base_cluster_type::get_k_cluster()[j];

	  for(int l=0; l<DIMENSION; l++)
	    k_j_minus_k_i[l] -= base_cluster_type::get_k_cluster()[i][l];

	  //k_j_minus_k_i = back_inside_cluster(k_j_minus_k_i);
	  k_j_minus_k_i = cluster_operations::translate_inside_cluster(k_j_minus_k_i, get_super_basis_vectors());
	  


	  /*
	  int index = -1;
	  for(int l=0; l<cluster_size; l++)
	    if(VECTOR_OPERATIONS::L2_NORM(k_j_minus_k_i, base_cluster_type::get_k_cluster()[l]) < 1.e-6)
	      index = l;
	  
	  assert(index >= 0 && index < cluster_size);
	  */
	  
	  //int index = get_K_index(k_j_minus_k_i);
	  int index = cluster_operations::index(k_j_minus_k_i, get_elements());

	  diff_indices[i + j*cluster_size] = index;//get_K_index(k_j_minus_k_i);
	}
    }

  return diff_indices;
}

template<class base_cluster_type >
std::vector<int> k_cluster<FULL, base_cluster_type >::initialize_addition_matrix()
{
  //assert(test_back_inside_cluster());

  int cluster_size = base_cluster_type::get_cluster_size();
  std::vector<int> diff_indices(cluster_size*cluster_size);

  for(int i=0; i<cluster_size; i++)
    {
      for(int j=0; j<cluster_size; j++)
	{
	  std::vector<double> k_j_plus_k_i = base_cluster_type::get_k_cluster()[j];

	  for(int l=0; l<DIMENSION; l++)
	    k_j_plus_k_i[l] += base_cluster_type::get_k_cluster()[i][l];

	  //k_j_plus_k_i = back_inside_cluster(k_j_plus_k_i);
	  k_j_plus_k_i = cluster_operations::translate_inside_cluster(k_j_plus_k_i, get_super_basis_vectors());

	  /*
	  int index = -1;
	  for(int l=0; l<cluster_size; l++)
	    if(VECTOR_OPERATIONS::L2_NORM(k_j_plus_k_i, base_cluster_type::get_k_cluster()[l]) < 1.e-6)
	      index = l;
	  
	  assert(index >= 0 && index < cluster_size);
	  */

	  //int index = get_K_index(k_j_plus_k_i);
	  int index = cluster_operations::index(k_j_plus_k_i, get_elements());

	  diff_indices[i + j*cluster_size] = index;
	}
    }

  return diff_indices;
}

/*
template<class base_cluster_type>
int k_cluster<FULL, base_cluster_type >::get_K_index(std::vector<double> k)
{

//   k.push_back(-1);

//   std::vector<std::vector<double> >& v = get_sorted_elements();

//   int index = int(lower_bound(v.begin(), v.end(), k, VECTOR_OPERATIONS::IS_LARGER_VECTOR) - v.begin());

//   index = v[index][DIMENSION];
//   assert(index>-1 and index<get_size());

//  k.pop_back();


  int index = cluster_operations::index(k, get_elements());

  {
    double diff = VECTOR_OPERATIONS::L2_NORM(k, get_elements()[index]);
    if(diff>1.e-6) {
      VECTOR_OPERATIONS::PRINT(k);
      VECTOR_OPERATIONS::PRINT(get_elements()[index]);
      throw std::logic_error(__FUNCTION__);
    }
  }

  return index;
}
*/

 /*
template<class base_cluster_type>
bool k_cluster<FULL, base_cluster_type >::is_high_symmetry_point(int k_ind)
{
  bool has_high_symmetry = false;

  int nb = 0;
  for(int O_ind=0; O_ind<sym_super_cell_dmn_t::dmn_size(); O_ind++)
    if(k_ind == get_symmetry_matrix()(k_ind, 0, O_ind).first)
      nb += 1;

  // since we always have the identity operation, it has to be larger than 1 !
  if(nb > 1)
    has_high_symmetry = true;

  return has_high_symmetry;
}
 */

/*
template<class base_cluster_type>
inline std::vector<double> k_cluster<FULL, base_cluster_type >::back_inside_cluster(std::vector<double>& k)
{
  std::vector<double> k_cluster(DIMENSION,0);

  k_cluster = cluster_operations::translate_inside_cluster(k, get_basis());

//   switch(DIMENSION)
//     {
//     case 1:
//       k_cluster = back_inside_cluster_1D(k);
//       break;
//     case 2:
//       k_cluster = back_inside_cluster_2D(k);
//       break;
//     case 3:
//       k_cluster = back_inside_cluster_3D(k);
//       break;
//     default:
//       throw std::logic_error(__FUNCTION__);
//     }

  return k_cluster;
}
*/

/*
template<class base_cluster_type>
inline std::vector<double> k_cluster<FULL, base_cluster_type >::back_inside_cluster_1D(std::vector<double>& k)
{
  std::vector<double> k_cluster(DIMENSION,0);
  throw std::logic_error(__FUNCTION__);
  return k_cluster;
}

template<class base_cluster_type>
inline std::vector<double> k_cluster<FULL, base_cluster_type >::back_inside_cluster_2D(std::vector<double>& k)
{
  std::vector<double> k_affine = get_affine_coordinate(k);

  for(int d=0; d<DIMENSION; ++d){
    while(k_affine[d]<-1.e-6)
      k_affine[d] += 1.;
    
    while(k_affine[d]>1-1.e-6)
      k_affine[d] -= 1.;
  }

  std::vector<double> k_cluster(DIMENSION,0);

  for(int d=0; d<DIMENSION; ++d)
    k_cluster[d] = (base_cluster_type::get_k_basis()[0][d]*k_affine[0] 
		    + base_cluster_type::get_k_basis()[1][d]*k_affine[1]);

  return k_cluster;
}

template<class base_cluster_type>
inline std::vector<double> k_cluster<FULL, base_cluster_type >::back_inside_cluster_3D(std::vector<double>& k)
{
  std::vector<double> k_affine = get_affine_coordinate(k);

  for(int d=0; d<DIMENSION; ++d){
    while(k_affine[d]<-1.e-6)
      k_affine[d] += 1.;
    
    while(k_affine[d]>1-1.e-6)
      k_affine[d] -= 1.;
  }

  std::vector<double> k_cluster(DIMENSION,0);

  for(int d=0; d<DIMENSION; ++d)
    k_cluster[d] = (base_cluster_type::get_k_basis()[0][d]*k_affine[0] 
		    + base_cluster_type::get_k_basis()[1][d]*k_affine[1]
		    + base_cluster_type::get_k_basis()[2][d]*k_affine[2]);

  return k_cluster;
}
*/

/*
template<class base_cluster_type >
bool k_cluster<FULL, base_cluster_type >::test_back_inside_cluster()
{
  static bool passed_test = false;

  if(!passed_test)
    {
      std::vector<double> k1, k2;

      for(size_t l=0; l<base_cluster_type::get_k_cluster().size(); l++)
	{
	  k1 = base_cluster_type::get_k_cluster()[l];
	  k2 = back_inside_cluster(k1);

	  if(VECTOR_OPERATIONS::L2_NORM(k1,k2) > 1.e-6)
	    throw std::logic_error(__FUNCTION__);
	}
	
      passed_test = true;
    }

  return passed_test;
}
*/

 /*
template<class base_cluster_type >
inline std::vector<double> k_cluster<FULL, base_cluster_type >::get_affine_coordinate(std::vector<double>& k)
{
  std::vector<double> coordinates(DIMENSION,0);

  coordinates = VECTOR_OPERATIONS::COORDINATES(k, get_basis());

//   switch(DIMENSION)
//     {
//     case 1:
//       coordinates = get_affine_coordinate_1D(k);
//       break;

//     case 2:
//       coordinates = get_affine_coordinate_2D(k);
//       break;

//     case 3:
//       coordinates = get_affine_coordinate_3D(k);
//       break;

//     default:
//       throw std::logic_error(__FUNCTION__);
//     }

  return coordinates;
}
*/

/*
template<class base_cluster_type >
inline std::vector<double> k_cluster<FULL, base_cluster_type >::get_affine_coordinate_1D(std::vector<double>& k)
{
  const static double one_over_two_pi = 1./(2.*M_PI);
 
  const static std::vector<std::vector<double> >& r_basis = base_cluster_type::get_r_basis();

  std::vector<double> k_affine(1,0);

  k_affine[0] = r_basis[0][0]*k[0];

  k_affine[0] *= one_over_two_pi;

  return k_affine;
}

template<class base_cluster_type >
inline std::vector<double> k_cluster<FULL, base_cluster_type >::get_affine_coordinate_2D(std::vector<double>& k)
{
  const static double one_over_two_pi = 1./(2.*M_PI);
 
  const static std::vector<std::vector<double> >& r_basis = base_cluster_type::get_r_basis();

  std::vector<double> k_affine(2,0);

  k_affine[0] = r_basis[0][0]*k[0]+r_basis[0][1]*k[1];
  k_affine[1] = r_basis[1][0]*k[0]+r_basis[1][1]*k[1];

  k_affine[0] *= one_over_two_pi;
  k_affine[1] *= one_over_two_pi;

  return k_affine;
}

template<class base_cluster_type >
inline std::vector<double> k_cluster<FULL, base_cluster_type >::get_affine_coordinate_3D(std::vector<double>& k)
{
  assert(k.size()==3);

  const static double one_over_two_pi = 1./(2.*M_PI);
 
  const static std::vector<std::vector<double> >& r_basis = base_cluster_type::get_r_basis();
  assert(r_basis.size()==3);
  assert(r_basis[0].size()==3); assert(r_basis[1].size()==3); assert(r_basis[2].size()==3);


  std::vector<double> k_affine(3,0);

  k_affine[0] = r_basis[0][0]*k[0]+r_basis[0][1]*k[1]+r_basis[0][2]*k[2];
  k_affine[1] = r_basis[1][0]*k[0]+r_basis[1][1]*k[1]+r_basis[1][2]*k[2];
  k_affine[2] = r_basis[2][0]*k[0]+r_basis[2][1]*k[1]+r_basis[2][2]*k[2];

  k_affine[0] *= one_over_two_pi;
  k_affine[1] *= one_over_two_pi;
  k_affine[2] *= one_over_two_pi;

  return k_affine;
}
*/



template<class base_cluster_type >
bool k_cluster<FULL, base_cluster_type >::test_subtract_matrix()
{
  return true;
//   static bool passed_test = false;

//   if(!passed_test)
//     {
//       cout << __FUNCTION__ << endl;

//       std::vector<int> diff_indices = initialize_subtract_matrix();
//       int cluster_size = base_cluster_type::get_k_cluster().size();

//       for(size_t i=0; i<base_cluster_type::get_k_cluster().size(); i++)
// 	{
// 	  for(size_t j=0; j<base_cluster_type::get_k_cluster().size(); j++)
// 	    {
// // 	      if(diff_indices[i + cluster_size*j] != get_PsiMag_crystal().subtract(j,i))
// // 		throw std::logic_error(__FUNCTION__);
// 	    }
// 	}
	
//       passed_test = true;
//     }

//   return passed_test;
}

template<class base_cluster_type >
bool k_cluster<FULL, base_cluster_type >::test_add_matrix()
{
  return true;
//   static bool passed_test = false;

//   if(!passed_test)
//     {
//       cout << __FUNCTION__ << endl;

//       std::vector<int> diff_indices = initialize_addition_matrix();
//       int cluster_size = base_cluster_type::get_k_cluster().size();

//       for(size_t i=0; i<base_cluster_type::get_k_cluster().size(); i++)
// 	{
// 	  for(size_t j=0; j<base_cluster_type::get_k_cluster().size(); j++)
// 	    {
// // 	      if(diff_indices[i + cluster_size*j] != get_PsiMag_crystal().add(j,i))
// // 		throw std::logic_error(__FUNCTION__);
// 	    }
// 	}
	
//       passed_test = true;
//     }

//   return passed_test;
}


template<class base_cluster_type >
template<class stream_type>
void k_cluster<FULL, base_cluster_type >::to_JSON(stream_type& ss)
{
  ss << "\"K-CLUSTER-OPERATIONS\" : \n {";
  
  {
    ss << "\"subtract_ij = k_j-k_i\" : [\n";
    for(int i=0; i<get_size(); i++){
      ss << "[";
      for(int j=0; j<get_size(); j++){
	ss << subtract(i,j);
	j == get_size()-1 ? ss << "]": ss << ", ";
      }
      i == get_size()-1 ? ss << "\n" : ss << ",\n"; 
    }
    ss << "],\n";
    
    ss << "\"add_ij = k_j+k_i\" : [\n";
    for(int i=0; i<get_size(); i++){
      ss << "[";
      for(int j=0; j<get_size(); j++){
	ss << add(i,j);
	j == get_size()-1 ? ss << "]": ss << ", ";
      }
      i == get_size()-1 ? ss << "\n" : ss << ",\n"; 
    }
    
    ss << "]\n";
  }
  
  ss << "}";
}

#endif






















/*
template<class base_cluster_type >
int k_cluster<FULL, base_cluster_type >::get_weight(int full_cluster_index)
{
  return full_cluster_index;

//   if(IS_EQUAL_TYPE<PsiMag_symmetry_2D, typename base_cluster_type::point_group_type>::check)
//     {
//       throw std::logic_error(__FUNCTION__);
//       //return get_PsiMag_crystal().siteDegree[full_cluster_index];
//     }
//   else
//     {
//       int irr_i = get_cluster_reduction().get_irreducible_index(full_cluster_index);
//       return get_cluster_reduction().get_weight(irr_i);
//     }
}

template<cluster_FULL FULL, class base_cluster_type >
int k_cluster<representation_type, base_cluster_type >::convert_k_to_k_irr(int k)
{
  return k;
//   if(IS_EQUAL_TYPE<PsiMag_symmetry_2D, typename base_cluster_type::point_group_type>::check)
//     {
//       throw std::logic_error(__FUNCTION__);
//       //return get_PsiMag_crystal().wedge.crystal2wedge[k];
//     }
//   else
//     return get_cluster_reduction().get_irreducible_index(k); 
}

template<cluster_representation_type representation_type, class base_cluster_type >
void k_cluster<representation_type, base_cluster_type >::convert_k1_k2_to_k1_irr_k2(int& k1, int& k2)
{
//   if(IS_EQUAL_TYPE<PsiMag_symmetry_2D, typename base_cluster_type::point_group_type>::check)
//     {
//       throw std::logic_error(__FUNCTION__);
// //       k2 = get_PsiMag_crystal().wedge.map2wedge_data(k1,k2);
// //       k1 = get_PsiMag_crystal().wedge.crystal2wedge[k1];
//     }
//     else
//     {
//       int i = k1;
//       int j = k2;
      
//       k1 = get_cluster_reduction().get_irreducible_pair(i,j).first;
//       k2 = get_cluster_reduction().get_irreducible_pair(i,j).second;
//     }
}
*/
