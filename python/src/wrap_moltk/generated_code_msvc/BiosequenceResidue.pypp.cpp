// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.h"
#include "biosequenceresidue.pypp.hpp"

namespace bp = boost::python;

struct BiosequenceResidue_wrapper : moltk::BiosequenceResidue, bp::wrapper< moltk::BiosequenceResidue > {

    BiosequenceResidue_wrapper(char one_letter_code_param, int residue_number_param )
    : moltk::BiosequenceResidue( one_letter_code_param, residue_number_param )
      , bp::wrapper< moltk::BiosequenceResidue >(){
        // constructor
    
    }

    BiosequenceResidue_wrapper(::moltk::BiosequenceResidue const & rhs )
    : moltk::BiosequenceResidue( boost::ref(rhs) )
      , bp::wrapper< moltk::BiosequenceResidue >(){
        // copy constructor
    
    }

    virtual char get_one_letter_code(  ) const  {
        if( bp::override func_get_one_letter_code = this->get_override( "get_one_letter_code" ) )
            return func_get_one_letter_code(  );
        else{
            return this->moltk::BiosequenceResidue::get_one_letter_code(  );
        }
    }
    
    char default_get_one_letter_code(  ) const  {
        return moltk::BiosequenceResidue::get_one_letter_code( );
    }

    virtual int get_residue_number(  ) const  {
        if( bp::override func_get_residue_number = this->get_override( "get_residue_number" ) )
            return func_get_residue_number(  );
        else{
            return this->moltk::BiosequenceResidue::get_residue_number(  );
        }
    }
    
    int default_get_residue_number(  ) const  {
        return moltk::BiosequenceResidue::get_residue_number( );
    }

};

void register_BiosequenceResidue_class(){

    bp::class_< BiosequenceResidue_wrapper, bp::bases< moltk::BaseBiosequence::BaseResidue > >( "BiosequenceResidue", bp::init< char, int >(( bp::arg("one_letter_code_param"), bp::arg("residue_number_param") )) )    
        .def( bp::init< moltk::BiosequenceResidue const & >(( bp::arg("rhs") )) )    
        .def( 
            "get_one_letter_code"
            , (char ( ::moltk::BiosequenceResidue::* )(  ) const)(&::moltk::BiosequenceResidue::get_one_letter_code)
            , (char ( BiosequenceResidue_wrapper::* )(  ) const)(&BiosequenceResidue_wrapper::default_get_one_letter_code) )    
        .def( 
            "get_residue_number"
            , (int ( ::moltk::BiosequenceResidue::* )(  ) const)(&::moltk::BiosequenceResidue::get_residue_number)
            , (int ( BiosequenceResidue_wrapper::* )(  ) const)(&BiosequenceResidue_wrapper::default_get_residue_number) );

}