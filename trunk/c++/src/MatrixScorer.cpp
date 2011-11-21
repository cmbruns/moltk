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

#include "moltk/MatrixScorer.hpp"
#include <sstream>
#include <cassert>
#include <cctype>
#include <map>

using namespace std;
using namespace moltk;

template<class SCORE_TYPE, int GAP_NSEGS>
/* explicit */
MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::MatrixScorer_(const SubstitutionMatrix_<SCORE_TYPE>& matrixParam)
    : end_gap_factor(0.0)
    , default_gap_open_score(-8.0 * moltk::units::one<SCORE_TYPE>())
    , default_gap_extension_score(-0.5 * moltk::units::one<SCORE_TYPE>())
    , matrix(matrixParam)
{}

/// Alignment of alignments
template<class SCORE_TYPE, int GAP_NSEGS>
void MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::create_positions(
    std::vector<dp::DPPosition<SCORE_TYPE, dp::DP_ALIGN_GAPPED_ALIGNMENTS, GAP_NSEGS>*>& result, 
    const Alignment& alignment) const
{
    typedef dp::DPPosition<SCORE_TYPE, dp::DP_ALIGN_GAPPED_ALIGNMENTS, GAP_NSEGS> PositionType;

    // Ensure table.clear_positions() was called before create_positions()
    assert(result.size() == 0);

    const size_t ncol = alignment.get_number_of_columns();
    const size_t nseq = alignment.get_number_of_sequences();
    if (ncol <= 0) return;

    // queryWeightIndexByResTypeIndex helps coalesce multiple instances of the same residue in a column
    std::vector< std::map<size_t, size_t> > queryWeightIndexByResTypeIndexByColumn;
    queryWeightIndexByResTypeIndexByColumn.assign( ncol, std::map<size_t, size_t>() );

    // Initialize positions with zero-scoring initial profile
    for (size_t col = 0; col <= ncol; ++col) // One more position than there are columns
    {
        PositionType* pos = new PositionType();
        pos->gap_score.open_score = moltk::units::zero<SCORE_TYPE>();
        pos->gap_score.extension_score = moltk::units::zero<SCORE_TYPE>();
        pos->index = col;
        pos->target_scores_by_residue_type_index.assign(matrix.size(), moltk::units::zero<SCORE_TYPE>());
        pos->extension_gap_score = moltk::units::zero<SCORE_TYPE>();
        pos->nongap_count = 0;
        result.push_back(pos);
    }
    // Add contributions from each residue of each sequence
    // Loop over sequences first
    for (size_t seqIx = 0; seqIx < nseq; ++seqIx) 
    {
        double seq_weight = alignment.get_row(seqIx).sequence_weight;

        // Special treatment for position zero, which corresponds to column -1
        // end gaps free?
        double gap_factor = end_gap_factor; // at first we are always at an end
        {
            PositionType& pos = 
                dynamic_cast<PositionType&>(*result[0]);
            // leave score zero, but set gap penalties
            pos.gap_score.extension_score += seq_weight * gap_factor * default_gap_extension_score;
            pos.gap_score.open_score += seq_weight * gap_factor * default_gap_open_score;
        }

        int colIx = -1;
        const BaseBiosequence& seq = alignment.get_sequence(seqIx);
        const EString& eString = alignment.get_estring(seqIx);
        EString::const_iterator e;
        // Inner loop over columns in this sequence
        for (e = eString.begin(); e != eString.end(); ++e) 
        {
            int eResIx = *e;
            if (eResIx >= 0) { // eResIx is an actual residue number
                // Is this an end gap?
                // This cannot be a left end-gap, but it could be
                // a right end gap.
                gap_factor = 1.0;
                if (eResIx >= ((int)seq.get_number_of_residues() - 1))
                {
                    gap_factor = end_gap_factor;
                }
                assert(eResIx <= ((int)seq.get_number_of_residues() - 1));
            }
            ++colIx;
            // Position[i+1] represents column i
            PositionType& pos = 
                dynamic_cast<PositionType&>(*result[colIx + 1]);
            // Set gap penalties
            pos.gap_score.extension_score += seq_weight * gap_factor * default_gap_extension_score;
            pos.gap_score.open_score += seq_weight * gap_factor * default_gap_open_score;
            if (eResIx >= 0) 
            { // eResIx is an actual residue number
                const BaseBiosequence::BaseResidue& res = seq.get_residue(eResIx);
                size_t resTypeIndex = matrix.get_character_indices()[res.get_one_letter_code()];
                // Target side
                // loop over scoring matrix positions
                for (size_t m = 0; m < matrix.size(); ++m) {
                    pos.target_scores_by_residue_type_index[m] += seq_weight * matrix[resTypeIndex][m];
                }
                // Query side
                std::map<size_t, size_t>& qmap = 
                    queryWeightIndexByResTypeIndexByColumn[colIx];
                if ( qmap.find(resTypeIndex) == qmap.end() )
                {
                    qmap[resTypeIndex] = pos.query_residue_type_index_weights.size();
                    pos.query_residue_type_index_weights.push_back( 
                        std::pair<size_t, double>(resTypeIndex, 0.0) );
                }
                pos.query_residue_type_index_weights[qmap[resTypeIndex]].second += seq_weight;

                // cache for nongap/gap extension score
                pos.nongap_count += seq_weight;
            }
            else
            {
                // cache count of internal gap characters at this position
                pos.extension_gap_score += seq_weight * gap_factor * default_gap_extension_score;
            }
        }
        assert(colIx == alignment.get_number_of_columns() - 1);
    }
}

