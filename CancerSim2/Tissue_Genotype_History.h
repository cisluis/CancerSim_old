//
//  Tissue_Genotype_History.h
//  CancerSim2
//
//  Created by Luis Cisneros on 11/8/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Tissue_Genotype_History.h
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


#ifndef TISSUE_GENOTYPE_HISTORY_HFILE
#define TISSUE_GENOTYPE_HISTORY_HFILE

//  The tissue genotype history maintains a record
//  of how many cells of each genotype are in the tissue.

//  It uses the limited history, meaning it will never
//  accumulate more than some fixed number of samples.

#include <vector>
#include "Limited_History.h"
#include "Tissue.h"

class Tissue_Genotype_History
{
public:
    
    Tissue_Genotype_History(int max_samples=1000);
    
    void create(int max_samples=1000);
    
    // record the time and number of cells of each genotype
    void sample(const Tissue &);
    
    // return the number of samples in the history.  This may
    // be less than the number of time sample() has called,
    // because a limited history is used.
    int num_samples() const { return m_time.size(); };
    
    // return all the stored samples for a particular genotype
    const int * samples(Tissue::Genotype g) const { return m_data[Tissue::index_of_genotype(g)].data(); };
    
    // return the ith sample for the given genotype.
    // use ith_time to see when this sample was taken.
    int ith_sample(Tissue::Genotype g, int i) const { return m_data[Tissue::index_of_genotype(g)][i]; };
    
    // return the times of all the samples
    const Tissue::Time * times() const { return m_time.data(); };
    
    // return time time of the ith sample
    Tissue::Time ith_time(int i) const { return m_time[i]; };
    
    // the maximum number of cells of any genotype in any sample.
    int max() const { assert(num_samples()); return m_max; };
    int max(Tissue::Genotype g) const { assert(num_samples()); return m_data[Tissue::index_of_genotype(g)].max(); };
    
    // re-initialize the history, throwing out all values previous appened.
    void discard();
    
protected:
    
    typedef std::vector<Limited_History<int> > Histories;
    Histories m_data;
    Limited_History<Tissue::Time> m_time;
    int m_max;
    
};

#endif
