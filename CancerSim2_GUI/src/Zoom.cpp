//
//  Zoom.cpp
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

/*
#ifdef __WXMAC__
#     include <OpenGL/gl.h>
#     include <GLUT/glut.h>
#else
#     include <GL/gl.h>
#     include <GL/glut.h>
#endif
*/
#include <GL/gl.h>
#include <GL/glut.h>

#include "Zoom.h"

Zoom::Zoom(float min, float max, float initial) :
  m_min(min), m_max(max), m_initial(initial), m_drag_handle(-1)
{
  for(int i=0; i < 16; ++i)
    m_transformation[i]=0.0f;
  for(int i=0; i < 16; i+=5)
    m_transformation[i]=1.0f;
  m_transformation[14]=(float)m_initial;
}

bool Zoom::drag_start(int y)
{
  m_drag_handle = y;
  return true;
}

bool Zoom::drag_stop()
{
  if(dragging())
  {
    m_drag_handle = -1;
    return true;
  }
  else
    return false;
}

bool Zoom::drag(int y)
{
  if(dragging())
  {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    m_transformation[14] += float(m_max-m_min)*float(y-m_drag_handle)/float(viewport[3]-viewport[1]);
    if(m_transformation[14] < m_min)
      m_transformation[14] = m_min;
    else if(m_transformation[14] > m_max)
      m_transformation[14] = m_max;
    return true;
  }
  else
    return false;
}
