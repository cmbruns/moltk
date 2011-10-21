// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "BaseBiosequence.pypp.hpp"

namespace bp = boost::python;

struct BaseBiosequence_wrapper : moltk::BaseBiosequence, bp::wrapper< moltk::BaseBiosequence > {

    struct BaseResidue_wrapper : moltk::BaseBiosequence::BaseResidue, bp::wrapper< moltk::BaseBiosequence::BaseResidue > {
    
        BaseResidue_wrapper( )
        : moltk::BaseBiosequence::BaseResidue( )
          , bp::wrapper< moltk::BaseBiosequence::BaseResidue >(){
            // null constructor
        
        }
    
        virtual char get_one_letter_code(  ) const {
            bp::override func_get_one_letter_code = this->get_override( "get_one_letter_code" );
            return func_get_one_letter_code(  );
        }
    
        virtual int get_residue_number(  ) const {
            bp::override func_get_residue_number = this->get_override( "get_residue_number" );
            return func_get_residue_number(  );
        }
    
    };

    BaseBiosequence_wrapper( )
    : moltk::BaseBiosequence( )
      , bp::wrapper< moltk::BaseBiosequence >(){
        // null constructor
    
    }

    virtual ::size_t get_number_of_residues(  ) const {
        bp::override func_get_number_of_residues = this->get_override( "get_number_of_residues" );
        return func_get_number_of_residues(  );
    }

    virtual ::moltk::BaseBiosequence::BaseResidue const & get_residue( ::size_t index ) const {
        throw std::logic_error("warning W1049: This method could not be overriden in Python - method returns reference to local variable!");
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

void register_BaseBiosequence_class(){

    { //::moltk::BaseBiosequence
        typedef bp::class_< BaseBiosequence_wrapper, boost::noncopyable > BaseBiosequence_exposer_t;
        BaseBiosequence_exposer_t BaseBiosequence_exposer = BaseBiosequence_exposer_t( "BaseBiosequence", "\n Parent class for macromolecule sequences and structures.\n", bp::init< >() );
        bp::scope BaseBiosequence_scope( BaseBiosequence_exposer );
        bp::class_< BaseBiosequence_wrapper::BaseResidue_wrapper, boost::noncopyable >( "BaseResidue", "\n Parent class for macromolecule sequence residues (nucleotides or amino acids)\n", bp::init< >("\n Parent class for macromolecule sequence residues (nucleotides or amino acids)\n") )    
            .def( 
                "get_one_letter_code"
                , bp::pure_virtual( (char ( ::moltk::BaseBiosequence::BaseResidue::* )(  ) const)(&::moltk::BaseBiosequence::BaseResidue::get_one_letter_code) )
                , "\n Parent class for macromolecule sequence residues (nucleotides or amino acids)\n" )    
            .def( 
                "get_residue_number"
                , bp::pure_virtual( (int ( ::moltk::BaseBiosequence::BaseResidue::* )(  ) const)(&::moltk::BaseBiosequence::BaseResidue::get_residue_number) ) )    
            .def( "__int__", &moltk::BaseBiosequence::BaseResidue::operator char  );
        { //::moltk::BaseBiosequence::get_number_of_residues
        
            typedef ::size_t ( ::moltk::BaseBiosequence::*get_number_of_residues_function_type )(  ) const;
            
            BaseBiosequence_exposer.def( 
                "get_number_of_residues"
                , bp::pure_virtual( get_number_of_residues_function_type(&::moltk::BaseBiosequence::get_number_of_residues) ) );
        
        }
        { //::moltk::BaseBiosequence::get_residue
        
            typedef ::moltk::BaseBiosequence::BaseResidue const & ( ::moltk::BaseBiosequence::*get_residue_function_type )( ::size_t ) const;
            
            BaseBiosequence_exposer.def( 
                "get_residue"
                , bp::pure_virtual( get_residue_function_type(&::moltk::BaseBiosequence::get_residue) )
                , ( bp::arg("index") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::moltk::BaseBiosequence::print_to_stream
        
            typedef void ( ::moltk::BaseBiosequence::*print_to_stream_function_type )( ::std::ostream & ) const;
            typedef void ( BaseBiosequence_wrapper::*default_print_to_stream_function_type )( ::std::ostream & ) const;
            
            BaseBiosequence_exposer.def( 
                "print_to_stream"
                , print_to_stream_function_type(&::moltk::BaseBiosequence::print_to_stream)
                , default_print_to_stream_function_type(&BaseBiosequence_wrapper::default_print_to_stream)
                , ( bp::arg("os") ) );
        
        }
    }

}
