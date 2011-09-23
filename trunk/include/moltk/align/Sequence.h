#ifndef MOLTK_ALIGN_SEQUENCE_H
#define MOLTK_ALIGN_SEQUENCE_H

#include <string>
#include <iostream>
#include <vector>

namespace moltk {

class SequencePosition
{
};


class Sequence : public std::vector<SequencePosition*>
{
public:
    typedef SequencePosition Position;

    Sequence(const std::string&);
};

} // namespace moltk

std::ostream& operator<<(std::ostream&, const moltk::Sequence&);

#endif MOLTK_ALIGN_SEQUENCE_H

