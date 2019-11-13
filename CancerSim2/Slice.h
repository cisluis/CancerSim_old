//
//  Slice.h
//  CancerSim2
//
//  Created by Luis Cisneros on 11/8/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Slice.h
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


#ifndef SLICEHFILE
#define SLICEHFILE

// access a 1-d collection as 2-d, assuming that the 1-d indices are
// linear combinations of the 2-d indexes.

template<class T>
class Slice
{
public:
    typedef typename T::reference reference;
    typedef typename T::const_reference const_reference;
    
    Slice() : m_ni(0), m_nj(0), m_wi(0), m_wj(0), m_c(0), m_1d(0) { };
    Slice(T * d, int ni, int nj, int wi, int wj, int c) : m_1d(d), m_ni(ni), m_nj(nj), m_wi(wi), m_wj(wj), m_c(c) { };
    //  void create(int ni, int nj, int wi, int wj, int c) { m_ni=ni; m_nj=nj; m_wi=wi; m_wj=wj; m_c=c; };
    reference at(int i, int j) { assert(is_index(i,j)); return (*m_1d)[i*m_wi+j*m_wj+m_c]; };
    reference operator()(int i, int j) { return at(i,j); };
    const_reference at(int i, int j) const { assert(is_index(i,j)); return (*m_1d)[i*m_wi+j*m_wj+m_c]; };
    const_reference operator()(int i, int j) const { return at(i,j); };
    bool is_index(int i, int j) const { return i >=0 && i < ni() && j >= 0 && j < nj(); };
    int ni() const { return m_ni; };
    int nj() const { return m_nj; };
    template<class E> void set_all(E e) { int i,j; for(i=0;i<ni();++i) for(j=0;j<nj();++j) at(i,j)=e; };
    
protected:
    int m_wi;
    int m_wj;
    int m_c;
    int m_ni;
    int m_nj;
    T * m_1d;
};

template<class T>
class Const_Slice
{
public:
    typedef typename T::const_reference const_reference;
    Const_Slice() : m_ni(0), m_nj(0), m_wi(0), m_wj(0), m_c(0), m_1d(0) { };
    Const_Slice(const T * d, int ni, int nj, int wi, int wj, int c) : m_1d(d), m_ni(ni), m_nj(nj), m_wi(wi), m_wj(wj), m_c(c) { };
    //  void create(int ni, int nj, int wi, int wj, int c) { m_ni=ni; m_nj=nj; m_wi=wi; m_wj=wj; m_c=c; };
    const_reference at(int i, int j) const { assert(is_index(i,j)); return (*m_1d)[i*m_wi+j*m_wj+m_c]; };
    const_reference operator()(int i, int j) const { return at(i,j); };
    bool is_index(int i, int j) const { return i >=0 && i < ni() && j >= 0 && j < nj(); };
    int ni() const { return m_ni; };
    int nj() const { return m_nj; };
    
protected:
    int m_wi;
    int m_wj;
    int m_c;
    int m_ni;
    int m_nj;
    const T * m_1d;
};

template<class S, class T>
void copy_slice(S s1, T s2)
{
    assert(s1.ni()==s2.ni()&&s1.nj()==s2.nj());
    for(int i=0; i<s1.ni(); ++i) for(int j=0; j<s1.nj(); ++j)
        s2(i,j)=s1(i,j);
};

template<class S, class T, class U, class OP>
void combine_slices(const S s1, const T s2, U s3, OP op) {
    assert(s1.ni()==s2.ni() && s2.ni()==s3.ni());
    assert(s1.nj()==s2.nj() && s2.nj()==s3.nj());
    
    for(int i=0; i<s1.ni(); ++i) for(int j=0; j<s1.nj(); ++j)
        s3(i,j)=op(s1(i,j),s2(i,j));
}

#include <iosfwd>

template<class T>
std::ostream & std::operator<<(ostream & os, const Slice<T> & s)
{
    for(int i=0; i < s.ni(); ++i)
    {
        for(int j=0; j < s.nj(); ++j)
            os << s(i,j) << ' ';
        os << std::endl;
    }
    return os;
}

template<class T>
std::ostream & operator<<(std::ostream & os, const Const_Slice<T> & s)
{
    for(int i=0; i < s.ni(); ++i)
    {
        for(int j=0; j < s.nj(); ++j)
            os << s(i,j) << ' ';
        os << std::endl;
    }
    return os;
}

#endif
