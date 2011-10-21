#ifndef MOLTK_ROTATION3D_HPP
#define MOLTK_ROTATION3D_HPP

#include "moltk/Real.hpp"

namespace moltk {

/*!
 * Rotation3D represents a rotation matrix.
 */
class Rotation3D
{
public:

    /*!
     * One row of three numbers in a Rotation3D rotation matrix.
     */
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
