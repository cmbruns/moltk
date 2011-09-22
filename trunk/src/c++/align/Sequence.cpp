#include "moltk/align/Sequence.h"

namespace moltk {

Sequence::Sequence(const std::string&) {}

} // namespace moltk

std::ostream& operator<<(std::ostream& os, const moltk::Sequence&)
{
    os << "sequence";
    return os;
}
