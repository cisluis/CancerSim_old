//
//  Limited_History.h
//  CancerSim2
//
//  Created by Luis Cisneros on 11/8/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Limited_History.h
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


#ifndef LIMITEDHISTORYHFILE
#define LIMITEDHISTORYHFILE

// A limited history is a sequence to which values can be appended.
// However, the size of the sequence is limited to a specified bound "b".
// Only 1/(2^floor((n-1)/b)) of the values are retained.
// Thus, all values are initially kept.  After the bound is first
// exceeded, only every other value is kept.  When it is exceeded
// again, only 1/4 are kept, and so on.

// This is useful to keep trending data over a long period of time
// when a potentially infinite amount of data must not be kept.

// As a special case, a limit of 0 can be specified, meaning there
// is no limit and the History will grow without bound.

#include <vector>

template<class T>
class Limited_History
{
public:
    
    Limited_History(int limit=0) : m_num_appended(0), m_limit(limit), m_every_n(1) { };
    
    void append(const T & data)
    {
        if(!num_appended())
            m_min = m_max = data;
        else if(data > m_max)
            m_max = data;
        else if(data < m_min)
            m_min = data;
        
        ++m_num_appended;
        
        if((m_num_appended-1) % m_every_n)
            return; // data rejected because of sparse gathering
        
        if(m_data.size() == m_limit && m_limit > 0)
        {
            if((m_num_appended-1) % (m_every_n*2))
                return; // data rejected because of sparse gathering
            divide();
        }
        
        m_data.push_back(data);
        assert(m_data.size() <= m_limit);
    }
    
    // the number of times append() has been called
    int num_appended() const { return m_num_appended; };
    
    // the number of values held by the limited history.
    // This is equal to the number of values appended until
    // the limit is reached; then it falls by half and grows
    // half as quickly, until the limit is reached again, and so on.
    int size() const { return static_cast<int>(m_data.size()); };
    
    const T & operator[](int i) const { assert(i>=0&&i<size()); return m_data[i]; };
    
    // re-initialize the history, throwing out all values previous appened.
    void discard()
    {
        m_num_appended = 0;
        m_every_n = 1;
        m_data.resize(0);
    }
    
    // set the maximum size of the history
    void set_limit(int n)
    {
        assert(n >= 0);
        if(n)
            while(size() > n)
                divide();
        m_limit = n;
    }
    
    int limit() const { return m_limit; };
    
    T min() const { assert(num_appended() > 0); return m_min; };
    T max() const { assert(num_appended() > 0); return m_max; };
    
    // the length of the returned data equals 'size()' (not num_appended)
    const T * data() const { assert(num_appended() > 0); return &m_data[0]; };
    
protected:
    
    // throw out half the data and cut the acceptance rate in half.
    void divide()
    {
        const int new_size = (static_cast<int>(m_data.size()) + 1)/2;
        for(int i=0; i < new_size; ++i)
            m_data[i] = m_data[2*i];
        m_data.resize(new_size);
        m_every_n *= 2;
    }
    
    int m_num_appended;
    int m_limit;
    int m_every_n;
    std::vector<T> m_data;
    T m_min;
    T m_max;
};

#include <iosfwd>
template<class T>
std::ostream & operator<<(std::ostream & os, const Limited_History<T> & h)
{
    for(int i=0; i < h.size(); ++i)
        os << h[i] << ' ';
    return os;
}

#endif
