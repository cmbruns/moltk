/*
 * DPTable.hpp
 *
 *  Created on: Nov 7, 2011
 *      Author: brunsc
 */

/*!
 * \file
 * Templated classes GapScore and DPPosition to efficiently represent sequence residues during alignment.
 */


#ifndef MOLTK_DPPOSITION_HPP_
#define MOLTK_DPPOSITION_HPP_

#include "moltk/units.hpp"
#include "moltk/dp_params.hpp"
#include <vector>
#include <map>

namespace moltk {

/// Namespace for dynamic programming classes.
/// 
/// Many of theses classes use hard-to-read C++ template specializations,
/// because we are trying to avoid the overhead of virtual functions and
/// and branching logic in the time-critical inner loop of dynamic programming.
namespace dp {

// Optimize speed by using template specializations to avoid
// virtual methods and extra logic in inner loop of dynamic
// programming algorithms.  (Most of the permutations are probably
// not implemented yet...)


/// Parameters for scoring indel gaps in sequence alignments
template<typename SCORE_TYPE, ///< Type of score value, e.g. double, Information, etc.
         int GAP_NSEGS ///< Number of piecewise segments in gap function (1 for affine)
         >
struct GapScore
{
public:
    SCORE_TYPE open_penalties[GAP_NSEGS]; ///< Penalties for creating an insertion gap in a sequence alignment.
    SCORE_TYPE extension_penalties[GAP_NSEGS]; ///< Penalties for extending an insertion gap by one position in a sequence alignment.
};


/// template specialization for affine gap score function
template<typename SCORE_TYPE>
struct GapScore<SCORE_TYPE, 1>
{
public:
    SCORE_TYPE open_score; ///< Penalty for creating an insertion gap in a sequence alignment
    SCORE_TYPE extension_score; ///< Penalty for extending an insertion gap by one position in a sequence alignment
};


/// Represents one column/residue in a sequence/alignment to be aligned.
/// Optimized for fast scoring during alignment.
/// Base class of QueryPosition and TargetPosition
template<typename SCORE_TYPE, ///< Type of score value, e.g. double, Information, etc.
         dp::DPAlignGapping DP_ALIGN_TYPE,
         int GAP_NSEGS ///< Number of piecewise segments in gap function (1 for affine)
         >
struct DPPosition;

/// Specialization of alignment column cache for alignment of alignments
template<typename SCORE_TYPE, ///< Type of score value, e.g. double, Information, etc.
         int GAP_NSEGS ///< Number of piecewise segments in gap function (1 for affine)
         >
struct DPPosition<SCORE_TYPE, dp::DP_ALIGN_GAPPED_ALIGNMENTS, GAP_NSEGS>
{
    SCORE_TYPE score(const DPPosition& rhs) const
    {
        SCORE_TYPE result = moltk::units::zero<SCORE_TYPE>();
        const DPPosition& lhs = *this;
        // nongap/nongap score
        typename QueryWeights::const_iterator i;
        const QueryWeights& queryWeights = rhs.query_residue_type_index_weights;
        for (i = queryWeights.begin();  i != queryWeights.end(); ++i)
        {
            double resTypeCount = i->second;
            size_t resTypeIndex = i->first;
            result += resTypeCount * lhs.target_scores_by_residue_type_index[resTypeIndex];
        }
        // gap/nongap score
        //   gap/nongap gap extension score
        result += lhs.extension_gap_score * rhs.nongap_count;
        result += rhs.extension_gap_score * lhs.nongap_count;
        //   gap/nongap gap open score
        //     (gap open score is computed in recurrence, where it belongs
        // gap/gap score is always zero, so requires no case here
        return result;
    }

