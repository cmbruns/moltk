from rotation import Vec3
from nose.tools import assert_equal

def test_vec3():
    v = Vec3([1,2,3])
    assert_equal(2, v[1])
    assert_equal(3, len(v))
    assert_equal(0, Vec3().norm())
    assert_equal(Vec3([5,5,5]), 5 * Vec3([1,1,1]))
    assert_equal(Vec3([5,5,5]), Vec3([1,1,1]) * 5)
    assert_equal(Vec3([5,5,5]), [5,5,5])
