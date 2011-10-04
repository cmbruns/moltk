// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.h"
#include "Aligner.pypp.hpp"

namespace bp = boost::python;

struct Aligner_wrapper : moltk::Aligner, bp::wrapper< moltk::Aligner > {

    Aligner_wrapper(moltk::Aligner const & arg )
    : moltk::Aligner( arg )
      , bp::wrapper< moltk::Aligner >(){
        // copy constructor
        
    }

    struct Position_wrapper : moltk::Aligner::Position, bp::wrapper< moltk::Aligner::Position > {
    
        Position_wrapper()
        : moltk::Aligner::Position()
          , bp::wrapper< moltk::Aligner::Position >(){
            // null constructor
            
        }
    
        virtual ::moltk::Aligner::Position * clone(  ) const {
            bp::override func_clone = this->get_override( "clone" );
            return func_clone(  );
        }
    
        virtual char getOneLetterCode(  ) const {
            bp::override func_getOneLetterCode = this->get_override( "getOneLetterCode" );
            return func_getOneLetterCode(  );
        }
    
        virtual ::moltk::units::Information score( ::moltk::Aligner::Position const & rhs ) const {
            bp::override func_score = this->get_override( "score" );
            return func_score( boost::ref(rhs) );
        }
    
    };

    struct Scorer_wrapper : moltk::Aligner::Scorer, bp::wrapper< moltk::Aligner::Scorer > {
    
        Scorer_wrapper()
        : moltk::Aligner::Scorer()
          , bp::wrapper< moltk::Aligner::Scorer >(){
            // null constructor
            
        }
    
        virtual ::moltk::Aligner::Position * createPosition( char sequenceLetter ) const {
            bp::override func_createPosition = this->get_override( "createPosition" );
            return func_createPosition( sequenceLetter );
        }
    
    };

    Aligner_wrapper( )
    : moltk::Aligner( )
      , bp::wrapper< moltk::Aligner >(){
        // null constructor
    
    }

};

