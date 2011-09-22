#ifndef MOLTK_ALIGN_ALIGNMENT_H
#define MOLTK_ALIGN_ALIGNMENT_H

#include <iostream>
#include <vector>
#include "Sequence.h"
#include "moltk/units.h"

namespace moltk {

// TODO use boost::units for information
typedef moltk::units::Information Information;

class Alignment {
public:
    Alignment(const Sequence&, const Sequence&);
    void align();

protected:
    Information gapOpenPenalty;
    Information gapExtensionPenalty;
    bool bEndGapsFree;
    bool bLocalAlignment;


    class Cell
    {
        // Gusfield nomenclature
        Information pairScore; // Wm, score of aligning position S1(i) with S2(j)
        Information pathScore; // V, best score through this cell
        Information g; // G, best ungapped score through this cell
        Information e; // E, 
        Information f; // F, 
    };


    typedef std::vector<Cell> DpRow;
    typedef std::vector<DpRow> DpTable;
};

} // namespace moltk

std::ostream& operator<<(std::ostream&, const moltk::Alignment&);

#endif // MOLTK_ALIGN_ALIGNMENT_H
