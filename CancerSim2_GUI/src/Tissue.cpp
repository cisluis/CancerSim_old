//
//  Tissue.cpp
//  CancerSim2_GUI
//
//  Created by Luis Cisneros on 11/5/19.
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

#include "Tissue.h"
#include <fstream>
#include <limits.h>
#include <iostream>

const char * const Tissue::mutation_names[] = {
    "Genetic Instability",
    "Ignore Growth Inhibit",
    "Evade Apoptosis",
    "Limitless Replication",
    "Cause Angiogenesis",
    "Self Growth" };

Tissue::Tissue() :
m_nutrition(0)
{
}

// Initial creation of tissue environment
void Tissue::create(int argc, char * argv[])
{
    // Parse command line argments
    m_params.parse_args(argc, argv);
    
    // side size of 3D lattice
    int ni, nj, nk;
    ni=nj=nk= (int) pow(m_params.m_num_cell_locations + 1e-5, 1.0/3);
    
    // initialize nutrition variable
    delete m_nutrition;
    m_nutrition=0;
    
    // initialize capillary lattice
    m_capillary.resize(ni,nj,nk);
    m_capillary.reset();
    m_num_capillaries = 0;
    
    // initialize sequence numbers
    m_sequence_number.resize(ni,nj,nk);
    m_sequence_number.set_all(0);
    
    // clear out event queue if is not empty
    while(!m_events.empty())
        m_events.pop();
    m_num_events_processed = 0;
    
    // initialize sim time
    m_time = 0;
    
    // clears out all cells
    m_genotype.resize(ni,nj,nk);
    m_genotype.set_all(DEAD);
    m_num_cells_living = 0;
    m_num_cells_created = 0;
    
    // clears out all histories
    m_genotype_histories.clear();
    m_cell_genotype_histories.resize(ni,nj,nk);
    m_cell_genotype_histories.set_all(m_genotype_histories.end());
    
    // initialize telomeres
    m_telomeres.resize(ni,nj,nk);
    
    // initialize genotypes
    m_count_genotype.resize(num_genotypes());
    std::fill(m_count_genotype.begin(), m_count_genotype.end(), 0);
    
    // initialize hungry flags
    m_hungry.resize(0);
    
    // set initial location (center)
    const int i = ni/2;
    const int j = nj/2;
    const int k = nk/2;
    
    // generate random numbers
    int randomize=0;
    m_rand.srand(m_params.random_seed(++randomize));
    
    // mutation event
    m_base_mutation_event.init(m_params.m_mutation_rate, m_params.random_seed(++randomize));
    // unstable mutation event
    m_unstable_mutation_event.init(m_params.m_mutation_rate / m_params.m_genetic_instability, m_params.random_seed(++randomize));
    // apoptotic event
    m_random_apoptosis_event.init(m_params.m_random_apoptosis, m_params.random_seed(++randomize));
    // growth inhibition event
    m_ignore_growth_inhibit_event.init(m_params.m_ignore_growth_inhibit, m_params.random_seed(++randomize));
    
    // the first cell is created via mitosis (with itself)
    mitosis(i,j,k, i,j,k);
    // capilaries are grown
    add_capillary(i,j,k);
}

Gradient_3d<float> * Tissue::nutrition()
{
    if(!m_nutrition)
    {
        m_nutrition = new Gradient_3d<float>(ni(), nj(), nk());
        for(int i=0; i < ni(); ++i)
            for(int j=0; j < nj(); ++j)
                for(int k=0; k < nk(); ++k)
                    if(m_capillary(i,j,k))
                        m_nutrition->add_source(i,j,k, 1.0f);
    }
    
    return m_nutrition;
}

void Tissue::report_genotype(std::ostream & os, const Genotype_History & h)
{
    const int n = h.size();
    if(n)
    {
        for(int i=0; i < n; ++i)
        {
            assert(h[i] > 0 && h[i] <= Tissue::num_mutation_types());
            os << (i?", ":"") << Tissue::mutation_name((Tissue::Mutation)(h[i]));
        }
    }
    else
        os << "(No Mutations)";
}

