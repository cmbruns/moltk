#ifndef MOLTK_PRINTABLE_H
#define MOLTK_PRINTABLE_H

#include <iostream>
#include <string>

namespace moltk {

/// moltk::Printable is an abstract base class to help streamline
/// string conversion in the moltk python bindings.
class Printable
{
public:
    virtual void print_to_stream(std::ostream& os) const = 0;
    virtual std::string __str__() const;
};

    std::ostream& operator<<(std::ostream& os, const moltk::Printable& p);

} // namespace moltk

#endif // MOLTK_PRINTABLE_H
