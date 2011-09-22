#ifndef MOLTK_UNITS_H
#define MOLTK_UNITS_H

#include <string>
#include <boost/units/io.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/make_system.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/base_dimension.hpp>
#include <boost/units/base_unit.hpp>
#include <boost/units/derived_dimension.hpp>

namespace moltk { namespace units {

struct information_base_dimension 
    : boost::units::base_dimension<information_base_dimension, 1>
{ };
typedef information_base_dimension::dimension_type information_dimension;

struct bit_base_unit : public boost::units::base_unit<bit_base_unit, information_dimension, 1>
{
    static std::string name() {return ("bit");}
    static std::string symbol() {return ("bit");}
};

typedef bit_base_unit::unit_type bit_t;
BOOST_UNITS_STATIC_CONSTANT(bit, bit_t);

}} // namespace moltk::units

#endif // MOLTK_UNITS_H

