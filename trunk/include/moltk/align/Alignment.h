#ifndef MOLTK_ALIGN_ALIGNMENT_H
#define MOLTK_ALIGN_ALIGNMENT_H

#include "Sequence.h"
#include <iostream>

namespace moltk {

class Alignment {
public:
    Alignment(const Sequence&, const Sequence&);
    void align();
};

} // namespace moltk

std::ostream& operator<<(std::ostream&, const moltk::Alignment&);

#endif // MOLTK_ALIGN_ALIGNMENT_H
