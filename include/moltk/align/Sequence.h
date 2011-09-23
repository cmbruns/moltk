#ifndef MOLTK_ALIGN_SEQUENCE_H
#define MOLTK_ALIGN_SEQUENCE_H

#include <string>
#include <iostream>
#include <vector>
#include "moltk/align/Position.h"

namespace moltk { namespace align {

class Sequence : public std::vector<Position*>
{
public:
    Sequence() {}
    Sequence(const Sequence& rhs);
    virtual ~Sequence();
    Sequence& operator=(const Sequence& rhs);
};

}} // namespace moltk::align

std::ostream& operator<<(std::ostream&, const moltk::align::Sequence&);

#endif MOLTK_ALIGN_SEQUENCE_H

