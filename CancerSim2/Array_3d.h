//
//  Array_3d.h
//  CancerSim2
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

#ifndef Array_3d_h
#define Array_3d_h

#include <assert.h>
#include <vector>

class Index_3d
{
public:
    
    // Default Constructure is an initiallitation list
    Index_3d() : m_ni(0), m_nj(0), m_nk(0), m_n(0) { };
    
    // Constructs an Index_3d object
    Index_3d(int ni, int nj, int nk) : m_ni(ni), m_nj(nj), m_nk(nk), m_n(ni*nj*nk) { };
    
    // Return values of indeces
    int ni() const { return m_ni; };
    int nj() const { return m_nj; };
    int nk() const { return m_nk; };
    int n() const { return m_n; };
    
    // overload function call to rotate indices (???)
    int operator()(int i, int j, int k) const { return k+nk()*(j+nj()*i); };
    
    // Resizes Index_3d object
    void resize(int ni, int nj, int nk) { m_ni=ni; m_nj=nj; m_nk=nk; m_n=ni*nj*nk; };
    
    // overload function call to compare two points
    bool operator==(const Index_3d & i) const { return
        ni()==i.ni() &&
        nj()==i.nj() &&
        nk()==i.nk(); };
    
    // check if index is in lattice
    const bool is_index(int i, int j, int k) const { return
        i >= 0 && i < ni() &&
        j >= 0 && j < nj() &&
        k >= 0 && k < nk(); };
    
protected:
    
    int m_ni;
    int m_nj;
    int m_nk;
    int m_n; // n = ni*nj*nk
};


template <class T, class CONT=std::vector<T> >
class Array_3d
{
public:
    
    typedef typename CONT::reference reference;
    typedef typename CONT::const_reference const_reference;
    
    // Constructors
    Array_3d() { };
    Array_3d(int ni, int nj, int nk) { resize(ni,nj,nk); };
    
    // return element at point(i,j,k)
    reference at(int i, int j, int k) {
        assert(is_index(i,j,k));
        return m_elem[m_3d(i,j,k)]; };
    
    // overload function call to return element at point(i,j,k)
    reference operator()(int i, int j, int k) { return at(i,j,k); };
    
    // return const element at point(i,j,k)
    const_reference at(int i, int j, int k) const {
        assert(is_index(i,j,k));
        return m_elem[m_3d(i,j,k)]; };
    
    // overload function call to return const element at point(i,j,k)
    const_reference operator()(int i, int j, int k) const { return at(i,j,k); };
    
    // check if point(i,j,k) is in lattice
    const bool is_index(int i, int j, int k) const { return m_3d.is_index(i,j,k); };
    
    // fill up elements in lattice
    void set_all(const T & t) { std::fill(m_elem.begin(), m_elem.end(), t); };
    
    // Resize lattice
    void resize(int ni, int nj, int nk) { m_3d.resize(ni,nj,nk); m_elem.resize(ni*nj*nk); };
    
    // Return side sizes of lattice
    int ni() const { return m_3d.ni(); };
    int nj() const { return m_3d.nj(); };
    int nk() const { return m_3d.nk(); };
    
    // 1d access is supported for quickly traversing all elements
    bool is_index(int i) const { return i >=0 && i < n(); };
    int n() const { return m_3d.n(); };
    reference operator[](int i) { assert(is_index(i)); return m_elem[i]; };
    const_reference operator[](int i) const { assert(is_index(i)); return m_elem[i]; };
    bool operator==(const Array_3d<T,CONT> & a) const { return m_elem == a.m_elem && m_3d == a.m_3d; };
    
protected:
    
    Index_3d m_3d;
    CONT m_elem;
};

#endif /* Array_3d_h */
