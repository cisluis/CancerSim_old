//
//  CLI.cpp
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

//************************************************************
/* Comand Line Arguments usage:
 
 * -n : Number of cell locations in the Tissue (>=27)
 * -i : Genetic instability parameter (>=1)
 * -g : Ignore growth inhibition parameter (>=1)
 * -t : Telomere length (>0)
 * -m : Mutation rate (>=0)
 * -e : Evade apoptosis parameter (>=1)
 * -a : Random apoptosis parameter (>1)
 * -q : Maximum simulation time (>=0)
 * -r : Random seed
 */

#include <iostream>
#include "Tissue.h"

int main(int argc, char * argv[])
{
    Tissue t;
    t.create(argc, argv);
    
    // Prints out initial status
    std::cout << t << std::endl;
    
    // Runs Simulation
    while(t.run_to_time(t.time()+1))
    {
        if(t.time() % 100 == 0) std::cout << t << std::endl;
    }
    
    // Prints out final status
    std::cout << t << std::endl << "All Done!!!" << std::endl;
    
    return 0;
}


