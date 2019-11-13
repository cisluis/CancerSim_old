//
//  Bit_Array_2d.h
//  CancerSim2
//
//  Created by Luis Cisneros on 11/8/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Bit_Array_2d.h
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

#ifndef BIT_ARRAY_2D_HFILE
#define BIT_ARRAY_2D_HFILE

#include "Bit_Vector.h"
#include "Array_2d.h"

typedef Array_2d<Bit_Vector::reference, Bit_Vector> Bit_Array_2d;

// This is problematic on the Mac (cc 3.3), so inline was added.

template <> inline void Bit_Array_2d::set_all(const_reference t) {
    if(t)
        m_elem.set();
    else
        m_elem.reset();
}

#endif
