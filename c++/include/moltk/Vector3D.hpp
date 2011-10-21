#ifndef MOLTK_VEC3_HPP
#define MOLTK_VEC3_HPP

#include "moltk/units.hpp"

namespace moltk {

template<class ELT>
class BaseVector3D
{
private:
    typedef BaseVector3D<ELT> ThisType;

public:
    BaseVector3D() {}
    BaseVector3D(const ELT& x, const ELT& y, const ELT& z) 
        : m_x(x), m_y(y), m_z(z) {}

    typedef ELT value_type;
    typedef size_t size_type;
    typedef size_t index_type;
    typedef value_type* iterator;
    typedef value_type const * const_iterator;
    static const size_type SIZE = 3;

protected:
    value_type m_x;
    value_type m_y;
    value_type m_z;

    iterator begin() {return &m_x;}
    iterator end() {return &(&m_x)[SIZE];}
    value_type& operator[](size_t i) {return (&m_x)[i];}
    operator value_type*() {return &m_x;}

    ThisType& operator/=(moltk::Real s) {
        m_x /= s;
        m_y /= s;
        m_z /= s;
        return *this;
    }

public:
    const value_type& x() const {return m_x;}
    const value_type& y() const {return m_y;}
    const value_type& z() const {return m_z;}
    const value_type& operator[](size_t i) const {return (&m_x)[i];}

    operator const value_type*() const {return &m_x;}
    size_type size() const {return SIZE;}

    const_iterator begin() const {return &m_x;}
    const_iterator end() const {return &(&m_x)[SIZE];}

    bool operator==(const ThisType& rhs) const {
        return ((m_x == rhs.m_x)
             && (m_y == rhs.m_y)
             && (m_z == rhs.m_z));
    }
    bool operator!=(const ThisType& rhs) const {
        return ((m_x != rhs.m_x)
             || (m_y != rhs.m_y)
             || (m_z != rhs.m_z));
    }

    value_type norm() const 
    {
        return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
    }

};


template<class ELT>
class Vector3D_ : public BaseVector3D<ELT>
{
private:
    typedef BaseVector3D<ELT> super;
    typedef Vector3D_<ELT> ThisType;

public:
    typedef ELT value_type;

    Vector3D_() {}
    Vector3D_(const value_type& x, const value_type& y, const value_type& z) 
        : super(x, y, z) {}

    using super::x;
    using super::y;
    using super::z;
    using super::operator[];

    using super::operator value_type*;
    using super::begin;
    using super::end;

};


class UnitVector3D : public BaseVector3D< moltk::Real >
{
private:
    typedef BaseVector3D< moltk::Real > super;

public:
    UnitVector3D() : super(1, 0, 0) {}
    UnitVector3D(const UnitVector3D& other) : super(other) {}
    // Expensive constructors scale to unit value
    template<class ELT>
    UnitVector3D(const Vector3D_<ELT>& v) : super(v / v.norm()) {}
    UnitVector3D(const value_type& x, const value_type& y, const value_type& z) 
        : super(x, y, z)
    {
        (*this) /= super::norm(); // don't use UnitVector3::norm()!
    }

    moltk::Real norm() const {return 1.0;}
    moltk::Real normSquared() const {return 1.0;}
};


typedef Vector3D_< moltk::Real > Vector3D;
typedef Vector3D_< moltk::units::Length > Displacement;


} // namespace moltk

#endif // MOLTK_VEC3_HPP

