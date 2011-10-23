// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "biosequence.pypp.hpp"

namespace bp = boost::python;

struct Biosequence_wrapper : moltk::Biosequence, bp::wrapper< moltk::Biosequence > {

    Biosequence_wrapper( )
    : moltk::Biosequence( )
      , bp::wrapper< moltk::Biosequence >(){
        // null constructor
    
    }

    Biosequence_wrapper(::std::string const & sequence, ::std::string const & description="" )
    : moltk::Biosequence( sequence, description )
      , bp::wrapper< moltk::Biosequence >(){
        // constructor
    
    }

    Biosequence_wrapper(char const * sequence, ::std::string const & description="" )
    : moltk::Biosequence( sequence, description )
      , bp::wrapper< moltk::Biosequence >(){
        // constructor
    
    }

    Biosequence_wrapper(::moltk::Biosequence const & rhs )
    : moltk::Biosequence( boost::ref(rhs) )
      , bp::wrapper< moltk::Biosequence >(){
        // copy constructor
    
    }

    virtual ::size_t get_number_of_residues(  ) const  {
        if( bp::override func_get_number_of_residues = this->get_override( "get_number_of_residues" ) )
            return func_get_number_of_residues(  );
        else{
            return this->moltk::Biosequence::get_number_of_residues(  );
        }
    }
    
    ::size_t default_get_number_of_residues(  ) const  {
        return moltk::Biosequence::get_number_of_residues( );
    }

    virtual void print_to_stream( ::std::ostream & os ) const  {
        if( bp::override func_print_to_stream = this->get_override( "print_to_stream" ) )
            func_print_to_stream( boost::ref(os) );
        else{
            this->moltk::BaseBiosequence::print_to_stream( boost::ref(os) );
        }
    }
    
    void default_print_to_stream( ::std::ostream & os ) const  {
        moltk::BaseBiosequence::print_to_stream( boost::ref(os) );
    }

};

void register_Biosequence_class(){

    { //::moltk::Biosequence
        typedef bp::class_< Biosequence_wrapper, bp::bases< moltk::BaseBiosequence > > Biosequence_exposer_t;
        Biosequence_exposer_t Biosequence_exposer = Biosequence_exposer_t( "Biosequence", "\n A macromolecule sequence (DNA or RNA or protein)\n", bp::init< >() );
        bp::scope Biosequence_scope( Biosequence_exposer );
        Biosequence_exposer.def( bp::init< std::string const &, bp::optional< std::string const & > >(( bp::arg("sequence"), bp::arg("description")="" )) );
        bp::implicitly_convertible< std::string const &, moltk::Biosequence >();
        Biosequence_exposer.def( bp::init< char const *, bp::optional< std::string const & > >(( bp::arg("sequence"), bp::arg("description")="" )) );
        bp::implicitly_convertible< char const *, moltk::Biosequence >();
        Biosequence_exposer.def( bp::init< moltk::Biosequence const & >(( bp::arg("rhs") )) );
        { //::moltk::Biosequence::get_description
        
            typedef ::std::string ( ::moltk::Biosequence::*get_description_function_type )(  ) const;
            
            Biosequence_exposer.def( 
                "get_description"
                , get_description_function_type( &::moltk::Biosequence::get_description ) );
        
        }
        { //::moltk::Biosequence::get_number_of_residues
        
            typedef ::size_t ( ::moltk::Biosequence::*get_number_of_residues_function_type )(  ) const;
            typedef ::size_t ( Biosequence_wrapper::*default_get_number_of_residues_function_type )(  ) const;
            
            Biosequence_exposer.def( 
                "get_number_of_residues"
                , get_number_of_residues_function_type(&::moltk::Biosequence::get_number_of_residues)
                , default_get_number_of_residues_function_type(&Biosequence_wrapper::default_get_number_of_residues) );
        
        }
        { //::moltk::Biosequence::get_residue
        
            typedef ::moltk::BiosequenceResidue const & ( ::moltk::Biosequence::*get_residue_function_type )( ::size_t ) const;
            
            Biosequence_exposer.def( 
                "get_residue"
                , get_residue_function_type(&::moltk::Biosequence::get_residue)
                , ( bp::arg("ix") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::moltk::Biosequence::load_fasta
        
            typedef ::moltk::Biosequence & ( ::moltk::Biosequence::*load_fasta_function_type )( ::std::string const & ) ;
            
            Biosequence_exposer.def( 
                "load_fasta"
                , load_fasta_function_type( &::moltk::Biosequence::load_fasta )
                , ( bp::arg("file_name") )
                , bp::return_self< >() );
        
        }
        { //::moltk::Biosequence::load_fasta
        
            typedef ::moltk::Biosequence & ( ::moltk::Biosequence::*load_fasta_function_type )( ::std::istream & ) ;
            
            Biosequence_exposer.def( 
                "load_fasta"
                , load_fasta_function_type( &::moltk::Biosequence::load_fasta )
                , ( bp::arg("is") )
                , bp::return_self< >() );
        
        }
        { //::moltk::Biosequence::load_stream
        
            typedef void ( ::moltk::Biosequence::*load_stream_function_type )( ::std::istream & ) ;
            
            Biosequence_exposer.def( 
                "load_stream"
                , load_stream_function_type( &::moltk::Biosequence::load_stream )
                , ( bp::arg("is") ) );
        
        }
        { //::moltk::Biosequence::print_string
        
            typedef void ( ::moltk::Biosequence::*print_string_function_type )( ::std::ostream & ) const;
            
            Biosequence_exposer.def( 
                "print_string"
                , print_string_function_type( &::moltk::Biosequence::print_string )
                , ( bp::arg("os") ) );
        
        }
        { //::moltk::Biosequence::repr
        
            typedef ::std::string ( ::moltk::Biosequence::*repr_function_type )(  ) const;
            
            Biosequence_exposer.def( 
                "repr"
                , repr_function_type( &::moltk::Biosequence::repr )
                , " repr is a helper for the python __repr__ method." );
        
        }
        { //::moltk::BaseBiosequence::print_to_stream
        
            typedef void ( ::moltk::BaseBiosequence::*print_to_stream_function_type )( ::std::ostream & ) const;
            typedef void ( Biosequence_wrapper::*default_print_to_stream_function_type )( ::std::ostream & ) const;
            
            Biosequence_exposer.def( 
                "print_to_stream"
                , print_to_stream_function_type(&::moltk::BaseBiosequence::print_to_stream)
                , default_print_to_stream_function_type(&Biosequence_wrapper::default_print_to_stream)
                , ( bp::arg("os") ) );
        
        }
        Biosequence_exposer.def( bp::self_ns::str( bp::self ) );
        Biosequence_exposer.def("__repr__", &::moltk::Biosequence::repr, "string representation");
    }

}
