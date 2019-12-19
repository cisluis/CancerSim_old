//
//  Bit_Vector.cpp
//  CancerSim2
//
//  Created by Luis Cisneros on 11/8/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Bit_Vector.cc
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


#include "Bit_Vector.h"

const size_t Bit_Vector::Word_Size = Bit_Vector_Word_Size;

std::ostream & operator<<(std::ostream & os, Bit_Vector v)
{
    for (int i=0; i < v.size(); ++i) os << (int) v[i];
    return os;
}
    
    //  int main(int argc, char * argv[])
    //  {
    //    Bit_Vector v1(40);
    //    Bit_Vector v2(v1);
    
    //    for(int i=0; i < v1.size(); ++i)
    //    {
    //      if(rand()%3 == 0)
    //        v1[i] = true;
    //      if(rand()%3 == 0)
    //        v2[i] = true;
    //    }
    
    //    Bit_Vector v3 = v2;
    
    //    cout << v1 << endl << v2 << endl;
    
    //    v2 &= v1;
    //    cout << "&" << endl << v2 << endl;
    
    //    v2=v3;
    //    v2|=v1;
    //    cout << "|" << endl << v2 << endl;
    
    //    v2=v3;
    //    v2^=v1;
    //    cout << "^" << endl << v2 << endl;
    
    //    return 0;
    
    //  }
//
//  Bit_Vector.cpp
//  CancerSim2
//
//  Created by Luis Cisneros on 12/17/19.
//  Copyright © 2019 Luis Cisneros. All rights reserved.
//

#include <stdio.h>
