// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrap_moltk.hpp"
#include "rotation3d.pypp.hpp"

namespace bp = boost::python;

void register_Rotation3D_class(){

    { //::moltk::Rotation3D
        typedef bp::class_< moltk::Rotation3D > Rotation3D_exposer_t;
        Rotation3D_exposer_t Rotation3D_exposer = Rotation3D_exposer_t( "Rotation3D", "\n Rotation3D represents a rotation matrix.\n", bp::init< >() );
        bp::scope Rotation3D_scope( Rotation3D_exposer );
        bp::class_< moltk::Rotation3D::Row >( "Row", "\n One row of three numbers in a Rotation3D rotation matrix.\n" )    
            .def( 
                "__getitem__"
                , (::moltk::Real const & ( ::moltk::Rotation3D::Row::* )( ::size_t ) const)( &::moltk::Rotation3D::Row::operator[] )
                , ( bp::arg("ix") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        Rotation3D_exposer.def( bp::init< moltk::units::Angle const &, moltk::UnitVector3D const & >(( bp::arg("angle"), bp::arg("unit_vector") )) );
        Rotation3D_exposer.def( bp::init< moltk::Quaternion const & >(( bp::arg("quaternion") )) );
        bp::implicitly_convertible< moltk::Quaternion const &, moltk::Rotation3D >();
        Rotation3D_exposer.def( bp::self * bp::self );
        { //::moltk::Rotation3D::operator[]
        
            typedef ::moltk::Rotation3D::Row const & ( ::moltk::Rotation3D::*__getitem___function_type )( ::size_t ) const;
            
            Rotation3D_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::moltk::Rotation3D::operator[] )
                , ( bp::arg("ix") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::moltk::Rotation3D::set_from_angle_about_unit_vector
        
            typedef ::moltk::Rotation3D & ( ::moltk::Rotation3D::*set_from_angle_about_unit_vector_function_type )( ::moltk::units::Angle const &,::moltk::UnitVector3D const & ) ;
            
            Rotation3D_exposer.def( 
                "set_from_angle_about_unit_vector"
                , set_from_angle_about_unit_vector_function_type( &::moltk::Rotation3D::set_from_angle_about_unit_vector )
                , ( bp::arg("angle"), bp::arg("unit_vector") )
                , bp::return_self< >() );
        
        }
        { //::moltk::Rotation3D::set_from_quaternion
        
            typedef ::moltk::Rotation3D & ( ::moltk::Rotation3D::*set_from_quaternion_function_type )( ::moltk::Quaternion const & ) ;
            
            Rotation3D_exposer.def( 
                "set_from_quaternion"
                , set_from_quaternion_function_type( &::moltk::Rotation3D::set_from_quaternion )
                , ( bp::arg("quaternion") )
                , bp::return_self< >() );
        
        }
    }

}
