// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "bitunit.pypp.hpp"

namespace bp = boost::python;

void register_BitUnit_class(){

    bp::class_< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, boost::noncopyable >( "BitUnit", "\n Unit represents a unit of measure, such as gram, nanometer, or second.\n" )    
        .def( 
            "get_instance"
            , (::moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > > const & (*)(  ))( &::moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >::get_instance )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "write_name"
            , (void (*)( ::std::basic_ostream< char, std::char_traits< char > > & ))( &::moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >::write_name )
            , ( bp::arg("os") ) )    
        .def( 
            "write_symbol"
            , (void (*)( ::std::basic_ostream< char, std::char_traits< char > > & ))( &::moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >::write_symbol )
            , ( bp::arg("os") ) )    
        .staticmethod( "get_instance" )    
        .staticmethod( "write_name" )    
        .staticmethod( "write_symbol" );

}
