//
//  Grid_View.cpp
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


#include "Grid_View.h"
#include <GL/gl.h>
#include "Point_3d.h"
//#include "Array_3d_Slice.h"
#include "Array_3d_Strip.h"
#include "Array_3d_Slice.h"
#include "Bit_Array_2d.h"

struct Xor { bool operator()(bool a, bool b) { return a^b; }; };

Grid_View::Face flood(const Const_Slice<Bit_Array_3d>& s1,
              const Const_Slice<Bit_Array_3d>& s2,
              int i, int j, int k,
              Bit_Array_2d *done)
{
  int i2 = i+1;
  int j2 = j+1;

  done->at(i,j)=true;
  for(; j2 < s1.nj() && !done->at(i,j2) && s1(i,j2)^s2(i,j2); ++j2)
    done->at(i,j2)=true;

  // grow i
  for(int jj=j2; jj==j2 && i2 < s1.ni(); )
  {
    for(jj=j; jj < j2 && !done->at(i2, jj) && s1(i2,jj)^s2(i2,jj); ++jj);
    if(jj == j2)
    {
      for(jj=j; jj < j2; ++jj)
    done->at(i2, jj) = true;
      ++i2;
    }
  }
    
  return Grid_View::Face(i,j,i2,j2,k);
}

void add_faces(std::vector<Grid_View::Face> * up_faces,
           std::vector<Grid_View::Face> * down_faces,
           const Const_Slice<Bit_Array_3d>& s1,
           const Const_Slice<Bit_Array_3d>& s2,
           int k)
{
  Bit_Array_2d done(s1.ni(), s1.nj());
  done.set_all(false);
  for(int i=0; i < s1.ni(); ++i)
    for(int j=0; j < s1.nj(); ++j)
      if(!done(i,j) && s1(i,j)^s2(i,j))
    (s1(i,j) ? up_faces : down_faces)->push_back(flood(s1, s2, i,j,k, &done));
}

