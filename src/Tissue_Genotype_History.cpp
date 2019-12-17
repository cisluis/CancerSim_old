//
//  Tissue_Genotype_History.cpp
//  CancerSim2
//
//  Created by Luis Cisneros on 11/8/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Tissue_Genotype_History.cc
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


#include "Tissue_Genotype_History.h"

Tissue_Genotype_History::Tissue_Genotype_History(int max_samples)
{
    create(max_samples);
}

void Tissue_Genotype_History::create(int max_samples)
{
    discard();
    m_time.set_limit(max_samples);
    m_data.resize(Tissue::num_genotypes());
    for(Histories::iterator i = m_data.begin(); i != m_data.end(); ++i)
        i->set_limit(max_samples);
}

void  Tissue_Genotype_History::sample(const Tissue & t)
{
    m_time.append(t.time());
    
    for(int i=0; i < Tissue::num_genotypes(); ++i)
    {
        const int n = t.count_genotype(Tissue::ith_genotype(i));
        m_data[i].append(n);
        if(n > m_max)
            m_max = n;
    }
}

void Tissue_Genotype_History::discard()
{
    m_max = 0;
    m_time.discard();
    for(Histories::iterator i = m_data.begin(); i != m_data.end(); ++i)
        i->discard();
}
