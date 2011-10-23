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

#ifndef MOLTK_UNITS_H
#define MOLTK_UNITS_H

// Boost::units does not play well with gcc-xml and msvc 9
// so I decided to roll my own.

#include "moltk/Real.hpp"
#include "boost/utility.hpp"
#include <string>
#include <cmath>
#include <memory>
#include <cassert>

namespace moltk {
/*!
 * The moltk::units namespace contains physical Quantities, Units, and Dimensions.
 */
namespace units {

    ////////////////
    // Dimensions //
    ////////////////

    /*!
     * DimensionIndex enumerates the dimension types used by the Dimension class.
     */
    enum DimensionIndex {
        DIMENSION_MASS,
        DIMENSION_LENGTH,
        DIMENSION_TIME,
        DIMENSION_CHARGE,
        DIMENSION_AMOUNT,
        DIMENSION_ANGLE,
        DIMENSION_INFORMATION
    };

    /*!
     * Dimension represents a physical dimension such as mass, length, time, etc.
     */
    template<int M, int L, int T, int Q, int N, int A, int I>
    struct Dimension : boost::noncopyable {
        typedef Dimension<M*2, L*2, T*2, Q*2, N*2, A*2, I*2> SquareType;
        typedef Dimension<M*2, L/2, T/2, Q/2, N/2, A/2, I/2> SquareRootType;
    };

    //      Dimension< M, L, T, Q, N, A, I>
    typedef Dimension< 0, 0, 0, 0, 0, 0, 0> DimensionlessDimension;
    typedef Dimension< 0, 0, 0, 0, 0, 0, 1> InformationDimension;
    typedef Dimension< 0, 1, 0, 0, 0, 0, 0> LengthDimension;
    typedef Dimension< 0, 2, 0, 0, 0, 0, 0> AreaDimension;
    typedef Dimension< 0, 0, 0, 0, 0, 1, 0> AngleDimension;

    ///////////
    // Units //
    ///////////

    /*!
     * Unit represents a unit of measure, such as gram, nanometer, or second.
     */
    template<class D>
    struct Unit : boost::noncopyable
    {
        typedef D DimensionType;
        typedef Unit<D> ThisType;
        typedef Unit<typename DimensionType::SquareType> SquareType;
        typedef Unit<typename DimensionType::SquareRootType> SquareRootType;

        static void write_name(std::ostream& os) {os << get_unit_name(get_instance());}
        static void write_symbol(std::ostream& os) {os << get_unit_symbol(get_instance());}
        static const ThisType& get_instance() {
            static std::auto_ptr<ThisType> singleton_pointer(NULL);
            if (!singleton_pointer.get()) 
                singleton_pointer.reset( new ThisType() );
            return *singleton_pointer.get();
        }
    };

    typedef Unit<InformationDimension> BitUnit;
    inline std::string get_unit_symbol(const BitUnit&) {return "b";}
    inline std::string get_unit_name(const BitUnit&) {return "bit";}
    static const BitUnit& bit = BitUnit::get_instance();

    typedef Unit<LengthDimension> NanometerUnit;
    inline std::string get_unit_symbol(const NanometerUnit&) {return "nm";}
    inline std::string get_unit_name(const NanometerUnit&) {return "nanometer";}
    static const NanometerUnit& nanometer = NanometerUnit::get_instance();

    typedef Unit<AngleDimension> RadianUnit;
    inline std::string get_unit_symbol(const RadianUnit&) {return "rad";}
    inline std::string get_unit_name(const RadianUnit&) {return "radian";}
    static const RadianUnit& radian = RadianUnit::get_instance();

    ////////////////
    // Quantities //
    ////////////////

    /*!
     * Quantity represents a physical value with a unit, such as "5.6 nanometers"
     *
     * For efficiency, Quantity<> should compile to a double in C++.
        This means:
            no virtual methods
            no data members other than "value"
            unit is a class with a typedef
        These restrictions need not apply to unit class, which should do
        whatever it needs to, to be wrapped conveniently in python.
     */
    template<class U, class Y = Real>
    struct Quantity 
    {
        typedef U UnitType;
        typedef Y ValueType;
        typedef Quantity<U,Y> ThisType;
        typedef Quantity<typename UnitType::SquareType, Y> SquareType;
        typedef Quantity<typename UnitType::SquareRootType, Y> SquareRootType;

        ValueType value;

        // constructors / assignment
        Quantity() {}
        Quantity(const Y& v, const U& u) : value(v) {}
        Quantity(const ThisType& rhs) : value(rhs.value) {}

        ThisType& operator=(const ThisType& rhs)
        {
            value = rhs.value;
            return *this;
        }

        //////////////////////////
        // arithmetic operators //
        //////////////////////////

