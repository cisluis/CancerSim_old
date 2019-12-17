//
//  Multi_Grid_View.cpp
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


#include "Multi_Grid_View.h"
#ifdef __WXMAC__
#     include <OpenGL/gl.h>
#else
#     include <GL/gl.h>
#endif

#include "Point_3d.h"
#include "Array_3d_Strip.h"
#include "Bit_Array_3d.h"

// this performs a logical xor; i.e. it returns true only if exactly one of the arguments is 0
template<class T>
inline bool l_xor(T a, T b) { return !a ^ !b; };

Multi_Grid_View::Face Multi_Grid_View::flood(
  const Const_Slice<Values>& s1,
  const Const_Slice<Values>& s2,
  int i, int j, int k,
  Bit_Array_2d *done)
{
  int i2 = i+1;
  int j2 = j+1;

  done->at(i,j)=true;

  const Value v = s1(i,j);
  const Value v2 = s2(i,j);

  for(; j2 < s1.nj() && !done->at(i,j2) && s1(i,j2)==v && s2(i,j2)==v2; ++j2)
    done->at(i,j2)=true;

  // grow i
  for(int jj=j2; jj==j2 && i2 < s1.ni(); )
  {
    for(jj=j; jj < j2 && !done->at(i2, jj) && s1(i2,jj)==v && s2(i2,jj)==v2; ++jj);
    if(jj == j2)
    {
      for(jj=j; jj < j2; ++jj)
    done->at(i2, jj) = true;
      ++i2;
    }
  }
    
  return Multi_Grid_View::Face(i,j,i2,j2,k,v?v:v2);
}

Multi_Grid_View::Multi_Grid_View()
{
  m_display_list=0;
}

Multi_Grid_View::~Multi_Grid_View()
{
//  if(m_display_list)
//    glDeleteLists(m_display_list, 1);
}

void Multi_Grid_View::add_faces(std::vector<Multi_Grid_View::Face> * up_faces,
           std::vector<Multi_Grid_View::Face> * down_faces,
           const Const_Slice<Values>& s1,
           const Const_Slice<Values>& s2,
           int k)
{
  Bit_Array_2d done(s1.ni(), s1.nj());
  done.set_all(false);
  for(int i=0; i < s1.ni(); ++i)
    for(int j=0; j < s1.nj(); ++j)
      if(!done(i,j) && l_xor(s1(i,j), s2(i,j)))
    (s1(i,j) ? up_faces : down_faces)->push_back(flood(s1, s2, i,j,k, &done));
}

void Multi_Grid_View::set_palette(const CancerSim::Palette & p)
{
  assert(p.size() >= m_palette.size());
  m_palette = p;
};

