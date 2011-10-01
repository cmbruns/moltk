#include "moltk/Alignment.h"

using namespace moltk;
using namespace std;

std::ostream& operator<<(std::ostream& os, const moltk::Alignment& alignment)
{
    moltk::Alignment::const_iterator i = alignment.begin();
    while (i != alignment.end()) {
        os << *i << endl;
        ++i;
    }
    return os;
}

