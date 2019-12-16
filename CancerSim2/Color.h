//
//  Color.h
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


#ifndef COLORHFILE
#define COLORHFILE

class Color
{
 public:
  
  typedef unsigned char Elem;

  Color() { };
  Color(Elem r, Elem g, Elem b) : m_r(r), m_g(g), m_b(b) { };
  Color operator*(double d) const { return Color((Elem)(m_r*d), (Elem)(m_g*d), (Elem)(m_b*d)); };
  bool operator==(const Color & c) const { return m_r==c.m_r && m_g==c.m_g && m_b==c.m_b; };
  Elem m_r, m_g, m_b;
};

#include <iostream>
inline std::ostream & operator<<(std::ostream & os, Color c)
{
  os << double(c.m_r) << ' ' << double(c.m_g) << ' ' << double(c.m_b);
  return os;
}

#endif
