//
//  Tissue.h
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


//  Event-based:

//  A cell is created.  This schedules the cell's first mitosis
//  in a short period of time relative to cell lifetime

//  Events:

//  1- Mitosis:
//      a) If the cell is not 'hungry' and there's a free space,
//         a cell is created and its first mitosis event is scheduled.
//      b) If the new cell is 'hungry', then if there are 'ready' vascular
//         cells within some radius, the nearest is selected and immediately
//         divides.  Otherwise the new cell is marked 'hungry.'

//  2- Vascular Mitosis:
//      a) When a vascular cell becomes ready, the strength of 'hunger'
//         signals reaching the cell are computed.
//      b) If sufficient, the cell divides to whichever neighboring cell
//         has strongest hunger signal.
//      c) Then the nutrition to each hunger cell is recomputed and they
//         are marked non-hungry as appropriate.
//      d) The amount of nutrition supplied to every location is cached.


#ifndef Tissue_h
#define Tissue_h

#include "Gradient_3d.h"
#include <queue>
#include "Bit_Array_3d.h"
#include "Random_Number_Generator.h"
#include "Unlikely_Event.h"
#include "Small_Sequence.h"
#include "Tissue_Parameters.h"
#include <map>

class Tissue
{
public:
    
    //************************************************************
    // Class public declarations
    
    // Telomere length
    typedef unsigned char Telomere_Length;
    
    // A large number of times are created, they should be small as possible
    typedef unsigned short Time;
    
    // Genotype string
    typedef char Genotype;
    
    // Mutations - A value for each mutation a cell may have.
    enum Mutation {
        GENETIC_INSTABILITY = 1,
        IGNORE_GROWTH_INHIBIT,
        EVADE_APOPTOSIS,
        LIMITLESS_REPLICATION,
        SUSTAINED_ANGIOGENESIS,
        SELF_GROWTH_SIGNAL,
        DEAD = -1 };
    
    
    //************************************************************
    // Class public properties
    
    
    
    //************************************************************
    // Class public member functions
    
    // Default Constructor
    Tissue();
    
    // Destructor
    ~Tissue() { delete m_nutrition; };
    
    // Must be the first call to the object.
    void create(int argc, char * argv[]);
    
    // Grid dimensions (All the arrays have the same size, so the choice
    // of delegating to m_genotype is arbitrary)
    int ni() const { return m_genotype.ni(); };
    int nj() const { return m_genotype.nj(); };
    int nk() const { return m_genotype.nk(); };
    
    // Retrieve the current simulation time, which is
    // the time of the last event handled, or 0 initially
    Time time() const { return m_time; };
    
    // Processes all events ocurring before the specified time.
    //  - If true is returned the simulation goes on. Time is t.
    //  - Returns 'false' if the simulation is now done (no events left or
    //    there are too many cells)
    bool run_to_time(Time t);
    
    //  - Returns true if and only if the current time is less than
    // or equal to 'time' and either there are no events left or
    // the next event doesn't occur until after 'time'; in other words,
    // only if the current state represents the final state at
    // the end of the time inteval 'time'.
    // bool is_time(Time t) const {
    //    return time() <= t && (m_events.empty() || m_events.top().m_time > t); };
    
    //  - Save all fields and return true if is_time(time) returns true;
    //  - Return false otherwise
    // bool save(const Time time);
    
    // Return the number of living cells.
    int num_cells() const { return m_num_cells_living; };
    
    // Return the total number of cells that have been created
    // (some of which have since died)
    int num_cells_created() const { return m_num_cells_created; };
    
    const Bit_Array_3d & capillaries() const { return m_capillary; };
    
    // creates a functionally equivalent copy of this object.
    // do not use the copy constructor of this class.
    void clone(Tissue * t) const;
    bool operator==(const Tissue & t) const;
    
    // enumerate mutations
    static int num_mutation_types() { return sizeof(mutation_names)/sizeof(mutation_names[0]); };
    static Mutation ith_mutation_type(int i) { assert(i>=0&&i<num_mutation_types()); return (Mutation)(i+1); };
    
    // get the name of a mutation
    static const char * mutation_name(Mutation m) { return mutation_names[index_of_mutation_type(m)]; };
    
    // determine whether cells have particular mutations
    bool has_mutation(Genotype g, Mutation m) const { return g & mutation_mask(m); };
    bool has_mutation(int i, int j, int k, Mutation m) const { return has_mutation(m_genotype(i,j,k), m); };
    
    static int mutation_mask(Mutation m) { assert(is_mutation_type(m)); return (1<<(m-1)); };
    
