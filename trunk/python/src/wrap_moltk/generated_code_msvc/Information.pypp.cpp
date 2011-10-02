// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.h"
#include "information.pypp.hpp"

namespace bp = boost::python;

void register_Information_class(){

    { //::moltk::units::Information
        typedef bp::class_< moltk::units::Information > Information_exposer_t;
        Information_exposer_t Information_exposer = Information_exposer_t( "Information", bp::init< >() );
        bp::scope Information_scope( Information_exposer );
        Information_exposer.def( bp::init< double >(( bp::arg("d") )) );
        bp::implicitly_convertible< double, moltk::units::Information >();
        Information_exposer.def( bp::self * bp::other< double >() );
        Information_exposer.def( bp::self + bp::self );
        Information_exposer.def( bp::self - bp::self );
        Information_exposer.def( -bp::self );
        Information_exposer.def( bp::self < bp::self );
        Information_exposer.def( bp::self <= bp::self );
        Information_exposer.def( bp::self > bp::self );
        Information_exposer.def( bp::self >= bp::self );
        Information_exposer.def_readwrite( "value", &moltk::units::Information::value );
    }

}
