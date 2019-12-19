//
//  gl_unproject.cpp
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


#include "gl_unproject.h"

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


void gl_unproject(double x, double y, double z,
                double * xx, double * yy, double * zz)
{
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  GLdouble modelMatrix[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

  GLdouble projMatrix[16];
  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

  gluUnProject(x,y,z, modelMatrix, projMatrix, viewport, xx, yy, zz);

}

