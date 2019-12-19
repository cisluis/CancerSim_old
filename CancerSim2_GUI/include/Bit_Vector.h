//
//  Bit_Vector.h
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

#ifndef Bit_Vector_h
#define Bit_Vector_h

#include <bitset>
#include <vector>

// this bit vector differs from the std::vector<bool> specialization
// in that it supports bitwise operators.

// this should not be used directly in code; use Bit_Vector::Word_Size wherever possible
#define Bit_Vector_Word_Size 32

class Bit_Vector
{
public:
    
    static const size_t Word_Size;
    typedef std::bitset<Bit_Vector_Word_Size> Elem;
    typedef Elem::reference reference;
    typedef bool const_reference;
    typedef std::vector<Elem> Cont;
    
    Bit_Vector() : m_size(0) { };
    Bit_Vector(size_t num_bits) { resize(num_bits); };
    
    reference at(size_t n)
    {
        assert(is_index(n));
        return m_bits[n/Word_Size][n%Word_Size];
        
    };
    
    reference operator[](size_t n)
    {
        return at(n);
        
    };
    
    const_reference at(size_t n) const
    {
        assert(is_index(n));
        return m_bits[n/Word_Size][n%Word_Size];
        
    };
    
    const_reference operator[](size_t n) const
    {
        return at(n);
        
    };
    
    bool is_index(size_t n) const
    {
        return n >= 0 && n < size();
        
    };
    
    // get the number of bits.
    size_t size() const
    {
        return m_size;
        
    };
    
    void resize(size_t n)
    {
        m_bits.resize(n/Word_Size + (n % Word_Size ? 1 : 0));
        m_size=n;
        
    };
    
    void set()
    {
        for (Cont::iterator i=m_bits.begin(); i!=m_bits.end(); ++i) i->set();
        
    };
    
    void reset()
    {
        for(Cont::iterator i=m_bits.begin(); i!=m_bits.end(); ++i) i->reset();
        
    };
    
    Bit_Vector & operator&=(const Bit_Vector & rhs)
    {
        assert(size() <= rhs.size());
        Cont::const_iterator i2=rhs.m_bits.begin();
        for(Cont::iterator i=m_bits.begin(); i!=m_bits.end(); ++i, ++i2) *i &= *i2;
        return *this;
        
    };
    
    Bit_Vector & operator|=(const Bit_Vector & rhs)
    {
        assert(size() <= rhs.size());
        Cont::const_iterator i2=rhs.m_bits.begin();
        for(Cont::iterator i=m_bits.begin(); i!=m_bits.end(); ++i, ++i2) *i |= *i2;
        return *this;
        
    };
    
    Bit_Vector & operator^=(const Bit_Vector & rhs)
    {
        assert(size() <= rhs.size());
        Cont::const_iterator i2=rhs.m_bits.begin();
        for(Cont::iterator i=m_bits.begin(); i!=m_bits.end(); ++i, ++i2) *i ^= *i2;
        return *this;
        
    };
    
    Bit_Vector & flip()
    {
        for(Cont::iterator i=m_bits.begin(); i!=m_bits.end(); ++i) i->flip();
        return *this;
        
    };
    
    std::bitset<Bit_Vector_Word_Size> word(int i) const
    {
        assert(i>=0&&i<(int)m_bits.size());
        return m_bits[i];
        
    };
    
protected:
    
    Cont m_bits;
    size_t m_size;
    
};

std::ostream & operator<<(std::ostream & os, Bit_Vector v);

#endif /* Bit_Vector_h */
