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

#ifndef MOLTK_ROTATION3D_HPP
#define MOLTK_ROTATION3D_HPP

#include "moltk/Quaternion.hpp"
#include "moltk/Real.hpp"
#include "moltk/units.hpp"

namespace moltk {

/*!
 * Rotation3D represents a rotation matrix.
 */
class Rotation3D
{
public:

    /*!
     * One row of three numbers in a Rotation3D rotation matrix.
     */
    class Row
    {
    public:
        friend class Rotation3D;
        const Real& operator[](size_t ix) const {return (&x)[ix];}

    protected:
        Real& operator[](size_t ix) {return (&x)[ix];}
        Real x, y, z;
    };


public:
    Rotation3D();
    Rotation3D(const units::Angle& angle, const UnitVector3D& unit_vector);
    explicit Rotation3D(const Quaternion& quaternion);

    Rotation3D& set_from_angle_about_unit_vector(
        const units::Angle& angle, const UnitVector3D& unit_vector);
    Rotation3D& set_from_quaternion(const Quaternion& quaternion);

    const Row& operator[](size_t ix) const {return (&row0)[ix];}

protected:
    Rotation3D& set_from_elements(
        Real e00, Real e01, Real e02,
        Real e10, Real e11, Real e12,
        Real e20, Real e21, Real e22);
    Row& operator[](size_t ix) {return (&row0)[ix];}
    Row row0, row1, row2;
};

} // namespace moltk

#endif // MOLTK_ROTATION3D_HPP
