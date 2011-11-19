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
 * SubstitutionMatrix class for look up of residue alignment scores
 */

#ifndef MOLTK_SUBSTITUTION_MATRIX_HPP_
#define MOLTK_SUBSTITUTION_MATRIX_HPP_

#include "moltk/units.hpp"
#include <iostream>
#include <vector>

namespace moltk {

/*!
 * SubstitutionMatrix scores alignments using a residue type
 * matrix such as BLOSUM62 or PAM250.
 */
template<class SCORE_TYPE>
class SubstitutionMatrix_
{
public:
    typedef std::vector<SCORE_TYPE> RowType;
    typedef std::vector<RowType> MatrixType;

    explicit SubstitutionMatrix_(const std::string& matrix_string, double bits_per_entry = 1.0);
    explicit SubstitutionMatrix_(std::istream& matrix_stream, double bits_per_entry = 1.0);
    std::istream& load_stream(std::istream&, double bits_per_entry = 1.0);

    /// Get matrix row by integer index
    const RowType& operator[](size_t ix) const {return matrix[ix];}
    /// Get matrix row by integer index
    const RowType& operator[](char c) const {return matrix[character_indices[c]];}
    /// Number of rows in substitution matrix
    size_t size() const {return matrix.size();}
    /// Read a string representation of a SubstitutionMatrix from a C++ stream.
    inline friend std::istream& 
        operator>>(std::istream& is, SubstitutionMatrix_& scorer)
    { return scorer.load_stream(is); }

    static const SubstitutionMatrix_& get_blosum62();
    static const SubstitutionMatrix_& get_pam250();
    const std::vector<int>& get_character_indices() const { return character_indices; }

protected:
    MatrixType matrix;
    std::vector<int> character_indices; // maps letters to matrix indices
};

typedef SubstitutionMatrix_<moltk::units::Information> SubstitutionMatrix;

} // namespace moltk

#endif // MOLTK_SUBSTITUTION_MATRIX_HPP_
