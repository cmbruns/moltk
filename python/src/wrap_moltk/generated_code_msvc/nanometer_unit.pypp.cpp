// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.h"
#include "nanometer_unit.pypp.hpp"

namespace bp = boost::python;

void register_nanometer_unit_class(){

    bp::class_< moltk::units::nanometer_unit, bp::bases< moltk::units::unit< moltk::units::length_dimension > > >( "nanometer_unit" );

}