    /// Gap opening component of alignment score after a match-match sequence
    SCORE_TYPE gap_open_after_match_score(const DPPosition& rhs) const
    {
        SCORE_TYPE result = moltk::units::zero<SCORE_TYPE>();
        const DPPosition& lhs = *this;
        std::map<int, Real>::const_iterator ci;
        // left with right...
        for (ci = lhs.insertion_close_lengths.begin(); ci != lhs.insertion_close_lengths.end(); ++ci)
        {
            typename std::map<int, SCORE_TYPE>::const_iterator ii = rhs.insertion_lengths.find(ci->first);
            if (ii != rhs.insertion_lengths.end())
            {
                // std::cout << "gap open " << lhs.index << ", " << rhs.index << ", " << ci->second << ", " << ii->second << endl;
                result += ci->second * ii->second;
            }
        }
        // ...plus right with left
        for (ci = rhs.insertion_close_lengths.begin(); ci != rhs.insertion_close_lengths.end(); ++ci)
        {
            typename std::map<int, SCORE_TYPE>::const_iterator ii = lhs.insertion_lengths.find(ci->first);
            if (ii != lhs.insertion_lengths.end())
            {
                // std::cout << "gap open " << lhs.index << ", " << rhs.index << ", " << ci->second << ", " << ii->second << endl;
                result += ci->second * ii->second;
            }
        }        
        return result;
    }

    /// Gap opening component of alignment score after a new insertion after right hand side position
    SCORE_TYPE gap_open_after_insertion_score(const DPPosition& rhs, int insertion_length) const
    {
        SCORE_TYPE result = moltk::units::zero<SCORE_TYPE>();
        const DPPosition& lhs = *this;
        std::map<int, Real>::const_iterator ci;
        // left with right...
        for (ci = lhs.insertion_close_lengths.begin(); ci != lhs.insertion_close_lengths.end(); ++ci)
        {
            int len = ci->first - insertion_length;  // include insertion length into comparison
            if (len < 0) continue;
            typename std::map<int, SCORE_TYPE>::const_iterator ii = rhs.insertion_lengths.find(len);
            if (ii != rhs.insertion_lengths.end())
            {
                // std::cout << "gap open " << lhs.index << ", " << rhs.index << ", " << ci->second << ", " << ii->second << endl;
                result += ci->second * ii->second;
            }
        }      
        return result;
    }

    GapScore<SCORE_TYPE, GAP_NSEGS> gap_score;
    size_t index; ///< column number of this position
    // query cache for score calculation
    typedef std::vector< std::pair<size_t, double> > QueryWeights;
    QueryWeights query_residue_type_index_weights;
    // target cache for score calculation
    std::vector<SCORE_TYPE> target_scores_by_residue_type_index;

    // Internal gap extension scores
    /// Number of gaps in column, weighted by sequence weights and end-gappiness
    SCORE_TYPE extension_gap_score;
    /// Number of non-gap residues, weighted by sequence weights
    Real nongap_count;

    // Internal gap open scores
    std::map<int, SCORE_TYPE> insertion_lengths; ///< count of sequences with gap of length n up through this position
    std::map<int, Real> insertion_close_lengths; ///< count of sequences with gap of length n-1 ending at previous position
};

/// Specialization of alignment column cache for alignment two individual sequences
template<typename SCORE_TYPE, ///< Type of score value, e.g. double, Information, etc.
         int GAP_NSEGS ///< Number of piecewise segments in gap function (1 for affine)
         >
struct DPPosition<SCORE_TYPE, dp::DP_ALIGN_UNGAPPED_SEQUENCES, GAP_NSEGS>
{
    SCORE_TYPE score(const DPPosition& rhs) const
    {
        SCORE_TYPE result = moltk::units::zero<SCORE_TYPE>();
        const DPPosition& lhs = *this;
        typename QueryWeights::const_iterator i;
        const QueryWeights& queryWeights = rhs.query_residue_type_index_weights;
        for (i = queryWeights.begin();  i != queryWeights.end(); ++i)
        {
            double resTypeCount = i->second;
            size_t resTypeIndex = i->first;
            result += resTypeCount * lhs.target_scores_by_residue_type_index[resTypeIndex];
        }
        return result;
    }

    GapScore<SCORE_TYPE, GAP_NSEGS> gap_score;
    size_t index; ///< column number of this position
    // query cache for score calculation
    // TODO - this is two sequence case, do don't need to cache all this stuff
    typedef std::vector< std::pair<size_t, double> > QueryWeights;
    QueryWeights query_residue_type_index_weights;
    // target cache for score calculation
    std::vector<SCORE_TYPE> target_scores_by_residue_type_index;
};

}} // namespace moltk::dp

#endif /* MOLTK_DPPOSITION_HPP_ */