void Tissue::report_genotype(std::ostream & os, int i, int j, int k) const
{
    report_genotype(os, m_cell_genotype_histories(i,j,k)->first);
}

void Tissue::clone(Tissue * t) const
{
    *t = *this;
    t->m_nutrition = 0;
    
    // make all the iterators in t->m_cell_genotype_histories reference t->m_genotype_histories
    // instead of m_genotype_histories
    
    for(int i=0; i < ni(); ++i)
        for(int j=0; j < nj(); ++j)
            for(int k=0; k < nk(); ++k)
                t->m_cell_genotype_histories(i,j,k) = t->m_genotype_histories.find(t->m_cell_genotype_histories(i,j,k)->first);
    
    //    for(int i=0; i < 100000000; ++i)
    //    {
    //      const int a = ((Tissue*)this)->m_rand();
    //      const int b = t->m_rand();
    //      if(a != b)
    //        cout << "ERROR " << &m_rand << ' ' << &t->m_rand << " -- " << a << ' ' << b << endl;
    //    }
    std::cout << "all done" << std::endl;
}

void Tissue::add_mutation(int i, int j, int k, Mutation m)
{
    assert(!has_mutation(i,j,k,m));
    Genotype_Histories::iterator prev_history_iter = m_cell_genotype_histories(i,j,k);
    --prev_history_iter->second.m_num_cells_living;
    Genotype_History history = prev_history_iter->first;
    history.append(m);
    
    std::pair<Genotype_Histories::iterator, bool> pair =
    m_genotype_histories.insert(Genotype_Histories::value_type(history, Genotype_History_Count()));
    if(pair.second)
    {
        pair.first->second.m_num_cells_created = 1;
        pair.first->second.m_num_cells_living = 1;
        pair.first->second.m_num_mutations = history.size();
    }
    else
    {
        ++pair.first->second.m_num_cells_created;
        ++pair.first->second.m_num_cells_living;
    }
    
    m_cell_genotype_histories(i,j,k) = pair.first;
    
    m_genotype(i,j,k) |= mutation_mask(m);
};

void Tissue::mitosis(int i, int j, int k, int ii, int jj, int kk)
{
    
    if(m_num_cells_created == 0)
    {
        // creation of primordial cell
        assert(i==ii && j==jj && k==kk);
        
        m_genotype(i,j,k)=0;
        m_telomeres(i,j,k)=m_params.m_telomere_length;
        assert(m_genotype_histories.size()==0);
        
        Genotype_History_Count genotype_history_count;
        genotype_history_count.m_num_cells_living = 1;
        genotype_history_count.m_num_cells_created = 1;
        genotype_history_count.m_num_mutations = 0;
        std::pair<Genotype_Histories::iterator, bool> pair =
        m_genotype_histories.insert(Genotype_Histories::value_type(Genotype_History(), genotype_history_count));
        assert(pair.second); // no other cell can already have this history because this is the first cell.
        m_cell_genotype_histories(i,j,k) = pair.first;
    }
    else
    {
        // cell division
        assert(i!=ii || j!=jj || k!=kk);
        assert(alive(i,j,k));
        assert(dead(ii,jj,kk)); // caller should kill off ii,jj,kk first if desired.
        
        m_genotype(ii,jj,kk) = m_genotype(i,j,k);
        
        // LIMITLESS_REPLICATION is implemented by telomerase
        if(!has_mutation(i,j,k,LIMITLESS_REPLICATION))
        {
            // mitosis cannot be performed if the chromosomes are out of telomeres
            assert(m_telomeres(i,j,k));
            --m_telomeres(i,j,k);
        }
        
        m_telomeres(ii,jj,kk) = m_telomeres(i,j,k);
        
        Genotype_Histories::iterator & mother_iter = m_cell_genotype_histories(i,j,k);
        Genotype_Histories::iterator & daughter_iter = m_cell_genotype_histories(ii,jj,kk);
        assert(mother_iter != m_genotype_histories.end());
        assert(daughter_iter == m_genotype_histories.end());
        ++mother_iter->second.m_num_cells_living;
        daughter_iter = mother_iter;
    }
    
    // mutate
    
    Genotype & g = m_genotype(ii,jj,kk);
    
    if(m_params.m_mutation_rate != 0)
    {
        // copy genotype to daughter cell, but with error
        for(int it=0; it < num_mutation_types(); ++it)
        {
            const Mutation m = ith_mutation_type(it);
            
            if(has_mutation(g, m))
                continue;
            
            if(has_mutation(g, GENETIC_INSTABILITY))
            {
                if(m_unstable_mutation_event() == false)
                    continue;
            }
            else
            {
                if(m_base_mutation_event() == false)
                    continue;
            }
            
            add_mutation(ii,jj,kk,m);
        }
    }
    
    ++m_num_cells_living;
    ++m_num_cells_created;
    ++m_count_genotype[index_of_genotype(m_genotype(ii,jj,kk))];
    
    if(is_hungry(ii,jj,kk))
        add_hungry(ii,jj,kk);
}

