// This file has been generated by Py++.

#include "boost/python.hpp"
#include "indexing_suite/container_suite.hpp"
#include "indexing_suite/vector.hpp"
#include "wrap_moltk.hpp"
#include "std_vector_Information.pypp.hpp"

namespace bp = boost::python;

void register_std_vector_Information_class(){

    bp::class_< std::vector< moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double> > >("std_vector_Information")    
        .def( bp::indexing::vector_suite< std::vector< moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double> > >() );

}
