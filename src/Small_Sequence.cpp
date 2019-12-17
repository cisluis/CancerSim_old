//
//  Small_Sequence.cpp
//  CancerSim2
//
//  Created by Luis Cisneros on 11/5/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//

//     Small_Sequence.cc
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


#include "Small_Sequence.h"
#include <iostream>

std::ostream& operator<< (std::ostream& os, const Small_Sequence& s)
{
    const int size = s.size();
    
    for (int i=0; i < size; ++i)
    {
        if (i > 0)
        {
            os << ',';
        }
        os << s[i];
    }
    return os;
}
            
            
/*
 #include <stdlib.h>
 #include <time.h>
 int main(int argc, char * argv[])
 {
 srand(time(0));
 Small_Sequence s;
 assert(s.empty());
 cout << s << endl;
 int i = 0;
 while(!s.full())
 {
 int n = rand()%0xF;
 s.append(n);
 assert(s.size() == ++i);
 assert(!s.empty());
 cout << "Appending " << n << ": " << s << "; size = " << s.size() << endl;
 }
 
 return 0;
 }
 */
