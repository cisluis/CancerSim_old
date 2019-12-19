//
//  Tissue_Parameters.h
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

#ifndef Tissue_Parameters_h
#define Tissue_Parameters_h

/*
 
 This struct holds tweakable settings for the 'Tissue' class.
 
 */

struct Tissue_Parameters
{
public:
    
    //************************************************************
    // Public properties
    
    // number of cell locations in the simulation
    int m_num_cell_locations;
    
    // Maximun simulation time
    int m_max_time;
    
    // the telomere length of the initial cell.
    int m_telomere_length;
    
    // seed for the random number generator.
    int m_random_seed_seed;
    
    // 1/n chance of random death each time through the cell cycle.
    int m_random_apoptosis;
    
    // Cells with n mutations have a n/mutation_apoptosis chance
    // of being detected and killed each cell cycle.
    // Cells with the EVADE_APOPTOSIS mutation, however, avoid detection altogether.
    int m_evade_apoptosis;
    
    // base mutation rate
    int m_mutation_rate;
    
    // genetically unstable cells have n times higher mutation rate.
    int m_genetic_instability;
    
    // cells with IGNORE_GROWTH_INHIBIT mutation have 1/n chance of success of killing off neighbors.
    int m_ignore_growth_inhibit;
    
    //************************************************************
    // Public member functions
    
    // Default Constructor; Assigns default values for parameters
    Tissue_Parameters();
    
    // Return a random seed based on m_random_seed_seed
    int random_seed(int hash) const;

    // changes the parameters as specified in the argv-style array.
    void parse_args(int argc, char * argv[]);
    
    // Overloads == operator
    bool operator==(const Tissue_Parameters &) const;
    
    // Overloads != operator
    bool operator!=(const Tissue_Parameters & p) const { return !(*this == p); };
    
};

// Overloads print operator
#include <iosfwd>
std::ostream & operator<<(std::ostream & os, const Tissue_Parameters & p);

#endif /* Tissue_Parameters_h */
