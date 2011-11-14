/*
 * DPTable.hpp
 *
 *  Created on: Nov 7, 2011
 *      Author: brunsc
 */

#ifndef MOLTK_DPPOSITION_HPP_
#define MOLTK_DPPOSITION_HPP_

#include "moltk/units.hpp"
#include <vector>

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
    SCORE_TYPE open_penalty; ///< Penalty for creating an insertion gap in a sequence alignment
    SCORE_TYPE extension_penalty; ///< Penalty for extending an insertion gap by one position in a sequence alignment
};


/// Represents one column/residue in a sequence/alignment to be aligned.
/// Optimized for fast scoring during alignment.
/// Base class of QueryPosition and TargetPosition
template<typename SCORE_TYPE, ///< Type of score value, e.g. double, Information, etc.
         int GAP_NSEGS ///< Number of piecewise segments in gap function (1 for affine)
         >
struct DPPosition
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
    typedef std::vector< std::pair<size_t, double> > QueryWeights;
    QueryWeights query_residue_type_index_weights;
    // target cache for score calculation
    std::vector<SCORE_TYPE> target_scores_by_residue_type_index;
};


}} // namespace moltk::dp

#endif /* MOLTK_DPPOSITION_HPP_ */
