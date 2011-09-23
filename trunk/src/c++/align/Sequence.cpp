#include "moltk/align/Sequence.h"

namespace moltk { namespace align {

}} // namespace moltk::align

std::ostream& operator<<(std::ostream& os, const moltk::align::Sequence&)
{
    os << "sequence";
    return os;
}
