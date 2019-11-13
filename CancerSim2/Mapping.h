//
//  Mapping.h
//  CancerSim2
//
//  Created by Luis Cisneros on 11/8/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Mapping.h
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


#ifndef MAPPING_HFILE
#define MAPPING_HFILE // prevent recursive includes

#include <iostream.h>

template<class _IN_, class _OUT_>
class Mapping
{
public:
// the Mapping is used to map values from one range ('in' range)
// into another ('out' range) by interpolation.

// The mapping a value is equivalent to the following:

//( value - in.min ) / ( in.max - in.min )
//         * ( out.max - out.min ) + out.min
    
// Types used
//     _IN_        type of values to be mapped.
//     _OUT_        Type of mapped values.  It may be useful, for instance,
//                to map a range of floats into a range of ints.
//                    There must be a cast operation from _IN_ to _OUT_

        // create a mapping that maps each number to itself.
    Mapping(const _IN_ & in_min = (_IN_)-1, const _IN_ & in_max = (_IN_)1,
        const _OUT_ & out_min = (_OUT_)-1, const _OUT_ & out_max = (_OUT_)1);

    _IN_ In_min() const { return m_in_min; };

    _IN_ In_max() const { return m_in_max; };

    _OUT_ Out_min() const { return m_out_min; };

    _OUT_ Out_max() const { return m_out_max; };

    bool operator==( const Mapping<_IN_,_OUT_> & param );
    // returns true iff *this == param

    inline bool Within_in_range( const _IN_ & ) const;
    // tells whether a value lies within the in range.

    void Round_off(bool x) { m_round_off = x; };
    // Call this with FASLE when mapping to integers to make
    // the mapping just drop fractions instead of rounding
    //  to the nearest integer.
    // By default it is on for integer types and is not for use
    // with non-integral types

    double Scale() const { return m_scale; };
    double Offset() const { return m_offset; };
    // retrieve the scale and offset currently used for mapping.

    void Set_in_range( const _IN_ & min, const _IN_ & max );
    
    void Set_out_range( const _OUT_ & min, const _OUT_ & max );

    void Set_ranges(const _IN_ & in_min, const _IN_ & in_max, const _OUT_ & out_min, const _OUT_ & out_max);
    
    _OUT_ operator()( const _IN_ & value ) const;  // do a mapping.
    _IN_ Unmap( const _OUT_ & out ) const; // undo a mapping.  This will not be quite accurate with integral types.

//    friend ostream & operator<<( ostream & os, const Mapping<_IN_,_OUT_> & );
//    friend istream & operator>>( istream & is, Mapping<_IN_,_OUT_> & );

protected:

    void Set_scale_and_offset();
    // initializes m_scale and m_offset according to the values of source and dest min and max.

    _IN_ m_min; // unmapped min and max values of data to be mapped.
    _IN_ m_max;

    _IN_ m_in_min; // mapping parameters.
    _IN_ m_in_max;
    _OUT_ m_out_min;
    _OUT_ m_out_max;

    double m_scale; // factors in mapping.  Determined by value of other member variables.
    double m_offset;

    bool m_round_off; // true of if rounding off numbers.

    int m_initialized; // true only if *this has been initialized.
};

template<class _IN_, class _OUT_>
Mapping<_IN_,_OUT_>::Mapping(const _IN_ & in_min, const _IN_ & in_max,
                               const _OUT_ & out_min, const _OUT_ & out_max) :
m_in_min(in_min),
m_in_max(in_max),
m_out_min(out_min),
m_out_max(out_max)
{
    Set_scale_and_offset();

    if( (_OUT_) 0.5 )            // by default, enable round off only for integral types
        m_round_off = false;
    else
        m_round_off = true;
}

template<class _IN_, class _OUT_>
bool Mapping<_IN_,_OUT_>::Within_in_range(const _IN_ & elem) const
{
    ASSERT(m_initialized);
    return ( elem >= m_in_min && elem <= m_in_max ? true : false );
}

template<class _IN_, class _OUT_>
bool Mapping<_IN_,_OUT_>::operator==( const Mapping<_IN_,_OUT_> & map )
{
    if( !m_initialized && !map.m_initialized )
        return false;

    return (
    m_min == m_min &&
    m_max == m_max &&
    m_in_min == m_in_min &&
    m_in_max == m_in_max &&
    m_out_min == m_out_min &&
    m_out_max == m_out_max &&
    m_initialized == m_initialized );

}

template<class _IN_, class _OUT_>
void Mapping<_IN_,_OUT_>::Set_in_range( const _IN_ & min, const _IN_ & max )
{
    m_in_min = min;
    m_in_max = max;
    Set_scale_and_offset();
}
    
template<class _IN_, class _OUT_>
void Mapping<_IN_,_OUT_>::Set_out_range( const _OUT_ & min, const _OUT_ & max )
{
    m_out_min = min;
    m_out_max = max;
    Set_scale_and_offset();
}

template<class _IN_, class _OUT_>
void Mapping<_IN_,_OUT_>::Set_ranges( const _IN_ & in_min, const _IN_ & in_max,
                                      const _OUT_ & out_min, const _OUT_ & out_max)
{
    m_in_min = in_min;
    m_in_max = in_max;
    m_out_min = out_min;
    m_out_max = out_max;

    Set_scale_and_offset();
}

template<class _IN_, class _OUT_>
void Mapping<_IN_,_OUT_>::Set_scale_and_offset()
{
    m_scale  = ( (double) m_out_max - m_out_min ) / ( m_in_max - m_in_min );
    m_offset = m_out_min - m_in_min * m_scale;
}

template<class _IN_, class _OUT_>
_OUT_ Mapping<_IN_,_OUT_>::operator()(const _IN_ & val) const
{
    if( !m_round_off )
        return (_OUT_)(val * m_scale + m_offset);
    else
    {
        double subresult = val * m_scale + m_offset;
        return (_OUT_)( subresult + (subresult < 0 ? -0.5 : 0.5) );
    }
}

template<class _IN_, class _OUT_>
_IN_ Mapping<_IN_,_OUT_>::Unmap( const _OUT_ & val ) const
{
    return (_IN_)( (val-m_offset) / m_scale );
}

//  template<class _IN_, class _OUT_>
//  ostream & operator<<( ostream & os, const Mapping<_IN_,_OUT_> & m )
//  {
//      os << m.m_in_max << ' '
//          << m.m_in_min << ' '
//          << m.m_initialized << ' '
//          << m.m_max << ' '
//          << m.m_min << ' '
//          << m.m_offset << ' '
//          << m.m_out_max << ' '
//          << m.m_out_min << ' '
//          << m.m_scale << ' ';

//      return os;
//  }

//  template<class _IN_, class _OUT_>
//  istream & operator>>( istream & is, Mapping<_IN_,_OUT_> & m )
//  {
//      is >> m.m_in_max
//          >> m.m_in_min
//          >> m.m_initialized
//          >> m.m_max
//          >> m.m_min
//          >> m.m_offset
//          >> m.m_out_max
//          >> m.m_out_min
//          >> m.m_scale;
        
//      return is;
//  }


#endif
