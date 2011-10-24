// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "EString.pypp.hpp"

namespace bp = boost::python;

void register_EString_class(){

    { //::moltk::EString
        typedef bp::class_< moltk::EString > EString_exposer_t;
        EString_exposer_t EString_exposer = EString_exposer_t( "EString", "\n A compact representation of the gapping pattern for one sequence in an alignment.\n", bp::init< >() );
        bp::scope EString_scope( EString_exposer );
        bp::class_< moltk::EString::const_iterator >( "const_iterator", "\n Iterator for accessing the residue positions encoded in an estring.\n\n EString::const_iterator emits one residue number for each\n column in an alignment.  Positions with a gap dereference to\n residue number -1.  Other positions dereference to the\n ordinal residue position index in the corresponding sequence.\n", bp::init< >("\n Iterator for accessing the residue positions encoded in an estring.\n\n EString::const_iterator emits one residue number for each\n column in an alignment.  Positions with a gap dereference to\n residue number -1.  Other positions dereference to the\n ordinal residue position index in the corresponding sequence.\n") )    
            .def( bp::init< std::vector< int > const &, int, int >(( bp::arg("runs"), bp::arg("run_index"), bp::arg("position_index") ), "\n Iterator for accessing the residue positions encoded in an estring.\n\n EString::const_iterator emits one residue number for each\n column in an alignment.  Positions with a gap dereference to\n residue number -1.  Other positions dereference to the\n ordinal residue position index in the corresponding sequence.\n") )    
            .def( bp::self != bp::self )    
            .def( bp::self == bp::self );
        { //::moltk::EString::append_run
        
            typedef ::moltk::EString & ( ::moltk::EString::*append_run_function_type )( int ) ;
            
            EString_exposer.def( 
                "append_run"
                , append_run_function_type( &::moltk::EString::append_run )
                , ( bp::arg("run") )
                , bp::return_self< >() );
        
        }
        { //::moltk::EString::begin
        
            typedef ::moltk::EString::const_iterator ( ::moltk::EString::*begin_function_type )(  ) const;
            
            EString_exposer.def( 
                "begin"
                , begin_function_type( &::moltk::EString::begin ) );
        
        }
        { //::moltk::EString::end
        
            typedef ::moltk::EString::const_iterator ( ::moltk::EString::*end_function_type )(  ) const;
            
            EString_exposer.def( 
                "end"
                , end_function_type( &::moltk::EString::end ) );
        
        }
        EString_exposer.def( bp::self != bp::self );
        EString_exposer.def( bp::self * bp::self );
        EString_exposer.def( bp::self == bp::self );
        { //::moltk::EString::reverse
        
            typedef void ( ::moltk::EString::*reverse_function_type )(  ) ;
            
            EString_exposer.def( 
                "reverse"
                , reverse_function_type( &::moltk::EString::reverse ) );
        
        }
        { //::moltk::EString::total_length
        
            typedef ::size_t ( ::moltk::EString::*total_length_function_type )(  ) const;
            
            EString_exposer.def( 
                "total_length"
                , total_length_function_type( &::moltk::EString::total_length ) );
        
        }
        { //::moltk::EString::ungapped_length
        
            typedef ::size_t ( ::moltk::EString::*ungapped_length_function_type )(  ) const;
            
            EString_exposer.def( 
                "ungapped_length"
                , ungapped_length_function_type( &::moltk::EString::ungapped_length ) );
        
        }
        EString_exposer.def( bp::self_ns::str( bp::self ) );
    }

}
