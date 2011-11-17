// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "SubstitutionMatrix.pypp.hpp"

namespace bp = boost::python;

void register_SubstitutionMatrix_class(){

    { //::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >
        typedef bp::class_< moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > > SubstitutionMatrix_exposer_t;
        SubstitutionMatrix_exposer_t SubstitutionMatrix_exposer = SubstitutionMatrix_exposer_t( "SubstitutionMatrix", "\n SubstitutionMatrix scores alignments using a residue type\n matrix such as BLOSUM62 or PAM250.\n", bp::init< std::string const &, bp::optional< double > >(( bp::arg("matrix_string"), bp::arg("bits_per_entry")=1.0e+0 )) );
        bp::scope SubstitutionMatrix_scope( SubstitutionMatrix_exposer );
        bp::implicitly_convertible< std::string const &, moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > >();
        SubstitutionMatrix_exposer.def( bp::init< std::basic_istream< char, std::char_traits< char > > &, bp::optional< double > >(( bp::arg("matrix_stream"), bp::arg("bits_per_entry")=1.0e+0 )) );
        bp::implicitly_convertible< std::basic_istream< char, std::char_traits< char > > &, moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > >();
        { //::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >::get_blosum62
        
            typedef moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > exported_class_t;
            typedef ::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > const & ( *get_blosum62_function_type )(  );
            
            SubstitutionMatrix_exposer.def( 
                "get_blosum62"
                , get_blosum62_function_type( &::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >::get_blosum62 )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >::get_character_indices
        
            typedef moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > exported_class_t;
            typedef ::std::vector< int > const & ( exported_class_t::*get_character_indices_function_type )(  ) const;
            
            SubstitutionMatrix_exposer.def( 
                "get_character_indices"
                , get_character_indices_function_type( &::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >::get_character_indices )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >::get_pam250
        
            typedef moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > exported_class_t;
            typedef ::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > const & ( *get_pam250_function_type )(  );
            
            SubstitutionMatrix_exposer.def( 
                "get_pam250"
                , get_pam250_function_type( &::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >::get_pam250 )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >::operator[]
        
            typedef moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > exported_class_t;
            typedef ::std::vector< moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double> > const & ( exported_class_t::*__getitem___function_type )( ::size_t ) const;
            
            SubstitutionMatrix_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >::operator[] )
                , ( bp::arg("ix") )
                , bp::return_value_policy< bp::copy_const_reference >()
                , "\n Get matrix row by integer index\n" );
        
        }
        { //::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >::operator[]
        
            typedef moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > exported_class_t;
            typedef ::std::vector< moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double> > const & ( exported_class_t::*__getitem___function_type )( char ) const;
            
            SubstitutionMatrix_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >::operator[] )
                , ( bp::arg("c") )
                , bp::return_value_policy< bp::copy_const_reference >()
                , "\n Get matrix row by integer index\n" );
        
        }
        { //::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >::size
        
            typedef moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > > exported_class_t;
            typedef ::size_t ( exported_class_t::*size_function_type )(  ) const;
            
            SubstitutionMatrix_exposer.def( 
                "size"
                , size_function_type( &::moltk::SubstitutionMatrix_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 0, 0, 0, 0, 0, 1 > >, double > >::size )
                , "\n Number of rows in substitution matrix\n" );
        
        }
        SubstitutionMatrix_exposer.staticmethod( "get_blosum62" );
        SubstitutionMatrix_exposer.staticmethod( "get_pam250" );
    }

}