void register_Aligner_class(){

    { //::moltk::Aligner
        typedef bp::class_< Aligner_wrapper > Aligner_exposer_t;
        Aligner_exposer_t Aligner_exposer = Aligner_exposer_t( "Aligner", "\n Aligner is the class that creates sequence or structure alignments.\n", bp::init< >() );
        bp::scope Aligner_scope( Aligner_exposer );
        bp::enum_< moltk::Aligner::Stage>("Stage")
            .value("EMPTY_STAGE", moltk::Aligner::EMPTY_STAGE)
            .value("SCORER_STAGE", moltk::Aligner::SCORER_STAGE)
            .value("SEQUENCE_STAGE", moltk::Aligner::SEQUENCE_STAGE)
            .value("ALLOCATED_STAGE", moltk::Aligner::ALLOCATED_STAGE)
            .value("TABLE_INITIALIZED_STAGE", moltk::Aligner::TABLE_INITIALIZED_STAGE)
            .value("RECURRENCE_COMPUTED_STAGE", moltk::Aligner::RECURRENCE_COMPUTED_STAGE)
            .value("TRACED_STAGE", moltk::Aligner::TRACED_STAGE)
            .export_values()
            ;
        bp::enum_< moltk::Aligner::TracebackPointer>("TracebackPointer")
            .value("TRACEBACK_UP", moltk::Aligner::TRACEBACK_UP)
            .value("TRACEBACK_UPLEFT", moltk::Aligner::TRACEBACK_UPLEFT)
            .value("TRACEBACK_LEFT", moltk::Aligner::TRACEBACK_LEFT)
            .value("TRACEBACK_DONE", moltk::Aligner::TRACEBACK_DONE)
            .value("TRACEBACK_NOT_INITIALIZED", moltk::Aligner::TRACEBACK_NOT_INITIALIZED)
            .export_values()
            ;
        bp::class_< moltk::Aligner::Cell >( "Cell", " An Aligner::Cell is one node in the dynamic programming table" )    
            .def_readwrite( "e", &moltk::Aligner::Cell::e, " E, best score with gap in sequence 1" )    
            .def_readwrite( "f", &moltk::Aligner::Cell::f, " F, best score with gap in sequence 2" )    
            .def_readwrite( "g", &moltk::Aligner::Cell::g, " G, best ungapped score through this cell" )    
            .def_readwrite( "s", &moltk::Aligner::Cell::s, " Wm, score of aligning position S1(i) with S2(j)\n s is the score of aligning position S1(i) with S2(j); (Wm in Gusfields nomenclature.)" )    
            .def_readwrite( "tracebackPointer", &moltk::Aligner::Cell::tracebackPointer, " breadcrumb to help reconstruct the final alignment" )    
            .def_readwrite( "v", &moltk::Aligner::Cell::v, " V, best score through this cell" );
        bp::class_< Aligner_wrapper::Position_wrapper, boost::noncopyable >( "Position", "\n Aligner::Position represents a special biosequence residue that knows how to score itself during alignment.\n" )    
            .def( 
                "clone"
                , bp::pure_virtual( (::moltk::Aligner::Position * ( ::moltk::Aligner::Position::* )(  ) const)(&::moltk::Aligner::Position::clone) )
                , bp::return_value_policy< bp::manage_new_object >()
                , "\n Aligner::Position represents a special biosequence residue that knows how to score itself during alignment.\n" )    
            .def( 
                "getOneLetterCode"
                , bp::pure_virtual( (char ( ::moltk::Aligner::Position::* )(  ) const)(&::moltk::Aligner::Position::getOneLetterCode) ) )    
            .def( 
                "score"
                , bp::pure_virtual( (::moltk::units::Information ( ::moltk::Aligner::Position::* )( ::moltk::Aligner::Position const & ) const)(&::moltk::Aligner::Position::score) )
                , ( bp::arg("rhs") ) );
        { //::moltk::Aligner::PositionList
            typedef bp::class_< moltk::Aligner::PositionList > PositionList_exposer_t;
            PositionList_exposer_t PositionList_exposer = PositionList_exposer_t( "PositionList", " Aligner::PositionList is a special sequence containing Aligner::Positions,\n which know how to score themselves.", bp::init< >(" Aligner::PositionList is a special sequence containing Aligner::Positions,\n which know how to score themselves.") );
            bp::scope PositionList_scope( PositionList_exposer );
            PositionList_exposer.def( bp::init< moltk::Aligner::PositionList const & >(( bp::arg("rhs") ), " Aligner::PositionList is a special sequence containing Aligner::Positions,\n which know how to score themselves.") );
            { //::moltk::Aligner::PositionList::operator=
            
                typedef ::moltk::Aligner::PositionList & ( ::moltk::Aligner::PositionList::*assign_function_type )( ::moltk::Aligner::PositionList const & ) ;
                
                PositionList_exposer.def( 
                    "assign"
                    , assign_function_type( &::moltk::Aligner::PositionList::operator= )
                    , ( bp::arg("rhs") )
                    , bp::return_self< >() );
            
            }
        }
        bp::class_< Aligner_wrapper::Scorer_wrapper, boost::noncopyable >( "Scorer", "\n Aligner::Scorer converts dumb sequence and structure residues into Aligner::Positions, which\n know how to quickly score themselves with other AlignerPositions.\n" )    
            .def( 
                "createPosition"
                , bp::pure_virtual( (::moltk::Aligner::Position * ( ::moltk::Aligner::Scorer::* )( char ) const)(&::moltk::Aligner::Scorer::createPosition) )
                , ( bp::arg("sequenceLetter") )
                , bp::return_value_policy< bp::manage_new_object >()
                , "\n Aligner::Scorer converts dumb sequence and structure residues into Aligner::Positions, which\n know how to quickly score themselves with other AlignerPositions.\n" );
        { //::moltk::Aligner::align
        
            typedef ::moltk::Alignment ( ::moltk::Aligner::*align_function_type )( ::moltk::FastaSequence const &,::moltk::FastaSequence const & ) ;
            
            Aligner_exposer.def( 
                "align"
                , align_function_type( &::moltk::Aligner::align )
                , ( bp::arg("arg0"), bp::arg("arg1") ) );
        
        }
        { //::moltk::Aligner::getDefaultScorer
        
            typedef ::moltk::Aligner::Scorer const & ( *getDefaultScorer_function_type )(  );
            
            Aligner_exposer.def( 
                "getDefaultScorer"
                , getDefaultScorer_function_type( &::moltk::Aligner::getDefaultScorer )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::moltk::Aligner::getEndGapsFree
        
            typedef bool ( ::moltk::Aligner::*getEndGapsFree_function_type )(  ) const;
            
            Aligner_exposer.def( 
                "getEndGapsFree"
                , getEndGapsFree_function_type( &::moltk::Aligner::getEndGapsFree ) );
        
        }
        { //::moltk::Aligner::setEndGapsFree
        
            typedef void ( ::moltk::Aligner::*setEndGapsFree_function_type )( bool ) ;
            
            Aligner_exposer.def( 
                "setEndGapsFree"
                , setEndGapsFree_function_type( &::moltk::Aligner::setEndGapsFree )
                , ( bp::arg("f") ) );
        
        }
        Aligner_exposer.staticmethod( "getDefaultScorer" );
        { //property "endGapsFree"[fget=::moltk::Aligner::getEndGapsFree, fset=::moltk::Aligner::setEndGapsFree]
        
            typedef bool ( ::moltk::Aligner::*fget )(  ) const;
            typedef void ( ::moltk::Aligner::*fset )( bool ) ;
            
            Aligner_exposer.add_property( 
                "endGapsFree"
                , fget( &::moltk::Aligner::getEndGapsFree )
                , fset( &::moltk::Aligner::setEndGapsFree ) );
        
        }
    }

}
