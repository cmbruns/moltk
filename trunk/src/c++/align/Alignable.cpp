#include "moltk/align/Alignable.h"

namespace moltk { namespace align {

Alignable::Alignable(const Alignable& rhs)
{
    *this = rhs;
}

Alignable& Alignable::operator=(const Alignable& rhs)
{
    if (this == &rhs) return *this;
    this->assign(rhs.size(), NULL);
    for (size_t i = 0; i < this->size(); ++i) {
        (*this)[i] = rhs[i]->clone();
    }
    return *this;
}

/* virtual */
Alignable::~Alignable()
{
    for (size_t i = 0; i < this->size(); ++i) {
        delete (*this)[i];
        (*this)[i] = NULL;
    }
}

}} // namespace moltk::align

std::ostream& operator<<(std::ostream& os, const moltk::align::Alignable&)
{
    os << "alignable";
    return os;
}
