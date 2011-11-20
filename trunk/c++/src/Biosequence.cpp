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

/*
 * Biosequence.cpp
 *
 *  Created on: Oct 5, 2011
 *      Author: brunsc
 */

#include "moltk/Biosequence.hpp"
#include <fstream>
#include <stdexcept>
#include <cassert>
#include <sstream>

using namespace std;

namespace moltk
{

/////////////////////////////
// BaseBiosequence methods //
/////////////////////////////

/* virtual */
void BaseBiosequence::write_to_stream(std::ostream& os) const
{
    for(size_t i = 0; i < get_number_of_residues(); ++i)
        os << get_residue(i).get_one_letter_code();
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

void Biosequence::load_stream(std::istream& is)
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
    // Read subsequent lines that do not start with ">" characters.
    char peek = is.peek();
    while ((peek != '>') && is) {
        getline(is, line);
        // Avoid parsing final line twice
        if ((!is) && (is.gcount() == 0)) break; // end of file with nothing read
        for (size_t i = 0; i < line.length(); ++i) 
        {
            push_back( Residue(line[i], residueNumber+1) );
            ++residueNumber;
        }
        peek = is.peek();
    }
}

Biosequence& Biosequence::load_fasta(const std::string& file_name)
{
    ifstream fasta_stream;
    fasta_stream.open(file_name.c_str());
    if (! fasta_stream) {
        std::string msg("Error opening fasta file ");
        msg += file_name;
        throw std::runtime_error(msg.c_str());
    }
    load_fasta(fasta_stream);
    return *this;
}

Biosequence& Biosequence::load_fasta(std::istream& is)
{
    load_stream(is);
    // throw std::runtime_error("load_fasta() not implemented.  Talk to Christopher.");
    return *this;
}

void Biosequence::write_fasta(std::ostream& os) const
{
    os << '>' << description;
    int pos = 0;
    for (const_iterator i = begin(); i != end(); ++i)
    {
        if (0 == (pos % 70)) // newline before first residue, and every 70 residues
            os << endl;
        os << *i;
        ++pos;
    }
}

void Biosequence::write_fasta(const std::string& file_name) const
{
    ofstream output_stream;
    output_stream.open(file_name.c_str());
    if (!output_stream) {
        std::string msg("Error: moltk.Biosequence unable to write to fasta file ");
        msg += file_name;
        throw std::runtime_error(msg.c_str());
    }
    write_fasta(output_stream);
}

std::string Biosequence::fasta() const
{
    ostringstream output_stream;
    write_fasta(output_stream);
    return output_stream.str();
}

void Biosequence::write_sequence_string(std::ostream& os) const
{
    for (const_iterator i = begin(); i != end(); ++i)
        os << *i;
}

/*!
 * repr is a helper for the python __repr__ method.
 */
std::string Biosequence::repr() const
{
    std::ostringstream s;
    s << "moltk.Biosequence(sequence='";
    for (const_iterator i = begin(); i != end(); ++i)
        s << *i;
    s << "', description='" << description << "')";
    return s.str();
}


//////////////////////////////////
// Biosequence::Residue methods //
//////////////////////////////////

} // namespace moltk
