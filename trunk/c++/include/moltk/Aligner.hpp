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

#ifndef MOLTK_ALIGN_ALIGNER_H
#define MOLTK_ALIGN_ALIGNER_H

#include "moltk/DPTable.hpp"
#include "moltk/Alignment.hpp"
#include "moltk/units.hpp"
#include "moltk/Biosequence.hpp"
#include <iostream>
#include <vector>

namespace moltk {

/*! The Aligner class creates macromolecule sequence alignments and structure alignments.
 *
 */
class Aligner 
{
public:
    // Inner classes and typedefs first, then Aligner methods.

    typedef moltk::units::Information Information;


    typedef dp::TargetPosition<Information, 1> TargetPosition;
    typedef dp::QueryPosition<Information, 1> QueryPosition;

    /*!
     * QueryPosition is an alignment column specialized to efficiently compute
     * an alignment score with a TargetPosition.
     */
    /*
    class QueryPosition // seq2
    {
        public:
            virtual QueryPosition* clone() const = 0;
            virtual ~QueryPosition() {}
            virtual moltk::units::Information get_gap_open_penalty() const = 0;
            virtual moltk::units::Information get_gap_extension_penalty() const = 0;
    };
    */
    /*! TargetPosition is an alignment column specialized to efficiently compute
     *  an alignment score with a QueryPosition.
     */
    /*
    class TargetPosition // seq1
    {
        public:
            virtual TargetPosition* clone() const = 0;
            virtual ~TargetPosition() {}
            virtual moltk::units::Information score(const QueryPosition& rhs) const = 0;
            virtual moltk::units::Information get_gap_open_penalty() const = 0;
            virtual moltk::units::Information get_gap_extension_penalty() const = 0;
    };
    */


    /*! Scorer can convert, in O(m+n) time, dumb sequence and structure residues into TargetPositions
     * and QueryPositions, which can efficiently compute alignment scores.
     */
    class Scorer
    {
    public:
        Scorer() 
            : b_end_gaps_free(true) 
            , default_gap_open_penalty(8.0 * moltk::units::bit)
            , default_gap_extension_penalty(0.5 * moltk::units::bit)
        {}
        virtual std::vector<const QueryPosition*> create_query_positions(const Alignment&) const = 0;
        virtual std::vector<const TargetPosition*> create_target_positions(const Alignment&) const = 0;
        bool get_end_gaps_free() const {return b_end_gaps_free;}
        void set_end_gaps_free(bool f) {b_end_gaps_free = f;}
        /// Alignment score reduction for initiating an alignment gap.
        Information get_default_gap_open_penalty() const {return default_gap_open_penalty;}
        /// Set alignment score reduction for initiating an alignment gap.
        void set_default_gap_open_penalty(Information penalty) {default_gap_open_penalty = penalty;}
        /// Alignment score reduction for increasing the length of an alignment gap by one position.
        Information get_default_gap_extension_penalty() const {return default_gap_extension_penalty;}
        /// Set Alignment score reduction for increasing the length of an alignment gap by one position.
        void set_default_gap_extension_penalty(Information penalty) {default_gap_extension_penalty = penalty;}

    protected:
        bool b_end_gaps_free;
        Information default_gap_open_penalty;
        Information default_gap_extension_penalty;
    };

    /*
    /// TracebackPointer help reconstruct the final alignment.
    enum TracebackPointer
    {
        TRACEBACK_UPLEFT,
        TRACEBACK_UP,
        TRACEBACK_LEFT,
        TRACEBACK_DONE,
        TRACEBACK_NOT_INITIALIZED
    };
    */


    /*
    /// An Aligner::Cell is one node in the dynamic programming table
    class Cell
    {
    public:
        TracebackPointer compute_traceback_pointer() const;
        moltk::units::Information compute_v() const;

        // Gusfield nomenclature
        moltk::units::Information v; ///< V, best score through this cell
        moltk::units::Information g; ///< G, best ungapped score through this cell
        moltk::units::Information e; ///< E, best score with gap in sequence 1
        moltk::units::Information f; ///< F, best score with gap in sequence 2

        inline friend std::ostream& operator<<(std::ostream& os, const Cell& c)
        {
            os << "Cell(v=" << c.v << ", g=" << c.g;
            os << ", e=" << c.e << ", f=" << c.f << ")";
            return os;
        }
    };
    typedef std::vector<Cell> DpRow;
    typedef std::vector<DpRow> DpTable;
    */


    /// Aligner::Stage represents current state of an incomplete alignment
    enum Stage {
        STAGE_EMPTY, // have nothing
        STAGE_SCORER, // have a scorer
        STAGE_SEQUENCE, // have sequences
        STAGE_ALLOCATED, // have dynamic programming table
        STAGE_TABLE_INITIALIZED, // first edge of table initialized
        STAGE_RECURRENCE_COMPUTED, // recurrence computed
        STAGE_TRACED // alignment computed
    };


public:
    // Finally, the actual Aligner methods
    Aligner();
    virtual ~Aligner();
    // Alignment align(const Alignment&);
    Alignment align(const Alignment&, const Alignment&);

    /// Whether alignment gaps before the start and after the end of a sequence are not penalized.
    bool get_end_gaps_free() const {return scorer->get_end_gaps_free();}
    /// Choose whether alignment gaps before the start and after the end of a sequence are not penalized.
    void set_end_gaps_free(bool f) {scorer->set_end_gaps_free(f);}
    /// Scorer object this Aligner uses to score alignments.
    const Scorer& get_scorer() const {return *scorer;}
    /// Scorer object this Aligner uses to score alignments.
    Scorer& get_scorer() {return *scorer;}
    Aligner& set_scorer(Scorer& scorer) {this->scorer = &scorer; return *this;}

    static const Scorer& get_default_scorer();
   /*!
     * Global shared aligner object used by align() method.
     */
    static Aligner& get_shared_aligner();

protected:
    void init();
    void allocate_dp_table();
    void initialize_dp_table();
    // void initialize_dp_row(size_t rowIndex, DpRow& row);
    void compute_recurrence();
    // void compute_cell_recurrence(int i, int j);
    // void compute_cell_recurrence_freeE(int i, int j);
    // void compute_cell_recurrence_freeF(int i, int j);
    // void compute_cell_recurrence_freeEF(int i, int j);
    Alignment compute_traceback();
    // void clear_positions();
    void clear_scorer();

    // Information gapOpenPenalty; // positive penalty (will be subtracted at gaps)
    // Information gapExtensionPenalty; // positive penalty (will be subtracted on extension)
    // bool bEndGapsFree;
    bool b_local_aligner;
    size_t m; // length of sequence 1
    size_t n; // length of sequence 2
    // DpTable dp_table;
    // std::vector<TargetPosition*> seq1;
    // std::vector<QueryPosition*> seq2;
    Scorer* scorer;
    Alignment output_alignment;
    Alignment query_alignment;
    Alignment target_alignment;

    // temporary instantiation to help debug new template classes
    // dp::DPCell<Information, dp::DP_ALIGN_SINGLE_SEQUENCES, 1> test_cell;
    dp::DPTable<Information, dp::DP_MEMORY_LARGE, dp::DP_ALIGN_UNGAPPED_SEQUENCES, 1> test_table;
};

/*
 * global align() methods helps get SEQUOIA-like conciseness in python.
 */
Alignment align(const Alignment& target_alignment, const Alignment& query_alignment);

} // namespace moltk

#endif // MOLTK_ALIGN_ALIGNER_H

