// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "NanometerUnit.pypp.hpp"

namespace bp = boost::python;

void register_NanometerUnit_class(){

    bp::class_< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, boost::noncopyable >( "NanometerUnit", "\n Unit represents a unit of measure, such as gram, nanometer, or second.\n" )    
        .def( 
            "get_instance"
            , (::moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > > const & (*)(  ))( &::moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >::get_instance )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "write_name"
            , (void (*)( ::std::basic_ostream< char, std::char_traits< char > > & ))( &::moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >::write_name )
            , ( bp::arg("os") ) )    
        .def( 
            "write_symbol"
            , (void (*)( ::std::basic_ostream< char, std::char_traits< char > > & ))( &::moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >::write_symbol )
            , ( bp::arg("os") ) )    
        .staticmethod( "get_instance" )    
        .staticmethod( "write_name" )    
        .staticmethod( "write_symbol" );

}