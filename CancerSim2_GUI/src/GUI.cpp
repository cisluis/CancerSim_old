//
//  GUI.cpp
//  CancerSim2_GUI
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


#include "GUI.h"
#include "Case.h"
#include "Color_Key.h"
#include "wx/bitmap.h"

IMPLEMENT_APP(GUI);

wxApp * theApp = &wxGetApp();

bool GUI::OnInit()
{
  add_case();
  
  return true;
}

Case * GUI::add_case()
{
  int x, y;

  Case * c = new Case(this, "Cancer Grid", wxPoint(-1,-1), wxSize(600,400));
  c->create(argc, argv);
  c->GetPosition(&x, &y);

  Color_Key * colorkey = new Color_Key(c, "Key", wxPoint(x, y+420));
  colorkey->render();
  colorkey->Show();
  SetTopWindow(colorkey);
  
  c->set_palette(colorkey->palette());
  c->Show();
  SetTopWindow(c);
  return c;
}

void GUI::case_holder_duplicate(const Case * c)
{
  Case * c2 = add_case();
  c->clone(c2);
}