    // Genotype - A combination of 0 or more mutations
    //            enumerate Genotypes
    static int num_genotypes() { return 1<<num_mutation_types(); };
    static Genotype ith_genotype(int i) { assert(i>=0&&i<num_genotypes()); return (Genotype)i; };
    static int index_of_genotype(Genotype g) { return g; };
    
    //  Genotype genotype(int i, int j, int k) const { return m_genotype(i,j,k); };
    // report the mutations of the specified cell.  Place the report in the ostream.
    void report_genotype(std::ostream & os, int i, int j, int k) const;
    const Array_3d<Genotype> & genotype() const { return m_genotype; };
    
    int count_genotype(Genotype g) const { return m_count_genotype[index_of_genotype(g)]; };
    
    // Write a description of the run into the specified ostream.
    // The output is merely descriptive; it doesn't give enough information to restart
    // the simulation from the current point.
    // The output contains newlines, but no empty lines.
    void status(std::ostream & os) const;
    
    const Tissue_Parameters & params() const { return m_params; };
    void set_params(const Tissue_Parameters & params);
    
    
protected:
    
    //************************************************************
    // Class private declarations
    
    // The events store many indices. It helps to use less memory.
    // 256 ^ 3 = 16777216
    typedef unsigned char Index;
    
    // Events scheduled for cells are not removed from the event
    // queue even if the cell dies prematurely (i.e. if some other cell
    // ignores contact inhibition and its daughter cell kills
    // off the cell). For this reason a sequence number is maintained
    // in combination with an index to uniquely identify a cell.
    // This number may roll over during a run without causing problems
    // so long as all events for the cell previously at the location
    // have been processed by the time the sequence number is reused.
    // The number is incremented only at apoptosis.
    typedef unsigned short Sequence_Number;
    
    // a Cell index uses a sequence number in combination with a Index_3d
    // to indentify a particual cell (not just a particular grid location)
    struct Cell_Index
    {
        Cell_Index() { };
        Cell_Index(Index i, Index j, Index k, Sequence_Number n) : m_i(i), m_j(j), m_k(k), m_n(n) { };
        
        Index m_i, m_j, m_k;
        Sequence_Number m_n;
    };
    
    // The history of mutations contributing to each genotype
    typedef Small_Sequence Genotype_History;
    
    struct Genotype_History_Count
    {
        int m_num_cells_living; // # of cells with this genotype history that are living
        int m_num_cells_created; // # of cells with this history that have been created.
        int m_num_mutations; // # of of mutations accrued in this history
    };
    typedef std::map<Genotype_History, Genotype_History_Count> Genotype_Histories;
    
    // Rather than having an Event base class and a number of specializations,
    // the enum/switch approach is taken.  There may be a large number of
    // event objects so the overhead of heap management and vtable pointers is unwanted.
    class Event
    {
    public:
        enum Type { MITOSIS, APOPTOSIS, NUM_TYPES };
        Event(Cell_Index i, Type type, Time time) : m_type(type), m_cell(i), m_time(time) { };
        struct Is_After { bool operator()(const Event & a, const Event & b) const { return a.m_time > b.m_time; }; };
        
        Cell_Index m_cell;
        unsigned char m_type;
        Time m_time;
    };
    
    //************************************************************
    // Class private properties
    
    // Tissue parameters
    Tissue_Parameters m_params;
    // Simulation Time
    Time m_time;
    
    // Sequence number Lattice for cells
    Array_3d<Sequence_Number> m_sequence_number;
    // Genotype Lattice, includes whether the cell is alive
    Array_3d<Genotype> m_genotype;
    // Whether there is a capillary at each point in the computational grid.
    Bit_Array_3d m_capillary;
    // Nutrition diffuses from capillaries
    Gradient_3d<float> * m_nutrition;
    // Adds nutrition sources on capillaries
    Gradient_3d<float> * nutrition();
    // hungry-cell flags
    std::vector<Cell_Index> m_hungry;
    
    // The length of the telomeres for each cell is decremented by 1
    // in both daughter cells upon mitosis.
    // When the telomere length reaches 0, the cell is scheduled to die.
    Array_3d<Telomere_Length> m_telomeres;
    
    // Random number
    Random_Number_Generator m_rand;
    
    // returns true with probability 1 / mutation_rate
    Unlikely_Event m_base_mutation_event;
    // returns true with probability 1 / (mutation_rate / genetic_instability)
    Unlikely_Event m_unstable_mutation_event;
    Unlikely_Event m_random_apoptosis_event;
    Unlikely_Event m_ignore_growth_inhibit_event;
    
    // queue of events using a priority_queue container adaptor
    std::priority_queue<Event,std::vector<Event>,Event::Is_After> m_events;
    
    // Statistics:
    // genotype counter
    std::vector<int> m_count_genotype;
    
