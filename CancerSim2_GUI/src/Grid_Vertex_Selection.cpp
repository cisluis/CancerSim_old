//
//  Grid_Vertex_Selection.cpp
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


#include "Grid_Vertex_Selection.h"
#include "Point_3d.h"

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

#include <math.h>

typedef Point_3d<double> Point;

/*
static inline bool is_index(const Bit_Array_3d & a, const Point_3d<double> & p)
{
  return a.is_index(int(floor(p.x)), int(floor(p.y)), int(floor(p.z)));
}
*/

// determine wither d is in the interval [d1, d2]
static inline bool between(double d, double d1, double d2)
{
  const double error = 1e-9;
  return d+error >= std::min(d1,d2) && d-error <= std::max(d1,d2);
}

// determine whether p is within the volume defined by pa and pb
static inline bool within(const Point & p, const Point & pa, const Point & pb)
{
  return
    between(p.x,pa.x,pb.x) &&
    between(p.y,pa.y,pb.y) &&
    between(p.z,pa.z,pb.z);
}

static bool rect_intersect(const Point & p1, const Point & p2, const Point & rmin, const Point & rmax, Point * pa, Point * pb)
{
  const Point pd(p2-p1);

  int nHits=0;
  for(int i=0; i < 6 && nHits < 2; ++i)
  {
    Point & p = nHits ? *pb : *pa;
    const int dim = i/2;
    if(fabs(pd[dim]) < 1e-8)
      continue;
    const Point & r = i%2 ? rmin : rmax;
    p = p1 + pd * ((r[dim]-p1[dim]) / pd[dim]);
    if(within(p, rmin, rmax))
      ++nHits;
  }

  return nHits == 2;
}

Grid_Vertex_Selection::Grid_Vertex_Selection() :
  m_i(-1),m_j(-1),m_k(-1)
{
}

void Grid_Vertex_Selection::set_data(const Bit_Array_3d & data)
{
  m_data = data;

  if(!m_data.is_index(m_i,m_j,m_k) ||
     !m_data(m_i,m_j,m_k))
    m_i=m_j=m_k=-1;
}

bool Grid_Vertex_Selection::select(int x, int y)
{
  m_i=m_j=m_k=-1;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glTranslated(-1.0, -1.0, -1.0);
  glScaled(2.0/m_data.ni(), 2.0/m_data.nj(), 2.0/m_data.nk());

  GLdouble modelview_matrix[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);

  GLdouble projection_matrix[16];
  glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  glPopMatrix();

  Point p0, p1; // p0, p1 define the line specified by the input
  gluUnProject(x, y, -5.0, modelview_matrix, projection_matrix, viewport, &p0.x, &p0.y, &p0.z);
  gluUnProject(x, y, -50.0, modelview_matrix, projection_matrix, viewport, &p1.x, &p1.y, &p1.z);

  Point pa, pb;

  if(!rect_intersect(p0, p1, Point(0,0,0), Point(m_data.ni(),m_data.nj(),m_data.nk()), &pa, &pb))
    return false; // the click is outside the bounding box for the entire grid.

  // ensure that pa is nearer the viewer than pb
  Point pap, pbp;
  gluProject(pa.x, pa.y, pa.z, modelview_matrix, projection_matrix, viewport, &pap.x, &pap.y, &pap.z);
  gluProject(pb.x, pb.y, pb.z, modelview_matrix, projection_matrix, viewport, &pbp.x, &pbp.y, &pbp.z);
  if(pap.z > pbp.z)
    std::swap(pa, pb);

  // iterate through the vertices touched by the segment to see which, if any, are of interest.
  for(Point p=pa; (p-pb).sos() > 1e-6; )
  {
    Point ip = pb + (p-pb) * 0.999999;
    int i = int(ip.x);
    int j = int(ip.y);
    int k = int(ip.z);
    
//    cout << "is it " << *i << ',' << *j << ',' << *k << '?' << endl;

    if(m_data(i, j, k))
    {
      m_i=i;
      m_j=j;
      m_k=k;

      m_x = p.x;
      m_y = p.y;
      m_z = p.z;

      return true;
    }

    if(!rect_intersect(pb, p, Point(i,j,k), Point(i+1, j+1, k+1), &p0, &p1))
    {
      // this can happen only due to numerical inaccuracy
      std::cout << "OOPS " << (p-p0).sos() << "   " << (p-p1).sos() << "     " << p0 << "           " << p1 << std::endl;
      return false;
    }
//    cout << (p-p0).sos() << "   " << (p-p1).sos() << "     " << p0 << "           " << p1 << endl;

    p = (p-p0).sos() < (p-p1).sos() ? p1 : p0;
  }

  return false;

  // find the intersections of these points
}

bool Grid_Vertex_Selection::selected() const
{
  if(m_i < 0 || m_j < 0 || m_k < 0)
  {
    assert(m_i < 0 && m_j < 0 && m_k < 0);
    return false;
  }
  else
    return true;
}

bool Grid_Vertex_Selection::selection(int * i, int * j, int * k) const
{
  if(!selected())
    return false;

  *i = m_i;
  *j = m_j;
  *k = m_k;

  return true;
}

bool Grid_Vertex_Selection::selection(double *x, double *y, double *z) const
{
  if(!selected())
    return false;

  *x = m_x;
  *y = m_y;
  *z = m_z;

  return true;
}

void Grid_Vertex_Selection::render()
{
  if(!selected())
    return;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glTranslated(-1.0, -1.0, -1.0);
  glScaled(2.0/m_data.ni(), 2.0/m_data.nj(), 2.0f/m_data.nk());

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glTranslated(0.0, 0.0, 1e-2);

  Point p, p2, sel(m_i,m_j,m_k);

  glBegin(GL_LINES);
  for(int dim=0; dim < 3; ++dim)
  {
    p[dim]=sel[dim];
    p2[dim]=sel[dim]+1;

    for(int i=0; i <= 1; ++i)
    {
      const int ii = (dim+1)%3;
      p[ii] = p2[ii] = sel[ii]+i;

      for(int j=0; j <= 1; ++j)
      {
    const int jj = (dim+2)%3;
    p[jj] = p2[jj] = sel[jj]+j;

    glVertex3dv(&p.x);
    glVertex3dv(&p2.x);
      }
    }
  }
  glEnd();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

}