void Grid_View::set_data(const Bit_Array_3d & data)
{
  m_ni=data.ni();
  m_nj=data.nj();
  m_nk=data.nk();

  Bit_Array_3d zeros(1,1,1);
  zeros(0,0,0)=0;
  Const_Slice<Bit_Array_3d> zero_slice;

  m_up_i_faces.resize(0);
  m_down_i_faces.resize(0);
  m_up_i_faces.reserve(1000);
  m_down_i_faces.reserve(1000);
  zero_slice = Const_Slice<Bit_Array_3d>(&zeros, m_nk, m_nj, 0, 0, 0);
  add_faces(&m_up_i_faces, &m_down_i_faces, zero_slice, slice_i(data,0), 0);
  for(int i=0; i < data.ni()-1; ++i)
    add_faces(&m_up_i_faces, &m_down_i_faces, slice_i(data,i), slice_i(data,i+1), i+1);
  add_faces(&m_up_i_faces, &m_down_i_faces, slice_i(data,data.ni()-1), zero_slice, data.ni());

  m_up_j_faces.resize(0);
  m_down_j_faces.resize(0);
  m_up_j_faces.reserve(1000);
  m_down_j_faces.reserve(1000);
  zero_slice = Const_Slice<Bit_Array_3d>(&zeros, m_ni, m_nk, 0, 0, 0);
  add_faces(&m_up_j_faces, &m_down_j_faces, zero_slice, slice_j(data,0), 0);
  for(int j=0; j < data.nj()-1; ++j)
    add_faces(&m_up_j_faces, &m_down_j_faces, slice_j(data,j), slice_j(data,j+1), j+1);
  add_faces(&m_up_j_faces, &m_down_j_faces, slice_j(data,data.nj()-1), zero_slice, data.nj());

  m_up_k_faces.resize(0);
  m_down_k_faces.resize(0);
  m_up_j_faces.reserve(1000);
  m_down_j_faces.reserve(1000);
  zero_slice = Const_Slice<Bit_Array_3d>(&zeros, m_ni, m_nj, 0, 0, 0);
  add_faces(&m_up_k_faces, &m_down_k_faces, zero_slice, slice_k(data,0), 0);
  for(int k=0; k < data.nk()-1; ++k)
    add_faces(&m_up_k_faces, &m_down_k_faces, slice_k(data,k), slice_k(data,k+1), k+1);
  add_faces(&m_up_k_faces, &m_down_k_faces, slice_k(data,data.nk()-1), zero_slice, data.nk());

//  cout << m_i_faces.size() << ' ' << m_j_faces.size() << ' ' << m_k_faces.size() << endl;
}
void Grid_View::render()
{
  if(m_ni == 0)
    return;

  int faces_rendered=0;

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

  glTranslatef(-1.0f, -1.0f, -1.0f);
  glScalef(2.0f/float(m_ni), 2.0f/float(m_nj), 2.0f/float(m_nk));
  
  float color[4];
  glGetFloatv(GL_CURRENT_COLOR, color);

  glBegin(GL_QUADS);

  glNormal3f(1.0f, 0.0f, 0.0f);
  glColor3f(color[0]*0.5f, color[1]*0.5f, color[2]*0.5f);
  for(int i=0; i < m_up_i_faces.size(); ++i)
  {
    const Face & face = m_up_i_faces[i];
    const float z1 = face.m_i1;
    const float z2 = face.m_i2;
    const float y1 = face.m_j1;
    const float y2 = face.m_j2;
    const float x = face.m_k;
    glVertex3f(x, y1, z1);
    glVertex3f(x, y2, z1);
    glVertex3f(x, y2, z2);
    glVertex3f(x, y1, z2);
  }

  glNormal3f(0.0f, 1.0f, 0.0f);
  glColor3f(color[0]*0.95f, color[1]*0.95f, color[2]*0.95f);
  for(int j=0; j < m_up_j_faces.size(); ++j)
  {
    const Face & face = m_up_j_faces[j];
    const float x1 = face.m_i1;
    const float x2 = face.m_i2;
    const float z1 = face.m_j1;
    const float z2 = face.m_j2;
    const float y = face.m_k;
    glVertex3f(x1, y, z2);
    glVertex3f(x2, y, z2);
    glVertex3f(x2, y, z1);
    glVertex3f(x1, y, z1);
  }

  glNormal3f(0.0f,0.0f, 1.0f);
  glColor3f(color[0]*0.75f, color[1]*0.75f, color[2]*0.75f);
  for(int k=0; k < m_up_k_faces.size(); ++k)
  {
    const Face & face = m_up_k_faces[k];
    const float x1 = face.m_i1;
    const float x2 = face.m_i2;
    const float y1 = face.m_j1;
    const float y2 = face.m_j2;
    const float z = face.m_k;
    glVertex3f(x1, y1, z);
    glVertex3f(x2, y1, z);
    glVertex3f(x2, y2, z);
    glVertex3f(x1, y2, z);
  }

  glNormal3f(-1.0f, 0.0f, 0.0f);
  glColor3f(color[0]*0.5f, color[1]*0.5f, color[2]*0.5f);
  for(int i=0; i < m_down_i_faces.size(); ++i)
  {
    const Face & face = m_down_i_faces[i];
    const float z1 = face.m_i1;
    const float y1 = face.m_j1;
    const float z2 = face.m_i2;
    const float y2 = face.m_j2;
    const float x = face.m_k;
    glVertex3f(x, y1, z1);
    glVertex3f(x, y1, z2);
    glVertex3f(x, y2, z2);
    glVertex3f(x, y2, z1);
  }

  glNormal3f(0.0f, -1.0f, 0.0f);
  glColor3f(color[0]*0.95f, color[1]*0.95f, color[2]*0.95f);
  for(int j=0; j < m_down_j_faces.size(); ++j)
  {
    const Face & face = m_down_j_faces[j];
    const float x1 = face.m_i1;
    const float x2 = face.m_i2;
    const float z1 = face.m_j1;
    const float z2 = face.m_j2;
    const float y = face.m_k;
    glVertex3f(x1, y, z1);
    glVertex3f(x2, y, z1);
    glVertex3f(x2, y, z2);
    glVertex3f(x1, y, z2);
  }

  glNormal3f(0.0f, 0.0f, -1.0f);
  glColor3f(color[0]*0.75f, color[1]*0.75f, color[2]*0.75f);
  for(int k=0; k < m_down_k_faces.size(); ++k)
  {
    const Face & face = m_down_k_faces[k];
    const float x1 = face.m_i1;
    const float x2 = face.m_i2;
    const float y1 = face.m_j1;
    const float y2 = face.m_j2;
    const float z = face.m_k;
    glVertex3f(x1, y2, z);
    glVertex3f(x2, y2, z);
    glVertex3f(x2, y1, z);
    glVertex3f(x1, y1, z);
  }

  glEnd();

  glColor4fv(color);

  glPopMatrix();
}


// g++ -o main Tissue_Viewer.cpp -I/home/rob/data/code/birdbox/glui_v2_1_beta -lglui -lglut -L/home/rob/data/code/birdbox/glui_v2_1_beta/lib/ Grid_View.cpp

