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

/*
 * Biosequence.cpp
 *
 *  Created on: Oct 5, 2011
 *      Author: brunsc
 */

#include "moltk/Biosequence.h"

namespace moltk
{

/////////////////////////////
// BaseBiosequence methods //
/////////////////////////////

/* virtual */
void BaseBiosequence::print_to_stream(std::ostream& os) const
{
    for(size_t i = 0; i < getNumberOfResidues(); ++i)
        os << getResidue(i).getOneLetterCode();
}


/////////////////////////
// Biosequence methods //
/////////////////////////

Biosequence::Biosequence(const std::string& str)
{
    for (size_t i = 0; i < str.length(); ++i)
        residues.push_back( Residue(str[i], i+1) );
}

Biosequence::Biosequence(const char * strParam)
{
    const std::string str(strParam);
    for (size_t i = 0; i < str.length(); ++i)
        residues.push_back( Residue(str[i], i+1) );
}

//////////////////////////////////
// Biosequence::Residue methods //
//////////////////////////////////


} // namespace moltk