    // number of grid locations occupied by capillaries.
    int m_num_capillaries;
    
    // number of living cells
    int m_num_cells_living;
    
    // total number of cells created
    int m_num_cells_created;
    
    // total number of events processed so far
    unsigned int m_num_events_processed;
    
    // Genotype history log
    Genotype_Histories m_genotype_histories;
    Array_3d<Genotype_Histories::iterator> m_cell_genotype_histories;
    
    
    //************************************************************
    // Class Private Member Functions
    
    // Event-related things:
    // handle the next event in the simulation.  (This does not necessarily advance
    // the simulation time, since many events can happen 'simultaneously')
    bool next_event();
    
    static void report_genotype(std::ostream & os, const Genotype_History &);
    
    // types of mutations
    static bool is_mutation_type(Mutation m) { return m > 0 && m <= num_mutation_types(); };
    static int index_of_mutation_type(Mutation m) { assert(is_mutation_type(m)); return m-1; };
    static const char * const mutation_names[6];
    
    // Determine whether i,j,k is a valid index to any of our 3d arrays
    // (All the arrays have the same size, so the choice of delegating to m_genotype is arbitrary)
    bool is_index(int i, int j, int k) const { return m_genotype.is_index(i,j,k); };
    
    bool alive(int i, int j, int k) const { return m_genotype(i,j,k) >= 0; };
    bool dead(int i, int j, int k) const { return !alive(i,j,k); };
    
    // add a cell a mutation to a cell's genotype
    void add_mutation(int i, int j, int k, Mutation m);
    
    // copy the genotype of cell (i,j,k) to cell (ii,jj,kk) with some probability
    // of error, and make cell ii, jj, kk alive.
    void mitosis(int i, int j, int k, int ii, int jj, int kk);
    
    // cell i, j, k is dead / no longer a cell
    void apoptosis(int i, int j, int k);
    
    // cell (i,j,k) divides unless hunger or contact inhibition stop it from doing so.
    // if mitosis is performed, returns true and returns index of the daughter cell
    bool mitosis_maybe(int i, int j, int k, int * ii, int * jj, int * kk);
    
    // create a capillary at the specified location
    void add_capillary(int i, int j, int k);
    
    // finds the nearest capillary to i,j,k, then finds the neighbor of
    // the capillary which is nearest i,j,k, which becomes a capillary
    void add_capillary_near(int i, int j, int k);
    
    // assuming cell (i,j,k) is ungoing mitosis currently, when will its next
    // mitosis event be?
    Time time_of_next_mitosis(int i, int j, int k) { return time()+5+m_rand()%6; };
    
    // return true if m_nutrition is less than some value at the specified location.
    // A hungry cell cannot undergo mitosis.
    bool is_hungry(int i, int j, int k) const { return ((Tissue*)this)->nutrition()->get(i,j,k) < 0.05f; };
    
    int max_num_cells() const { return ni()*nj()*nk()*9/10; };
    
    bool is_dead(Cell_Index cell) const { return m_sequence_number(cell.m_i,cell.m_j,cell.m_k) != cell.m_n; };
    
    // No mitosis events are created for cells which are 'hungry', since they don't have the
    // energy to reproduce.  But when a new capillary cell is created, some cells may no longer
    // be hungry, and mitosis events for them should be scheduled.  To speed this up we
    // keep a list of hungry cells.  A cell enters the list when created (if it is hungry) and
    // exits when a capillary is created so the cell is no longer hungry
    
    void add_hungry(int i, int j, int k) { m_hungry.push_back(Cell_Index(i,j,k,m_sequence_number(i,j,k))); };
    void remove_hungry(int i) { assert(i>=0&&i<m_hungry.size()); m_hungry[i]=m_hungry[m_hungry.size()-1]; m_hungry.pop_back(); };
    
    // schedule a mitosis event for the specified cell.
    void schedule_mitosis(int i, int j, int k)
    {
        assert(alive(i,j,k));
        m_events.push(Event(Cell_Index(i,j,k,m_sequence_number(i,j,k)), Event::MITOSIS, time_of_next_mitosis(i,j,k)));
    };
    
    void schedule_apoptosis(int i, int j, int k)
    {
        //  m_events.push(Event(Cell_Index(i,j,k,m_sequence_number(i,j,k)), Event::APOPTOSIS, time_to_die(i,j,k)));
        m_events.push(Event(Cell_Index(i,j,k,m_sequence_number(i,j,k)), Event::APOPTOSIS, time()+1));
    };
    
    
    
};

inline std::ostream & operator<<(std::ostream & os, const Tissue & t)
{
    t.status(os);
    return os;
}


#endif /* Tissue_h */
