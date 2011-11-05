// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "_moltk_free_functions.pypp.hpp"

namespace bp = boost::python;

void register_free_functions(){

    { //::moltk::align
    
        typedef ::moltk::Alignment ( *align_function_type )( ::moltk::Alignment const &,::moltk::Alignment const & );
        
        bp::def( 
            "align"
            , align_function_type( &::moltk::align )
            , ( bp::arg("target_alignment"), bp::arg("query_alignment") ) );
    
    }

    { //::moltk::units::cos
    
        typedef ::moltk::Real ( *cos_function_type )( ::moltk::units::Angle const & );
        
        bp::def( 
            "cos"
            , cos_function_type( &::moltk::units::cos )
            , ( bp::arg("angle") ) );
    
    }

    { //::moltk::units::get_unit_name
    
        typedef ::std::string ( *get_unit_name_function_type )( ::moltk::units::RadianUnit const & );
        
        bp::def( 
            "get_unit_name"
            , get_unit_name_function_type( &::moltk::units::get_unit_name )
            , ( bp::arg("arg0") ) );
    
    }

    { //::moltk::units::get_unit_name
    
        typedef ::std::string ( *get_unit_name_function_type )( ::moltk::units::NanometerUnit const & );
        
        bp::def( 
            "get_unit_name"
            , get_unit_name_function_type( &::moltk::units::get_unit_name )
            , ( bp::arg("arg0") ) );
    
    }

    { //::moltk::units::get_unit_name
    
        typedef ::std::string ( *get_unit_name_function_type )( ::moltk::units::BitUnit const & );
        
        bp::def( 
            "get_unit_name"
            , get_unit_name_function_type( &::moltk::units::get_unit_name )
            , ( bp::arg("arg0") ) );
    
    }

    { //::moltk::units::get_unit_symbol
    
        typedef ::std::string ( *get_unit_symbol_function_type )( ::moltk::units::RadianUnit const & );
        
        bp::def( 
            "get_unit_symbol"
            , get_unit_symbol_function_type( &::moltk::units::get_unit_symbol )
            , ( bp::arg("arg0") ) );
    
    }

    { //::moltk::units::get_unit_symbol
    
        typedef ::std::string ( *get_unit_symbol_function_type )( ::moltk::units::NanometerUnit const & );
        
        bp::def( 
            "get_unit_symbol"
            , get_unit_symbol_function_type( &::moltk::units::get_unit_symbol )
            , ( bp::arg("arg0") ) );
    
    }

    { //::moltk::units::get_unit_symbol
    
        typedef ::std::string ( *get_unit_symbol_function_type )( ::moltk::units::BitUnit const & );
        
        bp::def( 
            "get_unit_symbol"
            , get_unit_symbol_function_type( &::moltk::units::get_unit_symbol )
            , ( bp::arg("arg0") ) );
    
    }

    { //::moltk::load_fasta
    
        typedef ::moltk::Alignment ( *load_fasta_function_type )( ::std::string const & );
        
        bp::def( 
            "load_fasta"
            , load_fasta_function_type( &::moltk::load_fasta )
            , ( bp::arg("file_name") )
            , " global load_fasta method helps get SEQUOIA-like conciseness in python." );
    
    }

    { //::moltk::units::sin
    
        typedef ::moltk::Real ( *sin_function_type )( ::moltk::units::Angle const & );
        
        bp::def( 
            "sin"
            , sin_function_type( &::moltk::units::sin )
            , ( bp::arg("angle") ) );
    
    }

}
