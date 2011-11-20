// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "MatrixScorer.pypp.hpp"

namespace bp = boost::python;

void register_MatrixScorer_class(){

    { //::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >
        typedef bp::class_< moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 > > MatrixScorer_exposer_t;
        MatrixScorer_exposer_t MatrixScorer_exposer = MatrixScorer_exposer_t( "MatrixScorer", "\n MatrixScorer scores alignments using a residue type\n matrix such as BLOSUM62 or PAM250.\n", bp::init< moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > const & >(( bp::arg("matrix") )) );
        bp::scope MatrixScorer_scope( MatrixScorer_exposer );
        bp::implicitly_convertible< moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > const &, moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 > >();
        { //::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::create_positions
        
            typedef moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 > exported_class_t;
            typedef void ( exported_class_t::*create_positions_function_type )( ::std::vector< moltk::dp::DPPosition<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>* > &,::moltk::Alignment_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > const & ) const;
            
            MatrixScorer_exposer.def( 
                "create_positions"
                , create_positions_function_type( &::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::create_positions )
                , ( bp::arg("positions"), bp::arg("alignment") ) );
        
        }
        { //::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::get_default_gap_extension_penalty
        
            typedef moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 > exported_class_t;
            typedef ::moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > ( exported_class_t::*get_default_gap_extension_penalty_function_type )(  ) const;
            
            MatrixScorer_exposer.def( 
                "get_default_gap_extension_penalty"
                , get_default_gap_extension_penalty_function_type( &::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::get_default_gap_extension_penalty )
                , "\n Alignment score reduction for increasing the length of an alignment gap by one position.\n" );
        
        }
        { //::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::get_default_gap_open_penalty
        
            typedef moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 > exported_class_t;
            typedef ::moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > ( exported_class_t::*get_default_gap_open_penalty_function_type )(  ) const;
            
            MatrixScorer_exposer.def( 
                "get_default_gap_open_penalty"
                , get_default_gap_open_penalty_function_type( &::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::get_default_gap_open_penalty )
                , "\n Alignment score reduction for initiating an alignment gap.\n" );
        
        }
        { //::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::get_end_gaps_free
        
            typedef moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 > exported_class_t;
            typedef bool ( exported_class_t::*get_end_gaps_free_function_type )(  ) const;
            
            MatrixScorer_exposer.def( 
                "get_end_gaps_free"
                , get_end_gaps_free_function_type( &::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::get_end_gaps_free ) );
        
        }
        { //::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::set_default_gap_extension_penalty
        
            typedef moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 > exported_class_t;
            typedef void ( exported_class_t::*set_default_gap_extension_penalty_function_type )( ::moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > ) ;
            
            MatrixScorer_exposer.def( 
                "set_default_gap_extension_penalty"
                , set_default_gap_extension_penalty_function_type( &::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::set_default_gap_extension_penalty )
                , ( bp::arg("penalty") )
                , "\n Set Alignment score reduction for increasing the length of an alignment gap by one position.\n" );
        
        }
        { //::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::set_default_gap_open_penalty
        
            typedef moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 > exported_class_t;
            typedef void ( exported_class_t::*set_default_gap_open_penalty_function_type )( ::moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > ) ;
            
            MatrixScorer_exposer.def( 
                "set_default_gap_open_penalty"
                , set_default_gap_open_penalty_function_type( &::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::set_default_gap_open_penalty )
                , ( bp::arg("penalty") )
                , "\n Set alignment score reduction for initiating an alignment gap.\n" );
        
        }
        { //::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::set_end_gaps_free
        
            typedef moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 > exported_class_t;
            typedef void ( exported_class_t::*set_end_gaps_free_function_type )( bool ) ;
            
            MatrixScorer_exposer.def( 
                "set_end_gaps_free"
                , set_end_gaps_free_function_type( &::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 >::set_end_gaps_free )
                , ( bp::arg("f") ) );
        
        }
        { //property "end_gaps_free"[fget=::moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>::get_end_gaps_free, fset=::moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>::set_end_gaps_free]
        
            typedef moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1> exported_class_t;
            
            typedef bool ( exported_class_t::*fget )(  ) const;
            typedef void ( exported_class_t::*fset )( bool ) ;
            
            MatrixScorer_exposer.add_property( 
                "end_gaps_free"
                , fget( &::moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>::get_end_gaps_free )
                , fset( &::moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>::set_end_gaps_free ) );
        
        }
        { //property "default_gap_open_penalty"[fget=::moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>::get_default_gap_open_penalty, fset=::moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>::set_default_gap_open_penalty]
        
            typedef moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1> exported_class_t;
            
            typedef ::moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double> ( exported_class_t::*fget )(  ) const;
            typedef void ( exported_class_t::*fset )( ::moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double> ) ;
            
            MatrixScorer_exposer.add_property( 
                "default_gap_open_penalty"
                , fget( &::moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>::get_default_gap_open_penalty )
                , fset( &::moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>::set_default_gap_open_penalty ) );
        
        }
        { //property "default_gap_extension_penalty"[fget=::moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>::get_default_gap_extension_penalty, fset=::moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>::set_default_gap_extension_penalty]
        
            typedef moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1> exported_class_t;
            
            typedef ::moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double> ( exported_class_t::*fget )(  ) const;
            typedef void ( exported_class_t::*fset )( ::moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double> ) ;
            
            MatrixScorer_exposer.add_property( 
                "default_gap_extension_penalty"
                , fget( &::moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>::get_default_gap_extension_penalty )
                , fset( &::moltk::MatrixScorer_<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, 1>::set_default_gap_extension_penalty ) );
        
        }
    }

}