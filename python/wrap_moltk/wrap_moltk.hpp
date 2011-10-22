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

#ifndef WRAP_MOLTK_PYPLUSPLUS_H
#define WRAP_MOLTK_PYPLUSPLUS_H

// Try to avoid a boost include problem on Windows
#ifdef _MSC_VER
#define BOOST_MSVC _MSC_VER
#endif

#include "moltk.hpp"
#include <vector>

using namespace moltk;
using namespace moltk::units;

namespace pyplusplus { namespace aliases {

    typedef moltk::units::DimensionlessDimension DimensionlessDimension;
    typedef moltk::units::InformationDimension InformationDimension;
    typedef moltk::units::LengthDimension LengthDimension;
    typedef moltk::units::AreaDimension AreaDimension;
    typedef moltk::units::AngleDimension AngleDimension;

    typedef std::vector<PDBStructure::Atom> AtomList;
    typedef Unit<InformationDimension> BitUnit;
    typedef Quantity<BitUnit> Information;
    typedef Unit<LengthDimension> NanometerUnit;
    typedef Quantity<NanometerUnit> Length;
    typedef Unit<AngleDimension> RadianUnit;
    typedef Quantity<RadianUnit> Angle;
    typedef std::vector<int> std_vector_int;
    typedef std::vector<Aligner::QueryPosition*> QueryPositionList;
    typedef std::vector<Aligner::TargetPosition*> TargetPositionList;
    typedef Vector3D_< moltk::Real > Vector3D;
    typedef Vector3D_< moltk::units::Length > Displacement;
    typedef BaseVector3D< double > BaseVector3D_double;
    typedef BaseVector3D< moltk::units::Length > BaseDisplacement;

    struct _classes_to_instantiate 
    {
        // instantiate classes
        Vector3D vector3D;
        UnitVector3D unitVector3D;
        // instantiate methods
        void foo() {Angle a = 4.0 * radian;}
    };

}} // namespace pyplusplus::aliases

#endif // WRAP_MOLTK_PYPLUSPLUS_H

