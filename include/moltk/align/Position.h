#ifndef MOLTK_ALIGN_POSITION_H
#define MOLTK_ALIGN_POSITION_H

#include "moltk/units.h"

namespace moltk { namespace align {

class Position
{
public:
    virtual ~Position();
    virtual moltk::units::Information score(const Position& rhs) const;
};

}} // namespace moltk::align

#endif // MOLTK_ALIGN_POSITION_H