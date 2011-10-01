#ifndef MOLTK_ALIGN_ALIGNMENT_H
#define MOLTK_ALIGN_ALIGNMENT_H

#include <iostream>
#include <string>
#include <vector>

namespace moltk {

class Alignment : public std::vector< std::string >
{
};

} // namespace moltk

std::ostream& operator<<(std::ostream&, const moltk::Alignment&);

#endif // MOLTK_ALIGN_ALIGNMENT_H
