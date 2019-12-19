//
//  Caption.h
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


#ifndef CAPTIONHFILE
#define CAPTIONHFILE

#include <string>

class Caption
{
public:
  
  Caption(const char * caption = "");

  // sizes returned are in pixels, and do not change with transformations
  int width() const;
  int height() const;
  Caption & operator=(const char * s) { m_caption=s; return *this;};

  void render() const;
  void render(double x, double y, double z=0.0) const;

  // (x,y) in pixel coordinates with origin at upper left
  void render_2d(int x, int y) const;

private:
  std::string m_caption;
  void * m_font;
  int m_font_height;
};

#endif

