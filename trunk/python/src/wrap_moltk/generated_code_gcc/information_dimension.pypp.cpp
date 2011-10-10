// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.h"
#include "information_dimension.pypp.hpp"

namespace bp = boost::python;

void register_information_dimension_class(){

    bp::class_< moltk::units::information_dimension, bp::bases< moltk::units::dimension > >( "information_dimension" );

}
