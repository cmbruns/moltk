#ifndef MOLTK_ALIGN_ALIGNER_H
#define MOLTK_ALIGN_ALIGNER_H

#include <iostream>
#include <vector>
#include "Alignable.h"
#include "moltk/units.h"
#include "moltk/fasta.h"
#include "Alignment.h"
#include "Scorer.h"

namespace moltk { namespace align {

class Aligner 
{
public:
    typedef moltk::units::Information Information;

    Aligner();
    Alignment& align(const FastaSequence&);
    Alignment& align(const Alignment&);
    Alignment& align(const FastaSequence&, const FastaSequence&);

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
    void init(const Alignable& seq1Param, const Alignable& seq2Param);
    void allocate_dp_table();
    void initialize_dp_table();
    void initialize_dp_row(size_t rowIndex, DpRow& row);
    void compute_recurrence();
    void compute_traceback();

    Information gapOpenPenalty; // positive penalty (will be subtracted at gaps)
    Information gapExtensionPenalty; // positive penalty (will be subtracted on extension)
    bool bEndGapsFree;
    bool bLocalAligner;
    size_t m; // length of sequence 1
    size_t n; // length of sequence 2
    DpTable dpTable;
    Alignable seq1;
    Alignable seq2;
    Scorer* scorer;
};

}} // namespace moltk::align

std::ostream& operator<<(std::ostream&, const moltk::align::Aligner&);

#endif // MOLTK_ALIGN_ALIGNER_H
