//
//  Caption.cpp
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


#include "Caption.h"

#ifdef __WXMAC__
#     include <GLUT/glut.h>
#else
#     include <GL/glut.h>
#endif

#include "gl_pixel_projection.h"

Caption::Caption(const char * caption) :
  m_caption(caption),
  m_font(GLUT_BITMAP_HELVETICA_12),
  m_font_height(12)
{
}

int Caption::height() const
{
  return m_font_height;
}

int Caption::width() const
{
  int result=0;
  for(std::string::const_iterator i=m_caption.begin(); i != m_caption.end(); ++i)
    result += glutBitmapWidth(m_font, *i);
  return result;
}

void Caption::render() const
{
  for(std::string::const_iterator s = m_caption.begin(); s != m_caption.end(); ++s)
    glutBitmapCharacter(m_font, *s);
}

void Caption::render(double x, double y, double z) const
{
  glRasterPos3d(x,y,z);
  render();
}

void Caption::render_2d(int x, int y) const
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  pixel_projection(false);

  glMatrixMode(GL_MODELVIEW);
  glTranslated(0, height(), 0);
  render(x, y);

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

}
