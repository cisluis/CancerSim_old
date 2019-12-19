//
//  Tissue_Parameters.cpp
//  CancerSim2
//
//  Created by Luis Cisneros on 11/5/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Tissue_Parameters.cc
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


#include "Tissue_Parameters.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <ctype.h>
#include "Hash.h"
#include <assert.h>

//static const int unspecified_max_num_cells = -1;

//  Default values for Tissue Parameters (for default constructor)
Tissue_Parameters::Tissue_Parameters() :
m_num_cell_locations(125000),
m_max_time(50000),
m_telomere_length(40), // 2^(n-1) max cells will be created,
m_random_seed_seed(1974),
m_random_apoptosis(400),
m_evade_apoptosis(20),
m_mutation_rate(10000),
m_genetic_instability(100),
m_ignore_growth_inhibit(4) {}

// The seed is hashed with other parameters a specified number of times
int Tissue_Parameters::random_seed(int hash) const
{
    assert(hash >= 0);
    
    if(hash == 0) return m_random_seed_seed;
    
    Hash h;
    
    while(hash--)
    {
        h.add(m_num_cell_locations);
        h.add(m_max_time);
        h.add(m_mutation_rate);
        h.add(m_genetic_instability);
        h.add(m_ignore_growth_inhibit);
        h.add(m_telomere_length);
        h.add(m_random_seed_seed);
        h.add(m_random_apoptosis);
        h.add(m_evade_apoptosis);
    }
    
    return h.result();
}


// Parse parameter values from command line arguments
void Tissue_Parameters::parse_args(int argc, char * argv[])
{
    for(int i=1; i < argc; ++i)
    {
        if(!strncmp(argv[i], "-n", 2))
        {
            m_num_cell_locations = atoi(argv[i]+2);
            if(m_num_cell_locations < 27)
            {
                std::cerr << "Invalid number of cells \"" << (argv[i]+2) << "\" specified on command line; must be >= 27" << std::endl;
                exit(-1);
            }
        }
        else if(!strncmp(argv[i], "-i", 2))
        {
            m_genetic_instability = atoi(argv[i]+2);
            if(m_genetic_instability < 1)
            {
                std::cerr << "Invalid genetic instability \"" << (argv[i]+2) << "\" specified on command line; must be >= 1" << std::endl;
                exit(-1);
            }
        }
        else if(!strncmp(argv[i], "-g", 2))
        {
            m_ignore_growth_inhibit = atoi(argv[i]+2);
            if(m_ignore_growth_inhibit < 1)
            {
                std::cerr << "Ignore growth inhibit \"" << (argv[i]+2) << "\" specified on command line; must be >= 1" << std::endl;
                exit(-1);
            }
        }
        else if(!strncmp(argv[i], "-t", 2))
        {
            m_telomere_length = atoi(argv[i]+2);
            if(m_telomere_length != atoi(argv[i]+2) || m_telomere_length == 0)
            {
                std::cerr << "Invalid telomere length \"" << (argv[i]+2) << "\" specified on command line; must be > 0 and < " << (1<<8) << std::endl;
                exit(-1);
            }
        }
        else if(!strncmp(argv[i], "-m", 2) && isdigit(argv[i][2]))
        {
            m_mutation_rate = atoi(argv[i]+2);
            if(m_mutation_rate < 0)
            {
                std::cerr << "Invalid mutation rate \"" << (argv[i]+2) << "\" specified on command line; must be >= 0" << std::endl;
                exit(-1);
            }
        }
        else if(!strncmp(argv[i], "-e", 2))
        {
            m_evade_apoptosis = atoi(argv[i]+2);
            if(m_evade_apoptosis < 1)
            {
                std::cerr << "Invalid evade apoptosis setting \"" << (argv[i]+2) << "\" specified on command line; must be >= 1" << std::endl;
                exit(-1);
            }
        }
        else if(!strncmp(argv[i], "-a", 2))
        {
            m_random_apoptosis = atoi(argv[i]+2);
            if(m_random_apoptosis < 2)
            {
                std::cerr << "Invalid random apoptosis \"" << (argv[i]+2) << "\" specified on command line; must be > 1" << std::endl;
                exit(-1);
            }
        }
        else if(!strncmp(argv[i], "-q", 2))
        {
            m_max_time = atoi(argv[i]+2);
            if(m_max_time < 0)
            {
                std::cerr << "Invalid max time \"" << (argv[i]+2) << "\" specified on command line; must be >= 0" << std::endl;
                exit(-1);
            }
        }
        else if(!strncmp(argv[i], "-r", 2))
        {
            m_random_seed_seed = atoi(argv[i]+2);
        }
        else
        {
            std::cerr << "Not using command line arg " << argv[i] << std::endl;
            exit(-1);
        }
    }
}

bool Tissue_Parameters::operator==(const Tissue_Parameters & params) const
{
    return
    m_num_cell_locations == params.m_num_cell_locations &&
    m_max_time == params.m_max_time &&
    m_mutation_rate == params.m_mutation_rate &&
    m_genetic_instability == params.m_genetic_instability &&
    m_ignore_growth_inhibit == params.m_ignore_growth_inhibit &&
    m_telomere_length == params.m_telomere_length &&
    m_random_seed_seed == params.m_random_seed_seed &&
    m_random_apoptosis == params.m_random_apoptosis &&
    m_evade_apoptosis == params.m_evade_apoptosis;
}


std::ostream & operator<<(std::ostream & os, const Tissue_Parameters & p)
{
    os << "(*) Num Cell Locations: " << p.m_num_cell_locations << std::endl
    << "(*) Genetic Instability: " << p.m_genetic_instability << std::endl
    << "(*) Ignore Growth Inhibit: " << p.m_ignore_growth_inhibit << std::endl
    << "(*) Telomere Length: " << p.m_telomere_length << std::endl
    << "(*) Mutation Rate: " << p.m_mutation_rate << std::endl
    << "(*) Evade Apoptosis: " << p.m_evade_apoptosis << std::endl
    << "(*) Random Apoptosis: " << p.m_random_apoptosis << std::endl
    << "(*) Max Time: " << p.m_max_time << std::endl
    << "(*) Random Seed: " << p.m_random_seed_seed << std::endl;
    
    return os;
}
