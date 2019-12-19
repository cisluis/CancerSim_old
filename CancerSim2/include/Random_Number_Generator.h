//
//  Random_Number_Generator.h
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

#ifndef Random_Number_Generator_h
#define Random_Number_Generator_h

#include <vector>
#include <sys/types.h>

class Random_Number_Generator
{
public:
    
    Random_Number_Generator(int seed=1, int state_size=128);
    
    // state_size may be 8, 32, 64, 128, or 256.
    // Bigger=better (more randomish)
    void srand(unsigned int seed, int state_size=128);
    
    int rand();
    
    int operator()() { return rand(); };
    
    Random_Number_Generator & operator=(const Random_Number_Generator &);
    
protected:
    
    struct Random_Data
    {
        int32_t *fptr;        /* Front pointer.  */
        int32_t *rptr;        /* Rear pointer.  */
        int32_t *state;        /* Array of state values.  */
        int rand_type;        /* Type of random number generator.  */
        int rand_deg;        /* Degree of random number generator.  */
        int rand_sep;        /* Distance between front and rear.  */
        int32_t *end_ptr;        /* Pointer behind state table.  */
    };
    
    static int initstate_r (unsigned int seed, char * arg_state, size_t n, Random_Data * buf);
    static int random_r(Random_Data * buf, int32_t * result);
    static int srandom_r (unsigned int seed, Random_Data * buf);
    
    std::vector<char> m_state;
    Random_Data m_rd;
    
};

#endif /* Random_Number_Generator_h */
