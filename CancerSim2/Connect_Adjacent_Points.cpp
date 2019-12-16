//
//  Connect_Adjacent_Points.cpp
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
#include "Connect_Adjacent_Points.h"
#include <vector>
#include "Point_3d.h"

#ifdef __WXMAC__
#     include <OpenGL/gl.h>
#else
#     include <GL/gl.h>
#endif


typedef Point_3d<short> Offset;
typedef std::vector<Offset> Offsets;
static Offsets offsets;

Connect_Adjacent_Points::Connect_Adjacent_Points()
{
  if(offsets.size()==0)
    for(int i=0; i <= 1; ++i)
      for(int j=-1; j <= 1; ++j)
    for(int k=-1; k <= 1; ++k)
      if(Offset(i,j,k) > Offset(0,0,0))
        offsets.push_back(Offset(i,j,k));
}

void Connect_Adjacent_Points::set_data(const Bit_Array_3d & data)
{
  m_ni=data.ni();
  m_nj=data.nj();
  m_nk=data.nk();

  m_segments.resize(0);

  for(int i=m_ni-1; i--; )
    for(int j=m_nj-1; j--; )
      for(int k=m_nk-1; k--; )
    if(data(i,j,k))
      for(Offsets::const_iterator o=offsets.begin(); o != offsets.end(); ++o)
        if(data.is_index(i+o->x, j+o->y, k+o->z) && data(i+o->x, j+o->y, k+o->z))
          m_segments.push_back(Segment(i,j,k,o-offsets.begin()));
}

void Connect_Adjacent_Points::render() const
{
  if(m_segments.size() == 0)
    return;

  glPushMatrix();

  glTranslatef(-1.0f, -1.0f, -1.0f);
  glScalef(2.0f/float(m_ni-1), 2.0f/float(m_nj-1), 2.0f/float(m_nk-1));
  
  glBegin(GL_LINES);
  for(Segments::const_iterator i=m_segments.begin(); i != m_segments.end(); ++i)
  {
    glVertex3s(i->m_i, i->m_j, i->m_k);
    Offsets::const_iterator o = offsets.begin()+i->m_orientation;
    glVertex3s(i->m_i+o->x, i->m_j+o->y, i->m_k+o->z);
  }
  glEnd();

  glPopMatrix();
}


