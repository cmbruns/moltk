// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.h"
#include "Information.pypp.hpp"

namespace bp = boost::python;

void register_Information_class(){

    { //::moltk::units::quantity< moltk::units::unit< moltk::units::information_dimension >, double >
        typedef bp::class_< moltk::units::quantity< moltk::units::unit< moltk::units::information_dimension >, double > > Information_exposer_t;
        Information_exposer_t Information_exposer = Information_exposer_t( "Information", "\n For efficiency, quantity<> should compile to a double in C++.\n This means:\n     no virtual methods\n     no data members other than value\n     unit is a class with a typedef\n These restrictions need not apply to unit class, which should do\n whatever it needs to, to be wrapped conveniently in python.\n", bp::init< >() );
        bp::scope Information_scope( Information_exposer );
        Information_exposer.def( bp::init< double >(( bp::arg("v") )) );
        bp::implicitly_convertible< double, moltk::units::quantity< moltk::units::unit< moltk::units::information_dimension >, double > >();
        Information_exposer.def( bp::init< moltk::units::quantity< moltk::units::unit< moltk::units::information_dimension >, double > const & >(( bp::arg("rhs") )) );
        Information_exposer.def( bp::self * bp::other< moltk::Real >() );
        Information_exposer.def( bp::self + bp::self );
        Information_exposer.def( bp::self - bp::self );
        Information_exposer.def( -bp::self );
        Information_exposer.def( bp::self < bp::self );
        Information_exposer.def( bp::self <= bp::self );
        Information_exposer.def( bp::self > bp::self );
        Information_exposer.def( bp::self >= bp::self );
        { //::moltk::units::quantity< moltk::units::unit< moltk::units::information_dimension >, double >::print
        
            typedef moltk::units::quantity< moltk::units::unit< moltk::units::information_dimension >, double > exported_class_t;
            typedef void ( exported_class_t::*print_function_type )( ::std::basic_ostream< char, std::char_traits< char > > & ) const;
            
            Information_exposer.def( 
                "print"
                , print_function_type( &::moltk::units::quantity< moltk::units::unit< moltk::units::information_dimension >, double >::print )
                , ( bp::arg("os") ) );
        
        }
        Information_exposer.def_readwrite( "value", &moltk::units::quantity< moltk::units::unit< moltk::units::information_dimension >, double >::value );
    }

}