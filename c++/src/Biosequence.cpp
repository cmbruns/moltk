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

Biosequence::Biosequence(const std::string& sequence, const std::string& descriptionParam)
    : description(descriptionParam)
{
    for (size_t i = 0; i < sequence.length(); ++i)
        push_back( Residue(sequence[i], i+1) );
}

Biosequence::Biosequence(const char * sequence, const std::string& descriptionParam)
    : description(descriptionParam)
{
    const std::string str(sequence);
    for (size_t i = 0; i < str.length(); ++i)
        push_back( Residue(str[i], i+1) );
}

Biosequence::Biosequence(const Biosequence& rhs)
    : std::vector<Residue>(rhs)
    , description(rhs.description)
{}

void Biosequence::loadStream(std::istream& is)
{
    // Find first non-blank non-comment line
    std::string line;
    const char* spaces = " \t\n\r";
    size_t pos = 0;
    // while loop exits with first line of sequence data in line
    while (is.good() && pos != std::string::npos)
    {
        getline(is, line);
        pos = line.find_first_not_of(spaces);
        if (pos == std::string::npos) continue; // blank line
        char c = line[pos]; // first non-blank character
        if (c == '#') continue; // comment
        if (c == '>') // fasta format
        {
            description = "";
            pos = line.find_first_not_of(spaces, pos + 1);
            if (pos != std::string::npos)
                description = line.substr(pos);
            getline(is, line);
            break;
        }
    }
    // now "line" contains first line of sequence data
    int residueNumber = 0;
    for (size_t i = 0; i < line.length(); ++i) 
    {
        push_back( Residue(line[i], residueNumber+1) );
        ++residueNumber;
    }
    // TODO - subsequent lines - peek() for '>' characters
}


//////////////////////////////////
// Biosequence::Residue methods //
//////////////////////////////////


} // namespace moltk
