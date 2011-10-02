#include "moltk/Alignment.h"

using namespace moltk;
using namespace std;

/* virtual */
void moltk::Alignment::print_to_stream(std::ostream& os) const 
{
   const_iterator i = begin();
    while (i != end()) {
        os << *i << std::endl;
        ++i;
    }
}

