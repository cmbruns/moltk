/*
 * DPTable.hpp
 *
 *  Created on: Nov 7, 2011
 *      Author: brunsc
 */

/*!
 * \file
 * Templated classes encapsulating dynamic programming sequence alignment algorithms.
 */


#ifndef MOLTK_DPTABLE_HPP_
#define MOLTK_DPTABLE_HPP_

#include "moltk/MatrixScorer.hpp"
#include "moltk/dp_params.hpp"
#include <stdexcept>
#include <cassert>

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


// (forward declaration for use by RunningScore)
/// Generic node in dynamic programming table
template<typename SCORE_TYPE, ///< Type of score value, e.g. double, Information, etc.
         DPAlignGapping ALIGN_TYPE, ///< Whether alignment operands have preexisting gaps or not
         int GAP_NSEGS ///< Number of piecewise segments in gap function (1 for affine)
         >
struct DPCell;


/// Accumulated score for a sub-alignment with aligned positions i,j.
/// Generic accumulated score in a DPCell
///
/// Corresponds to element G in Gusfield's chapter 11 recurrence.
template<typename SCORE_TYPE, DPAlignGapping ALIGN_TYPE>
struct RunningScore;


/// Template specialization for affine-gap alignment of single sequences running score
/// Corresponds to element G in Gusfield's chapter 11 recurrence.
template<typename SCORE_TYPE>
struct RunningScore<SCORE_TYPE, DP_ALIGN_UNGAPPED_SEQUENCES>
{
    template<int GAP_NSEGS>
    void compute_recurrence(
        const DPCell<SCORE_TYPE, DP_ALIGN_UNGAPPED_SEQUENCES, GAP_NSEGS>& up_left,
        const DPPosition<SCORE_TYPE, DP_ALIGN_UNGAPPED_SEQUENCES, GAP_NSEGS>& pos1,
        const DPPosition<SCORE_TYPE, DP_ALIGN_UNGAPPED_SEQUENCES, GAP_NSEGS>& pos2)
    {
        score = up_left.v.score + pos1.score(pos2);
    }

    SCORE_TYPE score;
};


/// Non-localness of gap_open scores requires that we store second-order
/// traceback pointers.  ScoreTrio class helps manage that second level.
/// Set of scores from three paths leading to a particular DPCell element
template<typename SCORE_TYPE>
struct ScoreTrio
{
    SCORE_TYPE from_g;
    SCORE_TYPE from_e;
    SCORE_TYPE from_f;

    SCORE_TYPE max() const
    {
        if ((from_g >= from_e) && (from_g >= from_f))
            return from_g;
        if (from_e >= from_f)
            return from_e;
        return from_f;
    }

    TracebackPointer upstream_traceback_pointer() const
    {
        if ((from_g >= from_e) && (from_g >= from_f))
            return TRACEBACK_UPLEFT;
        if (from_e >= from_f)
            return TRACEBACK_LEFT;
        return TRACEBACK_UP;
    }
};


/// Template specialization affine-gap alignment-of-alignments running score
/// Corresponds to element G in Gusfield's chapter 11 recurrence.
template<typename SCORE_TYPE>
struct RunningScore<SCORE_TYPE, DP_ALIGN_GAPPED_ALIGNMENTS>
{
    template<int GAP_NSEGS>
    void compute_recurrence(
        const DPCell<SCORE_TYPE, DP_ALIGN_GAPPED_ALIGNMENTS, GAP_NSEGS>& up_left,
        const DPPosition<SCORE_TYPE, DP_ALIGN_GAPPED_ALIGNMENTS, GAP_NSEGS>& pos1,
        const DPPosition<SCORE_TYPE, DP_ALIGN_GAPPED_ALIGNMENTS, GAP_NSEGS>& pos2)
    {
        score = calc_upstream_scores(up_left, pos1, pos2).max();
    }

