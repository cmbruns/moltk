#ifndef MOLTK_ALIGN_ALIGNMENT_H
#define MOLTK_ALIGN_ALIGNMENT_H

#include <iostream>
#include <string>
#include <vector>
#include "moltk/Printable.h"

namespace moltk {

class Alignment : public std::vector< std::string > , public moltk::Printable
{
public:
    virtual void print_to_stream(std::ostream& os) const;
};

// std::ostream& operator<<(std::ostream& os, const moltk::Alignment& p);

} // namespace moltk


#endif // MOLTK_ALIGN_ALIGNMENT_H
