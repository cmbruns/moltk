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

/*!
 * \file
 * RigidTransform class representing rotation and translation between coordinate frames
 */

#ifndef MOLTK_RIGID_TRANSFORM_HPP
#define MOLTK_RIGID_TRANSFORM_HPP

#include "moltk/Rotation3D.hpp"

namespace moltk {

/*!
 * RigidTransform converts one reference frame to another with rotation and translation.
 */
class RigidTransform
{
protected:
    Displacement translation;
    Rotation3D rotation;
};

} // namespace moltk

#endif // MOLTK_RIGID_TRANSFORM_HPP
