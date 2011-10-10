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
    
    For questions contact: cmbruns@rotatingpenguin.com
*/

#ifndef WRAP_MOLTK_PYPLUSPLUS_H
#define WRAP_MOLTK_PYPLUSPLUS_H

// Try to avoid a boost include problem on Windows
#ifdef _MSC_VER
#define BOOST_MSVC _MSC_VER
#endif

#include "moltk.h"
#include <vector>

using namespace moltk;
using namespace moltk::units;

namespace pyplusplus { namespace aliases {

    typedef std::vector<Structure::Atom> AtomList;
    typedef unit<information_dimension> bit_t;
    typedef quantity<bit_t> Information;
    typedef unit<length_dimension> nanometer_t;
    typedef quantity<nanometer_t> Length;

}} // namespace pyplusplus::aliases

#endif // WRAP_MOLTK_PYPLUSPLUS_H

