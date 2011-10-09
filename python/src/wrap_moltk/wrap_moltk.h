#ifndef WRAP_MOLTK_PYPLUSPLUS_H
#define WRAP_MOLTK_PYPLUSPLUS_H

// Try to avoid a boost include problem on Windows
#ifdef _MSC_VER
#define BOOST_MSVC _MSC_VER
#endif

#include "moltk.h"
#include <vector>

using namespace moltk;
using namespace moltk::units;

namespace pyplusplus { namespace aliases {

    typedef std::vector<Structure::Atom> AtomList;
    typedef unit<information_dimension> base_information_unit;
    typedef quantity<bit_unit> Information;
    typedef quantity<nanometer_unit> Length;

}} // namespace pyplusplus::aliases

#endif // WRAP_MOLTK_PYPLUSPLUS_H

