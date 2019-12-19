//
//  Bit_Array_3d.h
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

#ifndef Bit_Array_3d_h
#define Bit_Array_3d_h

#include "Array_3d.h"
#include "Bit_Vector.h"

class Bit_Array_3d : public Array_3d<bool, Bit_Vector>
{
public:
    
    Bit_Array_3d() { };
    Bit_Array_3d(int ni, int nj, int nk) : Array_3d<bool, Bit_Vector>(ni,nj,nk) { };
    
    void reset() { m_elem.reset(); };
    void flip() { m_elem.flip(); };
    Bit_Array_3d & operator&=(const Bit_Array_3d & rhs) { m_elem &= rhs.m_elem; return *this; };
    Bit_Array_3d & operator|=(const Bit_Array_3d & rhs) { m_elem |= rhs.m_elem; return *this; };
    Bit_Array_3d & operator^=(const Bit_Array_3d & rhs) { m_elem ^= rhs.m_elem; return *this; };
    
};


#endif /* Bit_Array_3d_h */