    template<int GAP_NSEGS>
    ScoreTrio<SCORE_TYPE> calc_upstream_scores(
            const DPCell<SCORE_TYPE, DP_ALIGN_GAPPED_ALIGNMENTS, GAP_NSEGS>& up_left,
            const DPPosition<SCORE_TYPE, DP_ALIGN_GAPPED_ALIGNMENTS, GAP_NSEGS>& pos1,
            const DPPosition<SCORE_TYPE, DP_ALIGN_GAPPED_ALIGNMENTS, GAP_NSEGS>& pos2) const
    {
        ScoreTrio<SCORE_TYPE> result;
        SCORE_TYPE s = pos1.score(pos2);
        // TODO - incorporate open-after-match directly into result.from_g
        if (up_left.v.score == up_left.g.score) // match-match state
           s += pos1.gap_open_after_match_score(pos2);
        result.from_g = up_left.g.score + pos1.score(pos2) + pos1.gap_open_after_match_score(pos2);
        result.from_e = up_left.e.score + pos1.score(pos2);
        result.from_f = up_left.f.score + pos1.score(pos2);

        return result;
    }

    SCORE_TYPE score;
};


/// Accumulated score for an alignment with a new insertion gap in one sequence.
///
/// Intended to encapsulate all logic associated with GAP_NSEGS and AlignType
/// Corresponds to element E or F in Gusfield's chapter 11 recurrence.
template<typename SCORE_TYPE, DPAlignGapping ALIGN_TYPE, int GAP_NSEGS>
struct RunningGapScore;


/// Template specialization for single sequence affine gap running score (E or F in Gusfield recurrence)
template<typename SCORE_TYPE>
struct RunningGapScore<SCORE_TYPE, DP_ALIGN_UNGAPPED_SEQUENCES, 1>
{
    typedef DPPosition<SCORE_TYPE, DP_ALIGN_UNGAPPED_SEQUENCES, 1> PositionType;
    /// Update from previous dynamic programming cell
    void compute_recurrence(
            const RunningGapScore& pred,
            const RunningScore<SCORE_TYPE, DP_ALIGN_UNGAPPED_SEQUENCES>& v,
            const PositionType& pos1,
            const PositionType& pos2)
    {
        score = std::max(pred.score, v.score + pos1.gap_score.open_score)
                + pos1.gap_score.extension_score;
    }

    void initialize(const PositionType& pos1, const PositionType& pos2)
    {
        if (0 != pos1.index) {
            score = -moltk::units::infinity<SCORE_TYPE>();
            return;
        }
        score = (moltk::Real)pos2.index * pos1.gap_score.extension_score
                + pos1.gap_score.open_score;
    }

    SCORE_TYPE score;
};


/// Template specialization for alignment of alignments affine gap running score (E or F in Gusfield recurrence)
template<typename SCORE_TYPE>
struct RunningGapScore<SCORE_TYPE, DP_ALIGN_GAPPED_ALIGNMENTS, 1>
{
    typedef DPPosition<SCORE_TYPE, DP_ALIGN_GAPPED_ALIGNMENTS, 1> PositionType;
    /// Update from previous dynamic programming cell
    void compute_recurrence(
            const RunningGapScore& pred,
            const RunningScore<SCORE_TYPE, DP_ALIGN_GAPPED_ALIGNMENTS>& v,
            const PositionType& pos1,
            const PositionType& pos2)
    {
        score = std::max(pred.score, v.score + pos1.gap_score.open_score)
                + pos1.gap_score.extension_score * pos2.nongap_count;
    }

    void initialize(const PositionType& pos1, const PositionType& pos2)
    {
        if (0 != pos1.index) {
            score = -moltk::units::infinity<SCORE_TYPE>();
            return;
        }
        score = (moltk::Real)pos2.index * pos1.gap_score.extension_score
                + pos1.gap_score.open_score;
    }

    SCORE_TYPE score;
};


/// Generic node in dynamic programming table
template<typename SCORE_TYPE, ///< Type of score value, e.g. double, Information, etc.
         DPAlignGapping ALIGN_TYPE, ///< Whether alignment operands have preexisting gaps or not
         int GAP_NSEGS ///< Number of piecewise segments in gap function (1 for affine)
         >
struct DPCell
{
    typedef RunningGapScore<SCORE_TYPE, ALIGN_TYPE, GAP_NSEGS> GapScoreType;
    typedef RunningScore<SCORE_TYPE, ALIGN_TYPE> ScoreType;
    typedef DPPosition<SCORE_TYPE, ALIGN_TYPE, GAP_NSEGS> PositionType;

    void initialize(const PositionType& pos1, const PositionType& pos2)
    {
        // only initialize top row and column
        if ((pos1.index != 0) && (pos2.index != 0))
            throw std::runtime_error("Only top and left of dynamic programming table can be initialized");
        e.initialize(pos1, pos2);
        f.initialize(pos2, pos1);
        if ((pos1.index == 0) && (pos2.index == 0))
            g.score = moltk::units::zero<SCORE_TYPE>();
        else
            g.score = -moltk::units::infinity<SCORE_TYPE>();
        v.score = compute_v();
    }

