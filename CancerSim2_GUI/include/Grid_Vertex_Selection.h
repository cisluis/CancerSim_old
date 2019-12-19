//
//  Grid_Vertex_Selection.h
//  CancerSim2_GUI
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


// Given a screen coordinate, this class determines which node
// of interest in a grid was projected at the max z screen coordinate.

#include "Bit_Array_3d.h"

class Grid_Vertex_Selection
{
public:

  Grid_Vertex_Selection();

  // the data specifies the dimensions of the grid and also
  // the nodes 'of interest' (those in the Bit_Array which are set).
  void set_data(const Bit_Array_3d & data);
  
  // determine the index of the node under the screen coordinate x,y.
  // If none, false is returned; otherwise, true is returned and
  // i,j,k are set to the indices of the node.
  bool select(int x, int y);

  void render();

  // return false if no cell is selected, true otherwise.
  bool selected() const;

  // get the indices of the last selected cell, or return false if
  // no cell is selected.
  bool selection(int * i, int * j, int * k) const;

  // get the location of the selection, or false if none
  bool selection(double *x, double *y, double *z) const;

protected:

  Bit_Array_3d m_data;

  // index of last node selected, or -1,-1,-1 if none.
  int m_i,m_j,m_k;

  // the actual impact point.
  double m_x, m_y, m_z;

};
