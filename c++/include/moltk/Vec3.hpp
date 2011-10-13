#ifndef MOLTK_VEC3_HPP
#define MOLTK_VEC3_HPP

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

    union {
        ELT data[SIZE];
        struct { ELT x, y, z; };
    };

    operator ELT*() {return data;}
    operator const ELT*() const {return data;}
    static size_type size() {return SIZE;}

    iterator begin() {return &data[0];}
    const_iterator begin() const {return &data[0];}
    iterator end() {return &data[SIZE];}
    const_iterator end() const {return &data[SIZE];}
};

} // namespace moltk

#endif // MOLTK_VEC3_HPP

