#ifndef MOLTK_ALIGN_POSITION_H
#define MOLTK_ALIGN_POSITION_H

namespace moltk { namespace align {

class Position
{
public:
    virtual Information score(const Position& rhs) const;
};

}} // namespace moltk::align

#endif // MOLTK_ALIGN_POSITION_H