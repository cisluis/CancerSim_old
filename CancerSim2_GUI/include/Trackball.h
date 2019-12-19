//
//  Trackball.hpp
//  CancerSim2_GUI
//
//  Created by Luis Cisneros on 12/13/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
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


#ifndef TRACKBALLHFILE
#define TRACKBALLHFILE

#include "Point_3d.h"

class Trackball
{
public:
  
  Trackball(float radius=1.0f);

  void render();

  void reset();

  bool dragging() const { return m_drag_handle.sos() > 0.0f; };
  bool drag_start(int i, int j);
  bool drag_start(float x, float y, float z);
  bool drag(int i, int j);
  bool drag_stop();
  void set_radius(float);

  float m_transformation[16];

  void copy_view_angle(const Trackball &);
  
protected:
  
  Point_3d<float> m_drag_handle;
  float m_trackball_size;

  // determines where the line perpendicular to the screen and intersecting
  // it at x,y intersects a spere of radius 'radius' at the origin.  If
  // there is >=1 intersection, intersection nearest the viewer is stored in 'p'
  // and true is returned; otherwise, false is returned.
  static bool intersect_sphere(int x, int y, float radius,
                   Point_3d<float> * p, const Point_3d<float> * hint = 0);

  // determines where a line defined by p and p2 intersects a sphere
  // of radius r at the origin.  Returns the number of intersections
  // (which is 0, 1, or 2).  If the result pointers are non-null,
  // they will contain the points of intersection, if any.
  static int intersect_sphere(const Point_3d<float> p, const Point_3d<float> p2,
                  const float r,
                  Point_3d<float> * result_1_ptr, Point_3d<float> * result_2_ptr);

};

#endif