    void compute_recurrence(const DPCell& up_left,
                            const DPCell& up,
                            const DPCell& left,
                            const PositionType& pos1,
                            const PositionType& pos2) // score for insertion gap in sequence 2
    {
        // This recurrence comes from Gusfield chapter 11.
        // best score with ungapped alignment of i with j
        g.compute_recurrence(up_left, pos1, pos2);
        // best score with insertion gap in sequence 1
        e.compute_recurrence(left.e, left.v, pos1, pos2);
        // best score with insertion gap in sequence 2
        f.compute_recurrence(up.f, up.v, pos2, pos1);
        // best score overall
        v.score = compute_v();
    }

    TracebackPointer compute_traceback_pointer(TracebackPointer previous_pointer) const
    {
        if ( (g.score >= e.score) && (g.score >= f.score) )
            return TRACEBACK_UPLEFT;
        else if (e.score >= f.score)
            return TRACEBACK_LEFT;
        else
            return TRACEBACK_UP;
    }

    SCORE_TYPE compute_v() const
    {
        TracebackPointer tp = compute_traceback_pointer(TRACEBACK_NONE);
        switch(tp)
        {
        case TRACEBACK_UPLEFT:
            return g.score;
            break;
        case TRACEBACK_UP:
            return f.score;
            break;
        case TRACEBACK_LEFT:
            return e.score;
            break;
        }
        return e.score;
    }

    // operator== needed for vector storage
    // TODO
    bool operator==(const DPCell& rhs) const
    {
        if (v.score != rhs.v.score) return false;
        if (g.score != rhs.g.score) return false;
        if (e.score != rhs.e.score) return false;
        if (f.score != rhs.f.score) return false;
        return true;
    }
    bool operator<(const DPCell& rhs) const
    {
        return (v.score < rhs.v.score);
    }

    // Gusfield nomenclature
    ScoreType v; ///< V, best score through this cell
    ScoreType g; ///< G, best ungapped score through this cell
    GapScoreType e; ///< E, best score with gap in sequence 1
    GapScoreType f; ///< F, best score with gap in sequence 2
};


/// Generic dynamic programming table (not implemented yet...)
template<typename SCORE_TYPE, ///< Type of score value, e.g. double, Information, etc.
         DPMemoryModel MEMORY_MODEL, ///< Whether to use slower small memory alignment
         DPAlignGapping ALIGN_TYPE, ///< Whether alignment operands have preexisting gaps or not
         int GAP_NSEGS ///< Number of piecewise segments in gap function (1 for affine)
         >
struct DPTable;


/// Gap structure and score of a pairwise sequence alignment, 
/// used as an intermediate value during sequence alignment.
template<typename SCORE_TYPE>
struct AlignmentResult
{
    AlignmentResult() {score = moltk::units::zero<SCORE_TYPE>();}
    moltk::EString eString1;
    moltk::EString eString2;
    SCORE_TYPE score;
};


/// template specialization of DPTable for Affine alignment in large memory.
template<typename SCORE_TYPE, DPAlignGapping ALIGN_TYPE>
struct DPTable<SCORE_TYPE, DP_MEMORY_LARGE, ALIGN_TYPE, 1>
{
    typedef DPCell<SCORE_TYPE, ALIGN_TYPE, 1> CellType;
    typedef std::vector<CellType> RowType;
    typedef std::vector<RowType> TableType;
    typedef AlignmentResult<SCORE_TYPE> AlignmentResultType;

    ~DPTable() {clear_positions();}

    void clear_positions()
    {
        for (size_t i = 0; i < target_positions.size(); ++i)
        {
            delete target_positions[i];
            target_positions[i] = NULL;
        }
        target_positions.clear();
        for (size_t i = 0; i < query_positions.size(); ++i)
        {
            delete query_positions[i];
            query_positions[i] = NULL;
        }
        query_positions.clear();
    }

    void allocate()
    {
        // TODO - small memory version allocates just 2 or 3 rows.
        table.assign(num_rows(), RowType(num_columns()));
    }

