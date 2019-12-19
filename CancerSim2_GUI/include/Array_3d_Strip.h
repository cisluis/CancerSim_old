//
//  Array_3d_Strip.h
//  CancerSim2_GUI
//
//  Created by Luis Cisneros on 11/8/19.
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


// Utility functions for creating strips of 3d arrays.

#include "Array_3d.h"
#include "Strip.h"

// create a strip to access value at constant j,k
template<class T> Strip<Array_3d<T> > strip_jk(Array_3d<T>& a, int j, int k)
{
  return Strip<Array_3d<T> >(&a, a.ni(), a.nk()*a.nj(), k+a.nk()*j);
};

// create a strip to access value at constant i,k
template<class T> Strip<Array_3d<T> > strip_ik(Array_3d<T>& a, int i, int k)
{
  return Strip<Array_3d<T> >(&a, a.nj(), a.nk(), k+i*a.nk()*a.nj());
};

// create a strip to access value at constant i,j
template<class T> Strip<Array_3d<T> > strip_ij(Array_3d<T>& a, int i, int j)
{
  return Strip<Array_3d<T> >(&a, a.nk(), 1, a.nk()*j+a.nk()*a.nj()*i);
};

template<class T> Strip<Array_3d<T> > strip(int dim, Array_3d<T> & a, int i, int j)
{
  switch(dim)
  {
  case 0: return strip_jk(a, j, i);
  case 1: return strip_ik(a, i, j);
  case 2: return strip_ij(a, i, j);
  default: assert(0); return Strip<Array_3d<T> >();
  };
};
