//
//  Strip.h
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


// access a subset of a 1-d collection using a 1-d index.
// The elements in the subset must be addressed by a linear function.

template<class T>
class Strip
{
public:
  typedef typename T::reference reference;
  typedef typename T::const_reference const_reference;

  Strip() : m_n(0), m_w(0), m_c(0), m_1d(0) { };
  Strip(T * d, int n, int w, int c) : m_1d(d), m_n(n), m_w(w), m_c(c) { };
//  void create(int n, int w, int c) { m_n=n; m_w=w; m_c=c; };
  reference at(int i) { assert(is_index(i)); return (*m_1d)[i*m_w+m_c]; };
  reference operator[](int i) { return at(i); };
  const_reference at(int i) const { assert(is_index(i)); return (*m_1d)[i*m_w+m_c]; };
  const_reference operator[](int i) const { return at(i); };
  bool is_index(int i) const { return i >=0 && i < n(); };
  int n() const { return m_n; };

protected:
  int m_w;
  int m_c;
  int m_n;
  T * m_1d;
  
};

template<class T>
class Const_Strip
{
public:
  typedef typename T::const_reference const_reference;

  Const_Strip() : m_n(0), m_w(0), m_c(0), m_1d(0) { };
  Const_Strip(const T * d, int n, int w, int c) : m_1d(d), m_n(n), m_w(w), m_c(c) { };
//  void create(int n, int w, int c) { m_n=n; m_w=w; m_c=c; };
  const_reference at(int i) { assert(is_index(i)); return (*m_1d)[i*m_w+m_c]; };
  const_reference operator[](int i) { return at(i); };
  bool is_index(int i) const { return i >=0 && i < n(); };
  int n() const { return m_n; };

protected:
  int m_w;
  int m_c;
  int m_n;
  const T * m_1d;
  
};

#include <iosfwd>

template<class T>
std::ostream & operator<<(std::ostream & os, const Strip<T> & s)
{
  for(int i=0; i < s.n(); ++i)
    os << s[i] << ' ' << std::endl;
  return os;
}

template<class T>
std::ostream & operator<<(std::ostream & os, const Const_Strip<T> & s)
{
  for(int i=0; i < s.n(); ++i)
    os << s[i] << ' ' << std::endl;
  return os;
}
