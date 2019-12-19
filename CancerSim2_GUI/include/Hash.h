//
//  Hash.h
//  CancerSim2_GUI
//
//  Created by Luis Cisneros on 11/5/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Hash.h
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

#ifndef Hash_h
#define Hash_h

// hash a sequence of ints into one int value.

class Hash
{
public:
    
    Hash() : m_state(0) { };
    
    void reset() { m_state=0; };
    
    void add(int n);
    
    int result() const { return m_state; };
    
protected:
    
    int m_state;
    
    void rotate();
    
};

#include <iosfwd>

std::ostream & operator<<(std::ostream &, const Hash &);

#endif /* Hash_h */
