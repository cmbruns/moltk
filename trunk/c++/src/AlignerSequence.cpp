#include "moltk/Aligner.h"

namespace moltk {

Aligner::PositionList::PositionList(const PositionList& rhs)
{
    *this = rhs;
}

Aligner::PositionList& Aligner::PositionList::operator=(const PositionList& rhs)
{
    if (this == &rhs) return *this;
    assign(rhs.size(), NULL);
    for (size_t i = 0; i < size(); ++i) {
        (*this)[i] = rhs[i]->clone();
    }
    return *this;
}

/* virtual */
Aligner::PositionList::~PositionList()
{
    for (size_t i = 0; i < size(); ++i) {
        delete (*this)[i];
        (*this)[i] = NULL;
    }
}

} // namespace moltk

