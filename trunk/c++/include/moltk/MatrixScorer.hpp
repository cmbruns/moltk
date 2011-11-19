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
 * MatrixScorer class to efficiently compare DPPositions during alignment
 */

#ifndef MOLTK_ALIGN_MATRIX_SCORER_H
#define MOLTK_ALIGN_MATRIX_SCORER_H

#include "moltk/DPPosition.hpp"
#include "moltk/Alignment.hpp"
#include "moltk/SubstitutionMatrix.hpp"

namespace moltk {

/*!
 * MatrixScorer scores alignments using a residue type
 * matrix such as BLOSUM62 or PAM250.
 */
template<class SCORE_TYPE, int GAP_NSEGS>
class MatrixScorer_
{
public:
    /// Create a MatrixScorer using a particular substitution matrix
    explicit MatrixScorer_(const moltk::SubstitutionMatrix_<SCORE_TYPE>& matrix);
    /// Create precached data structures for alignment of two individual sequences
    void create_positions(
        std::vector<dp::DPPosition<SCORE_TYPE, dp::DP_ALIGN_UNGAPPED_SEQUENCES, GAP_NSEGS>*>& positions, 
        const Alignment& alignment) const;
    /// Create precached data structures for alignment of two alignments
    void create_positions(
        std::vector<dp::DPPosition<SCORE_TYPE, dp::DP_ALIGN_GAPPED_ALIGNMENTS, GAP_NSEGS>*>& positions, 
        const Alignment& alignment) const;
    bool get_end_gaps_free() const {return b_end_gaps_free;}
    void set_end_gaps_free(bool f) {b_end_gaps_free = f;}
    /// Alignment score reduction for initiating an alignment gap.
    SCORE_TYPE get_default_gap_open_score() const {return default_gap_open_score;}
    /// Set alignment score reduction for initiating an alignment gap.
    void set_default_gap_open_score(SCORE_TYPE score) {default_gap_open_score = score;}
    /// Alignment score reduction for increasing the length of an alignment gap by one position.
    SCORE_TYPE get_default_gap_extension_score() const {return default_gap_extension_score;}
    /// Set Alignment score reduction for increasing the length of an alignment gap by one position.
    void set_default_gap_extension_score(SCORE_TYPE score) {default_gap_extension_score = score;}

    /// Look up score of two residue one-letter-codes in the matrix
    SCORE_TYPE score(char residue1, char residue2) const {return matrix.score(residue1, residue2);}
    /// Inefficient computation of sum-of-pairs score, for use in testing and debugging.
    SCORE_TYPE calc_explicit_sum_of_pairs_score(const Alignment_<SCORE_TYPE>& alignment) const;
    /// Compute pair score between two sequences in an alignment
    SCORE_TYPE calc_explicit_pair_score(int i, int j, const Alignment_<SCORE_TYPE>& alignment) const;

protected:
    bool b_end_gaps_free;
    SCORE_TYPE default_gap_open_score;
    SCORE_TYPE default_gap_extension_score;
    SubstitutionMatrix_<SCORE_TYPE> matrix;
};

typedef MatrixScorer_<moltk::units::Information, 1> MatrixScorer;

} // namespace moltk

#endif // MOLTK_ALIGN_MATRIX_SCORER_H

