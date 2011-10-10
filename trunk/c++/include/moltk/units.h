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

    struct information_dimension {};
    struct length_dimension {};

    ///////////
    // Units //
    ///////////

    template<class D>
    struct unit 
    {
        typedef D dimension_type;
    };

    struct bit_unit : public unit<information_dimension> 
    {
        static void print_name(std::ostream& os) {os << "bit";}
        static void print_symbol(std::ostream& os) {os << "b";}
    };
    static const bit_unit bit = bit_unit();

    struct nanometer_unit : public unit<length_dimension> {};
    static const nanometer_unit nanometer = nanometer_unit();

    ////////////////
    // Quantities //
    ////////////////

    /// For efficiency, quantity<> should compile to a double in C++.
    /// This means:
    ///     no virtual methods
    ///     no data members other than "value"
    ///     unit is a class with a typedef
    /// These restrictions need not apply to unit class, which should do
    /// whatever it needs to, to be wrapped conveniently in python.
    template<class U, class Y = Real>
    struct quantity 
    {
        typedef U unit_type;
        typedef Y value_type;

        value_type value;

        // constructors
        quantity() {}
        explicit quantity(Y v) : value(v) {}
        quantity(const quantity<U,Y>& rhs) : value(rhs.value) {}

        // arithmetic operators
        quantity<U,Y> operator+(const quantity<U,Y>& rhs) const {
            return quantity(value + rhs.value);
        }

        quantity<U,Y> operator-(const quantity<U,Y>& rhs) const {
            return quantity<U,Y>(value - rhs.value);
        }

        quantity<U,Y> operator-() const {
            return quantity<U,Y>(-value);
        }

        quantity<U,Y> operator*(Real rhs) const {
            return moltk::units::quantity<U,Y>(value * rhs);
        }

        bool operator<(const quantity<U,Y>& rhs) const {
            return value < rhs.value;
        }

        bool operator>(const quantity<U,Y>& rhs) const {
            return value > rhs.value;
        }

        bool operator<=(const quantity<U,Y>& rhs) const {
            return value <= rhs.value;
        }

        bool operator>=(const quantity<U,Y>& rhs) const {
            return value >= rhs.value;
        }

        void print(std::ostream& os) const {
            os << value << " ";
            unit_type::print_symbol(os);
        }

    };

    typedef quantity<bit_unit> Information;
    typedef quantity<nanometer_unit> Length;

template<class U, class Y>
inline std::ostream& operator<<(std::ostream& os, const quantity<U,Y>& q) {
    q.print(os);
    return os;
}

template<class U, class Y>
inline quantity<U,Y> operator*(const Y& lhs, const U& rhs) {
    return quantity<U,Y>(lhs);
}

template<class U, class Y>
inline quantity<U,Y> operator*(moltk::Real lhs, const quantity<U,Y>& rhs) {
    return quantity<U,Y>(lhs * rhs.value);
}

}} // namespace moltk::units


#endif // MOLTK_UNITS_H

