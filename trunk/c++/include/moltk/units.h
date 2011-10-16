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

    struct dimension {};
    struct information_dimension : public dimension {};
    struct length_dimension : public dimension {};

    ///////////
    // Units //
    ///////////

    template<class D>
    struct unit 
    {
        typedef D dimension_type;
        static void print_name(std::ostream& os) {os << unit_name(getInstance());}
        static void print_symbol(std::ostream& os) {os << unit_symbol(getInstance());}
        static const unit& getInstance() {
            static unit* singleton_pointer = NULL;
            if (! singleton_pointer) {
                singleton_pointer = new unit();
            }
            return *singleton_pointer;
        }
    private:
        unit() {} // singleton provides no constructor
        unit(const unit&) {}
    };

    typedef unit<information_dimension> bit_t;
    inline std::string unit_symbol(const bit_t&) {return "b";}
    inline std::string unit_name(const bit_t&) {return "bit";}
    static const bit_t& bit = bit_t::getInstance();

    typedef unit<length_dimension> nanometer_t;
    inline std::string unit_symbol(const nanometer_t&) {return "nm";}
    inline std::string unit_name(const nanometer_t&) {return "nanometer";}
    static const nanometer_t& nanometer = nanometer_t::getInstance();

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
        typedef quantity<U,Y> this_type;

        value_type value;

        // constructors / assignment
        quantity() {}
        quantity(const Y& v, const U& u) : value(v) {}
        quantity(const this_type& rhs) : value(rhs.value) {}

        this_type& operator=(const this_type& rhs)
        {
            value = rhs.value;
            return *this;
        }

        //////////////////////////
        // arithmetic operators //
        //////////////////////////

        // unary +/-
        const this_type& operator+() const { return *this; }
        this_type& operator+() { return *this; }
        this_type operator-() const { return this_type(-value); }

        // addition
        this_type operator+(const this_type& rhs) const 
        {
            return quantity(value + rhs.value);
        }

        this_type& operator+=(const this_type& rhs) {
            value += rhs.value;
            return *this;
        }

        // subtraction
        this_type operator-(const this_type& rhs) const {
            return this_type(value - rhs.value);
        }

        this_type& operator-=(const this_type& rhs) {
            value -= rhs.value;
            return *this;
        }

        // multiplication
        // quantity = quantity * real
        this_type operator*(Real rhs) const {
            return moltk::units::this_type(value * rhs);
        }

        this_type operator*=(Real rhs) const {
            value *= rhs;
            return *this;
        }

        // quantity = real * quantity
        inline friend this_type operator*(moltk::Real lhs, const this_type& rhs) {
            return this_type(lhs * rhs.value);
        }

        // quantity = value * unit, e.g. noseLength = 5.6 * centimeter;
        inline friend this_type operator*(const Y& lhs, const U& rhs) 
        {
            return this_type(lhs);
        }

        // division
        this_type operator/(Real rhs) const {
            return moltk::units::this_type(value / rhs);
        }

        this_type operator/=(Real rhs) const {
            value /= rhs;
            return *this;
        }

        // value = quantity / unit
        const Y& operator/(const U& rhs) const
        {
            return value;
        }

        Y& operator/(const U& rhs)
        {
            return value;
        }

        Y operator/(const this_type& rhs) const
        {
            return value/rhs.value;
        }

        //////////////////////////
        // Comparison operators //
        //////////////////////////

        bool operator==(const this_type& rhs) const 
        {
            return value == rhs.value;
        }

        bool operator!=(const this_type& rhs) const
        {
            return value != rhs.value;
        }

        bool operator<(const this_type& rhs) const 
        {
            return value < rhs.value;
        }

        bool operator>(const this_type& rhs) const 
        {
            return value > rhs.value;
        }

        bool operator<=(const this_type& rhs) const 
        {
            return value <= rhs.value;
        }

        bool operator>=(const this_type& rhs) const 
        {
            return value >= rhs.value;
        }

        // I/O

        inline friend std::ostream& operator<<(std::ostream& os, const this_type& q)
        {
            os << q.value << " ";
            this_type::unit_type::print_symbol(os);
            return os;
        }

    protected:
        explicit quantity(const Y& v) : value(v) {}
    };

    typedef quantity<bit_t> Information;
    typedef quantity<nanometer_t> Length;

}} // namespace moltk::units


#endif // MOLTK_UNITS_H

