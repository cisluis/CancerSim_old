//
//  Grid_View.hpp
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


#include "Bit_Array_3d.h"

class Grid_View
{
public:
  
  void set_data(const Bit_Array_3d & data);
  
  void render();
  
  typedef short Index;

  struct Face {
    Face() { };
    Face(Index i1, Index j1, Index i2, Index j2, Index k) :
      m_i1(i1), m_j1(j1), m_i2(i2), m_j2(j2), m_k(k) { };
    Index m_i1, m_j1, m_i2, m_j2, m_k;
  };

protected:
  
  std::vector<Face> m_up_i_faces;
  std::vector<Face> m_down_i_faces;
  std::vector<Face> m_up_j_faces;
  std::vector<Face> m_down_j_faces;
  std::vector<Face> m_up_k_faces;
  std::vector<Face> m_down_k_faces;

  int m_ni, m_nj, m_nk;
};
