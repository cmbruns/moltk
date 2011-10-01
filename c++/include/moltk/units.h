#ifndef MOLTK_UNITS_H
#define MOLTK_UNITS_H

/*
#include <boost/units/base_dimension.hpp> // cannot include in MSVC 9
#include <string>
#include <boost/units/io.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/make_system.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/base_unit.hpp>
#include <boost/units/derived_dimension.hpp>
*/

namespace boost { namespace units {

    /*
    struct information_base_dimension 
        : boost::units::base_dimension<information_base_dimension, 1342>
    { };
    */

}} // namespace boost::units


namespace moltk { namespace units {


/*
typedef information_base_dimension::dimension_type information_dimension;

struct bit_base_unit : public boost::units::base_unit<bit_base_unit, information_dimension, 1>
{
    static std::string name() {return ("bit");}
    static std::string symbol() {return ("bit");}
};

typedef bit_base_unit::unit_type bit_t;
BOOST_UNITS_STATIC_CONSTANT(bit, bit_t);

typedef boost::units::make_system<
    bit_base_unit>::type unit_system;

typedef boost::units::quantity<bit_t> Information;
*/

typedef double Information;
static const Information bit;

}} // namespace moltk::units

#endif // MOLTK_UNITS_H