/// Alignment of two individual sequences
// TODO - remove alignment of alignments specific work
template<class SCORE_TYPE, int GAP_NSEGS>
void MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::create_positions(
    std::vector<dp::DPPosition<SCORE_TYPE, dp::DP_ALIGN_UNGAPPED_SEQUENCES, GAP_NSEGS>*>& result, 
    const Alignment& alignment) const
{
    typedef dp::DPPosition<SCORE_TYPE, dp::DP_ALIGN_UNGAPPED_SEQUENCES, GAP_NSEGS> PositionType;

    // Ensure table.clear_positions() was called before create_positions()
    assert(result.size() == 0);

    const size_t ncol = alignment.get_number_of_columns();
    const size_t nseq = alignment.get_number_of_sequences();
    if (ncol <= 0) return;

    // queryWeightIndexByResTypeIndex helps coalesce multiple instances of the same residue in a column
    std::vector< std::map<size_t, size_t> > queryWeightIndexByResTypeIndexByColumn;
    queryWeightIndexByResTypeIndexByColumn.assign( ncol, std::map<size_t, size_t>() );

    // Initialize positions with zero-scoring initial profile
    for (size_t col = 0; col <= ncol; ++col) // One more position than there are columns
    {
        PositionType* pos = new PositionType();
        pos->gap_score.open_score = moltk::units::zero<SCORE_TYPE>();
        pos->gap_score.extension_score = moltk::units::zero<SCORE_TYPE>();
        pos->index = col;
        pos->target_scores_by_residue_type_index.assign(matrix.size(), moltk::units::zero<SCORE_TYPE>());
        result.push_back(pos);
    }
    // Add contributions from each residue of each sequence
    for (size_t seqIx = 0; seqIx < nseq; ++seqIx) 
    {
        // Special treatment for position zero, which corresponds to column -1
        // end gaps free?
        double gap_factor = end_gap_factor;
        {
            PositionType& pos = 
                dynamic_cast<PositionType&>(*result[0]);
            // leave score zero, but set gap penalties
            pos.gap_score.extension_score = gap_factor * default_gap_extension_score;
            pos.gap_score.open_score = gap_factor * default_gap_open_score;
        }
        int colIx = -1;
        const BaseBiosequence& seq = alignment.get_sequence(seqIx);
        const EString& eString = alignment.get_estring(seqIx);
        EString::const_iterator e;
        for (e = eString.begin(); e != eString.end(); ++e) 
        {
            int eResIx = *e;
            if (eResIx >= 0) { // eResIx is an actual residue number
                // Is this an end gap?
                // This cannot be a left end-gap, but it could be
                // a right end gap.
                gap_factor = 1.0;
                if (eResIx >= ((int)seq.get_number_of_residues() - 1))
                {
                    gap_factor = end_gap_factor;
                }
                assert(eResIx <= ((int)seq.get_number_of_residues() - 1));
            }
            ++colIx;
            // Position[i+1] represents column i
            PositionType& pos = 
                dynamic_cast<PositionType&>(*result[colIx + 1]);
            // Set gap penalties
            pos.gap_score.extension_score = gap_factor * default_gap_extension_score;
            pos.gap_score.open_score = gap_factor * default_gap_open_score;
            if (eResIx >= 0) 
            { // eResIx is an actual residue number
                const BaseBiosequence::BaseResidue& res = seq.get_residue(eResIx);
                size_t resTypeIndex = matrix.get_character_indices()[res.get_one_letter_code()];
                // Target side
                // loop over scoring matrix positions
                for (size_t m = 0; m < matrix.size(); ++m) {
                    pos.target_scores_by_residue_type_index[m] += matrix[resTypeIndex][m];
                }

                // Query side
                std::map<size_t, size_t>& qmap = 
                    queryWeightIndexByResTypeIndexByColumn[colIx];
                if ( qmap.find(resTypeIndex) == qmap.end() )
                {
                    qmap[resTypeIndex] = pos.query_residue_type_index_weights.size();
                    pos.query_residue_type_index_weights.push_back( 
                        std::pair<size_t, double>(resTypeIndex, 0.0) );
                }
                pos.query_residue_type_index_weights[qmap[resTypeIndex]].second += 1.0;
            }
        }
        assert(colIx == alignment.get_number_of_columns() - 1);
    }
}

