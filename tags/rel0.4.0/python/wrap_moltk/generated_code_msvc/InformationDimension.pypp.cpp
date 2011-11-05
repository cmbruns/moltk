// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "informationdimension.pypp.hpp"

namespace bp = boost::python;

void register_InformationDimension_class(){

    bp::class_< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 >, boost::noncopyable >( "InformationDimension", "\n Dimension represents a physical dimension such as mass, length, time, etc.\n" );

}