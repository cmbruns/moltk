// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.h"
#include "bit_t.pypp.hpp"

namespace bp = boost::python;

void register_bit_t_class(){

    bp::class_< moltk::units::unit< moltk::units::information_dimension >, boost::noncopyable >( "bit_t", "\n", bp::no_init )    
        .def( 
            "getInstance"
            , (::moltk::units::unit< moltk::units::information_dimension > const & (*)(  ))( &::moltk::units::unit< moltk::units::information_dimension >::getInstance )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "print_name"
            , (void (*)( ::std::basic_ostream< char, std::char_traits< char > > & ))( &::moltk::units::unit< moltk::units::information_dimension >::print_name )
            , ( bp::arg("os") ) )    
        .def( 
            "print_symbol"
            , (void (*)( ::std::basic_ostream< char, std::char_traits< char > > & ))( &::moltk::units::unit< moltk::units::information_dimension >::print_symbol )
            , ( bp::arg("os") ) )    
        .staticmethod( "getInstance" )    
        .staticmethod( "print_name" )    
        .staticmethod( "print_symbol" );

}