void Tissue::apoptosis(int i, int j, int k)
{
    assert(alive(i,j,k));
    
    assert(m_num_cells_living > 0);
    --m_num_cells_living;
    
    assert(m_count_genotype[index_of_genotype(m_genotype(i,j,k))] > 0);
    --m_count_genotype[index_of_genotype(m_genotype(i,j,k))];
    
    m_genotype(i,j,k) = DEAD;
    ++m_sequence_number(i,j,k);
    assert(m_sequence_number(i,j,k)); // starts as 0 and should be big enough to avoid wraparound.
    
    Genotype_Histories::iterator & iter = m_cell_genotype_histories(i,j,k);
    assert(iter != m_genotype_histories.end());
    //const int nnnnn = iter->second.m_num_cells_living;
    assert(iter->second.m_num_cells_living > 0);
    --iter->second.m_num_cells_living;
    iter = m_genotype_histories.end();
}

// returns -1 if i < j, 0 if i==j, 1 otherwise
inline int intcmp(int i, int j)
{
    if(i < j)
        return -1;
    if(i > j)
        return 1;
    return 0;
}

void Tissue::add_capillary_near(int i, int j, int k)
{
    int imin = ni()/2;
    int jmin = nj()/2;
    int kmin = nk()/2;
    assert(m_capillary(imin,jmin,kmin));
    int smin = (imin-i)*(imin-i) + (jmin-j)*(jmin-j) + (kmin-k)*(kmin-k);
    
    int nscanned=0;
    for(int ii=0; ii<ni(); ++ii)
    {
        const int di = (ii-i)*(ii-i);
        if(di >= smin)
            continue;
        
        for(int jj=0; jj<nj(); ++jj)
        {
            const int dj = (jj-j)*(jj-j);
            if(di+dj >= smin)
                continue;
            
            for(int kk=0; kk<nk(); ++kk)
            {
                ++nscanned;
                if(!m_capillary(ii,jj,kk))
                    continue;
                const int dk = (kk-k)*(kk-k);
                if(di+dj+dk < smin)
                {
                    smin = di+dj+dk;
                    imin = ii;
                    jmin = jj;
                    kmin = kk;
                }
            }
        }
    }
    
    //  cout << "scanned " << nscanned << " of " << ni()*nj()*nk() << endl;
    
    // now imin,jmin,kmin is the nearest capillary.  Now find the neighbor
    // nearest i,j,k
    imin += intcmp(i, imin);
    jmin += intcmp(j, jmin);
    kmin += intcmp(k, kmin);
    
    assert(!m_capillary(imin,jmin,kmin));
    
    add_capillary(imin,jmin,kmin);
}

