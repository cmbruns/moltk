// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.h"
#include "fastasequence.pypp.hpp"

namespace bp = boost::python;

struct FastaSequence_wrapper : moltk::FastaSequence, bp::wrapper< moltk::FastaSequence > {

    FastaSequence_wrapper(::std::string const & s )
    : moltk::FastaSequence( s )
      , bp::wrapper< moltk::FastaSequence >(){
        // constructor
    
    }

    virtual ::size_t getNumberOfResidues(  ) const  {
        if( bp::override func_getNumberOfResidues = this->get_override( "getNumberOfResidues" ) )
            return func_getNumberOfResidues(  );
        else{
            return this->moltk::Biosequence::getNumberOfResidues(  );
        }
    }
    
    ::size_t default_getNumberOfResidues(  ) const  {
        return moltk::Biosequence::getNumberOfResidues( );
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

void register_FastaSequence_class(){

    { //::moltk::FastaSequence
        typedef bp::class_< FastaSequence_wrapper, bp::bases< moltk::Biosequence >, boost::noncopyable > FastaSequence_exposer_t;
        FastaSequence_exposer_t FastaSequence_exposer = FastaSequence_exposer_t( "FastaSequence", bp::init< std::string const & >(( bp::arg("s") )) );
        bp::scope FastaSequence_scope( FastaSequence_exposer );
        bp::implicitly_convertible< std::string const &, moltk::FastaSequence >();
        { //::moltk::Biosequence::getNumberOfResidues
        
            typedef ::size_t ( ::moltk::Biosequence::*getNumberOfResidues_function_type )(  ) const;
            typedef ::size_t ( FastaSequence_wrapper::*default_getNumberOfResidues_function_type )(  ) const;
            
            FastaSequence_exposer.def( 
                "getNumberOfResidues"
                , getNumberOfResidues_function_type(&::moltk::Biosequence::getNumberOfResidues)
                , default_getNumberOfResidues_function_type(&FastaSequence_wrapper::default_getNumberOfResidues) );
        
        }
        { //::moltk::Biosequence::getResidue
        
            typedef ::moltk::BaseBiosequence::BaseResidue const & ( ::moltk::Biosequence::*getResidue_function_type )( ::size_t ) const;
            
            FastaSequence_exposer.def( 
                "getResidue"
                , getResidue_function_type(&::moltk::Biosequence::getResidue)
                , ( bp::arg("index") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::moltk::BaseBiosequence::print_to_stream
        
            typedef void ( ::moltk::BaseBiosequence::*print_to_stream_function_type )( ::std::ostream & ) const;
            typedef void ( FastaSequence_wrapper::*default_print_to_stream_function_type )( ::std::ostream & ) const;
            
            FastaSequence_exposer.def( 
                "print_to_stream"
                , print_to_stream_function_type(&::moltk::BaseBiosequence::print_to_stream)
                , default_print_to_stream_function_type(&FastaSequence_wrapper::default_print_to_stream)
                , ( bp::arg("os") ) );
        
        }
    }

}
