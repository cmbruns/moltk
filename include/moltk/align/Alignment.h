#ifndef MOLTK_ALIGN_ALIGNMENT_H
#define MOLTK_ALIGN_ALIGNMENT_H

#include <iostream>
#include <vector>
#include "Sequence.h"
#include "moltk/units.h"

namespace moltk { namespace align {

class Alignment 
{
public:
    typedef moltk::units::Information Information;

    Alignment(const Sequence&, const Sequence&);
    void align();

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


protected:
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
};

}} // namespace moltk::align

std::ostream& operator<<(std::ostream&, const moltk::align::Alignment&);

#endif // MOLTK_ALIGN_ALIGNMENT_H
