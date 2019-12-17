//
//  Timer.h
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


class Timer
{
public:
  Timer() : { };

protected:

  static time() {
  timeval tv;
  gettimeofday(&tv, 0);
  double s1 = tv.tv_sec + tv.tv_usec / 1000000.0;

  gettimeofday(&tv, 0);
  double s3 = tv.tv_sec + tv.tv_usec / 1000000.0;

  double d = s2-s1;
  static double total = 0.0;
  total += d;
  cout << "update: " << d << ' ' << total << endl;


};
