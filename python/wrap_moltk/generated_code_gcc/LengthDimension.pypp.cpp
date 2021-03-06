// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "LengthDimension.pypp.hpp"

namespace bp = boost::python;

void register_LengthDimension_class(){

    bp::class_< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 >, boost::noncopyable >( "LengthDimension", "\n Dimension represents a physical dimension such as mass, length, time, etc.\n" );

}
