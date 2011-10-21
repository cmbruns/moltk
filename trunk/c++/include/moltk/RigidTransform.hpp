#ifndef MOLTK_RIGID_TRANSFORM_HPP
#define MOLTK_RIGID_TRANSFORM_HPP

#include "moltk/Vector3D.hpp"
#include "moltk/Rotation3D.hpp"

namespace moltk {

/*!
 * RigidTransform converts one reference frame to another with rotation and translation.
 */
class RigidTransform
{
protected:
    Displacement translation;
    Rotation3D rotation;
};

} // namespace moltk

#endif // MOLTK_RIGID_TRANSFORM_HPP
