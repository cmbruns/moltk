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

#include "moltk/SubstitutionMatrix.hpp"
#include "moltk/substitution_matrix_data.hpp"
#include <sstream>
#include <cassert>
#include <cctype>
#include <map>

using namespace std;
using namespace moltk;

template<class SCORE_TYPE>
/* static */
const SubstitutionMatrix_<SCORE_TYPE>& SubstitutionMatrix_<SCORE_TYPE>::get_blosum62() 
{
    static const SubstitutionMatrix_<SCORE_TYPE> blosum62_matrix(blosum62_string, 0.50);
    return blosum62_matrix;
}

template<class SCORE_TYPE>
/* static */
const SubstitutionMatrix_<SCORE_TYPE>& SubstitutionMatrix_<SCORE_TYPE>::get_pam250() 
{
    static const SubstitutionMatrix_<SCORE_TYPE> pam250_matrix(pam250_string, 0.333);
    return pam250_matrix;
}

template<class SCORE_TYPE>
/* explicit */
SubstitutionMatrix_<SCORE_TYPE>::SubstitutionMatrix_(const std::string& matrix_string, double bits_per_entry) 
{
    istringstream ss(matrix_string);
    load_stream(ss, bits_per_entry);
}

template<class SCORE_TYPE>
/* explicit */
SubstitutionMatrix_<SCORE_TYPE>::SubstitutionMatrix_(std::istream& matrixStream, double bits_per_entry)
{
    load_stream(matrixStream, bits_per_entry);
}

/// Look up score of two residue one-letter-codes in the matrix
template<class SCORE_TYPE>
SCORE_TYPE SubstitutionMatrix_<SCORE_TYPE>::score(char residue1, char residue2) const
{
    int index1 = character_indices[residue1];
    int index2 = character_indices[residue2];
    return matrix[index1][index2];
}

template<class SCORE_TYPE>
istream& SubstitutionMatrix_<SCORE_TYPE>::load_stream(istream& is, double bits_per_entry)
{
    char lineBuffer[1000];
    // skip header comments
    while (is.good()) {
        is.getline(lineBuffer, 999);
        std::string line(lineBuffer);
        if (line[0] == '#') 
            continue;
        if (line.length() < 5)
            continue;
        break; // finished with header
    }
    // cout << lineBuffer << endl;
    // Parse list of characters supported by this matrix into 
    // characterIndices array.
    stringstream oneLetterStream(lineBuffer);
    int index = 0;
    character_indices.assign(256, -1);
    while (oneLetterStream.good()) {
        std::string letterString;
        oneLetterStream >> letterString;
        // cout << "Here is a letter: " << letterString[0] << endl;
        int upperChar = std::toupper(letterString[0]);
        character_indices[upperChar] = index;
        ++index;
    }
    // Allocate matrix
    int nChars = index;
    matrix.assign( nChars, std::vector<SCORE_TYPE>(nChars, moltk::units::zero<SCORE_TYPE>()) );
    // Read entries into matrix from stream
    while (is.good()) {
        is.getline(lineBuffer, 999);
        std::string lineString(lineBuffer);
        if (lineString.length() < 5) continue;
        stringstream lineStream(lineString);
        std::string sequenceLetterString;
        lineStream >> sequenceLetterString;
        int rowIndex = character_indices[std::toupper(sequenceLetterString[0])];
        std::vector<SCORE_TYPE>& row = matrix[rowIndex];
        // cout << sequenceLetterString << " : " << rowIndex << endl;
        double value;
        size_t columnIndex = 0;
        while (lineStream.good() && (columnIndex < row.size())) {
            lineStream >> value;
            // cout << "  " << value << " ";
            // cout << columnIndex << " " << row.size() << endl;
            // Matrices are usually in 1/2 bit units...
            // TODO this assumes units are bits
            row[columnIndex] = value * bits_per_entry * moltk::units::one<SCORE_TYPE>();
            ++columnIndex;
        }
        // cout << "#" << endl;
    }
    return is;
}

template class SubstitutionMatrix_<moltk::units::Information>;
