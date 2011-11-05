/*
    MolTK is a Python and C++ toolkit for protein sequence/structure alignment and visualization
    Copyright (C) 2011  Christopher M. Bruns

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    
    Commercial users should ask about our dual licensing model.
    For questions contact: cmbruns@rotatingpenguin.com
*/

#ifndef MOLTK_ESTRING_HPP
#define MOLTK_ESTRING_HPP

#include <iostream>
#include <vector>

namespace moltk {

/// A compact representation of the gapping pattern for one sequence in an alignment.
class EString
{
public:

    /*!
     * Iterator for accessing the residue positions encoded in an estring.
     *
     * EString::const_iterator emits one residue number for each
     * column in an alignment.  Positions with a gap dereference to
     * residue number -1.  Other positions dereference to the
     * ordinal residue position index in the corresponding sequence.
     */
    class const_iterator
    {
    public:
        const_iterator() {}
        const_iterator(const std::vector<int>& runs
                , int run_index
                , int position_index);
        const_iterator& operator++();
        int operator*() const ;
        bool operator!=(const const_iterator& rhs);
        bool operator==(const const_iterator& rhs);

    protected:
        const std::vector<int>* runs;
        size_t run_index;
        int position_index;
        size_t sequence_index;
    };


    EString();
    EString operator*(const EString& rhs) const;
    EString& append_run(int run);
    EString& operator<<(int run) {append_run(run); return *this;}
    size_t ungapped_length() const;
    size_t total_length() const;
    const_iterator begin() const;
    const_iterator end() const;
    void reverse();
    bool operator==(const EString& rhs) const { return runs == rhs.runs; }
    bool operator!=(const EString& rhs) const { return runs != rhs.runs; }
    inline friend std::ostream& operator<<(std::ostream& os, const EString& e)
    {
        os << "<";
        std::vector<int>::const_iterator i;
        for (i = e.runs.begin(); i != e.runs.end(); ++i) 
        {
            if (i != e.runs.begin()) os << ", ";
            os << *i;
        }
        os << ">";
        return os;
    }

protected:
    std::vector<int> runs;
    size_t m_ungapped_length;
    size_t m_total_length;
};

} // namespace moltk


#endif // MOLTK_ESTRING_HPP