/// Inefficient computation of sum-of-pairs score, for use in testing and debugging.
template<class SCORE_TYPE, int GAP_NSEGS>
SCORE_TYPE MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::calc_explicit_sum_of_pairs_score(const Alignment_<SCORE_TYPE>& alignment) const
{
    SCORE_TYPE result = moltk::units::zero<SCORE_TYPE>();
    const int nseq = alignment.get_number_of_sequences();
    for (int i = 0; i < (nseq - 1); ++i)
        for (int j = i + 1; j < nseq; ++j)
        {
            // TODO - scale by sequence weight
            result += calc_explicit_pair_score(i, j, alignment);
        }
    return result;
}

/// Compute pair score between two sequences in an alignment
template<class SCORE_TYPE, int GAP_NSEGS>
SCORE_TYPE MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::calc_explicit_pair_score(int i, int j, const Alignment_<SCORE_TYPE>& alignment) const
{
    const MatrixScorer_<SCORE_TYPE, GAP_NSEGS>& scorer = *this;
    SCORE_TYPE result = moltk::units::zero<SCORE_TYPE>();
    // Loop over estrings
    EString::const_iterator e1 = alignment.get_estring(i).begin();
    EString::const_iterator e2 = alignment.get_estring(j).begin();
    const BaseBiosequence& seq1 = alignment.get_sequence(i);
    const BaseBiosequence& seq2 = alignment.get_sequence(j);
    bool b_was_gap = false; // did the previous column contain exactly one gap?
    bool b_end_gap_1 = true; // Are we in an end gap (left or right) of sequence 1?
    bool b_end_gap_2 = true; // Are we in an end gap (left or right) of sequence 2?
    while(e1 != alignment.get_estring(i).end())
    {
        // Update end gap status
        if (*e1 >= 0)
            b_end_gap_1 = (*e1 == ((int)seq1.get_number_of_residues() - 1));
        if (*e2 >= 0)
            b_end_gap_2 = (*e2 == ((int)seq2.get_number_of_residues() - 1));

        // Ignore positions where both sequences have gaps
        if ( (*e1 < 0) && (*e2 < 0) ) 
            ; 
        // Neither sequence has a gap - use match score
        else if ( (*e1 >= 0) && (*e2 >= 0) ) 
        {
            char c1 = seq1.get_residue(*e1).get_one_letter_code();
            char c2 = seq2.get_residue(*e2).get_one_letter_code();
            result += scorer.score(c1, c2);
            b_was_gap = false;
        }
        // One position has a gap, the other does not - so score a gap
        else
        {
            // compute gap score
            Real gap_factor = 1.0;
            if (b_end_gap_1 && (*e1 < 0))
                gap_factor = end_gap_factor;
            if (b_end_gap_2 && (*e2 < 0))
                gap_factor = end_gap_factor;
            // gap opening
            if (! b_was_gap)
                result += gap_factor * default_gap_open_score;
            // gap extension
            result += gap_factor * default_gap_extension_score;
            b_was_gap = true;
        }
        ++e1;
        ++e2;
    }
    return result;
}

template class MatrixScorer_<moltk::units::Information, 1>;
