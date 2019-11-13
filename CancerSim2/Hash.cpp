//
//  Hash.cpp
//  CancerSim2
//
//  Created by Luis Cisneros on 11/5/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Hash.cc
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


#include "Hash.h"
//#include <assert>

#include <iostream>

// shift bit values in sequence state
void Hash::add(int n)
{
    // rotate the internal state by r
    const int r = 7;
    //const int prev_state = m_state;
    
    m_state = (m_state << r) | ((unsigned int &) m_state) >> (((sizeof(m_state)<<3)-r));
    m_state ^= n;
    
    
}

// Overloads print operator
std::ostream & operator<<(std::ostream & os, const Hash & hash)
{
    int h = hash.result();
    // set only the most significant bit.
    int m = ~(~0u>>1);
    
    os << ((h & m) ?  1 : 0);
    for (h = ((h << 1) | 1); h != m; h<<=1)
    {
        os << ((h&m) ? 1 : 0);
    }
    
    return os;
    
}

/*
#ifdef HASHTESTING
    
int main(int argc, char * argv[])
{
    Hash h;
    
    h.add(1);
    h.add(2);
    
    for(int i=0; i < 33; ++i)
    {
        cout << h << endl;
        h.add(0);
    }
}
    
#endif
*/
