// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.h"
#include "_moltk_free_functions.pypp.hpp"

namespace bp = boost::python;

void register_free_functions(){

    { //::moltk::units::unit_name
    
        typedef ::std::string ( *unit_name_function_type )( ::moltk::units::nanometer_t const * );
        
        bp::def( 
            "unit_name"
            , unit_name_function_type( &::moltk::units::unit_name )
            , ( bp::arg("arg0") ) );
    
    }

    { //::moltk::units::unit_name
    
        typedef ::std::string ( *unit_name_function_type )( ::moltk::units::bit_t const * );
        
        bp::def( 
            "unit_name"
            , unit_name_function_type( &::moltk::units::unit_name )
            , ( bp::arg("arg0") ) );
    
    }

    { //::moltk::units::unit_symbol
    
        typedef ::std::string ( *unit_symbol_function_type )( ::moltk::units::nanometer_t const * );
        
        bp::def( 
            "unit_symbol"
            , unit_symbol_function_type( &::moltk::units::unit_symbol )
            , ( bp::arg("arg0") ) );
    
    }

    { //::moltk::units::unit_symbol
    
        typedef ::std::string ( *unit_symbol_function_type )( ::moltk::units::bit_t const * );
        
        bp::def( 
            "unit_symbol"
            , unit_symbol_function_type( &::moltk::units::unit_symbol )
            , ( bp::arg("arg0") ) );
    
    }

}
