#ifndef MOLTK_UNITS_H
#define MOLTK_UNITS_H

// Boost::units does not play well with gcc-xml and msvc 9
// so I decided to roll my own.

namespace moltk { namespace units {


// typedef double Information;
// static const Information bit;
    struct Information 
    {
        Information() {}

        Information(double d) : value(d) {}

        Information operator+(const Information& rhs) const {
            return Information(value + rhs.value);
        }
        Information operator-(const Information& rhs) const {
            return Information(value - rhs.value);
        }

        Information operator-() const {
            return Information(-value);
        }

        Information operator*(double rhs) const {
            return moltk::units::Information(value * rhs);
        }

        bool operator<(const Information& rhs) const {
            return value < rhs.value;
        }

        bool operator>(const Information& rhs) const {
            return value > rhs.value;
        }

        bool operator<=(const Information& rhs) const {
            return value <= rhs.value;
        }

        bool operator>=(const Information& rhs) const {
            return value >= rhs.value;
        }

        std::ostream& print(std::ostream& os) const {
            os << value << " bits"; // TODO
            return os;
        }

        double value;
    };

    struct bit_unit {
    };

    static const bit_unit bit = bit_unit();

}} // namespace moltk::units

inline std::ostream& operator<<(std::ostream& os, const moltk::units::Information& info) {
    return info.print(os);
}

inline moltk::units::Information operator*(double lhs, const moltk::units::bit_unit& rhs) {
    return moltk::units::Information(lhs);
}

inline moltk::units::Information operator*(double lhs, const moltk::units::Information& rhs) {
    return moltk::units::Information(lhs * rhs.value);
}

#endif // MOLTK_UNITS_H

