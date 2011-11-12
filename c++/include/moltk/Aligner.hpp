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
#include "moltk/MatrixScorer.hpp"
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


    // typedef dp::TargetPosition<Information, 1> TargetPosition;
    // typedef dp::QueryPosition<Information, 1> QueryPosition;

    /*! Scorer can convert, in O(m+n) time, dumb sequence and structure residues into TargetPositions
     * and QueryPositions, which can efficiently compute alignment scores.
     */
    /*
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
    */

public:
    // Finally, the actual Aligner methods
    Aligner();
    virtual ~Aligner();
    // Alignment align(const Alignment&);
    Alignment align(const Alignment&, const Alignment&);

    /// Whether alignment gaps before the start and after the end of a sequence are not penalized.
    bool get_end_gaps_free() const {return scorer.get_end_gaps_free();}
    /// Choose whether alignment gaps before the start and after the end of a sequence are not penalized.
    void set_end_gaps_free(bool f) {scorer.set_end_gaps_free(f);}
    /// MatrixScorer object this Aligner uses to score alignments.
    const MatrixScorer& get_scorer() const {return scorer;}
    /// MatrixScorer object this Aligner uses to score alignments.
    MatrixScorer& get_scorer() {return scorer;}
    Aligner& set_scorer(const MatrixScorer& scorer) {this->scorer = scorer; return *this;}

   /*!
     * Global shared aligner object used by align() method.
     */
    static Aligner& get_shared_aligner();

    /// The actual dynamic programming table
    dp::DPTable<Information, dp::DP_MEMORY_LARGE, dp::DP_ALIGN_UNGAPPED_SEQUENCES, 1> test_table;

protected:
    void init();
    Alignment compute_traceback();

    // Information gapOpenPenalty; // positive penalty (will be subtracted at gaps)
    // Information gapExtensionPenalty; // positive penalty (will be subtracted on extension)
    // bool bEndGapsFree;
    bool b_local_aligner;
    size_t m; // length of sequence 1
    size_t n; // length of sequence 2
    // DpTable dp_table;
    // std::vector<TargetPosition*> seq1;
    // std::vector<QueryPosition*> seq2;
    MatrixScorer scorer;
    Alignment output_alignment;
    Alignment query_alignment;
    Alignment target_alignment;

};

/*
 * global align() methods helps get SEQUOIA-like conciseness in python.
 */
Alignment align(const Alignment& target_alignment, const Alignment& query_alignment);

} // namespace moltk

#endif // MOLTK_ALIGN_ALIGNER_H

