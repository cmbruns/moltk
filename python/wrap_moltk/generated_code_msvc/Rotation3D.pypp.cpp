// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "rotation3d.pypp.hpp"

namespace bp = boost::python;

void register_Rotation3D_class(){

    { //::moltk::Rotation3D
        typedef bp::class_< moltk::Rotation3D > Rotation3D_exposer_t;
        Rotation3D_exposer_t Rotation3D_exposer = Rotation3D_exposer_t( "Rotation3D" );
        bp::scope Rotation3D_scope( Rotation3D_exposer );
        bp::class_< moltk::Rotation3D::Row >( "Row" );
    }

}
