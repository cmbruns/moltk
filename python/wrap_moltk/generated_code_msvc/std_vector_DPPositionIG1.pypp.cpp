// This file has been generated by Py++.

#include "boost/python.hpp"
#include "indexing_suite/container_suite.hpp"
#include "indexing_suite/vector.hpp"
#include "wrap_moltk.hpp"
#include "std_vector_dppositionig1.pypp.hpp"

namespace bp = boost::python;

void register_std_vector_DPPositionIG1_class(){

    { //::std::vector< moltk::dp::DPPosition<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPAlignGapping)1, 1>* >
        typedef bp::class_< std::vector< moltk::dp::DPPosition<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPAlignGapping)1, 1>* > > std_vector_DPPositionIG1_exposer_t;
        std_vector_DPPositionIG1_exposer_t std_vector_DPPositionIG1_exposer = std_vector_DPPositionIG1_exposer_t( "std_vector_DPPositionIG1" );
        bp::scope std_vector_DPPositionIG1_scope( std_vector_DPPositionIG1_exposer );
        std_vector_DPPositionIG1_exposer.def( bp::indexing::vector_suite< std::vector< moltk::dp::DPPosition<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPAlignGapping)1, 1>* > >::with_policies(bp::return_internal_reference< >()) );
    }

}
