//
//  Strip_Slice_Array_3d_Test.cpp
//  CancerSim2
//
//  Created by Luis Cisneros on 12/13/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.

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


#include <iostream>
#include "Array_3d_Slice.h"
#include "Array_3d_Strip.h"

// tests strips and slices of 3d Arrays.

int main(int argc, char * argv[])
{
  Array_3d<int> a(3,4,5);
  
  for(int i=0; i < a.ni(); ++i)
    for(int j=0; j < a.nj(); ++j)
      for(int k=0; k < a.nk(); ++k)
    a(i,j,k) = i*100+j*10+k;

  cout << "the digit in the 100's place should always be 1" << endl << slice_i(a,1) << endl;
  cout << "the digit in the 10's place should always be 2" << endl << slice_j(a,2) << endl;
  cout << "the digit in the 1's place should always be 3" << endl << slice_k(a,3) << endl;
  cout << "the digit in the 10's place should always be 2, and the 1's place is 3" << endl << strip_jk(a,2,3) << endl;
  cout << "the digit in the 100's place should always be 1, and the 1's place is 2" << endl << strip_ik(a,1,2) << endl;
  cout << "the digit in the 100's place should always be 2, and the 10's place is 3" << endl << strip_ij(a,2,3) << endl;

  return 0;
}
