/*
 * DPTable.hpp
 *
 *  Created on: Nov 7, 2011
 *      Author: brunsc
 */

#ifndef MOLTK_DPTABLE_HPP_
#define MOLTK_DPTABLE_HPP_

#include <vector>

namespace moltk {
/// Namespace for dynamic programming classes.
/// 
/// Many of theses classes use hard-to-read C++ template specializations,
/// because we are trying to avoid the overhead of virtual functions and
/// and branching logic in the time-critical inner loop of dynamic programming.
namespace dp {

// Various options that affect the details of the
// dynamic programming alignment algorithm

/// Fast O(n^2) memory use vs. 2X slower O(n) memory use.
enum DPMemoryModel {
    DP_MEMORY_LARGE, ///< Faster O(n^2) memory model
    DP_MEMORY_SMALL ///< 2X slower O(n) memory model
};


/// Whether alignment operands have preexisting gaps (e.g. alignments) or not (e.g. single sequences)
enum DPAlignType {
    DP_ALIGN_SINGLE_SEQUENCES, ///< Alignment with no preexisting gaps can be faster and truly optimal
    DP_ALIGN_ALIGNMENTS ///< Alignment of pregapped sequences is slower and heuristic (problem is NP-hard)
};


/// Global vs. local alignment
enum DPAlignScale {
    DP_ALIGN_GLOBAL, ///< Global alignment (Needleman Wunsch)
    DP_ALIGN_LOCAL, ///< Local alignment (Smith Waterman)
};


/// TracebackPointer to help reconstruct the final alignment.
enum TracebackPointer
{
    TRACEBACK_UPLEFT, // consume one column/residue from each of both alignments/sequences
    TRACEBACK_UP, // consume one column/residue of alignment/sequence 1
    TRACEBACK_LEFT // consume one column/residue of alignment/sequence 2
};


// Optimize speed by using template specializations to avoid
// virtual methods and extra logic in inner loop of dynamic
// programming algorithms.  (Most of the permutations are probably
// not implemented yet...)


/// Parameters for scoring indel gaps in sequence alignments
template<typename SCORE_TYPE, ///< Type of score value, e.g. double, Information, etc.
         int GAP_NSEGS ///< Number of piecewise segments in gap function (1 for affine)
         >
struct GapScorer
{
public:
    SCORE_TYPE open_penalties[GAP_NSEGS]; ///< Penalties for creating an insertion gap in a sequence alignment.
    SCORE_TYPE extension_penalties[GAP_NSEGS]; ///< Penalties for extending an insertion gap by one position in a sequence alignment.
};


/// template specialization for affine gap score function
template<typename SCORE_TYPE>
struct GapScorer<SCORE_TYPE, 1>
{
public:
    SCORE_TYPE open_penalty; ///< Penalty for creating an insertion gap in a sequence alignment
    SCORE_TYPE extension_penalty; ///< Penalty for extending an insertion gap by one position in a sequence alignment
};


/// Accumulated score for an alignment with a new insertion gap in one sequence.
///
/// Intended to encapsulate all logic associated with GAP_NSEGS and AlignType
/// Corresponds to element E or F in Gusfield's chapter 11 recurrence.
template<typename SCORE_TYPE, DPAlignType ALIGN_TYPE, int GAP_NSEGS>
struct RunningGapScore;


/// Template specialization for single sequence affine gap running score (E or F in Gusfield recurrence)
template<typename SCORE_TYPE>
struct RunningGapScore<SCORE_TYPE, DP_ALIGN_SINGLE_SEQUENCES, 1>
{
    /// Update from previous dynamic programming cell
    void compute_recurrence(const RunningGapScore& pred, const SCORE_TYPE& v, const GapScorer<SCORE_TYPE, 1>& gap_scorer)
    {
        score = std::max(pred.score, v - gap_scorer.open_penalty)
                - gap_scorer.extension_penalty;
    }
    SCORE_TYPE score;
};


/// Generic node in dynamic programming table
template<typename SCORE_TYPE, ///< Type of score value, e.g. double, Information, etc.
         DPAlignType ALIGN_TYPE, ///< Whether alignment operands have preexisting gaps or not
         int GAP_NSEGS ///< Number of piecewise segments in gap function (1 for affine)
         >
struct DPCell
{
    typedef RunningGapScore<SCORE_TYPE, ALIGN_TYPE, GAP_NSEGS> GapScoreType;

    void compute_recurrence(const DPCell& left,
                            const DPCell& up,
                            const DPCell& left_up,
                            const SCORE_TYPE& score, // match score of positions i and j
                            const GapScorer<SCORE_TYPE, GAP_NSEGS>& gap_score1, // score for insertion gap in sequence 1
                            const GapScorer<SCORE_TYPE, GAP_NSEGS>& gap_score2) // score for insertion gap in sequence 2
    {
        // This recurrence comes from Gusfield chapter 11.
        // best score with ungapped alignment of i with j
        g = left_up.v + score;
        // best score with insertion gap in sequence 1
        e.compute_recurrence(left.e, left.v, gap_score1);
        // best score with insertion gap in sequence 2
        f.compute_recurrence(up.f, up.v, gap_score2);
        // best score overall
        v = compute_v();
    }

    TracebackPointer compute_traceback_pointer() const
    {
        if ( (g >= e.score) && (g >= f.score) )
            return TRACEBACK_UPLEFT;
        else if (e.score >= f.score)
            return TRACEBACK_LEFT;
        else
            return TRACEBACK_UP;
    }

    SCORE_TYPE compute_v() const
    {
        TracebackPointer tp = compute_traceback_pointer();
        switch(tp)
        {
        case TRACEBACK_UPLEFT:
            return g;
        case TRACEBACK_LEFT:
            return e.score;
        default:
            return f.score;
        }
    }

    // Gusfield nomenclature
    SCORE_TYPE v; ///< V, best score through this cell
    SCORE_TYPE g; ///< G, best ungapped score through this cell
    GapScoreType e; ///< E, best score with gap in sequence 1
    GapScoreType f; ///< F, best score with gap in sequence 2
};


/// Generic dynamic programming table (not implemented yet...)
template<typename SCORE_TYPE, ///< Type of score value, e.g. double, Information, etc.
         DPMemoryModel MEMORY_MODEL, ///< Whether to use slower small memory alignment
         DPAlignType ALIGN_TYPE, ///< Whether alignment operands have preexisting gaps or not
         int GAP_NSEGS ///< Number of piecewise segments in gap function (1 for affine)
         >
struct DPTable;


/// template specialization of DPTable for Affine alignment in large memory.
template<typename SCORE_TYPE, DPAlignType ALIGN_TYPE>
struct DPTable<SCORE_TYPE, DP_MEMORY_LARGE, ALIGN_TYPE, 1>
{
    typedef DPCell<SCORE_TYPE, ALIGN_TYPE, 1> CellType;
    typedef std::vector<CellType> RowType;
    typedef std::vector<RowType> TableType;

    void allocate(int number_of_rows, int number_of_columns)
    {
        table.assign(number_of_rows, RowType(number_of_columns));
    }
    // TODO - initialize, compute_recurrence, compute_traceback

    TableType table;
};


}} // namespace moltk::dp

#endif /* MOLTK_DPTABLE_HPP_ */
