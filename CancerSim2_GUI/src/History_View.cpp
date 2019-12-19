//
//  History_View.cpp
//  CancerSim2_GUI
//
//  Created by Luis Cisneros on 12/13/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.

//     History_View.cc
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


#include "History_View.h"

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

#include <stdio.h>
#include "gl_save_backbuffer_as_bmp.h"
#include "wx/dcclient.h"
#include "gl_unproject.h"
#include "Caption.h"

BEGIN_EVENT_TABLE(History_View, wxGLCanvas)
  EVT_PAINT(History_View::OnPaint)
  EVT_SIZE(History_View::OnSize)
  EVT_ERASE_BACKGROUND(History_View::OnEraseBackground)
  EVT_MOUSE_EVENTS(History_View::OnMouseEvent)
END_EVENT_TABLE()

History_View::History_View(wxWindow * parent) :
  wxGLCanvas(parent, -1),
  m_tissue(0),
  m_log(true),
  m_border(4),
  m_marker_count(-1),
  m_lbutton_down(false)
{
  m_log=false;
}

History_View::~History_View()
{
}

void History_View::OnPaint(wxPaintEvent & e)
{
  wxPaintDC dc(this);
//  cout << "History_View::OnPaint" << endl;
  SetCurrent();
  render();
  SwapBuffers();
}

void History_View::OnSize(wxSizeEvent& event)
{
  GetClientSize(&m_width, &m_height);
}

void History_View::OnEraseBackground(wxEraseEvent & e)
{
//  cout << "History_View::Erase" << endl;
}

void History_View::create(const Tissue * tissue)
{
  m_tissue = tissue;
}

void History_View::clone(History_View * h) const
{
  h->m_history = m_history;
  h->m_palette = m_palette;
  h->m_log = m_log;
  h->m_marker_count = m_marker_count;
  h->m_marker_time = m_marker_time;
  h->m_marker_genotype = m_marker_genotype;
  h->m_border = m_border;
  h->m_height = m_height;
}

void History_View::render()
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glDisable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  int height = m_height-2*m_border;
  glViewport(m_border, m_border, m_width-2*m_border, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  const int n = m_history.num_samples();
  if(n > 1)
  {
    const Tissue::Time * times = m_history.times();

    glTranslated(-1, -1, 0);
    //    glScaled(2.0/times[n-1], 2.0/ceil(map(m_history.max())), 1.0);
    glScaled(1.9/times[n-1], 1.9/ceil(map(m_history.max())), 1.0);

    for(int i=0; i < Tissue::num_genotypes(); ++i)
    {
      const Tissue::Genotype g = Tissue::ith_genotype(i);
      if(m_history.max(g) == 0)
    continue; // never have been any cells with this genotype
      const int * samples = m_history.samples(g);

      glColor3ubv(&m_palette[i].m_r);
      glBegin(GL_LINE_STRIP);
      for(int j=0; j < n; ++j)
    glVertex2d(times[j], map(samples[j]));
      glEnd();
    }

    if(m_marker_count > 0)
    {
      glColor3ubv(&m_palette[Tissue::index_of_genotype(m_marker_genotype)].m_r);
      float prev_point_size;
      glGetFloatv(GL_POINT_SIZE, &prev_point_size);
      glPointSize(6);
      glBegin(GL_POINTS);
      glVertex2d(m_marker_time, m_marker_count);
      glEnd();
      glPointSize(prev_point_size);
      char buf[1000];
      snprintf(buf, 100, "t=%d, n=%d", m_marker_time, m_marker_count);
      glColor3ub(0,0,0);
      Caption(buf).render_2d(0,0);
    }
  }
}

void History_View::sample()
{
  assert(m_tissue);
  m_history.sample(*m_tissue);
//  Refresh(false);
}

void History_View::set_palette(const CancerSim::Palette & p)
{
  m_palette = p;
}

void History_View::update_marker(int x, int y)
{
  double xx, yy, zz;
  gl_unproject(x, m_height-y+2*m_border, 0, &xx, &yy, &zz);
  const int time = int(xx+0.5);
  const int count = int(yy+0.5);
    
  const int n = m_history.num_samples();
  if(n == 0) return;
  const Tissue::Time * times = m_history.times();
  int i;
  for(i=0; i < n-1 && *times < time; ++i, ++times);
  m_marker_time = *times;

  m_marker_genotype = Tissue::ith_genotype(0);
  for(int j=1; j < Tissue::num_genotypes(); ++j)
    if(abs(count-m_history.ith_sample(Tissue::ith_genotype(j),i)) < abs(count-m_history.ith_sample(m_marker_genotype,i)))
      m_marker_genotype = Tissue::ith_genotype(j);
    
  m_marker_count = m_history.ith_sample(m_marker_genotype, i);
    
  Refresh(FALSE);
}

void History_View::OnMouseEvent(wxMouseEvent & e)
{
  if(e.ButtonDown(1))
  {
    m_lbutton_down = true;
    update_marker(e.m_x,e.m_y);
  }
  else if(e.ButtonUp(1))
    m_lbutton_down = false;
  else if(e.Dragging() && e.m_leftDown)
    update_marker(e.m_x,e.m_y);
}

void History_View::discard()
{
  m_history.discard();
  m_marker_count=-1;
}

