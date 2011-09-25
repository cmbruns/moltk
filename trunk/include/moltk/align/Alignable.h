#ifndef MOLTK_ALIGN_ALIGNABLE_H
#define MOLTK_ALIGN_ALIGNABLE_H

#include <string>
#include <iostream>
#include <vector>
#include "moltk/align/Position.h"

namespace moltk { namespace align {

class Alignable : public std::vector<Position*>
{
public:
    Alignable() {}
    Alignable(const Alignable& rhs);
    virtual ~Alignable();
    Alignable& operator=(const Alignable& rhs);
};

}} // namespace moltk::align

#endif MOLTK_ALIGN_ALIGNABLE_H