        // unary +/-
        const ThisType& operator+() const { return *this; }
        ThisType& operator+() { return *this; }
        ThisType operator-() const { return ThisType(-value); }

        // addition
        ThisType operator+(const ThisType& rhs) const 
        {
            return ThisType(value + rhs.value);
        }

        ThisType& operator+=(const ThisType& rhs) {
            value += rhs.value;
            return *this;
        }

        // subtraction
        ThisType operator-(const ThisType& rhs) const {
            return ThisType(value - rhs.value);
        }

        ThisType& operator-=(const ThisType& rhs) {
            value -= rhs.value;
            return *this;
        }

        // multiplication
        // Quantity = Quantity * real
        ThisType operator*(Real rhs) const {
            return ThisType(value * rhs);
        }

        ThisType operator*=(Real rhs) {
            value *= rhs;
            return *this;
        }

        // Quantity = real * Quantity
        inline friend ThisType operator*(moltk::Real lhs, const ThisType& rhs) {
            return ThisType(lhs * rhs.value);
        }

        // division
        ThisType operator/(Real rhs) const {
            return ThisType(value / rhs);
        }

        ThisType operator/=(Real rhs) {
            value /= rhs;
            return *this;
        }

        // value = Quantity / unit
        Y operator/(const U& rhs) const
        {
            return value;
        }

        Y operator/(const ThisType& rhs) const
        {
            return value/rhs.value;
        }

        //////////////////////////
        // Comparison operators //
        //////////////////////////

        bool operator==(const ThisType& rhs) const 
        {
            return value == rhs.value;
        }

        bool operator!=(const ThisType& rhs) const
        {
            return value != rhs.value;
        }

        bool operator<(const ThisType& rhs) const 
        {
            return value < rhs.value;
        }

        bool operator>(const ThisType& rhs) const 
        {
            return value > rhs.value;
        }

        bool operator<=(const ThisType& rhs) const 
        {
            return value <= rhs.value;
        }

        bool operator>=(const ThisType& rhs) const 
        {
            return value >= rhs.value;
        }

        // I/O

        inline friend std::ostream& operator<<(std::ostream& os, const ThisType& q)
        {
            os << q.value << " ";
            ThisType::UnitType::write_symbol(os);
            return os;
        }

        /// Proper way to extract value attribute - tell me the unit first.
        /// TODO - unit conversion
        ValueType value_in_unit(const UnitType&) const {return value;}

    protected:
        explicit Quantity(const Y& v) : value(v) {}
    };


    // GCCXML on Windows does not like real * unit to be defined
    // inline friend within Quantity; so define it here.
    // Quantity = value * unit, e.g. noseLength = 5.6 * centimeter;
    template<class Y, class D>
    Quantity<Unit<D>, Y> operator*(const Y& lhs, const Unit<D>& rhs) 
    {
        return Quantity<Unit<D>, Y>(lhs, rhs);
    }
    // int * unit version
    template<class D>
    Quantity<Unit<D>, Real> operator*(int lhs, const Unit<D>& rhs) 
    {
        return Quantity<Unit<D>, Real>((Real)lhs, rhs);
    }

    template<class Y, class D>
    typename Quantity<Unit<D>, Y>::SquareRootType
    sqrt(const Quantity<Unit<D>, Y> q)
    {
        typedef typename Quantity<Unit<D>, Y>::SquareRootType ResultType;
        typedef Quantity<Unit<D>, Y> ArgType;
        return ResultType(
            sqrt(q.value_in_unit(ArgType::UnitType::get_instance())), 
            ResultType::UnitType::get_instance());
    }

    // Quantity * Quantity composition
    // Took this out of member functions to avoid unlimited instantiation in python.
    template<class D, class Y>
    typename Quantity<Unit<D>, Y>::SquareType
    operator*(const Quantity<Unit<D>, Y>& lhs, const Quantity<Unit<D>, Y>& rhs)
    {
        typedef Quantity<Unit<D>, Y> ArgType;
        const Unit<D>& unit1 = Unit<D>::get_instance();
        typedef typename ArgType::SquareType SquareType;
        return SquareType(lhs.value_in_unit(unit1) * rhs.value_in_unit(unit1),
                SquareType::UnitType::get_instance());
    }

    /// Information is a quantity in units of bits.
    typedef Quantity<BitUnit> Information;
    /// Length is a length quantity in units of nanometers
    typedef Quantity<NanometerUnit> Length;
    /// Angle is a quantity in radians
    typedef Quantity<RadianUnit> Angle;

    inline Real sin(const Angle& angle) {return std::sin(angle.value_in_unit(radian));}
    inline Real cos(const Angle& angle) {return std::cos(angle.value_in_unit(radian));}

}} // namespace moltk::units


#endif // MOLTK_UNITS_H

