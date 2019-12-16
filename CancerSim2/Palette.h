//
//  Palette.h
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


#ifndef PALETTEHFILE
#define PALETTEHFILE

#include <vector>
#include "Color.h"

namespace CancerSim {
  class Palette
    {
    public:
      
      Palette() {} ;
      Palette(int n) : m_entries(n) { };
      
      int size() const { return m_entries.size(); };
      
      void resize(int n) { m_entries.resize(n); };
      
      void spread(Color c, Color c2);
      void spread() { spread(Color(0,0,0),Color(255,255,255)); };
      
      const Color & operator[](int i) const { return at(i); };
      const Color & at(int i) const { assert(is_index(i)); return m_entries[i]; };
      
      Color & operator[](int i) { return at(i); };
      Color & at(int i) { assert(is_index(i)); return m_entries[i]; };
      
      bool is_index(int i) const { return i >= 0 && i < size(); };
      
    protected:
      
      std::vector<Color> m_entries;
      
    };
  
  inline std::ostream & operator<<(std::ostream & os, const CancerSim::Palette & p)
    {
      for(int i=0; i < p.size(); ++i)
    os << p[i] << std::endl;
      return os;
    }
}

#endif
