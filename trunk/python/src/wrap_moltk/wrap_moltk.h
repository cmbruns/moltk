#ifndef WRAP_MOLTK_PYPLUSPLUS_H
#define WRAP_MOLTK_PYPLUSPLUS_H

// Try to avoid a boost include problem on Windows
#ifdef _MSC_VER
#define BOOST_MSVC _MSC_VER
#endif

#include "moltk.h"
#include <vector>

namespace pyplusplus { namespace aliases {

    typedef std::vector<moltk::Structure::Atom> AtomList;

}} // namespace pyplusplus::aliases

#endif // WRAP_MOLTK_PYPLUSPLUS_H

