//
//  Multi_Grid_View.hpp
//  CancerSim2
//
//  Created by Luis Cisneros on 12/13/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
 
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


#include "Array_3d.h"
#include "Palette.h"
#include "Array_3d_Slice.h"
#include "Bit_Array_2d.h"


class Bit_Array_3d;

// The Multi_Grid_View can draw different blocks in different colors.
// To draw in only one color, use the less complicated Grid_View instead.

// No blocks are drawn where the value is 0.
// The datatype to set_data is unsigned char, so up to 255 different
// colored cubes may be drawn.

class Multi_Grid_View
{
public:
  
  typedef unsigned char Value;
  typedef Array_3d<Value> Values;
  
  Multi_Grid_View();
  ~Multi_Grid_View();

  // if max_data is unspecified, it will be determined.
  void set_data(const Values & data, const Value max_data=0);
  
  void render(bool color);
  
  typedef unsigned short Index;

  struct Face {
    Face() { };
    Face(Index i1, Index j1, Index i2, Index j2, Index k, Value value) :
      m_i1(i1), m_j1(j1), m_i2(i2), m_j2(j2), m_k(k), m_value(value) { };
    Index m_i1, m_j1, m_i2, m_j2, m_k;
    Value m_value;
  };

  void set_palette(const CancerSim::Palette & p);
  const CancerSim::Palette & palette() const { return m_palette; };

protected:
  
  CancerSim::Palette m_palette;
  
  std::vector<Face> m_up_i_faces;
  std::vector<Face> m_down_i_faces;
  std::vector<Face> m_up_j_faces;
  std::vector<Face> m_down_j_faces;
  std::vector<Face> m_up_k_faces;
  std::vector<Face> m_down_k_faces;

  int m_ni, m_nj, m_nk;

  int m_display_list;

  static Face flood(const Const_Slice<Values >& s1,
            const Const_Slice<Values >& s2,
            int i, int j, int k,
            Bit_Array_2d *done);

  static void add_faces(std::vector<Face> * up_faces,
            std::vector<Face> * down_faces,
            const Const_Slice<Values>& s1,
            const Const_Slice<Values>& s2,
            int k);
};


