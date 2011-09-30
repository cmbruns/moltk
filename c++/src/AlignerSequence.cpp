#include "moltk/Aligner.h"

namespace moltk {

Aligner::Sequence::Sequence(const Sequence& rhs)
{
    *this = rhs;
}

Aligner::Sequence& Aligner::Sequence::operator=(const Sequence& rhs)
{
    if (this == &rhs) return *this;
    assign(rhs.size(), NULL);
    for (size_t i = 0; i < size(); ++i) {
        (*this)[i] = rhs[i]->clone();
    }
    return *this;
}

/* virtual */
Aligner::Sequence::~Sequence()
{
    for (size_t i = 0; i < size(); ++i) {
        delete (*this)[i];
        (*this)[i] = NULL;
    }
}

} // namespace moltk

