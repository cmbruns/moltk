#include "moltk/Alignable.h"

namespace moltk {

Alignable::Alignable(const Alignable& rhs)
{
    *this = rhs;
}

Alignable& Alignable::operator=(const Alignable& rhs)
{
    if (this == &rhs) return *this;
    positions.assign(rhs.positions.size(), NULL);
    for (size_t i = 0; i < positions.size(); ++i) {
        positions[i] = rhs.positions[i]->clone();
    }
    return *this;
}

/* virtual */
Alignable::~Alignable()
{
    for (size_t i = 0; i < positions.size(); ++i) {
        delete positions[i];
        positions[i] = NULL;
    }
}

} // namespace moltk

