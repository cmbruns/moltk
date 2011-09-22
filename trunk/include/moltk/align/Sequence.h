#ifndef MOLTK_ALIGN_SEQUENCE_H
#define MOLTK_ALIGN_SEQUENCE_H

#include <string>
#include <iostream>

namespace moltk {

class Sequence
{
public:
    Sequence(const std::string&);
};

} // namespace moltk

std::ostream& operator<<(std::ostream&, const moltk::Sequence&);

#endif MOLTK_ALIGN_SEQUENCE_H

