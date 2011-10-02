#include "moltk/Printable.h"

// using namespace moltk;

std::ostream& moltk::operator<<(std::ostream& os, const moltk::Printable& p) {
    p.print_to_stream(os);
    return os;
}

/* virtual */
std::string moltk::Printable::__str__() const
{
    return "foo";
}

