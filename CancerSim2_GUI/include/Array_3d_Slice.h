//
//  Array_3d_Slice.h
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


// Utility functions for creating slices of 3d arrays
// Placing the functions here prevents dependencies
// among slices and arrays

#include "Slice.h"

// create a slice to access values at constant i
template<class T> Slice<T> slice_i(T & a, int i)
{
    return Slice<T>(&a, a.nk(), a.nj(), 1, a.nk(), a.nk()*a.nj()*i);
};

// create a slice to access values at constant j
template<class T> Slice<T> slice_j(T & a, int j)
{
    return Slice<T>(&a, a.ni(), a.nk(), a.nk()*a.nj(), 1, j*a.nk());
};

// create a slice to access values at constant k
template<class T> Slice<T> slice_k(T& a, int k)
{
    return Slice<T>(&a, a.ni(), a.nj(), a.nk()*a.nj(), a.nk(), k);
};

// create a slice of paramaterized dimension
// Use 0, 1, or 2 for i, j, or k, respectively
template<class T> Slice<T> slice(int dim, T& a, int n)
{
    switch(dim)
    {
        case 0: return slice_i(a,n);
        case 1: return slice_j(a,n);
        case 2: return slice_k(a,n);
        default: assert(!"the 'dim' argument to 'slice' must be 0, 1, or 2");
            return Slice<T>();
    };
};


// create a slice to access values at constant i
template<class T> Const_Slice<T> slice_i(const T& a, int i)
{
    return Const_Slice<T>(&a, a.nk(), a.nj(), 1, a.nk(), a.nk()*a.nj()*i);
};

// create a slice to access values at constant j
template<class T> Const_Slice<T> slice_j(const T& a, int j)
{
    return Const_Slice<T>(&a, a.ni(), a.nk(), a.nk()*a.nj(), 1, j*a.nk());
};

// create a slice to access values at constant k
template<class T> Const_Slice<T> slice_k(const T& a, int k)
{
    return Const_Slice<T>(&a, a.ni(), a.nj(), a.nk()*a.nj(), a.nk(), k);
};

// create a slice of paramaterized dimension
// Use 0, 1, or 2 for i, j, or k, respectively
template<class T> Const_Slice<T> slice(int dim, const T& a, int n)
{
    switch(dim)
    {
        case 0: return slice_i(a,n);
        case 1: return slice_j(a,n);
        case 2: return slice_k(a,n);
        default: assert(!"the 'dim' argument to 'slice' must be 0, 1, or 2");
            return Const_Slice<T>();
    };
};



//    Slice<Array_3d<int> > slice;

//    slice = slice_i(a, 1);
//    for(int i=0; i < slice.ni(); ++i)
//    {
//      for(int j=0; j < slice.nj(); ++j)
//        cout << slice(i,j) << ' ';
//      cout << endl;
//    }
//    cout << endl;
//    for(int k=0; k < a.nk(); ++k)
//    {
//      for(int j=0; j < a.nj(); ++j)
//        cout << a(1,j,k) << ' ';
//      cout << endl;
//    }
//    cout << endl << endl;


//    slice = slice_j(a, 1);
//    for(int i=0; i < slice.ni(); ++i)
//    {
//      for(int j=0; j < slice.nj(); ++j)
//        cout << slice(i,j) << ' ';
//      cout << endl;
//    }
//    cout << endl;
//    for(int i=0; i < a.ni(); ++i)
//    {
//      for(int k=0; k < a.nk(); ++k)
//        cout << a(i,1,k) << ' ';
//      cout << endl;
//    }
//    cout << endl << endl;


//    slice = slice_k(a, 1);
//    for(int i=0; i < slice.ni(); ++i)
//    {
//      for(int j=0; j < slice.nj(); ++j)
//        cout << slice(i,j) << ' ';
//      cout << endl;
//    }
//    cout << endl;
//    for(int i=0; i < a.ni(); ++i)
//    {
//      for(int j=0; j < a.nj(); ++j)
//        cout << a(i,j,1) << ' ';
//      cout << endl;
//    }

//    return 0;
//  }

