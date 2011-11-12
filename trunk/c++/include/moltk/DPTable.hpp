/*
 * DPTable.hpp
 *
 *  Created on: Nov 7, 2011
 *      Author: brunsc
 */

#ifndef MOLTK_DPTABLE_HPP_
#define MOLTK_DPTABLE_HPP_

#include "moltk/DPPosition.hpp"
#include "moltk/MatrixScorer.hpp"
#include "moltk/Alignment.hpp"
#include "moltk/EString.hpp"
#include "moltk/units.hpp"
#include "moltk/dp_params.hpp"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <limits>

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


/// Generic accumulated score in a DPCell
template<typename SCORE_TYPE> struct RunningScore
{
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
    typedef DPPosition<SCORE_TYPE, 1> PositionType;
    /// Update from previous dynamic programming cell
    void compute_recurrence(
            const RunningGapScore& pred,
            const RunningScore<SCORE_TYPE>& v,
            const DPPosition<SCORE_TYPE, 1>& pos1,
            const DPPosition<SCORE_TYPE, 1>& pos2)
    {
        score = std::max(pred.score, v.score - pos1.gap_score.open_penalty)
                - pos1.gap_score.extension_penalty;
    }
    void initialize(const PositionType& pos1, const PositionType& pos2)
    {
        if (0 != pos1.index) {
            score = -moltk::units::infinity<SCORE_TYPE>();
            return;
        }
        score = -(moltk::Real)pos2.index * pos1.gap_score.extension_penalty
                -pos1.gap_score.open_penalty;
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
    typedef RunningScore<SCORE_TYPE> ScoreType;
    typedef DPPosition<SCORE_TYPE, GAP_NSEGS> TargetPosType;
    typedef DPPosition<SCORE_TYPE, GAP_NSEGS> QueryPosType;

    void initialize(const TargetPosType& pos1, const QueryPosType& pos2)
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
                            const DPPosition<SCORE_TYPE, GAP_NSEGS>& pos1,
                            const DPPosition<SCORE_TYPE, GAP_NSEGS>& pos2) // score for insertion gap in sequence 2
    {
        // This recurrence comes from Gusfield chapter 11.
        // best score with ungapped alignment of i with j
        g.score = up_left.v.score + pos1.score(pos2);
        // best score with insertion gap in sequence 1
        e.compute_recurrence(left.e, left.v, pos1, pos2);
        // best score with insertion gap in sequence 2
        f.compute_recurrence(up.f, up.v, pos2, pos1);
        // best score overall
        v.score = compute_v();
    }

    TracebackPointer compute_traceback_pointer() const
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
        TracebackPointer tp = compute_traceback_pointer();
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


template<typename SCORE_TYPE>
struct AlignmentResult
{
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
        result.score = table[i][j].v.score;
        // cout << "final alignment score = " << dp_table[i][j].v << endl;
        TracebackPointer tracebackPointer = table[i][j].compute_traceback_pointer();
        while( (i > 0) || (j > 0) )
        {
            // cout << "traceback[" << i << "][" << j << "]" << endl;
            switch(tracebackPointer)
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
            tracebackPointer = table[i][j].compute_traceback_pointer();
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
        target_positions = scorer->create_target_positions(s1);
        query_positions = scorer->create_query_positions(s2);
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

    /// Print out a summary of the positions and dynamic programming table for debugging
    inline friend std::ostream& operator<<(std::ostream& os, const DPTable& t)
    {
        // print out first/target sequence
        // Position index
        os << " position";
        for (size_t i = 0; i < t.target_positions.size(); ++i)
        { 
            os.width(7); os << i;
        } 
        os << endl;
        // Open penalty
        os << " gap open";
        for (size_t i = 0; i < t.target_positions.size(); ++i)
        { 
            os.width(7); 
            os << t.target_positions[i]->gap_score.open_penalty.value;
        } 
        os << endl;
        // Extension penalty
        os << "extension";
        for (size_t i = 0; i < t.target_positions.size(); ++i)
        { 
            os.width(7); 
            os << t.target_positions[i]->gap_score.extension_penalty.value;
        } 
        os << endl;

        os << t.table;
        return os;
    }

    /// Print out a summary of the dynamic programming table for debugging
    inline friend std::ostream& operator<<(std::ostream& os, const TableType& t)
    {
        size_t num_rows = t.size();
        if (num_rows < 1) return os;
        size_t num_cols = t[0].size();
        if (num_cols < 1) return os;

        enum CellField{V, G, E, F};
        os << "row\\col";
        for (size_t j = 0; j < num_cols; ++j)
        {
            // Top row of column indices
            os.width(6);
            os << j;
        }
        os << std::endl;
        os << std::endl;
        for(size_t i = 0; i < num_rows; ++i)
        {
            // Print out one row
            for (int e = V; e <= F; ++e)
            {
                // Print out one cell element
                switch(e) {
                    case V:
                        // row number
                        os.width(5);
                        os << i;
                        os.width(2);
                        os << 'V';
                        for (size_t j = 0; j < num_cols; ++j)
                        {
                            os.width(8);
                            os << t[i][j].v.score.value;
                        }
                        break;
                    case G:
                        os << "     ";
                        os.width(2);
                        os << 'G';
                        for (size_t j = 0; j < num_cols; ++j)
                        {
                            os.width(8);
                            os << t[i][j].g.score.value;
                        }
                        break;
                    case E:
                        os << "     ";
                        os.width(2);
                        os << 'E';
                        for (size_t j = 0; j < num_cols; ++j)
                        {
                            os.width(8);
                            os << t[i][j].e.score.value;
                        }
                        break;
                    case F:
                        os << "     ";
                        os.width(2);
                        os << 'F';
                        for (size_t j = 0; j < num_cols; ++j)
                        {
                            os.width(8);
                            os << t[i][j].f.score.value;
                        }
                        break;
                    default:
                        break;
                }
                os << std::endl; // end of row/element
            }
            os << std::endl; // empty line between rows
        }
        return os;
    }

    TableType table;
    std::vector<DPPosition<SCORE_TYPE, 1>*> query_positions;
    std::vector<DPPosition<SCORE_TYPE, 1>*> target_positions;
};


}} // namespace moltk::dp

#endif /* MOLTK_DPTABLE_HPP_ */
