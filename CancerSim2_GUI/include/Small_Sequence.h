//
//  Small_Sequence.h
//  CancerSim2_GUI
//
//  Created by Luis Cisneros on 10/30/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Copyright (C) 2002  Robert Abbott
//     This file is part of CancerSim

//     This program is free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.

//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.

//     You should have received a copy of the GNU General Public License
//     along with this program; if not, write to the Free Software
//     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


// Small_Sequence is a short sequence of small nonnegative numbers.

#ifndef Small_Sequence_h
#define Small_Sequence_h

#include <assert.h>

class Small_Sequence
{
    typedef unsigned int State;
    
public:
    
    // Construct empty sequence
    Small_Sequence() : m_s(~0) { };
    
    // Construct a copy from input s
    Small_Sequence(const Small_Sequence &s) : m_s(s.m_s) { };
    
    // get size of sequence (leftmost empty byte)
    int size() const {
        State s=0xF;
        int i;
        for(i=0; i<8; ++i, s<<=4) {
            if((m_s & s) == s) return i;  }
        return i; };
    
    // check if the sequence is empty
    bool empty() const {
        return *this == Small_Sequence(); };
    //    return m_s == ~0; };
    
    // check if the sequence is full
    bool full() const {
        return (m_s >> 28) != 0xF; };
    
    // append an int to leftmost empty byte of sequence
    void append(unsigned int n) {
        assert( !full() );
        assert( n < 0xF );
        m_s += ((n+~0xE)<<(4*size())); };
    
    // Overloads [] operator to access sequence elements in array style
    // returns integer value in byte i
    int operator[] (int i) const {
        assert( i>=0 && i<size() );
        return 0xF & (m_s >> (i*4)); };
    
    // Overloads == operator to compare two sequences
    bool operator== (const Small_Sequence& s) const {
        return m_s == s.m_s; };
    
    // Overloads assigment operator
    //Small_Sequence& operator= (const Small_Sequence& s) {
    //    m_s = s.m_s;
    //    return *this; };
    
    // Overloads < operator to assign a sequence
    bool operator< (const Small_Sequence & s) const {
        return m_s < s.m_s; };
    
    // Returns value of sequence
    unsigned int as_int() const {
        return m_s; };
    
private:
    
    State m_s;
};

// Overloads print operator
#include <iosfwd>
std::ostream& operator<< (std::ostream &os, const Small_Sequence &s);

#endif /* Small_Sequence_h */
