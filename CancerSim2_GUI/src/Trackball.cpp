//
//  Trackball.cpp
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

#include "Trackball.h"
#include <algorithm>

Trackball::Trackball(float trackball_size) : m_drag_handle(0.0f, 0.0f, 0.0f), m_trackball_size(trackball_size)
{
}

void Trackball::reset()
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glRotated(20, 0, 1, 0);
  glRotated(15, 1, 0, 0);
  glGetFloatv(GL_MODELVIEW_MATRIX, m_transformation);
  glPopMatrix();
}

void Trackball::render()
{
  if(dragging())
  {
//    glutWireSphere(m_trackball_size, 20, 20);
  }
}

void Trackball::copy_view_angle(const Trackball & t)
{
  std::copy(t.m_transformation+0, t.m_transformation+16, m_transformation+0);
}

bool Trackball::drag_start(float x, float y, float z)
{
  m_trackball_size = (float) sqrt(x*x+y*y+z*z);
  m_drag_handle.set(x, y, z);
  return true;
}

bool Trackball::drag_start(int x, int y)
{
  if(intersect_sphere(x,y,m_trackball_size, &m_drag_handle))
    return true;
  else
  {
    m_drag_handle.set(0.0f, 0.0f, 0.0f);
    return false;
  }
}

bool Trackball::drag_stop()
{
  if(dragging())
  {
    m_drag_handle.set(0.0f, 0.0f, 0.0f);
    return true;
  }
  else
    return false;
}

bool Trackball::drag(int x, int y)
{
  Point_3d<float> p;
  if(dragging() &&
    intersect_sphere(x,y,m_trackball_size, &p, &m_drag_handle))
  {
    if(m_drag_handle == p)
      return false;

    Point_3d<float> axis;
    axis = p.cross(m_drag_handle);
    axis.normalize();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(m_transformation);
    const float theta = p.angle(m_drag_handle)*180.0f/3.14159f;
    if(fabs(theta) > 0.0 && fabs(theta) < 360.0)
      glRotatef(-theta, axis.x, axis.y, axis.z);
    glGetFloatv(GL_MODELVIEW_MATRIX, m_transformation);
    glPopMatrix();

    return true;
  }
  else
    return false;
}


// determine where a line defined by p1 and p2 intersects a sphere
// centered at the origin with radius r.  The results (0, 1, or 2)
// are stored in result_1_ptr, result_2_ptr

int Trackball::intersect_sphere(
  const Point_3d<float> p, const Point_3d<float> p2,
  const float r, Point_3d<float> * result_1_ptr, Point_3d<float> * result_2_ptr)
{
  const Point_3d<float> d(p2-p);

//  r = sqrt((x1+s*xd)^2+(y1+s*yd)^2+(z1+s*zd)^2);  - solve for s
//  r^2 = (x1+s*xd)^2+(y1+s*yd)^2+(z1+s*zd)^2
//  r^2 = x1^2+2*x1*s*xd+s^2*xd^2 + y1^2+2*y1*s*yd+s^2*yd^2 + z1^2+2*z1*s*zd+s^2*zd^2
//  r^2 = s^2*xd^2+s^2*yd^2+s^2*zd^2 +2*x1*s*xd+2*y1*s*yd+2*z1*s*zd + x1^2+y1^2+z1^2
//  r^2 = s^2*xd^2+s^2*yd^2+s^2*zd^2 +2*x1*s*xd+2*y1*s*yd+2*z1*s*zd + x1^2+y1^2+z1^2
//  r^2 = s^2*(xd^2+yd^2+zd^2) + s*(2*x1*xd+2*y1*yd+2*z1*zd) + x1^2+y1^2+z1^2
//  0 = s^2*(xd^2+yd^2+zd^2) + s*(2*x1*xd+2*y1*yd+2*z1*zd) + x1^2+y1^2+z1^2-r^2
//  0 = s^2*(xd^2+yd^2+zd^2) + s*2*(x1*xd+y1*yd+z1*zd) + x1^2+y1^2+z1^2-r^2

//  a = xd^2+yd^2+zd^2
//  b = 2*(x1*xd+y1*yd+z1*zd)
//  c = x1^2+y1^2+z1^2-r^2

  const float a = d.dot(d);
  const float b = 2.0f*p.dot(d);
  const float c = p.dot(p)-r*r;
  const float b24ac = b*b-4*a*c;

  if(b24ac < 0.0f)
    return 0; ////// early return; the line doesn't touch the sphere.

  const float sqrt_b24ac = (float) sqrt(b24ac);

  const float n = (-b+sqrt_b24ac)/(a+a);
  if(result_1_ptr)
    *result_1_ptr = p+d*n;

  if(sqrt_b24ac == 0.0)
    return 1; // 1 answer
  else
  {
    const float n = (-b-sqrt_b24ac)/(a+a);
    if(result_2_ptr)
      *result_2_ptr = p+d*n;
    return 2;  // 2 answers
  }
}

bool Trackball::intersect_sphere(int x, int y, float radius, Point_3d<float> * p, const Point_3d<float> * hint)
{
  GLdouble modelview_matrix[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);

  GLdouble projection_matrix[16];
  glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  // unprojected coordinates of the mouse click.
  Point_3d<float> p0((float)x, (float)y, -5.0f);
  Point_3d<float> p1((float)x, (float)y, -25.0f);
  
  Point_3d<double> pd; // a point of doubles.

  gluUnProject(p0.x, p0.y, p0.z, modelview_matrix, projection_matrix, viewport, &pd.x, &pd.y, &pd.z);
  p0.set((float)pd.x, (float)pd.y, (float)pd.z);

  gluUnProject(p1.x, p1.y, p1.z, modelview_matrix, projection_matrix, viewport, &pd.x, &pd.y, &pd.z);
  p1.set((float)pd.x, (float)pd.y, (float)pd.z);

  // where does the click intersect the trackball
  Point_3d<float> track1;
  Point_3d<float> track2;
  int n = intersect_sphere(p0, p1, radius, &track1, &track2);
  if(n==0)
    return false;
  else if(n==1)
  {
    if(p)
      *p=track1;
  }
  else
  {
    if(hint)
    {
      if(hint->distance(track1) < hint->distance(track2))
      {
    if(p)
      *p = track1;
      }
      else
      {
    if(p)
      *p = track2;
      }
    }
    else
    {

      // project the intersection points.
      gluProject(track1.x, track1.y, track1.z, modelview_matrix, projection_matrix, viewport, &pd.x, &pd.y, &pd.z);
      const double z1 = pd.z;
      gluProject(track2.x, track2.y, track2.z, modelview_matrix, projection_matrix, viewport, &pd.x, &pd.y, &pd.z);
      if(pd.z > z1)
      {
    if(p)
      *p = track1;
      }
      else
      {
    if(p)
      *p = track2;
      }
    }
  }

  return true;
}

void Trackball::set_radius(float radius)
{
  assert(!dragging());
  m_trackball_size = radius;
}
