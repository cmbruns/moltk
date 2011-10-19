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
    
    For questions contact: cmbruns@rotatingpenguin.com
*/

#ifndef MOLTK_UNITS_H
#define MOLTK_UNITS_H

// Boost::units does not play well with gcc-xml and msvc 9
// so I decided to roll my own.

#include "moltk/Real.h"

namespace moltk { namespace units {

    ////////////////
    // Dimensions //
    ////////////////

    struct Dimension {};
    struct InformationDimension : public Dimension {};
    struct LengthDimension : public Dimension {};

    ///////////
    // Units //
    ///////////

    template<class D>
    struct Unit
    {
        typedef D DimensionType;
        typedef Unit<D> ThisType;
        static void print_name(std::ostream& os) {os << get_unit_name(get_instance());}
        static void print_symbol(std::ostream& os) {os << get_unit_symbol(get_instance());}
        static const ThisType& get_instance() {
            static std::auto_ptr<ThisType> singleton_pointer(NULL);
            if (!singleton_pointer.get()) 
                singleton_pointer.reset( new ThisType() );
            return *singleton_pointer.get();
        }
    private:
        Unit() {} // singleton provides no constructor
        Unit(const ThisType&) {}
    };

    typedef Unit<InformationDimension> BitUnit;
    inline std::string get_unit_symbol(const BitUnit&) {return "b";}
    inline std::string get_unit_name(const BitUnit&) {return "bit";}
    static const BitUnit& bit = BitUnit::get_instance();

    typedef Unit<LengthDimension> NanometerUnit;
    inline std::string get_unit_symbol(const NanometerUnit&) {return "nm";}
    inline std::string get_unit_name(const NanometerUnit&) {return "nanometer";}
    static const NanometerUnit& nanometer = NanometerUnit::get_instance();

    ////////////////
    // Quantities //
    ////////////////

    /// For efficiency, Quantity<> should compile to a double in C++.
    /// This means:
    ///     no virtual methods
    ///     no data members other than "value"
    ///     unit is a class with a typedef
    /// These restrictions need not apply to unit class, which should do
    /// whatever it needs to, to be wrapped conveniently in python.
    template<class U, class Y = Real>
    struct Quantity 
    {
        typedef U UnitType;
        typedef Y ValueType;
        typedef Quantity<U,Y> ThisType;

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
            ThisType::UnitType::print_symbol(os);
            return os;
        }

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

    typedef Quantity<BitUnit> Information;
    typedef Quantity<NanometerUnit> Length;

}} // namespace moltk::units


#endif // MOLTK_UNITS_H

