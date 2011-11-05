// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "length.pypp.hpp"

namespace bp = boost::python;

void register_Length_class(){

    { //::moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, double >
        typedef bp::class_< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, double > > Length_exposer_t;
        Length_exposer_t Length_exposer = Length_exposer_t( "Length", "\n Quantity represents a physical value with a unit, such as 5.6 nanometers\n\n For efficiency, Quantity<> should compile to a double in C++.\n        This means:\n            no virtual methods\n            no data members other than value\n            unit is a class with a typedef\n        These restrictions need not apply to unit class, which should do\n        whatever it needs to, to be wrapped conveniently in python.\n", bp::init< >() );
        bp::scope Length_scope( Length_exposer );
        Length_exposer.def( bp::init< double const &, moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > > const & >(( bp::arg("v"), bp::arg("u") )) );
        Length_exposer.def( bp::init< moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, double > const & >(( bp::arg("rhs") )) );
        Length_exposer.def( bp::self != bp::self );
        Length_exposer.def( bp::self * bp::other< moltk::Real >() );
        Length_exposer.def( bp::self *= bp::other< moltk::Real >() );
        Length_exposer.def( +bp::self );
        Length_exposer.def( +bp::self );
        Length_exposer.def( bp::self + bp::self );
        Length_exposer.def( bp::self += bp::self );
        Length_exposer.def( -bp::self );
        Length_exposer.def( bp::self - bp::self );
        Length_exposer.def( bp::self -= bp::self );
        Length_exposer.def( bp::self / bp::other< moltk::Real >() );
        Length_exposer.def( bp::self / bp::other< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > > >() );
        Length_exposer.def( bp::self / bp::self );
        Length_exposer.def( bp::self /= bp::other< moltk::Real >() );
        Length_exposer.def( bp::self < bp::self );
        Length_exposer.def( bp::self <= bp::self );
        { //::moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, double >::operator=
        
            typedef moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, double > exported_class_t;
            typedef ::moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, double > & ( exported_class_t::*assign_function_type )( ::moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, double > const & ) ;
            
            Length_exposer.def( 
                "assign"
                , assign_function_type( &::moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, double >::operator= )
                , ( bp::arg("rhs") )
                , bp::return_self< >() );
        
        }
        Length_exposer.def( bp::self == bp::self );
        Length_exposer.def( bp::self > bp::self );
        Length_exposer.def( bp::self >= bp::self );
        { //::moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, double >::value_in_unit
        
            typedef moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, double > exported_class_t;
            typedef double ( exported_class_t::*value_in_unit_function_type )( ::moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > > const & ) const;
            
            Length_exposer.def( 
                "value_in_unit"
                , value_in_unit_function_type( &::moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, double >::value_in_unit )
                , ( bp::arg("arg0") )
                , "\n Proper way to extract value attribute - tell me the unit first.\n TODO - unit conversion\n" );
        
        }
        Length_exposer.def_readwrite( "value", &moltk::units::Quantity< moltk::units::Unit< moltk::units::Dimension< 0, 1, 0, 0, 0, 0, 0 > >, double >::value );
        Length_exposer.def( bp::other< moltk::Real >() * bp::self );
        Length_exposer.def( bp::self_ns::str( bp::self ) );
    }

}
