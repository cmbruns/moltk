/*
 * dp_params.hpp
 *
 */

/*!
 * \file
 * Defines options that affect the details of the dynamic programming sequence alignment algorithm.
 */


#ifndef MOLTK_DP_PARAMS_HPP_
#define MOLTK_DP_PARAMS_HPP_

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
enum DPAlignGapping {
    DP_ALIGN_UNGAPPED_SEQUENCES, ///< Alignment with no preexisting gaps can be faster and truly optimal
    DP_ALIGN_GAPPED_ALIGNMENTS ///< Alignment of pregapped sequences is slower and heuristic (problem is NP-hard)
};


/// Global vs. local alignment
enum DPAlignScale {
    DP_ALIGN_GLOBAL, ///< Global alignment (Needleman Wunsch)
    DP_ALIGN_LOCAL, ///< Local alignment (Smith Waterman)
};


/// TracebackPointer to help reconstruct the final alignment.
enum TracebackPointer
{
    TRACEBACK_UPLEFT, ///< consume/align one column/residue from each of both alignments/sequences
    TRACEBACK_UP, ///< consume/delete one column/residue of alignment/sequence 1
    TRACEBACK_LEFT, ///< consume/delete one column/residue of alignment/sequence 2
    TRACEBACK_NONE ///< unknown or non applicable
};


}} // namespace moltk::dp

#endif /* MOLTK_DP_PARAMS_HPP_ */
