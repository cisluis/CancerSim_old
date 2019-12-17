//
//  Limited_History.cpp
//  CancerSim2
//
//  Created by Luis Cisneros on 11/8/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Limited_History.cc
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


#include <iostream>
#include "Limited_History.h"

/*
int main(int argc, char * argv[])
{
    Limited_History<int> h(7);
    
    std::cout << "max is " << h.limit() << std::endl;
    for(int i=0; i < 50; ++i)
    {
        h.append(i);
        std::cout << h << std::endl;
    }
    
    h.set_limit(4);
    std::cout << "max is " << h.limit() << std::endl << h << std::endl;
    
    h.discard();
    for(int i=0; i < 50; ++i)
    {
        h.append(i);
        std::cout << h << std::endl;
    }
    
    return 0;
}
*/

