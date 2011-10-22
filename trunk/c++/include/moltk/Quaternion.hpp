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

#ifndef MOLTK_QUATERNION_HPP
#define MOLTK_QUATERNION_HPP

#include "Vector3D.hpp"

namespace moltk {

/*!
 * Quaternion is an elegant 4-element representation of a 3D rotation.
 */
class Quaternion
{
public:
    typedef Real value_type;
    typedef size_t size_type;
    typedef size_t index_type;
    typedef value_type* iterator;
    typedef value_type const * const_iterator;

    Quaternion();
    Quaternion(const units::Angle& angle, const UnitVector3D& unit_vector);

    Quaternion& set_from_angle_axis(const units::Angle& angle, const UnitVector3D& unit_vector);
    value_type& operator[](size_t i) {return data[i];}
    const value_type& operator[](size_t i) const {return data[i];}
    size_t size() const {return 4;}

    const_iterator begin() const {return &data[0];}
    const_iterator end() const {return &data[4];}
    iterator begin() {return &data[0];}
    iterator end() {return &data[4];}

protected:
    value_type data[4];
};

} // namespace moltk

#endif // MOLTK_QUATERNION_HPP
