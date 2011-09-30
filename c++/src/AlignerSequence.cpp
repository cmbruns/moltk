#include "moltk/Aligner.h"

namespace moltk {

Aligner::Sequence::Sequence(const Sequence& rhs)
{
    *this = rhs;
}

Sequence& Aligner::Sequence::operator=(const Sequence& rhs)
{
    if (this == &rhs) return *this;
    positions.assign(rhs.positions.size(), NULL);
    for (size_t i = 0; i < positions.size(); ++i) {
        positions[i] = rhs.positions[i]->clone();
    }
    return *this;
}

/* virtual */
Aligner::Sequence::~Sequence()
{
    for (size_t i = 0; i < positions.size(); ++i) {
        delete positions[i];
        positions[i] = NULL;
    }
}

} // namespace moltk

