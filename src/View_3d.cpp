//
//  View_3d.cpp
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


#include "View_3d.h"
#include "Tissue.h"

#ifdef __WXMAC__
#     include <OpenGL/gl.h>
#     include <GLUT/glut.h>
#else
#     include <GL/gl.h>
#     include <GL/glut.h>
#endif

#include "algorithm"
#include "gl_save_backbuffer_as_bmp.h"
#include "wx/dcclient.h"
#include "Caption.h"
#include <sstream>

BEGIN_EVENT_TABLE(View_3d, wxGLCanvas)
  EVT_PAINT(View_3d::OnPaint)
  EVT_SIZE(View_3d::OnSize)
  EVT_ERASE_BACKGROUND(View_3d::OnEraseBackground)
  EVT_MOUSE_EVENTS(View_3d::OnMouseEvent)
END_EVENT_TABLE()

View_3d::View_3d(wxWindow * parent) :
  wxGLCanvas(parent,-1),
  m_tissue(0),
  m_trackball(float(sqrt(2.0f))),
  m_zoom(-4.0f, -0.05f, -3.0f),
  m_holder(0),
  m_first_time(true)
{
  m_cell_visibility.resize(1<<m_tissue->num_mutation_types());
  std::fill(m_cell_visibility.begin(), m_cell_visibility.end(), true);
}

View_3d::~View_3d()
{
}

void View_3d::OnEraseBackground(wxEraseEvent & e)
{
}

void View_3d::create(const Tissue * tissue, View_3d_Holder * holder)
{
  m_tissue = tissue;
  m_holder = holder;
}

void View_3d::OnPaint(wxPaintEvent & e)
{
  wxPaintDC dc(this);
  SetCurrent();
  render();
  SwapBuffers();
}

void View_3d::OnSize(wxSizeEvent& event)
{
  GetClientSize(&m_width, &m_height);
  if(GetContext()){
    SetCurrent();
    glViewport(0, 0, (GLint) m_width, (GLint) m_height);
  }
}

void View_3d::render()
{
  if(m_first_time)
  {
    m_trackball.reset();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE); // I think this should work, but it causes white spots to appear
    m_first_time = false;
  }

  glViewport(0, 0, m_width, m_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)m_width/m_height, 0.01, 20.0);

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  // the trackball update must be done *before* destroying
  // the old modelview matrix.
  if(m_trackball.drag(m_mouse_x,m_mouse_y))
    if(m_holder)
      m_holder->view_3d_rotation(this);
  SetCurrent();
  m_zoom.drag(m_mouse_y);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glMultMatrixf(m_zoom.m_transformation);
  glMultMatrixf(m_trackball.m_transformation);

  glColor3f(1.0f, 0.0f, 0.0f);
  m_capillary_view.render();

  if(some_cells_are_visible())
  {
    m_cell_view.render(true);
    
//      glColor3ub(0,0,0);
//      glDepthFunc(GL_LEQUAL);
//      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//      m_cell_view.render(false);
//      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glColor3f(1.0f, 1.0f, 0.0f);
    if(m_trackball.dragging())
    {
      m_cell_selection.render();
      glColor3ub(0,0,0);
      int xx, yy, zz;
      m_cell_selection.selection(&xx, &yy, &zz);
      std::stringstream caption;
      m_tissue->report_genotype(caption, xx, yy, zz);
      Caption(caption.str().c_str()).render_2d(0,0);
    }
  }
 
  glEnable(GL_BLEND);
  glColor4f(0.0f, 0.0f, 0.0f, 0.2f);
  m_trackball.render();
  glDisable(GL_BLEND);

}

void View_3d::copy_view_angle(const View_3d & v)
{
  m_trackball.copy_view_angle(v.m_trackball);
  Refresh(FALSE);
}

void View_3d::update()
{
  // set data for m_cell_view and m_cell_selection
  if(some_cells_are_visible())
  {
    const Array_3d<Tissue::Genotype> & genotype = m_tissue->genotype();
    Multi_Grid_View::Values data(m_tissue->ni(),m_tissue->nj(),m_tissue->nk());

    for(int i=0; i < genotype.n(); ++i)
    {
      const Tissue::Genotype g = genotype[i];
      if(g >= 0 && m_cell_visibility[g])
    data[i] = g+1;
      else
    data[i] = 0;
    }
    m_cell_view.set_data(data, Tissue::num_genotypes());

    Bit_Array_3d on(m_tissue->ni(),m_tissue->nj(),m_tissue->nk());
    for(int i=0; i < data.n(); ++i)
      on[i] = data[i] ? true : false;
    m_cell_selection.set_data(on);
  }
  
  // set data for capillaries
  m_capillary_view.set_data(m_tissue->capillaries());
  Refresh(FALSE);
}


void View_3d::OnMouseEvent(wxMouseEvent & e)
{
  int x = m_mouse_x = e.m_x;
  int y = m_mouse_y = m_height - e.m_y;

  if(e.ButtonDown(1))
  {
    if(m_cell_selection.select(x,y))
    {
      double xx, yy, zz;
      m_cell_selection.selection(&xx, &yy, &zz);
      xx = xx*2.0/m_tissue->ni()-1.0;
      yy = yy*2.0/m_tissue->nj()-1.0;
      zz = zz*2.0/m_tissue->nk()-1.0;
      m_trackball.drag_start(xx,yy,zz);
      Refresh(false);
    }
  }

  else if(e.ButtonUp(1))
  {
//    cout << "left button up" << endl;
    m_trackball.drag_stop();
  }

  else if(e.ButtonDown(3))
  {
    m_cell_selection.select(x,y);
    int i, j, k;
    if(m_cell_selection.selection(&i,&j,&k))
      m_cell_visibility[m_tissue->genotype()(i,j,k)] = false;
    else
      std::fill(m_cell_visibility.begin(), m_cell_visibility.end(), true);
    update();
  }

  else if(e.Dragging() && e.m_leftDown)
  {
    Refresh(false);
  }

//   case 1: // zoom
//      if(state == 0)
//        m_zoom.drag_start(y);
//      else
//        m_zoom.drag_stop();
//      break;
//    default:
//    assert(0);

//  }
}

bool View_3d::some_cells_are_visible() const
{
  return std::find(m_cell_visibility.begin(), m_cell_visibility.end(), true) != m_cell_visibility.end();
}
