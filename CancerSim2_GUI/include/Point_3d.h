//
//  Point_3d.h
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


#ifndef I3DPOINTHFILE
#define I3DPOINTHFILE

#include <math.h>
#include <assert.h>

template<class T>
class Point_3d
{
public:
  T x, y, z;

  Point_3d() { };
  Point_3d(T X, T Y, T Z) : x(X), y(Y), z(Z) { };
  T angle(const Point_3d<T> & p) const { return (T) acos(normalized().dot(p.normalized())); };
  // angle between 2 vectors is returned.
  Point_3d<T> component_min(const Point_3d<float> & b) { return Point_3d<T>((x<b.x?x:b.x),(y<b.y?y:b.y),(z<b.z?z:b.z)); };
  // returns component-wise minimum of *this and b.
  Point_3d<T> cross(const Point_3d<T> & b) const { return Point_3d<T>(y*b.z-b.y*z, z*b.x-b.z*x, x*b.y-b.x*y); };
  T distance(const Point_3d<T> & b) const { return (*this-b).length(); };
  // distance between *this and another vector.
  T dot(const Point_3d<T> & b) const { return x*b.x+y*b.y+z*b.z; };
  T sos() const { return x * x + y * y + z * z; }; // sum of squares.
  Point_3d operator*(const Point_3d<T> & b) const { return Point_3d<T>(x*b.x,y*b.y,z*b.z); };
    bool operator>(const Point_3d<T> & b) const { return x > b.x || (x == b.x && (y > b.y || (y == b.y && z > b.z))); };
  // component-wise product of 2 3d points.
  Point_3d & operator+=(const Point_3d<T> & b) { x+=b.x; y+=b.y; z+=b.z; return *this; };
  bool operator==(const Point_3d<T> & b) const { return x==b.x && y==b.y && z==b.z; };
  Point_3d operator*(T n) const { return Point_3d<T>(x*n,y*n,z*n); };
  Point_3d operator/(T n) const { return Point_3d<T>(x/n,y/n,z/n); };
  T & operator[](int dim) { assert(dim>=0&&dim<3); return *(&x+dim); };
  T operator[](int dim) const { assert(dim>=0&&dim<3); return *(&x+dim); };
  Point_3d<T> operator+(const Point_3d<T> & p) const { return Point_3d<T>(x+p.x, y+p.y, z+p.z); };
  Point_3d<T> operator-(const Point_3d<T> & p) const { return Point_3d<T>(x-p.x, y-p.y, z-p.z); };
  Point_3d<T> & operator/=(T t) { x/=t; y/=t; z/=t; return *this; };
  Point_3d<T> & operator*=(T t) { x*=t; y*=t; z*=t; return *this; };
  bool operator<(const Point_3d<T> & p) const { return x < p.x ? true : x > p.x ? false : y < p.y ? true : y > p.y ? false : z < p.z; };
  T length() const { return (T) sqrt(x*x+y*y+z*z); };
  Point_3d<T> & normalize() { return (*this /= length()); };
  Point_3d<T> normalized() const { return (*this / length()); };
  void set(T X, T Y, T Z) { x=X; y=Y; z=Z; };
};

#include <iostream>

template<class T>
std::ostream & operator<<(std::ostream & os, const Point_3d<T> & p)
{
  os << p.x << ' ' << p.y << ' ' << p.z;
  return os;
}

#endif
