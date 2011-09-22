#include "moltk/align/Alignment.h"

namespace moltk {

///////////////////////
// Alignment methods //
///////////////////////

Alignment::Alignment(const Sequence&, const Sequence&) 
{
}

void Alignment::align()
{
    // TODO - start on page 244 of Gusfield
}

} // namespace moltk


////////////////////
// global methods //
////////////////////

std::ostream& operator<<(std::ostream& os, const moltk::Alignment&)
{
    os << "sequence";
    return os;
}
