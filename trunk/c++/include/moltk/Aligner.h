#ifndef MOLTK_ALIGN_ALIGNER_H
#define MOLTK_ALIGN_ALIGNER_H

#include <iostream>
#include <vector>
#include "Alignment.h"
#include "moltk/units.h"
#include "moltk/FastaSequence.h"

namespace moltk {

/// Aligner is the class that creates sequence or structure alignments.
class Aligner 
{
public:
    // Inner classes and typedefs first, then Aligner methods.

    typedef moltk::units::Information Information;


    /// Aligner::Position represents a special biosequence residue that knows how to score itself during alignment.
    class Position
    {
        public:
            virtual Position* clone() const = 0;
            virtual ~Position() {}
            virtual moltk::units::Information score(const Position& rhs) const = 0;
            virtual char getOneLetterCode() const = 0;
    };


    /// Aligner::PositionList is a special sequence containing Aligner::Positions,
    /// which know how to score themselves.
    class PositionList : public std::vector<Position*>
    {
    public:
        PositionList() {}
        PositionList(const PositionList& rhs);
        virtual ~PositionList();
        PositionList& operator=(const PositionList& rhs);
    };


    /// Aligner::Scorer converts dumb sequence and structure residues into Aligner::Positions, which
    /// know how to quickly score themselves with other AlignerPositions.
    class Scorer
    {
    public:
        virtual Aligner::Position* createPosition(char sequenceLetter) const = 0;
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
        // Gusfield nomenclature
        // TODO - remove s, which does not need to be stored
        /// s is the score of aligning position S1(i) with S2(j); (Wm in Gusfield's nomenclature.)
        Information s; ///< Wm, score of aligning position S1(i) with S2(j)
        Information v; ///< V, best score through this cell
        Information g; ///< G, best ungapped score through this cell
        Information e; ///< E, best score with gap in sequence 1
        Information f; ///< F, best score with gap in sequence 2
        TracebackPointer tracebackPointer; ///< breadcrumb to help reconstruct the final alignment
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
    // Alignment align(const FastaSequence&);
    // Alignment align(const Alignment&);
    Alignment align(const FastaSequence&, const FastaSequence&);

    static const Scorer& getDefaultScorer();
    bool getEndGapsFree() const {return bEndGapsFree;}
    void setEndGapsFree(bool f) {bEndGapsFree = f;}

protected:
    void init();
    void init(const PositionList& seq1Param, const PositionList& seq2Param);
    void allocate_dp_table();
    void initialize_dp_table();
    void initialize_dp_row(size_t rowIndex, DpRow& row);
    void compute_recurrence();
    Alignment compute_traceback();

    Information gapOpenPenalty; // positive penalty (will be subtracted at gaps)
    Information gapExtensionPenalty; // positive penalty (will be subtracted on extension)
    bool bEndGapsFree;
    bool bLocalAligner;
    size_t m; // length of sequence 1
    size_t n; // length of sequence 2
    DpTable dpTable;
    PositionList seq1;
    PositionList seq2;
    Scorer* scorer;
    Alignment alignment;
};

} // namespace moltk

#endif // MOLTK_ALIGN_ALIGNER_H

