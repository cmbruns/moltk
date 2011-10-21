#ifndef MOLTK_ROTATION3D_HPP
#define MOLTK_ROTATION3D_HPP

#include "moltk/Real.hpp"

namespace moltk {

class Rotation3D
{
public:

    class Row
    {
    public:
    protected:
        moltk::Real x, y, z;
    };


public:

protected:
    Row row0, row1, row2;
};

} // namespace moltk

#endif // MOLTK_ROTATION3D_HPP
