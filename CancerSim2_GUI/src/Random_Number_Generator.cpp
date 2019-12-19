//
//  Random_Number_Generator.cpp
//  CancerSim2_GUI
//
//  Created by Luis Cisneros on 11/8/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * This is derived from the Berkeley source:
 *    @(#)random.c    5.5 (Berkeley) 7/6/88
 * It was reworked for the GNU C Library by Roland McGrath.
 */

/* An improved random number generation package.  In addition to the standard
 rand()/srand() like interface, this package also has a special state info
 interface.  The initstate() routine is called with a seed, an array of
 bytes, and a count of how many bytes are being passed in; this array is
 then initialized to contain information for random number generation with
 that much state information.  Good sizes for the amount of state
 information are 32, 64, 128, and 256 bytes.  The state can be switched by
 calling the setstate() function with the same array as was initiallized
 with initstate().  By default, the package runs with 128 bytes of state
 information and generates far better random numbers than a linear
 congruential generator.  If the amount of state information is less than
 32 bytes, a simple linear congruential R.N.G. is used.  Internally, the
 state information is treated as an array of longs; the zeroeth element of
 the array is the type of R.N.G. being used (small integer); the remainder
 of the array is the state information for the R.N.G.  Thus, 32 bytes of
 state information will give 7 longs worth of state information, which will
 allow a degree seven polynomial.  (Note: The zeroeth word of state
 information also has some other information stored in it; see setstate
 for details).  The random number generation technique is a linear feedback
 shift register approach, employing trinomials (since there are fewer terms
 to sum up that way).  In this approach, the least significant bit of all
 the numbers in the state table will act as a linear feedback shift register,
 and will have period 2^deg - 1 (where deg is the degree of the polynomial
 being used, assuming that the polynomial is irreducible and primitive).
 The higher order bits will have longer periods, since their values are
 also influenced by pseudo-random carries out of the lower bits.  The
 total period of the generator is approximately deg*(2**deg - 1); thus
 doubling the amount of state information has a vast influence on the
 period of the generator.  Note: The deg*(2**deg - 1) is an approximation
 only good for large deg, when the period of the shift register is the
 dominant factor.  With deg equal to seven, the period is actually much
 longer than the 7*(2**7 - 1) predicted by this formula.  */



/* For each of the currently supported random number generators, we have a
 break value on the amount of state information (you need at least thi
 bytes of state info to support this random number generator), a degree for
 the polynomial (actually a trinomial) that the R.N.G. is based on, and
 separation between the two lower order coefficients of the trinomial.  */


#include "Random_Number_Generator.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

Random_Number_Generator::Random_Number_Generator(int seed, int state_size)
{
    m_rd.state=0;
    srand(seed, state_size);
}

/* Linear congruential.  */
#define    TYPE_0        0
#define    BREAK_0        8
#define    DEG_0        0
#define    SEP_0        0

/* x**7 + x**3 + 1.  */
#define    TYPE_1        1
#define    BREAK_1        32
#define    DEG_1        7
#define    SEP_1        3

/* x**15 + x + 1.  */
#define    TYPE_2        2
#define    BREAK_2        64
#define    DEG_2        15
#define    SEP_2        1

/* x**31 + x**3 + 1.  */
#define    TYPE_3        3
#define    BREAK_3        128
#define    DEG_3        31
#define    SEP_3        3

/* x**63 + x + 1.  */
#define    TYPE_4        4
#define    BREAK_4        256
#define    DEG_4        63
#define    SEP_4        1



#define    MAX_TYPES    5    /* Max number of types above.  */

struct Random_Poly_Info
{
    int seps[MAX_TYPES];
    int degrees[MAX_TYPES];
};

static const Random_Poly_Info random_poly_info =
{
    { SEP_0, SEP_1, SEP_2, SEP_3, SEP_4 },
    { DEG_0, DEG_1, DEG_2, DEG_3, DEG_4 }
};



int Random_Number_Generator::initstate_r (unsigned int seed, char * arg_state, size_t n, Random_Data * buf)
{
    int type;
    int degree;
    int separation;
    int32_t *state;
    
    if (buf == NULL)
        goto fail;
    
    if (n >= BREAK_3)
        type = n < BREAK_4 ? TYPE_3 : TYPE_4;
    else if (n < BREAK_1)
    {
        if (n < BREAK_0)
        {
            //      __set_errno (EINVAL);
            goto fail;
        }
        type = TYPE_0;
    }
    else
        type = n < BREAK_2 ? TYPE_1 : TYPE_2;
    
    degree = random_poly_info.degrees[type];
    separation = random_poly_info.seps[type];
    
    buf->rand_type = type;
    buf->rand_sep = separation;
    buf->rand_deg = degree;
    state = &((int32_t *) arg_state)[1];    /* First location.  */
    /* Must set END_PTR before srandom.  */
    buf->end_ptr = &state[degree];
    
    buf->state = state;
    
    srandom_r (seed, buf);
    
    state[-1] = TYPE_0;
    if (type != TYPE_0)
        state[-1] = static_cast<int>((buf->rptr - state) * MAX_TYPES + type);
    
    return 0;
    
fail:
    //  __set_errno (EINVAL);
    return -1;
}

int Random_Number_Generator::random_r(Random_Data * buf, int32_t * result)
{
    int32_t *state;
    
    if (buf == NULL || result == NULL)
        goto fail;
    
    state = buf->state;
    
    if (buf->rand_type == TYPE_0)
    {
        int32_t val = state[0];
        val = ((state[0] * 1103515245) + 12345) & 0x7fffffff;
        state[0] = val;
        *result = val;
    }
    else
    {
        int32_t *fptr = buf->fptr;
        int32_t *rptr = buf->rptr;
        int32_t *end_ptr = buf->end_ptr;
        int32_t val;
        
        val = *fptr += *rptr;
        /* Chucking least random bit.  */
        *result = (val >> 1) & 0x7fffffff;
        ++fptr;
        if (fptr >= end_ptr)
        {
            fptr = state;
            ++rptr;
        }
        else
        {
            ++rptr;
            if (rptr >= end_ptr)
                rptr = state;
        }
        buf->fptr = fptr;
        buf->rptr = rptr;
    }
    return 0;
    
fail:
    //  __set_errno (EINVAL);
    return -1;
}

int  Random_Number_Generator::srandom_r (unsigned int seed, Random_Data * buf)
{
    int type;
    int32_t *state;
    long int i;
    long int word;
    int32_t *dst;
    int kc;
    
    if (buf == NULL)
        goto fail;
    type = buf->rand_type;
    if ((unsigned int) type >= MAX_TYPES)
        goto fail;
    
    state = buf->state;
    /* We must make sure the seed is not 0.  Take arbitrarily 1 in this case.  */
    if (seed == 0)
        seed = 1;
    state[0] = seed;
    if (type == TYPE_0)
        goto done;
    
    dst = state;
    word = seed;
    kc = buf->rand_deg;
    for (i = 1; i < kc; ++i)
    {
        /* This does:
         state[i] = (16807 * state[i - 1]) % 2147483647;
         but avoids overflowing 31 bits.  */
        long int hi = word / 127773;
        long int lo = word % 127773;
        word = 16807 * lo - 2836 * hi;
        if (word < 0)
            word += 2147483647;
        *++dst = static_cast<int>(word);
    }
    
    buf->fptr = &state[buf->rand_sep];
    buf->rptr = &state[0];
    kc *= 10;
    while (--kc >= 0)
    {
        int32_t discard;
        (void) random_r (buf, &discard);
    }
    
done:
    return 0;
    
fail:
    return -1;
}

void Random_Number_Generator::srand(unsigned int seed, int state_size)
{
    assert(state_size == 8 || state_size == 32 || state_size == 64 || state_size == 128 || state_size == 256);
    m_state.resize(state_size);
    
    int error;
    
    error = initstate_r(seed, &m_state[0], state_size, &m_rd);
    assert(!error);
}

int Random_Number_Generator::rand()
{
    int result;
    int error;
    
    error = random_r(&m_rd, &result);
    assert(!error);
    
    return result;
}



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



Random_Number_Generator & Random_Number_Generator::operator=(const Random_Number_Generator & rhs)
{
    m_state = rhs.m_state;
    m_rd = rhs.m_rd;
    
    int32_t * begin = (int32_t*) &m_state[0];
    const int32_t * rhs_begin = (int32_t*) &rhs.m_state[0];
    
    m_rd.fptr = begin + (rhs.m_rd.fptr-rhs_begin);
    m_rd.rptr = begin + (rhs.m_rd.rptr-rhs_begin);
    m_rd.state = begin + (rhs.m_rd.state-rhs_begin);
    m_rd.end_ptr = begin + (rhs.m_rd.end_ptr-rhs_begin);
    
    return *this;
}

#ifdef RANDOM_NUMBER_GENERATOR_TEST

int main(int argc, char *argv[])
{
    Random_Number_Generator r1(1974), r2(1), r3(2), r4(1974);
    
    
    for(int i=0; i < 5; ++i)
        std::cout << r1() << ' ' << r4() << std::endl;
    std::cout << std::endl;
    
    for(int i=0; i < 100; ++i)
        std::cout << r1() << std::endl;
    
    r3 = r1;
    
    std::cout << r1() << ' ' << r1() << std::endl << r3() << ' ' << r3() << std::endl;
    
    Random_Number_Generator r5(time(0));
    int num = 1;
    int denom = 2;
    int nn = 100000;
    int n0 = 0;
    for(int i=denom*nn; i--; )
        if( (int)(((double)denom*r5() / (RAND_MAX+1.0)) < num) )
            ++n0;
    
    std::cout << n0 << " should be roughly " << num*nn << std::endl;
    
    return 0;
}

#endif
