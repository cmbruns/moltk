/*
    MolTK is a Python and C++ toolkit for protein sequence/structure alignment and visualization
    Copyright (C) 2011  Christopher M. Bruns

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    
    Commercial users should ask about our dual licensing model.
    For questions contact: cmbruns@rotatingpenguin.com
*/

#include "moltk/Quaternion.hpp"
#include "moltk/Rotation3D.hpp"

using namespace moltk;
using namespace moltk::units;

Rotation3D::Rotation3D()
{
    set_from_elements(1, 0, 0,
                      0, 1, 0,
                      0, 0, 1);
}

Rotation3D::Rotation3D(const Angle& angle, const UnitVector3D& unit_vector) 
{
    set_from_angle_about_unit_vector(angle, unit_vector);
}

/* explicit */
Rotation3D::Rotation3D(const Quaternion& quaternion)
{
    set_from_quaternion(quaternion);
}

Rotation3D& Rotation3D::set_from_angle_about_unit_vector(const Angle& angle, const UnitVector3D& unit_vector) 
{
    Quaternion q(angle, unit_vector);
    set_from_quaternion(q);
    return *this;
}

Rotation3D& Rotation3D::set_from_quaternion(const Quaternion& q) 
{
    const Real q00=q[0]*q[0], q11=q[1]*q[1], q22=q[2]*q[2], q33=q[3]*q[3];
    const Real q01=q[0]*q[1], q02=q[0]*q[2], q03=q[0]*q[3];
    const Real q12=q[1]*q[2], q13=q[1]*q[3], q23=q[2]*q[3];

    set_from_elements(q00+q11-q22-q33,   2*(q12-q03)  ,   2*(q13+q02),
                        2*(q12+q03)  , q00-q11+q22-q33,   2*(q23-q01),
                        2*(q13-q02)  ,   2*(q23+q01)  , q00-q11-q22+q33);

    return *this;
}

Rotation3D& Rotation3D::set_from_elements(
    Real e00, Real e01, Real e02,
    Real e10, Real e11, Real e12,
    Real e20, Real e21, Real e22) 
{
    Rotation3D& R = *this;
    R[0][0] = e00; R[0][1] = e01; R[0][2] = e02;
    R[1][0] = e10; R[1][1] = e11; R[1][2] = e12;
    R[2][0] = e20; R[2][1] = e21; R[2][2] = e22;
    return *this;
}

Rotation3D Rotation3D::operator*(const Rotation3D& rhs) const
{
    const Rotation3D& lhs = *this;
    Rotation3D result;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            result[i][j] = 0.0;
            for (int k = 0; k < 3; ++k)
            {
                result[i][j] += lhs[i][k] * rhs[k][j];
            }
        }
    return result;
}
