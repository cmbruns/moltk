#ifndef MOLTK_VEC3_HPP
#define MOLTK_VEC3_HPP

#include "moltk/units.h"

namespace moltk {

template<class ELT>
class Vec3
{
public:
    typedef ELT value_type;
    typedef size_t size_type;
    typedef size_t index_type;
    typedef ELT* iterator;
    typedef ELT const * const_iterator;
    static const size_type SIZE = 3;

    ELT x;
    ELT y;
    ELT z;

    operator ELT*() {return &x;}
    operator const ELT*() const {return &x;}
    static size_type size() {return SIZE;}

    iterator begin() {return &x;}
    const_iterator begin() const {return &x;}
    iterator end() {return &(&x)[SIZE];}
    const_iterator end() const {return &(&x)[SIZE];}
};

typedef Vec3< moltk::units::Length > Displacement;

} // namespace moltk

#endif // MOLTK_VEC3_HPP

