#include "chem/Structure.h"

namespace moltk { namespace pdb {

Structure::Structure(std::istream& istream)
{
    loadFromStream(istream);
}

/* explicit */
void Structure::loadFromStream(std::istream& istream)
{
    // TODO
}

}} // namespace moltk::pdb
