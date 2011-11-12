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

#ifndef MOLTK_ALIGN_MATRIX_SCORER_H
#define MOLTK_ALIGN_MATRIX_SCORER_H

#include "moltk/DPPosition.hpp"
#include "moltk/Alignment.hpp"
#include "moltk/SubstitutionMatrix.hpp"
#include "moltk/units.hpp"
#include <string>
#include <iostream>
#include <vector>

namespace moltk {

/*!
 * MatrixScorer scores alignments using a residue type
 * matrix such as BLOSUM62 or PAM250.
 */
template<class SCORE_TYPE, int GAP_NSEGS>
class MatrixScorer_
{
public:
    typedef SCORE_TYPE ScoreType;
    typedef dp::DPPosition<SCORE_TYPE, GAP_NSEGS> QueryPosition;
    typedef dp::DPPosition<SCORE_TYPE, GAP_NSEGS> TargetPosition;

    explicit MatrixScorer_(const moltk::SubstitutionMatrix_<SCORE_TYPE>& matrix);
    void create_positions(
        std::vector<dp::DPPosition<SCORE_TYPE, GAP_NSEGS>*>& positions, 
        const Alignment& alignment) const;
    bool get_end_gaps_free() const {return b_end_gaps_free;}
    void set_end_gaps_free(bool f) {b_end_gaps_free = f;}
    /// Alignment score reduction for initiating an alignment gap.
    SCORE_TYPE get_default_gap_open_penalty() const {return default_gap_open_penalty;}
    /// Set alignment score reduction for initiating an alignment gap.
    void set_default_gap_open_penalty(SCORE_TYPE penalty) {default_gap_open_penalty = penalty;}
    /// Alignment score reduction for increasing the length of an alignment gap by one position.
    SCORE_TYPE get_default_gap_extension_penalty() const {return default_gap_extension_penalty;}
    /// Set Alignment score reduction for increasing the length of an alignment gap by one position.
    void set_default_gap_extension_penalty(SCORE_TYPE penalty) {default_gap_extension_penalty = penalty;}

protected:
    bool b_end_gaps_free;
    SCORE_TYPE default_gap_open_penalty;
    SCORE_TYPE default_gap_extension_penalty;
    SubstitutionMatrix_<SCORE_TYPE> matrix;
};

typedef MatrixScorer_<moltk::units::Information, 1> MatrixScorer;

} // namespace moltk

#endif // MOLTK_ALIGN_MATRIX_SCORER_H

