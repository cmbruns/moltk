#include "moltk/align/Alignment.h"

namespace moltk {

Alignment::Alignment(const Sequence&, const Sequence&) {}

} // namespace moltk

std::ostream& operator<<(std::ostream& os, const moltk::Alignment&)
{
    os << "sequence";
    return os;
}
