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
            const_iterator(const std::vector<int>& runsParam
                    , int runIndexParam
                    , int positionIndexParam) 
                    : runIndex(runIndexParam)
                    , runs(&runsParam)
                    , positionIndex(positionIndexParam)
                    , sequenceIndex(-1) // default to gap start
            {
                if ( (runs->size() > runIndex) && ((*runs)[runIndex] > 0) )
                    sequenceIndex = 0; // EString starts with non-gap
            }

            const_iterator& operator++() 
            {
                ++positionIndex;
                if (positionIndex >= std::abs((*runs)[runIndex])) 
                {
                    positionIndex = 0;
                    ++runIndex;
                }
                if ( (runIndex < runs->size()) 
                    && ( (*runs)[runIndex] > 0 ) )
                {
                    ++sequenceIndex;
                }
                return *this;
            }

            int operator*() const 
            {
                if (runIndex >= runs->size()) return -1; // end()
                if ((*runs)[runIndex] < 0) return -1; // gap
                return sequenceIndex; // current sequence position
            }

            bool operator!=(const const_iterator& rhs) 
            {
                if (runIndex != rhs.runIndex) return true;
                if (positionIndex != rhs.positionIndex) return true;
                return false;
            }
            bool operator==(const const_iterator& rhs)
            {
                return ! (*this != rhs);
            }

        protected:
            const std::vector<int>* runs;
            size_t runIndex;
            int positionIndex;
            size_t sequenceIndex;
        };


        EString operator*(const EString& rhs) const;

        EString& appendRun(int run) {
            if (run == 0) return *this; // run of nothing
            if (runs.size() == 0) { // first run
                runs.push_back(run);
                return *this;
            }
            if ( (run * runs.back()) > 0 ) { // same sign: combine
                runs.back() += run;
                return *this;
            }
            // different sign: append
            runs.push_back(run);
            return *this;
        }
        size_t ungappedLength() const;
        size_t totalLength() const;
        const_iterator begin() const {
            return const_iterator(runs, 0, 0);
        }
        const_iterator end() const {
            return const_iterator(runs, runs.size(), 0);
        }

        void reverse() {
            for (size_t i = 0; i < runs.size(); ++i)
                std::reverse(runs.begin(), runs.end());
        }

    protected:
        std::vector<int> runs;
    };

    
    class Row
    {
    public:
        List list; // which list: sequences or structures?
        int listIndex;
        Real sequenceWeight;
        EString eString;
    };


public:
    Alignment() {}
    /* implicit */ Alignment(const Biosequence&);
    /* implicit */ Alignment(const std::string&);
    /* implicit */ Alignment(const char*);
    explicit Alignment(std::istream& is);
    ~Alignment() {}
    Alignment& appendSequence(const Biosequence& seq);
    void loadString(const std::string& s);
    void printString(std::ostream& os) const;
    size_t getNumberOfColumns() const;
    Alignment align(const Alignment&, const EString&, const EString&) const;

    inline friend std::ostream& operator<<(std::ostream& os, const Alignment& ali)
    {
        ali.printString(os);
        return os;
    }


protected:
    std::vector<Biosequence> sequences;
    std::vector<PdbStructure::Chain> structures;
    std::vector<Row> rows;
};

} // namespace moltk


#endif // MOLTK_ALIGN_ALIGNMENT_H