bool Tissue::mitosis_maybe(int i, int j, int k, int * pii, int * pjj, int * pkk)
{
    assert(alive(i,j,k));
    
    if(is_hungry(i,j,k))
        return false; // not enough resources for division.
    
    int dx = (i-ni()/2);
    int dz = (k-nk()/2);
    
    if(!has_mutation(i,j,k,SELF_GROWTH_SIGNAL) && j > nj()*11/20-int(sqrt(2*dx*dx+dz*dz)/5))
        return false;
    
    int ii, jj, kk; // the destination of the daughter cell
    
    // determine where the daughter cell will go
    
    // If there are emtpy slots in the surroundings, choose one.
    int ai[26];
    int aj[26];
    int ak[26];
    int an=0;
    for(ii=i-1; ii<=i+1; ++ii)
        for(jj=j-1; jj<=j+1; ++jj)
            for(kk=k-1; kk<=k+1; ++kk)
                if(is_index(ii,jj,kk) && dead(ii,jj,kk))
                {
                    ai[an]=ii;
                    aj[an]=jj;
                    ak[an]=kk;
                    ++an;
                }
    
    if(an)
    {
        // there is at least one unoccupied neighboring slot.
        const int n = m_rand()%an;
        ii = ai[n];
        jj = aj[n];
        kk = ak[n];
    }
    else
    {
        // all the neighboring slots are full, but there's still a chance if the cell ignores growth inhibit.
        
        if(!has_mutation(i,j,k,IGNORE_GROWTH_INHIBIT))
            return false; // early return.  Cell has full set of neighbors already.
        
        if(!m_ignore_growth_inhibit_event())
            return false; // early return.  Cell tries to kill off neighbor but fails.
        
        do
        {
            ii = i + m_rand()%3 - 1;
            jj = j + m_rand()%3 - 1;
            kk = k + m_rand()%3 - 1;
        }
        while(!is_index(ii,jj,kk) || ((ii==i) && (jj==j) && (kk==k)));
        
        apoptosis(ii,jj,kk);
    }
    
    assert(dead(ii,jj,kk));
    mitosis(i,j,k, ii,jj,kk);
    assert(alive(ii,jj,kk));
    schedule_mitosis(ii,jj,kk);
    
    if(is_hungry(ii,jj,kk))
    {
        // the new daughter cell doesn't have enough nutrition and
        // signals capillary growth, if possible.
        const int cx = ni()/2;
        const int cy = nj()/2;
        const int cz = nk()/2;
        const int dx = cx-i;
        const int dy = cy-j;
        const int dz = cz-k;
        if( sqrt(dx*dx*1.2 + dy*dy*0.8 + dz*dz) < sqrt(cx*cx+cy*cy+cz*cz)/3 ||
           has_mutation(ii,jj,kk,SUSTAINED_ANGIOGENESIS))
            add_capillary_near(ii,jj,kk);
    }
    
    *pii = ii;
    *pjj = jj;
    *pkk = kk;
    
    return true;
}


bool Tissue::run_to_time(Time t)
{
    // System exceeds maximum number of cells allowed
    if (num_cells() >= max_num_cells())
        return false;
    
    // Simulation Time exceeds maximum allowed value
    if (time() > m_params.m_max_time)
        return false;
    
    // No more events scheduled
    if (m_events.empty())
        return false;
    
    // run all events to time t
    int n;
    for (n=0; !m_events.empty() && m_events.top().m_time < t; ++n)
        next_event();
    m_time = t;
    
    return true;
}