    void initialize()
    {
        if (num_rows() < 1) return;
        if (num_columns() < 1) return;
        // top left corner
        table[0][0].initialize(*target_positions[0],
                               *query_positions[0]);
        // Left column
        for(size_t i = 1; i < num_rows(); ++i)
            table[i][0].initialize(*target_positions[i],
                                   *query_positions[0]);
        // Top row
        for(size_t j = 1; j < num_columns(); ++j)
            table[0][j].initialize(*target_positions[0],
                                   *query_positions[j]);
    }

    void compute_recurrence()
    {
        for (size_t i = 1; i < num_rows(); ++i)
            for (size_t j = 1; j < num_columns(); ++j)
            {
                table[i][j].compute_recurrence(
                        table[i-1][j-1], // up_left
                        table[i-1][j], // up
                        table[i][j-1], // left
                        *target_positions[i],
                        *query_positions[j]);
            }
    }

    AlignmentResultType compute_traceback()
    {
        AlignmentResultType result;
        int i = num_rows() - 1;
        int j = num_columns() - 1;
        if (i < 0) return result;
        if (j < 0) return result;
        result.score = table[i][j].v.score;
        // cout << "final alignment score = " << dp_table[i][j].v << endl;
        TracebackPointer previous_pointer = TRACEBACK_NONE;
        TracebackPointer traceback_pointer = table[i][j].compute_traceback_pointer(previous_pointer);
        while( (i > 0) || (j > 0) )
        {
            // cout << "traceback[" << i << "][" << j << "]" << endl;
            switch(traceback_pointer)
            {
            case TRACEBACK_UPLEFT:
                --i; --j;
                result.eString1.append_run(1);
                result.eString2.append_run(1);
                // cout << "upleft" << endl;
                break;
            case TRACEBACK_UP:
                --i;
                result.eString1.append_run(1);
                result.eString2.append_run(-1);
                // cout << "up" << endl;
                break;
            case TRACEBACK_LEFT:
                --j;
                result.eString1.append_run(-1);
                result.eString2.append_run(1);
                // cout << "left" << endl;
                break;
            default:
                std::cerr << "Traceback error!" << std::endl;
                assert(false);
                break;
            }
            TracebackPointer p = previous_pointer;
            previous_pointer = traceback_pointer;
            traceback_pointer = table[i][j].compute_traceback_pointer(p);
        }
        assert(i == 0);
        assert(j == 0);
        // OK, the sequences are actually backwards here
        result.eString1.reverse();
        result.eString2.reverse();
        return result;
    }

    AlignmentResultType align()
    {
        allocate();
        initialize();
        compute_recurrence();
        return compute_traceback();
    }

    moltk::Alignment_<SCORE_TYPE> align(const moltk::Alignment_<SCORE_TYPE>& s1, 
                                 const moltk::Alignment_<SCORE_TYPE>& s2,
                                 const moltk::MatrixScorer& scorer)
    {
        clear_positions();
        scorer.create_positions(target_positions, s1);
        scorer.create_positions(query_positions, s2);
        AlignmentResult<SCORE_TYPE> alignment_result = align();
        moltk::Alignment_<SCORE_TYPE> result = 
            s1.align(s2, 
                     alignment_result.eString1, 
                     alignment_result.eString2);
        result.set_score(result.get_score() + alignment_result.score);
        return result;
    }

    size_t num_rows() const {return target_positions.size();}
    size_t num_columns() const {return query_positions.size();}

    TableType table;
    std::vector<DPPosition<SCORE_TYPE, ALIGN_TYPE, 1>*> query_positions;
    std::vector<DPPosition<SCORE_TYPE, ALIGN_TYPE, 1>*> target_positions;
};

template<typename SCORE_TYPE, DPMemoryModel MEMORY_MODEL, DPAlignGapping ALIGN_TYPE, int GAP_NSEGS>
std::ostream& operator<<(std::ostream& os, 
        const moltk::dp::DPTable<SCORE_TYPE,MEMORY_MODEL,ALIGN_TYPE,GAP_NSEGS>& t);

template<typename SCORE_TYPE,DPAlignGapping ALIGN_TYPE,int GAP_NSEGS>
std::ostream& operator<<(std::ostream& os, 
        const std::vector<std::vector<moltk::dp::DPCell<SCORE_TYPE, ALIGN_TYPE, GAP_NSEGS> > >&);

}} // namespace moltk::dp

#endif /* MOLTK_DPTABLE_HPP_ */
