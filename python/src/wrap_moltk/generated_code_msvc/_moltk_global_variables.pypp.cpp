// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.h"
#include "_moltk_global_variables.pypp.hpp"

namespace bp = boost::python;

void register_global_variables(){

    bp::scope().attr("bit") = bp::object( boost::ref( moltk::units::bit ) );

    bp::scope().attr("nanometer") = bp::object( boost::ref( moltk::units::nanometer ) );

}
