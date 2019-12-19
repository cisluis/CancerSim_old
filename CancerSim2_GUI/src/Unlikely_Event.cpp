//
//  Unlikely_Event.cpp
//  CancerSim2_GUI
//
//  Created by Luis Cisneros on 11/8/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Unlikely_Event.cc
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


#include "Unlikely_Event.h"
#include <math.h>

Unlikely_Event::Unlikely_Event(double p, int seed, int state_size) :
m_p(p), m_rand(seed, state_size)
{
    reset();
}

void Unlikely_Event::init(double p, unsigned int seed, int state_size)
{
    m_p = p;
    m_rand.srand(seed, state_size);
    reset();
}

void Unlikely_Event::reset()
{
    double n = ceil(log((double)m_rand()/RAND_MAX) / log(1-1/m_p));
    if(n > INT_MAX)
        // what to do about this?  It should be very rare for 'small' n, but...
        n = INT_MAX;
    m_n = int(n);
}

/*
#ifdef UNLIKELY_EVENT_UNIT_TEST

#include <iostream>

int main(int argc, char * argv[])
{
        Unlikely_Event e;
    
        e.init(5.5, time(0));
    
        int x=0;
        int numx=0;
        int totalx=0;
        for(int i=0; i < 100000; ++i)
        {
          ++x;
          if(e())
          {
            totalx += x;
            x=0;
            ++numx;
          }
        }
    
        cout << double(totalx)/numx << endl;
    
        for(int i=0; i < 100; ++i)
          cout << (e() ? 'X' : '-') << ' ';
        cout << endl;
    
    
    
        for(int i=10000; i <= 1000000; i += 5000)
        {
          Unlikely_Event e(i, i*i);
          double total = 0.0;
          int j;
          for(j=0; j < 1000; ++j)
            total += e.when();
          cout << i << ' ' << total / j << endl;
        }
    
    
        Unlikely_Event e1(100, time(0)), e2(10000, 30);
    
        for(int i=0; i < 1024; ++i)
          e1();
        e2=e1;
        for(int i=0; i < 1000; ++i)
        {
          if(e1())
            cout << e2() << ' ';
          else
          {
            if(e2())
          cout << "ERROR" << endl;
          }
        }
    
    return 0;
}

#endif
 
 */
