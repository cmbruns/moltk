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
        Information v;
        Information g;
        Information e;
        Information f;
    };


    typedef std::vector<std::vector<Cell> > DpTable;
};

} // namespace moltk

std::ostream& operator<<(std::ostream&, const moltk::Alignment&);

#endif // MOLTK_ALIGN_ALIGNMENT_H
