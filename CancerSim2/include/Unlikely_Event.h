//
//  Unlikely_Event.h
//  CancerSim2
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


#ifndef Unlikely_Event_h
#define Unlikely_Event_h

#include "Random_Number_Generator.h"

// The Unlikely_Event uses the geometric distribution to efficiently
// implement a random event that occurrs only rarely.

// The formula used for the geometric distribution is due to Knuth,
// "Seminumerical Algorithms"

class Unlikely_Event
{
public:
    
    // the event occurs with 1/p probability
    Unlikely_Event(double p=2, int seed=1, int state_size=128);
    
    // state_size may be 8, 32, 64, 128, or 256.
    // Bigger=better (more randomish)
    void init(double p, unsigned int seed, int state_size=128);
    
    bool now()
    {
        // move event up the list queue,
        // if first => false, if not first => reset
        --m_n;
        if (m_n) return false;
        reset();
        return true;
        
    };
    
    int when()
    {
        // return value current value of m_n and reset it
        const int n(m_n);
        reset();
        return n;
        
    };
    
    bool operator()() { return now(); };
    
private:
    
    void reset();
    
    Random_Number_Generator m_rand;
    double m_p;
    int m_n;
    
};

#endif /* Unlikely_Event_h */
