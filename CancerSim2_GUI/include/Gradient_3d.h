//
//  Gradient_3d.h
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

// a 3d array supporting only dimensions which are powers of 2


#ifndef Gradient_3d_h
#define Gradient_3d_h

#include "Array_3d.h"
#include <math.h>

// This class is supposed to represent the steady-state concentrations
// resulting from diffusion from multiple sources

template<class T>
class Gradient_3d
{
public:
    
    Gradient_3d() { };
    Gradient_3d(int ni, int nj, int nk) { resize(ni,nj,nk); };
    
    void add_source(int srci, int srcj, int srck, T strength)
    {
        for(int i=0; i < ni(); ++i)
        {
            const int di = (i-srci)*(i-srci);
            for(int j=0; j < nj(); ++j)
            {
                const int dj = (j-srcj)*(j-srcj);
                for(int k=0; k < nk(); ++k)
                {
                    const int dk = (k-srck)*(k-srck);
                    
                    const int sos = di+dj+dk;
                    
                    if(sos < m_cache.size())
                        m_data(i,j,k) += strength * m_cache[sos];
                    
                    assert(m_data(i,j,k) >= 0.0f);
                }
            }
        }
    };
    
    void remove_all_sources() { m_data.set_all((T)0); };
    
    // retrieve the concentration at a point
    const T & get(int i, int j, int k) const { return m_data(i,j,k); };
    const T & operator()(int i, int j, int k) const { return m_data(i,j,k); };
    
    const bool is_index(int i, int j, int k) const { return i >= 0 && i < ni() &&
        j >= 0 && j < nj() &&
        k >= 0 && k < nk(); };
    
    //  void resize(int ni, int nj, int nk) { m_data.resize(ni,nj,nk); remove_all_sources(); };
    
    void resize(int ni, int nj, int nk)
    {
        m_data.resize(ni,nj,nk);
        remove_all_sources();
        
        // don't consider the contribution of a source to points where its
        // strength is less than 1/'precision'
        const int precision = 10000;
        const int cache_size = (int) ceil(pow(10.0, 2.0/3.0*log10(precision)));
        m_cache.resize(cache_size);
        m_cache[0] = 1.0f;
        for(int i=1; i < m_cache.size(); ++i)
            m_cache[i] = (float)(1.0 / pow(i, 1.5));
    };
    
    int ni() const { return m_data.ni(); };
    int nj() const { return m_data.nj(); };
    int nk() const { return m_data.nk(); };
    
protected:
    
    Array_3d<T> m_data;
    std::vector<T> m_cache;
};

#endif /* Gradient_3d_h */
