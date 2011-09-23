#include "moltk/align/Sequence.h"

namespace moltk { namespace align {

Sequence::Sequence(const Sequence& rhs)
{
    *this = rhs;
}

Sequence& Sequence::operator=(const Sequence& rhs)
{
    if (this == &rhs) return *this;
    this->assign(rhs.size(), NULL);
    for (size_t i = 0; i < this->size(); ++i) {
        (*this)[i] = rhs[i]->clone();
    }
    return *this;
}

/* virtual */
Sequence::~Sequence()
{
    for (size_t i = 0; i < this->size(); ++i) {
        delete (*this)[i];
        (*this)[i] = NULL;
    }
}

}} // namespace moltk::align

std::ostream& operator<<(std::ostream& os, const moltk::align::Sequence&)
{
    os << "sequence";
    return os;
}
