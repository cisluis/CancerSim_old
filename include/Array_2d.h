//
//  Array_2d.h
//  CancerSim2
//
//  Created by Luis Cisneros on 11/8/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Array_2d.h
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


#ifndef ARRAY2DHFILE
#define ARRAY2DHFILE

#include <assert.h>
#include <vector>

template<class T, class C = std::vector<T> >
class Array_2d
{
public:
    
    typedef typename C::reference reference;
    typedef typename C::const_reference const_reference;
    
    Array_2d() : m_rows(0), m_cols(0) { };
    Array_2d(int rows, int cols) { resize(rows,cols); };
    
    reference at(int row, int col)
    {
        assert(is_index(row,col));
        return m_elem[map2d(row,col)];
        
    };
    
    reference operator()(int row, int col){ return at(row,col); };
    
    const_reference at(int row, int col) const
    {
        assert(is_index(row,col));
        return m_elem[map2d(row,col)];
        
    };
    
    const_reference operator()(int row, int col) const { return at(row,col); };
    
    const bool is_index(int row, int col) const
    {
        return row >= 0 && row < rows() && col >= 0 && col < cols();
        
    };
    
    void set_all(const_reference t) { std::fill(m_elem.begin(), m_elem.end(), t); };
    
    void resize(int rows, int cols)
    {
        m_rows=rows;
        m_cols=cols;
        m_elem.resize(rows*cols);
        
    };
    
    int rows() const { return m_rows; };
    int cols() const { return m_cols; };
    
protected:
    
    int map2d(int row, int col) const { return col+row*m_cols; };
    
    int m_rows;
    int m_cols;
    
    C m_elem;
};

#include <iosfwd>

template<class T>
std::ostream & operator<<(std::ostream & os, const Array_2d<T> & a)
{
    for(int i=0; i < a.rows(); ++i)
    {
        for(int j=0; j < a.cols(); ++j)
            os << a(i,j) << ' ';
        os << std::endl;
    }
    return os;
}

#endif
