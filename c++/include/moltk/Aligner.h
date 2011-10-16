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
    
    For questions contact: cmbruns@rotatingpenguin.com
*/

#ifndef MOLTK_ALIGN_ALIGNER_H
#define MOLTK_ALIGN_ALIGNER_H

#include <iostream>
#include <vector>
#include "moltk/Alignment.h"
#include "moltk/units.h"
#include "moltk/Biosequence.h"

namespace moltk {

/// Aligner is the class that creates sequence or structure alignments.
class Aligner 
{
public:
    // Inner classes and typedefs first, then Aligner methods.

    typedef moltk::units::Information Information;


    /// Aligner::Position represents a special biosequence residue that knows how to score itself during alignment.
    class QueryPosition // seq2
    {
        public:
            virtual QueryPosition* clone() const = 0;
            virtual ~QueryPosition() {}
            virtual moltk::units::Information gapOpenPenalty() const = 0;
            virtual moltk::units::Information gapExtensionPenalty() const = 0;
    };
    class TargetPosition // seq1
    {
        public:
            virtual TargetPosition* clone() const = 0;
            virtual ~TargetPosition() {}
            virtual moltk::units::Information score(const QueryPosition& rhs) const = 0;
            virtual moltk::units::Information gapOpenPenalty() const = 0;
            virtual moltk::units::Information gapExtensionPenalty() const = 0;
    };


    /// Aligner::Scorer converts dumb sequence and structure residues into Aligner::Positions, which
    /// know how to quickly score themselves with other AlignerPositions.
    class Scorer
    {
    public:
        Scorer() 
            : endGapsFree(true) 
            , defaultGapOpenPenalty(5.0 * moltk::units::bit)
            , defaultGapExtensionPenalty(0.5 * moltk::units::bit)
        {}
        virtual std::vector<QueryPosition*> createQueryPositions(const Alignment&) const = 0;
        virtual std::vector<TargetPosition*> createTargetPositions(const Alignment&) const = 0;
        bool getEndGapsFree() const {return endGapsFree;}
        void setEndGapsFree(bool f) {endGapsFree = f;}

    protected:
        bool endGapsFree;
        Information defaultGapOpenPenalty;
        Information defaultGapExtensionPenalty;
    };

    /// TracebackPointer is an Aligner::Cell attribute that helps reconstruct the final alignment.
    enum TracebackPointer
    {
        TRACEBACK_UP,
        TRACEBACK_UPLEFT,
        TRACEBACK_LEFT,
        TRACEBACK_DONE,
        TRACEBACK_NOT_INITIALIZED
    };


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


    // Aligner::Stage represents current state of an incomplete alignment
    enum Stage {
        EMPTY_STAGE, // have nothing
        SCORER_STAGE, // have a scorer
        SEQUENCE_STAGE, // have sequences
        ALLOCATED_STAGE, // have dynamic programming table
        TABLE_INITIALIZED_STAGE, // first edge of table initialized
        RECURRENCE_COMPUTED_STAGE, // recurrence computed
        TRACED_STAGE // alignment computed
    };


public:
    // Finally, the actual Aligner methods
    Aligner();
    virtual ~Aligner();
    // Alignment align(const Alignment&);
    Alignment align(const Alignment&, const Alignment&);

    static const Scorer& getDefaultScorer();
    bool getEndGapsFree() const {return scorer->getEndGapsFree();}
    void setEndGapsFree(bool f) {scorer->setEndGapsFree(f);}

protected:
    void init();
    void allocate_dp_table();
    void initialize_dp_table();
    void initialize_dp_row(size_t rowIndex, DpRow& row);
    void compute_recurrence();
    void compute_cell_recurrence(int i, int j);
    void compute_cell_recurrence_freeE(int i, int j);
    void compute_cell_recurrence_freeF(int i, int j);
    void compute_cell_recurrence_freeEF(int i, int j);
    Alignment compute_traceback();
    void clearPositions();
    void clearScorer();

    // Information gapOpenPenalty; // positive penalty (will be subtracted at gaps)
    // Information gapExtensionPenalty; // positive penalty (will be subtracted on extension)
    // bool bEndGapsFree;
    bool bLocalAligner;
    size_t m; // length of sequence 1
    size_t n; // length of sequence 2
    DpTable dpTable;
    std::vector<TargetPosition*> seq1;
    std::vector<QueryPosition*> seq2;
    Scorer* scorer;
    Alignment outputAlignment;
    Alignment queryAlignment;
    Alignment targetAlignment;
};

} // namespace moltk

#endif // MOLTK_ALIGN_ALIGNER_H