void Multi_Grid_View::set_data(const Values & data, const Value max_value)
{
  m_ni=data.ni();
  m_nj=data.nj();
  m_nk=data.nk();

  Values zeros(1,1,1);
  zeros(0,0,0)=0;
  Const_Slice<Values > zero_slice;

  m_up_i_faces.resize(0);
  m_down_i_faces.resize(0);
  m_up_i_faces.reserve(1000);
  m_down_i_faces.reserve(1000);
  zero_slice = Const_Slice<Values >(&zeros, m_nk, m_nj, 0, 0, 0);
  add_faces(&m_up_i_faces, &m_down_i_faces, zero_slice, slice_i(data,0), 0);
  for(int i=0; i < data.ni()-1; ++i)
    add_faces(&m_up_i_faces, &m_down_i_faces, slice_i(data,i), slice_i(data,i+1), i+1);
  add_faces(&m_up_i_faces, &m_down_i_faces, slice_i(data,data.ni()-1), zero_slice, data.ni());

  m_up_j_faces.resize(0);
  m_down_j_faces.resize(0);
  m_up_j_faces.reserve(1000);
  m_down_j_faces.reserve(1000);
  zero_slice = Const_Slice<Values >(&zeros, m_ni, m_nk, 0, 0, 0);
  add_faces(&m_up_j_faces, &m_down_j_faces, zero_slice, slice_j(data,0), 0);
  for(int j=0; j < data.nj()-1; ++j)
    add_faces(&m_up_j_faces, &m_down_j_faces, slice_j(data,j), slice_j(data,j+1), j+1);
  add_faces(&m_up_j_faces, &m_down_j_faces, slice_j(data,data.nj()-1), zero_slice, data.nj());

  m_up_k_faces.resize(0);
  m_down_k_faces.resize(0);
  m_up_j_faces.reserve(1000);
  m_down_j_faces.reserve(1000);
  zero_slice = Const_Slice<Values >(&zeros, m_ni, m_nj, 0, 0, 0);
  add_faces(&m_up_k_faces, &m_down_k_faces, zero_slice, slice_k(data,0), 0);
  for(int k=0; k < data.nk()-1; ++k)
    add_faces(&m_up_k_faces, &m_down_k_faces, slice_k(data,k), slice_k(data,k+1), k+1);
  add_faces(&m_up_k_faces, &m_down_k_faces, slice_k(data,data.nk()-1), zero_slice, data.nk());

  Value max=0;
  for(int i=0; i < data.n(); ++i)
    if(data[i] > max)
      max = data[i];
  if(max_value)
  {
    assert(max_value >= max);
    max = max_value;
  }
  if(m_palette.size() < max-1)
  {
    m_palette.resize(max-1);
    m_palette.spread(Color(100,100,100),Color(250,250,250));
  }

  if(m_display_list)
  {
    glDeleteLists(m_display_list, 1);
    m_display_list = 0;
  }

//  cout << m_i_faces.size() << ' ' << m_j_faces.size() << ' ' << m_k_faces.size() << endl;
}
void Multi_Grid_View::render(bool color)
{
  if(m_ni == 0)
    return;

  int faces_rendered=0;

  if(m_display_list)
    glCallList(m_display_list);
  else
  {
//    m_display_list=glGenLists(1); // using the display list causes a massive memory leak. figure out what's wrong sometime...
//    assert(m_display_list);
//    glNewList(m_display_list, GL_COMPILE_AND_EXECUTE);

    glPushMatrix();

    glTranslatef(-1.0f, -1.0f, -1.0f);
    glScalef(2.0f/float(m_ni), 2.0f/float(m_nj), 2.0f/float(m_nk));
  
    glBegin(GL_QUADS);

    glNormal3f(1.0f, 0.0f, 0.0f);
    for(int i=0; i < m_up_i_faces.size(); ++i)
    {
      const Face & face = m_up_i_faces[i];
      if(color) glColor3ubv(&(m_palette[face.m_value-1]*0.5).m_r);
      const short z1 = face.m_i1;
      const short z2 = face.m_i2;
      const short y1 = face.m_j1;
      const short y2 = face.m_j2;
      const short x = face.m_k;
      glVertex3s(x, y1, z1);
      glVertex3s(x, y2, z1);
      glVertex3s(x, y2, z2);
      glVertex3s(x, y1, z2);
    }

    glNormal3f(0.0f, 1.0f, 0.0f);
    for(int j=0; j < m_up_j_faces.size(); ++j)
    {
      const Face & face = m_up_j_faces[j];
      if(color) glColor3ubv(&(m_palette[face.m_value-1]*1.0).m_r);
      const short x1 = face.m_i1;
      const short x2 = face.m_i2;
      const short z1 = face.m_j1;
      const short z2 = face.m_j2;
      const short y = face.m_k;
      glVertex3s(x1, y, z2);
      glVertex3s(x2, y, z2);
      glVertex3s(x2, y, z1);
      glVertex3s(x1, y, z1);
    }

    glNormal3f(0.0f,0.0f, 1.0f);
    for(int k=0; k < m_up_k_faces.size(); ++k)
    {
      const Face & face = m_up_k_faces[k];
      if(color) glColor3ubv(&(m_palette[face.m_value-1]*0.75).m_r);
      const short x1 = face.m_i1;
      const short x2 = face.m_i2;
      const short y1 = face.m_j1;
      const short y2 = face.m_j2;
      const short z = face.m_k;
      glVertex3s(x1, y1, z);
      glVertex3s(x2, y1, z);
      glVertex3s(x2, y2, z);
      glVertex3s(x1, y2, z);
    }

    glNormal3f(-1.0f, 0.0f, 0.0f);
    for(int i=0; i < m_down_i_faces.size(); ++i)
    {
      const Face & face = m_down_i_faces[i];
      if(color) glColor3ubv(&(m_palette[face.m_value-1]*0.5).m_r);
      const short z1 = face.m_i1;
      const short y1 = face.m_j1;
      const short z2 = face.m_i2;
      const short y2 = face.m_j2;
      const short x = face.m_k;
      glVertex3s(x, y1, z1);
      glVertex3s(x, y1, z2);
      glVertex3s(x, y2, z2);
      glVertex3s(x, y2, z1);
    }

    glNormal3f(0.0f, -1.0f, 0.0f);
    for(int j=0; j < m_down_j_faces.size(); ++j)
    {
      const Face & face = m_down_j_faces[j];
      if(color) glColor3ubv(&(m_palette[face.m_value-1]*1.0).m_r);
      const short x1 = face.m_i1;
      const short x2 = face.m_i2;
      const short z1 = face.m_j1;
      const short z2 = face.m_j2;
      const short y = face.m_k;
      glVertex3s(x1, y, z1);
      glVertex3s(x2, y, z1);
      glVertex3s(x2, y, z2);
      glVertex3s(x1, y, z2);
    }

    glNormal3f(0.0f, 0.0f, -1.0f);
    for(int k=0; k < m_down_k_faces.size(); ++k)
    {
      const Face & face = m_down_k_faces[k];
      if(color) glColor3ubv(&(m_palette[face.m_value-1]*0.75).m_r);
      const short x1 = face.m_i1;
      const short x2 = face.m_i2;
      const short y1 = face.m_j1;
      const short y2 = face.m_j2;
      const short z = face.m_k;
      glVertex3s(x1, y2, z);
      glVertex3s(x2, y2, z);
      glVertex3s(x2, y1, z);
      glVertex3s(x1, y1, z);
    }

    glEnd();

    glPopMatrix();

//    glEndList();
   }
}
