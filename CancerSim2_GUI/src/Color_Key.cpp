//
//  Color_Key.cpp
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


#include "Color_Key.h"

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

#include "Tissue.h"
#include "gl_save_backbuffer_as_bmp.h"
#include "gl_pixel_projection.h"
#include "Caption.h"
#include <algorithm>

// count the number of bits set in an integer.
static int count_bits(unsigned int n)
{
int result;
for(result=0; n > 0; n>>=1)
if(n&1)
++result;
return result;
}

Color_Key::Color_Key(wxWindow * holder, const wxString& title, const wxPoint& pos) :
// the exact width doesn't matter since the boxes are scaled.
  wxFrame(holder,-1,title, pos, wxSize(600, Caption().height()*(Tissue::num_mutation_types()+2)*3/2)),
  m_canvas(0)
{
  m_palette.resize(Tissue::num_genotypes());

  Color * begin = &m_palette[0];
  Color * end = &m_palette[Tissue::num_genotypes()-1];
  std::fill(begin, end, Color(0,0,0));

// specify 'nice' colors for the most likely genotypes  - those with the smallest
// number of mutations.  The rest are chosen pseudorandomly.

  m_palette[0] = Color(128,128,128); // no mutation

  unsigned char nice_colors[][3] = {
    { 120,120,120 },
    { 255,0,0 },
    { 0,255,0 },
    { 255,255,0 },
    { 0,0,255 },
    { 255,0,255 },
    { 0,255,255 },
    { 128,0,0 },
    { 200,200,200 },
    { 255,200,200 },
    { 200,255,200 },
    { 0,128,0 },
    { 200,200,255 },
    { 0,0,128 },
    { 128,0,128 },
    { 0,128,128 } };

  // determine the number of 'nice' colors to use.
//  const int num_nice_colors = std::min(m_palette.size(), int(sizeof(nice_colors)/sizeof(nice_colors[0])));
  int num_nice_colors = int(sizeof(nice_colors)/sizeof(nice_colors[0]));
  if(m_palette.size() < num_nice_colors)
    num_nice_colors = m_palette.size();

  // assign the nice colors to the genotypes with the smallest number of mutations
  int num_nice_colors_used = 0;
  for(int num_bits = 0; num_nice_colors_used < num_nice_colors; ++num_bits)
  {
    for(int i=0; i < m_palette.size() && num_nice_colors_used < num_nice_colors; ++i)
      if(count_bits(i) == num_bits)
      {
    m_palette[i] = Color(nice_colors[num_nice_colors_used][0], nice_colors[num_nice_colors_used][1], nice_colors[num_nice_colors_used][2]);
    ++num_nice_colors_used;
      }
  }

  // assign the rest of the palette arbitrarily
  Random_Number_Generator random(1974); // arbitrary seed.
  for(Color * cur = begin; cur != end; ++cur)
  {
    if(*cur == Color(0,0,0))
       *cur = Color(random(), random(), random());
  }

  m_canvas = new wxGLCanvas2(this, this);
  
  wxLayoutConstraints * c = new wxLayoutConstraints;
  c->left.SameAs(this, wxLeft);
  c->right.SameAs(this, wxRight);
  c->bottom.SameAs(this, wxBottom);
  c->top.SameAs(this, wxTop);
  m_canvas->SetConstraints(c);
  Layout();
  SetAutoLayout(true);
}

void Color_Key::render()
{
  pixel_projection(false);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  glColor3i(0,0,0);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  const double outerSpacing = 4.0;
  const double innerSpacing = 2.0;

  // determine max width of any caption
  int maxTotalWidth=0;
  for(int iMut=0; iMut < Tissue::num_mutation_types(); ++iMut)
  {
    const int totalWidth = Caption(Tissue::mutation_name(Tissue::ith_mutation_type(iMut))).width();
    if(totalWidth > maxTotalWidth)
      maxTotalWidth = totalWidth;
  }

  double widthPerType = (viewport[2]-2*outerSpacing-Tissue::num_genotypes()*innerSpacing-maxTotalWidth)
    / Tissue::num_genotypes();

  // draw
  for(int iMut=-1; iMut < Tissue::num_mutation_types(); ++iMut)
  {
    glTranslated(0, Caption().height()+innerSpacing, 0);
    glPushMatrix();
    glTranslated(outerSpacing, 0.0, 0.0);
    
    if(iMut >= 0)
    {
      const Tissue::Mutation mut = Tissue::ith_mutation_type(iMut);
      const char * c = Tissue::mutation_name(mut);
      Caption(c).render(0,0,0);
    }

    glTranslated(maxTotalWidth+innerSpacing, 0, 0);

    for(int iType=0; iType < Tissue::num_genotypes(); ++iType)
    {
      if(iMut < 0)
    glColor3ubv(&m_palette[iType].m_r);

      if(iMut < 0 || (1<<iMut)&iType)
      {
    glBegin(GL_QUAD_STRIP);
    glVertex2d(0, 0);
    glVertex2d(0, -Caption().height());
    glVertex2d(widthPerType, 0);
    glVertex2d(widthPerType, -Caption().height());
    glEnd();
      }
      glTranslated(widthPerType+innerSpacing, 0, 0);
    }
    glColor3ub(0,0,0);
    glPopMatrix();
  }

  glPopMatrix();
}

//  void Color_Key::keyboard_func(unsigned char key, int x, int y)
//  {
//    if(key == ' ')
//    {
//      // make sure an up-to-date rendering is in the backbuffer (lame)
//      render();
//      const char *filename = "screen.bmp";
//      gl_save_backbuffer_as_bmp(filename);
//    }
//  }
