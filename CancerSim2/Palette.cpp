//
//  Palette.cpp
//  CancerSim2
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


#include "Palette.h"
#include <math.h>

void CancerSim::Palette::spread(Color c, Color c2)
{
  // integer overflow occurs if size > 256
  assert(size() <= 256);

  if(size()==0)
    return;
  else if(c == c2 || size() == 1)
    std::fill(m_entries.begin(), m_entries.end(), c);
  else
  {
    unsigned int dr = int(c2.m_r)-c.m_r+1;
    unsigned int dg = int(c2.m_r)-c.m_r+1;
    unsigned int db = int(c2.m_r)-c.m_r+1;
    unsigned int a = dr*dg*db;
    for(int i=0; i < size(); ++i)
    {
      unsigned int rem;
      rem = (a-1)*i/(size()-1);
      unsigned int ddr = rem / (dg*db);
      unsigned int ddg = rem / db % dg;
      unsigned int ddb = rem % db;
      at(i)=Color(Color::Elem(c.m_r+ddr), Color::Elem(c.m_r+ddg), Color::Elem(c.m_r+ddb));
    }
  }
}

//  int main(int argc, char * argv[])
//  {
//    for(int i=1; i <= 256; i *= 2)
//    {
//      CancerSim::Palette p(i);
//      p.spread();
//      cout << p << endl;
//    }
//    return 0;
//  }

