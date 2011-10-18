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
    
    For questions contact: cmbruns@rotatingpenguin.com
*/

#ifndef MOLTK_ALIGN_ALIGNMENT_H
#define MOLTK_ALIGN_ALIGNMENT_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // reverse
#include "moltk/Biosequence.h"
#include "moltk/PdbStructure.h"
#include "moltk/Real.h"
#include "moltk/units.h"

namespace moltk {

class Alignment
{
public:


    // index into displayOrder
    enum List {
        SequenceList,
        StructureList
    };


    class EString
    {
    public:

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

    
    class Row
    {
    public:
        List list; // which list: sequences or structures?
        int list_index;
        Real sequence_weight;
        EString eString;
    };


public:
    Alignment() : m_score(0.0 * moltk::units::bit) {}
    /* implicit */ Alignment(const Biosequence&);
    /* implicit */ Alignment(const std::string&);
    /* implicit */ Alignment(const char*);
    // explicit Alignment(std::istream& is);
    ~Alignment() {}
    Alignment& append_sequence(const Biosequence& seq);
    void load_string(const std::string& s);
    void print_string(std::ostream& os) const;
    size_t get_number_of_columns() const;
    // get_number_of_sequences() includes both sequences and structures
    size_t get_number_of_sequences() const {return rows.size();}
    Alignment align(const Alignment&, const EString&, const EString&) const;
    const BaseBiosequence& get_sequence(size_t index) const
    {
        const Row& row = rows[index];
        if (row.list == SequenceList)
            return sequences[row.list_index];
        else
            return structures[row.list_index];
    }
    const EString& get_estring(size_t index) const
    {
        return rows[index].eString;
    }
    const moltk::units::Information& score() const {return m_score;}
    Alignment& set_score(const moltk::units::Information& s) 
    {
        m_score = s;
        return *this;
    }

    inline friend std::ostream& operator<<(std::ostream& os, const Alignment& ali)
    {
        ali.print_string(os);
        return os;
    }


protected:
    std::vector<Biosequence> sequences;
    std::vector<PdbStructure::Chain> structures;
    std::vector<Row> rows;
    moltk::units::Information m_score;
};

} // namespace moltk


#endif // MOLTK_ALIGN_ALIGNMENT_H
