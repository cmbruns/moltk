/*
 * DPTable.cpp
 *
 *  Created on: Nov 14, 2011
 *      Author: brunsc
 */

#include "moltk/DPTable.hpp"

using moltk::units::Information;

namespace moltk { namespace dp {

/// Print out a summary of the dynamic programming table for debugging
template<typename SCORE_TYPE,DPAlignGapping ALIGN_TYPE,int GAP_NSEGS>
std::ostream& operator<<(std::ostream& os, 
        const std::vector<std::vector<moltk::dp::DPCell<SCORE_TYPE, ALIGN_TYPE, GAP_NSEGS> > >& t)
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

/// Print out a summary of the positions and dynamic programming table for debugging
template<typename SCORE_TYPE, DPMemoryModel MEMORY_MODEL, DPAlignGapping ALIGN_TYPE, int GAP_NSEGS>
std::ostream& operator<<(std::ostream& os, 
        const moltk::dp::DPTable<SCORE_TYPE,MEMORY_MODEL,ALIGN_TYPE,GAP_NSEGS>& t)
{
    // print out first/target sequence
    // Position index
    os << " position";
    for (size_t i = 0; i < t.target_positions.size(); ++i)
    {
        os.width(7); os << i;
    }
    os << std::endl;
    // Open score
    os << " gap open";
    for (size_t i = 0; i < t.target_positions.size(); ++i)
    {
        os.width(7);
        os << t.target_positions[i]->gap_score.open_score.value;
    }
    os << std::endl;
    // Extension score
    os << "extension";
    for (size_t i = 0; i < t.target_positions.size(); ++i)
    {
        os.width(7);
        os << t.target_positions[i]->gap_score.extension_score.value;
    }
    os << std::endl;

    os << t.table;
    return os;
}

/*
template std::ostream& operator<<(
     std::ostream& os, 
     const DPTable<Information,DP_MEMORY_LARGE,DP_ALIGN_UNGAPPED_SEQUENCES,1>::TableType& t);
*/

// Instantiate
template std::ostream& operator<<(
     std::ostream& os, 
     const moltk::dp::DPTable<moltk::units::Information, moltk::dp::DP_MEMORY_LARGE, moltk::dp::DP_ALIGN_UNGAPPED_SEQUENCES, 1>& t);

template std::ostream& operator<<(
     std::ostream& os, 
     const moltk::dp::DPTable<moltk::units::Information, moltk::dp::DP_MEMORY_LARGE, moltk::dp::DP_ALIGN_GAPPED_ALIGNMENTS, 1>& t);

}} // namespace moltk::dp