bool Tissue::next_event()
{
    // early return; nothing to do.
    if(m_events.empty())
        return false;
    
    // pop top event in queue
    const Event event = m_events.top();
    m_events.pop();
    ++m_num_events_processed;
    m_time = event.m_time;
    
    if(is_dead(event.m_cell))
    {
        // The event was scheduled for a cell which is now dead.
        // This isn't an error; just a condition to take care of
        // by discarding the event.
        return true;
    } else
    {
        std::cout << "CAUGHT ON SEQUENCE NUMBER" << std::endl;
    }
    
    // get location of cell
    const int i = event.m_cell.m_i;
    const int j = event.m_cell.m_j;
    const int k = event.m_cell.m_k;
    
    /*{
     int s = m_sequence_number(i,j,k);
     Genotype g = m_genotype(i,j,k);
     assert(alive(i,j,k));
     }*/
    
    if( m_random_apoptosis_event() )
    {
        //    cout << "random apoptosis" << endl;
        apoptosis(i,j,k);
    }
    else if( !has_mutation(i,j,k,EVADE_APOPTOSIS) &&
            (m_rand() % m_params.m_evade_apoptosis) < m_cell_genotype_histories(i,j,k)->second.m_num_mutations )
    {
        //    cout << "mutation detected - cell killed" << endl;
        apoptosis(i,j,k);
    }
    else if( m_telomeres(i,j,k) == 0 )
    {
        //    cout << "telomere length == 0 - cell killed" << endl;
        apoptosis(i,j,k);
    }
    else
    {
        int ii, jj, kk;
        if(mitosis_maybe(i,j,k,&ii,&jj,&kk))
            schedule_mitosis(ii,jj,kk);
        schedule_mitosis(i,j,k);
    }
    
    return true;
}


void Tissue::add_capillary(int i, int j, int k)
{
    assert(!m_capillary(i,j,k));
    m_capillary(i,j,k) = true;
    ++m_num_capillaries;
    nutrition()->add_source(i, j, k, 1.0f);
    
    for(int i=0; i < m_hungry.size(); )
    {
        const Cell_Index cell = m_hungry[i];
        
        if(is_dead(cell))
            remove_hungry(i);
        else if(!is_hungry(cell.m_i,cell.m_j,cell.m_k))
        {
            schedule_mitosis(cell.m_i, cell.m_j, cell.m_k);
            remove_hungry(i);
        }
        else
            ++i; // the cell is still hungry; leave it alone.
    }
}


void Tissue::set_params(const Tissue_Parameters & params)
{
    if(m_params != params)
    {
        m_params = params;
        
        int randomize=0;
        m_base_mutation_event.init(m_params.m_mutation_rate, m_params.random_seed(++randomize));
        m_unstable_mutation_event.init(m_params.m_mutation_rate / m_params.m_genetic_instability, m_params.random_seed(++randomize));
        m_random_apoptosis_event.init(m_params.m_random_apoptosis, m_params.random_seed(++randomize));
        m_ignore_growth_inhibit_event.init(m_params.m_ignore_growth_inhibit, m_params.random_seed(++randomize));
    }
}


bool Tissue::operator==(const Tissue & t) const
{
    return m_genotype == t.m_genotype &&
    m_time==t.m_time &&
    m_events.size()==t.m_events.size();
}


void Tissue::status(std::ostream & os) const
{
    if(m_time == 0)
        os << "===> Settings: " << std::endl << m_params << std::endl;
    
    os << "===> Sim Status: " << std::endl;
    os << "# Sim Time: " << m_time << std::endl;
    os << "# Num Events Processed: " << m_num_events_processed << std::endl;
    os << "# Num Capillaries: " << m_num_capillaries << std::endl;
    os << "# Num Cells Created: " << m_num_cells_created << std::endl;
    os << "# Num Cells Living: " << m_num_cells_living << std::endl;
    
    for(Genotype_Histories::const_iterator i = m_genotype_histories.begin();
        i != m_genotype_histories.end(); ++i)
    {
        os << "# Num Cells Created With History " << i->first << ": \"";
        Tissue::report_genotype(os, i->first);
        os << "\" " << i->second.m_num_cells_created << std::endl;
    }
    
    for(Genotype_Histories::const_iterator i = m_genotype_histories.begin();
        i != m_genotype_histories.end(); ++i)
        if(i->second.m_num_cells_living > 0)
        {
            os << "# Num Cells Living With History " << i->first << ": \"";
            Tissue::report_genotype(os, i->first);
            os << "\" " << i->second.m_num_cells_living << std::endl;
        }
}
