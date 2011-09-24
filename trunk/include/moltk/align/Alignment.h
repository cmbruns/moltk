#ifndef MOLTK_ALIGN_ALIGNMENT_H
#define MOLTK_ALIGN_ALIGNMENT_H

#include <iostream>
#include <vector>
#include "Sequence.h"
#include "moltk/units.h"
#include "moltk/fasta.h"
#include "Scorer.h"

namespace moltk { namespace align {

class Alignment 
{
public:
    typedef moltk::units::Information Information;

    Alignment();
    Alignment(const Sequence&, const Sequence&);
    Alignment(const moltk::FastaSequence&, 
              const moltk::FastaSequence&,
              const Scorer& scorer = getDefaultScorer());
    void align();

    static const Scorer& getDefaultScorer();

    class Cell
    {
    public:
        // Gusfield nomenclature
        Information s; // Wm, score of aligning position S1(i) with S2(j)
        Information v; // V, best score through this cell
        Information g; // G, best ungapped score through this cell
        Information e; // E, 
        Information f; // F, 
    };
    typedef std::vector<Cell> DpRow;
    typedef std::vector<DpRow> DpTable;

    enum Stage {
        EMPTY_STAGE,
        SEQUENCE_STAGE, // have sequences
        ALLOCATED_STAGE, // have dynamic programming table
        TABLE_INITIALIZED_STAGE, // first edge of table initialized
        RECURRENCE_COMPUTED_STAGE, // recurrence computed
        TRACED_STAGE // alignment computed
    };

protected:
    void init();
    void init(const Sequence& seq1Param, const Sequence& seq2Param);
    void allocate_dp_table();
    void initialize_dp_table();
    void initialize_dp_row(size_t rowIndex, DpRow& row);
    void compute_recurrence();
    void compute_traceback();

    Information gapOpenPenalty; // positive penalty (will be subtracted at gaps)
    Information gapExtensionPenalty; // positive penalty (will be subtracted on extension)
    bool bEndGapsFree;
    bool bLocalAlignment;
    size_t m; // length of sequence 1
    size_t n; // length of sequence 2
    DpTable dpTable;
    Sequence seq1;
    Sequence seq2;
    Scorer* scorer;
};

}} // namespace moltk::align

std::ostream& operator<<(std::ostream&, const moltk::align::Alignment&);

#endif // MOLTK_ALIGN_ALIGNMENT_H
