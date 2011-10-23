/*
    MolTK is a Python and C++ toolkit for protein sequence/structure alignment and visualization
    Copyright (C) 2011  Christopher M. Bruns

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    
    Commercial users should ask about our dual licensing model.
    For questions contact: cmbruns@rotatingpenguin.com
*/

#ifndef MOLTK_VEC3_HPP
#define MOLTK_VEC3_HPP

#include "moltk/units.hpp"

namespace moltk {

/*!
 * BaseVector3D is a parent class for points and vectors.
 */
template<class ELT>
class BaseVector3D
{
private:
    typedef BaseVector3D<ELT> ThisType;

public:
    BaseVector3D() {}
    BaseVector3D(const ELT& x, const ELT& y, const ELT& z) 
        : m_x(x), m_y(y), m_z(z) {}

    typedef ThisType container;
    typedef size_t size_type;
    typedef ELT value_type;
    typedef value_type* iterator;
    typedef value_type& reference;
    typedef value_type key_type;
    typedef size_t index_type;
    typedef value_type const * const_iterator;
    static const size_type SIZE = 3;

// hide methods not-exposed by UnitVector3D inside protected block
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

    ThisType& operator*=(moltk::Real s) {
        m_x *= s;
        m_y *= s;
        m_z *= s;
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

/*!
 * Vector3D_<> is an arbitrary point or vector in 3-space.
 *
 * Vector3D_<> can contain Quantities with Units.
 */
template<class ELT>
class Vector3D_ : public BaseVector3D<ELT>
{
private:
    typedef BaseVector3D<ELT> super;
    typedef Vector3D_<ELT> ThisType;

public:
    typedef Vector3D_<ELT> container;
    typedef ELT value_type;

    Vector3D_() {}
    Vector3D_(const value_type& x, const value_type& y, const value_type& z) 
        : super(x, y, z) {}
    Vector3D_(const super& rhs) : super(rhs) {}

    // expose protected members of base class
    using super::x;
    using super::y;
    using super::z;
    using super::operator[];
    using super::operator*=;
    using super::operator/=;

    using super::operator value_type*;
    using super::begin;
    using super::end;

};

/*!
 * Vector3D is an arbitrary unitless vector or point in 3 space.
 */
typedef Vector3D_< moltk::Real > Vector3D;
typedef Vector3D_< moltk::units::Length > Displacement;

/*!
 * UnitVector3D represents a direction in 3 space, and, unlike Vector3D_<>, does NOT have Units.
 */
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

    Vector3D operator*(Real s) const 
    {
        Vector3D result(*this);
        result *= s;
        return result;
    }
    inline friend Vector3D operator*(Real s, const UnitVector3D& unit_vector)
    {
        Vector3D result(unit_vector);
        result *= s;
        return result;
    }

};


// Vector * unit
template<class D, class Y>
Vector3D_<moltk::units::Quantity<moltk::units::Unit<D>, Y> >
operator*(const Vector3D_<Y>& vector, const moltk::units::Unit<D>& unit)
{
    typedef Vector3D_<moltk::units::Quantity<moltk::units::Unit<D>, Y> >
        ResultType;
    return ResultType(vector.x() * unit,
                      vector.y() * unit,
                      vector.z() * unit);
}

} // namespace moltk

#endif // MOLTK_VEC3_HPP

