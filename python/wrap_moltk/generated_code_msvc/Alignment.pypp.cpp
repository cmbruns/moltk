// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "alignment.pypp.hpp"

namespace bp = boost::python;

void register_Alignment_class(){

    { //::moltk::Alignment
        typedef bp::class_< moltk::Alignment > Alignment_exposer_t;
        Alignment_exposer_t Alignment_exposer = Alignment_exposer_t( "Alignment", "\n Alignment represents a set of aligned macromolecule sequences and/or structures.\n", bp::init< >() );
        bp::scope Alignment_scope( Alignment_exposer );
        bp::enum_< moltk::Alignment::List>("List")
            .value("LIST_SEQUENCE", moltk::Alignment::LIST_SEQUENCE)
            .value("LIST_STRUCTURE", moltk::Alignment::LIST_STRUCTURE)
            .export_values()
            ;
        bp::class_< moltk::Alignment::Row >( "Row", "\n Meta-data for one sequence in an Alignment\n" )    
            .def_readwrite( "e_string", &moltk::Alignment::Row::e_string )    
            .def_readwrite( "list", &moltk::Alignment::Row::list, "\n Meta-data for one sequence in an Alignment\n" )    
            .def_readwrite( "list_index", &moltk::Alignment::Row::list_index )    
            .def_readwrite( "sequence_weight", &moltk::Alignment::Row::sequence_weight );
        Alignment_exposer.def( bp::init< moltk::Biosequence const & >(( bp::arg("sequence") )) );
        bp::implicitly_convertible< moltk::Biosequence const &, moltk::Alignment >();
        Alignment_exposer.def( bp::init< std::string const & >(( bp::arg("alignment_string") )) );
        bp::implicitly_convertible< std::string const &, moltk::Alignment >();
        Alignment_exposer.def( bp::init< char const * >(( bp::arg("alignment_string") )) );
        bp::implicitly_convertible< char const *, moltk::Alignment >();
        { //::moltk::Alignment::align
        
            typedef ::moltk::Alignment ( ::moltk::Alignment::*align_function_type )( ::moltk::Alignment const &,::moltk::EString const &,::moltk::EString const & ) const;
            
            Alignment_exposer.def( 
                "align"
                , align_function_type( &::moltk::Alignment::align )
                , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("arg2") ) );
        
        }
        { //::moltk::Alignment::append_sequence
        
            typedef ::moltk::Alignment & ( ::moltk::Alignment::*append_sequence_function_type )( ::moltk::Biosequence const & ) ;
            
            Alignment_exposer.def( 
                "append_sequence"
                , append_sequence_function_type( &::moltk::Alignment::append_sequence )
                , ( bp::arg("sequence") )
                , bp::return_self< >() );
        
        }
        { //::moltk::Alignment::fasta
        
            typedef ::std::string ( ::moltk::Alignment::*fasta_function_type )(  ) const;
            
            Alignment_exposer.def( 
                "fasta"
                , fasta_function_type( &::moltk::Alignment::fasta ) );
        
        }
        { //::moltk::Alignment::get_estring
        
            typedef ::moltk::EString const & ( ::moltk::Alignment::*get_estring_function_type )( ::size_t ) const;
            
            Alignment_exposer.def( 
                "get_estring"
                , get_estring_function_type( &::moltk::Alignment::get_estring )
                , ( bp::arg("index") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::moltk::Alignment::get_number_of_columns
        
            typedef ::size_t ( ::moltk::Alignment::*get_number_of_columns_function_type )(  ) const;
            
            Alignment_exposer.def( 
                "get_number_of_columns"
                , get_number_of_columns_function_type( &::moltk::Alignment::get_number_of_columns ) );
        
        }
        { //::moltk::Alignment::get_number_of_sequences
        
            typedef ::size_t ( ::moltk::Alignment::*get_number_of_sequences_function_type )(  ) const;
            
            Alignment_exposer.def( 
                "get_number_of_sequences"
                , get_number_of_sequences_function_type( &::moltk::Alignment::get_number_of_sequences ) );
        
        }
        { //::moltk::Alignment::get_sequence
        
            typedef ::moltk::BaseBiosequence const & ( ::moltk::Alignment::*get_sequence_function_type )( ::size_t ) const;
            
            Alignment_exposer.def( 
                "get_sequence"
                , get_sequence_function_type( &::moltk::Alignment::get_sequence )
                , ( bp::arg("index") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::moltk::Alignment::load_fasta
        
            typedef ::moltk::Alignment & ( ::moltk::Alignment::*load_fasta_function_type )( ::std::istream & ) ;
            
            Alignment_exposer.def( 
                "load_fasta"
                , load_fasta_function_type( &::moltk::Alignment::load_fasta )
                , ( bp::arg("input_stream") )
                , bp::return_self< >() );
        
        }
        { //::moltk::Alignment::load_fasta
        
            typedef ::moltk::Alignment & ( ::moltk::Alignment::*load_fasta_function_type )( ::std::string const & ) ;
            
            Alignment_exposer.def( 
                "load_fasta"
                , load_fasta_function_type( &::moltk::Alignment::load_fasta )
                , ( bp::arg("file_name") )
                , bp::return_self< >() );
        
        }
        { //::moltk::Alignment::load_string
        
            typedef void ( ::moltk::Alignment::*load_string_function_type )( ::std::string const & ) ;
            
            Alignment_exposer.def( 
                "load_string"
                , load_string_function_type( &::moltk::Alignment::load_string )
                , ( bp::arg("alignment_string") ) );
        
        }
        { //::moltk::Alignment::pretty
        
            typedef ::std::string ( ::moltk::Alignment::*pretty_function_type )(  ) const;
            
            Alignment_exposer.def( 
                "pretty"
                , pretty_function_type( &::moltk::Alignment::pretty ) );
        
        }
        { //::moltk::Alignment::score
        
            typedef ::moltk::units::Information const & ( ::moltk::Alignment::*score_function_type )(  ) const;
            
            Alignment_exposer.def( 
                "score"
                , score_function_type( &::moltk::Alignment::score )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::moltk::Alignment::set_score
        
            typedef ::moltk::Alignment & ( ::moltk::Alignment::*set_score_function_type )( ::moltk::units::Information const & ) ;
            
            Alignment_exposer.def( 
                "set_score"
                , set_score_function_type( &::moltk::Alignment::set_score )
                , ( bp::arg("s") )
                , bp::return_self< >() );
        
        }
        { //::moltk::Alignment::write_fasta
        
            typedef void ( ::moltk::Alignment::*write_fasta_function_type )( ::std::ostream & ) const;
            
            Alignment_exposer.def( 
                "write_fasta"
                , write_fasta_function_type( &::moltk::Alignment::write_fasta )
                , ( bp::arg("output_stream") ) );
        
        }
        { //::moltk::Alignment::write_fasta
        
            typedef void ( ::moltk::Alignment::*write_fasta_function_type )( ::std::string const & ) const;
            
            Alignment_exposer.def( 
                "write_fasta"
                , write_fasta_function_type( &::moltk::Alignment::write_fasta )
                , ( bp::arg("file_name") ) );
        
        }
        { //::moltk::Alignment::write_pretty
        
            typedef void ( ::moltk::Alignment::*write_pretty_function_type )( ::std::ostream & ) const;
            
            Alignment_exposer.def( 
                "write_pretty"
                , write_pretty_function_type( &::moltk::Alignment::write_pretty )
                , ( bp::arg("output_stream") ) );
        
        }
        { //::moltk::Alignment::write_pretty
        
            typedef void ( ::moltk::Alignment::*write_pretty_function_type )( ::std::string const & ) const;
            
            Alignment_exposer.def( 
                "write_pretty"
                , write_pretty_function_type( &::moltk::Alignment::write_pretty )
                , ( bp::arg("file_name") ) );
        
        }
        Alignment_exposer.def( bp::self_ns::str( bp::self ) );
    }

}
