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

using namespace moltk;
using namespace moltk::units;

Quaternion::Quaternion() 
    : m_w(1.0), m_v(0.0, 0.0, 0.0)
{}

Quaternion::Quaternion(const Angle& angle, const UnitVector3D& unit_vector)
{
    set_from_angle_axis(angle, unit_vector);
}

Quaternion& Quaternion::set_from_angle_axis(const Angle& angle, const UnitVector3D& unit_vector)
{
    Real ca2 = cos(angle/2.0); Real sa2 = sin(angle/2.0);
    if (ca2 < 0) {ca2 = -ca2; sa2 = -sa2;} // keep first element positive
    m_w = ca2;
    m_v = sa2 * unit_vector;
    return *this;
}
