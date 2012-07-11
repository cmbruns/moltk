// This file has been generated by Py++.

#include "boost/python.hpp"
#include "indexing_suite/container_suite.hpp"
#include "indexing_suite/vector.hpp"
#include "wrap_moltk.hpp"
#include "TableType.pypp.hpp"

namespace bp = boost::python;

void register_TableType_class(){

    { //::std::vector< std::vector< moltk::dp::DPCell<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPAlignGapping)0, 1> > >
        typedef bp::class_< std::vector< std::vector< moltk::dp::DPCell<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPAlignGapping)0, 1> > > > TableType_exposer_t;
        TableType_exposer_t TableType_exposer = TableType_exposer_t( "TableType", "\n  A standard container which offers fixed time access to\n  individual elements in any order.\n\n  fngroup Containers\n  fngroup Sequences\n\n  Meets the requirements of a <a href=tables.html#65>container</a>, a\n  <a href=tables.html#66>reversible container</a>, and a\n  <a href=tables.html#67>sequence</a>, including the\n  <a href=tables.html#68>optional sequence requirements</a> with the\n  %exception of @c push_front and @c pop_front.\n\n  In some terminology a %vector can be described as a dynamic\n  C-style array, it offers fast and efficient access to individual\n  elements in any order and saves the user from worrying about\n  memory and size allocation.  Subscripting ( @c [] ) access is\n  also provided as with C-style arrays.\n" );
        bp::scope TableType_scope( TableType_exposer );
        TableType_exposer.def( bp::indexing::vector_suite< std::vector< std::vector< moltk::dp::DPCell<moltk::units::Quantity<moltk::units::Unit<moltk::units::Dimension<0, 0, 0, 0, 0, 0, 1> >, double>, (moltk::dp::DPAlignGapping)0, 1> > > >() );
    }

}