// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "DPTableILU1.pypp.hpp"

namespace bp = boost::python;

void register_DPTableILU1_class(){

    bp::class_< moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 > >( "DPTableILU1", "\n Generic dynamic programming table (not implemented yet...)\n" )    
        .def( 
            "align"
            , (::moltk::dp::AlignmentResult< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > ( ::moltk::dp::DPTable<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1>::* )(  ) )( &::moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 >::align ) )    
        .def( 
            "align"
            , (::moltk::Alignment_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > ( ::moltk::dp::DPTable<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1>::* )( ::moltk::Alignment_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > const &,::moltk::Alignment_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > const &,::moltk::MatrixScorer_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, 1 > const & ) )( &::moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 >::align )
            , ( bp::arg("s1"), bp::arg("s2"), bp::arg("scorer") ) )    
        .def( 
            "allocate"
            , (void ( ::moltk::dp::DPTable<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1>::* )(  ) )( &::moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 >::allocate ) )    
        .def( 
            "clear_positions"
            , (void ( ::moltk::dp::DPTable<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1>::* )(  ) )( &::moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 >::clear_positions ) )    
        .def( 
            "compute_recurrence"
            , (void ( ::moltk::dp::DPTable<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1>::* )(  ) )( &::moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 >::compute_recurrence ) )    
        .def( 
            "compute_traceback"
            , (::moltk::dp::AlignmentResult< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > ( ::moltk::dp::DPTable<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1>::* )(  ) )( &::moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 >::compute_traceback ) )    
        .def( 
            "initialize"
            , (void ( ::moltk::dp::DPTable<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1>::* )(  ) )( &::moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 >::initialize ) )    
        .def( 
            "num_columns"
            , (::size_t ( ::moltk::dp::DPTable<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1>::* )(  ) const)( &::moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 >::num_columns ) )    
        .def( 
            "num_rows"
            , (::size_t ( ::moltk::dp::DPTable<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1>::* )(  ) const)( &::moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 >::num_rows ) )    
        .def_readwrite( "query_positions", &moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 >::query_positions )    
        .def_readwrite( "table", &moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 >::table )    
        .def_readwrite( "target_positions", &moltk::dp::DPTable< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double >, (moltk::dp::DPMemoryModel)0, (moltk::dp::DPAlignGapping)0, 1 >::target_positions );

}
