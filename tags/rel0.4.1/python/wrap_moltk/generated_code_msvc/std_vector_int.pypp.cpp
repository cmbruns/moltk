// This file has been generated by Py++.

#include "boost/python.hpp"
#include "indexing_suite/container_suite.hpp"
#include "indexing_suite/vector.hpp"
#include "wrap_moltk.hpp"
#include "std_vector_int.pypp.hpp"

namespace bp = boost::python;

void register_std_vector_int_class(){

    { //::std::vector< int >
        typedef bp::class_< std::vector< int > > std_vector_int_exposer_t;
        std_vector_int_exposer_t std_vector_int_exposer = std_vector_int_exposer_t( "std_vector_int" );
        bp::scope std_vector_int_scope( std_vector_int_exposer );
        std_vector_int_exposer.def( bp::indexing::vector_suite< std::vector< int > >() );
    }

}
