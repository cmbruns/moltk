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
    : matrix(matrixParam)
    , default_gap_open_penalty(8.0 * moltk::units::one<SCORE_TYPE>())
    , default_gap_extension_penalty(0.5 * moltk::units::one<SCORE_TYPE>())
{}

template<class SCORE_TYPE, int GAP_NSEGS>
void MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::create_positions(std::vector<dp::DPPosition<SCORE_TYPE, GAP_NSEGS>*>& result, const Alignment& alignment) const
{
    typedef dp::DPPosition<SCORE_TYPE, GAP_NSEGS> POS;

    // TODO - release old memory in result
    result.clear();
    const size_t ncol = alignment.get_number_of_columns();
    const size_t nseq = alignment.get_number_of_sequences();
    if (ncol <= 0) return;

    // queryWeightIndexByResTypeIndex helps coalesce multiple instances of the same residue in a column
    std::vector< std::map<size_t, size_t> > queryWeightIndexByResTypeIndexByColumn;
    queryWeightIndexByResTypeIndexByColumn.assign( ncol, std::map<size_t, size_t>() );

    // Initialize positions with zero-scoring initial profile
    for (size_t col = 0; col <= ncol; ++col) // One more position than there are columns
    {
        POS* pos = new POS();
        pos->gap_score.open_penalty = moltk::units::zero<SCORE_TYPE>();
        pos->gap_score.extension_penalty = moltk::units::zero<SCORE_TYPE>();
        pos->index = col;
        pos->target_scores_by_residue_type_index.assign(matrix.size(), moltk::units::zero<SCORE_TYPE>());
        result.push_back(pos);
    }
    // Add contributions from each residue of each sequence
    for (size_t seqIx = 0; seqIx < nseq; ++seqIx) 
    {
        // Special treatment for position zero, which corresponds to column -1
        // end gaps free?
        double gapFactor = 1.0;
        if (get_end_gaps_free())
            gapFactor = 0.0; // Always starts at a left end gap
        {
            POS& pos = 
                dynamic_cast<POS&>(*result[0]);
            // leave score zero, but set gap penalties
            pos.gap_score.extension_penalty = gapFactor * default_gap_extension_penalty;
            pos.gap_score.open_penalty = gapFactor * default_gap_open_penalty;
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
                gapFactor = 1.0;
                if (   get_end_gaps_free() 
                    && (eResIx >= ((int)seq.get_number_of_residues() - 1) ) )
                {
                    gapFactor = 0.0;
                }
                assert(eResIx <= ((int)seq.get_number_of_residues() - 1));
            }
            ++colIx;
            // Position[i+1] represents column i
            POS& pos = 
                dynamic_cast<POS&>(*result[colIx + 1]);
            // Set gap penalties
            pos.gap_score.extension_penalty = gapFactor * default_gap_extension_penalty;
            pos.gap_score.open_penalty = gapFactor * default_gap_open_penalty;
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

template class MatrixScorer_<moltk::units::Information, 1>;

/*
// createTargetPositions() is used to precompute everything that can be precomputed
// in O(n) time, so that alignment score can be computed as quickly as possible
// durint the O(n^2) alignment phase.
template<class SCORE_TYPE, int GAP_NSEGS>
*/
/* virtual */
/*
std::vector<const dp::TargetPosition<SCORE_TYPE>*> MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::create_target_positions(const Alignment& alignment) const
{
    std::vector<dp::TargetPosition<SCORE_TYPE>*> result =
        create_foo_positions<dp::TargetPosition<SCORE_TYPE>, dp::TargetPosition<SCORE_TYPE>>(alignment);

    const size_t ncol = alignment.get_number_of_columns();

    // queryWeightIndexByResTypeIndex helps coalesce multiple instances of the same residue in a column
    const size_t ncol = alignment.get_number_of_columns();
    std::vector< std::map<size_t, size_t> > queryWeightIndexByResTypeIndexByColumn;
    queryWeightIndexByResTypeIndexByColumn.assign( ncol, std::map<size_t, size_t>() );

    // initialize
    for (size_t col = 0; col <= ncol; ++col) // One more position than there are columns
    {
        MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::TargetPosition& pos = 
                dynamic_cast<MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::TargetPosition&>(*result[col]);
        pos.target_scores_by_residue_type_index.assign(matrix.size(), moltk::units::zero<SCORE_TYPE>());
    }
    const size_t nseq = alignment.get_number_of_sequences();
    for (size_t seqIx = 0; seqIx < nseq; ++seqIx) 
    {
        const BaseBiosequence& seq = alignment.get_sequence(seqIx);
        const EString& eString = alignment.get_estring(seqIx);
        EString::const_iterator e;
        int colIx = -1;
        for (e = eString.begin(); e != eString.end(); ++e) 
        {
            int eResIx = *e;
            colIx++;
            if (eResIx < 0) // This is a gap position
                continue; // gap - zero score
            // Compute alignment score
            MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::TargetPosition& pos = 
                dynamic_cast<MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::TargetPosition&>(*result[colIx + 1]);
            const BaseBiosequence::BaseResidue& res = seq.get_residue(eResIx);
            size_t resTypeIndex = character_indices[res.get_one_letter_code()];
            // loop over scoring matrix positions
            for (size_t m = 0; m < matrix.size(); ++m) {
                pos.target_scores_by_residue_type_index[m] += matrix[resTypeIndex][m];
            }
            // cerr << pos.scoresByResidueTypeIndex[17] << endl;
       }
    }

    // Copy vector of non-const pointer to const pointer, now that we are done modifying them.
    std::vector<const Aligner::TargetPosition*> result1;
    for (size_t i = 0; i < result.size(); ++i)
        result1.push_back(result[i]);
    return result1;
}

template<class SCORE_TYPE, int GAP_NSEGS>
*/
/* virtual */
/*
std::vector<const Aligner::QueryPosition*> MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::create_query_positions(const Alignment& alignment) const
{
    std::vector<Aligner::QueryPosition*> result =
        create_foo_positions<Aligner::QueryPosition, MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::QueryPosition>(alignment);

    // queryWeightIndexByResTypeIndex helps coalesce multiple instances of the same residue in a column
    const size_t ncol = alignment.get_number_of_columns();
    std::vector< std::map<size_t, size_t> > queryWeightIndexByResTypeIndexByColumn;
    queryWeightIndexByResTypeIndexByColumn.assign( ncol, std::map<size_t, size_t>() );

    const size_t nseq = alignment.get_number_of_sequences();
    for (size_t seqIx = 0; seqIx < nseq; ++seqIx) 
    {
        const BaseBiosequence& seq = alignment.get_sequence(seqIx);
        const EString& eString = alignment.get_estring(seqIx);
        EString::const_iterator e;
        int colIx = -1;
        for (e = eString.begin(); e != eString.end(); ++e) 
        {
            int eResIx = *e; // NOT size_t, could be -1!
            colIx++;
            if (eResIx < 0) // This is a gap position
                continue; // gap - zero score
            // Compute alignment score
            MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::QueryPosition& pos = 
                dynamic_cast<MatrixScorer_<SCORE_TYPE, GAP_NSEGS>::QueryPosition&>(*result[colIx + 1]);
            const BaseBiosequence::BaseResidue& res = seq.get_residue(eResIx);
            size_t resTypeIndex = character_indices[res.get_one_letter_code()];
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

    // Copy vector of non-const pointer to const pointer, now that we are done modifying them.
    std::vector<const Aligner::QueryPosition*> result1;
    for (size_t i = 0; i < result.size(); ++i)
        result1.push_back(result[i]);
    return result1;
}
*/
