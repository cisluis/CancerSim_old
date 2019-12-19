//
//  Tissue_Parameters_Dialog.cpp
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


#include "Tissue_Parameters_Dialog.h"
#include "wx/statline.h"
#include <iostream>
#include <assert.h>

BEGIN_EVENT_TABLE(Tissue_Parameters_Dialog, wxDialog)
  EVT_BUTTON(wxID_OK, Tissue_Parameters_Dialog::ok)
  EVT_BUTTON(wxID_CANCEL, Tissue_Parameters_Dialog::cancel)
END_EVENT_TABLE()


Tissue_Parameters_Dialog::Tissue_Parameters_Dialog(wxWindow * parent, const Tissue_Parameters & params) :
  wxDialog(parent, -1, wxString("CancerSim Parameters")),
  m_params(params)
{
  wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

  const char * descriptions[] = {
    "Mutation Rate (-m)\nEach gene is copied with a 1/m chance of mutation",
    "Random Seed (-r)\nSeed value for the pseudorandom number generator",
    "Random Apoptosis (-a)\nEach cell cycle exposes every cell to a 1/a chance of death\nfrom nonspecific causes",
    "Telomere Length (-t)\nTelomere shortening limits the initial cell to t replications",
    "Evade Apoptosis (-e)\nA cell with n mutations has an extra n/e liklihood of dying each cell cycle,\nunless one of its mutations is \"Evade Apoptosis\"",
    "Genetic Instability (-i)\nIncrease the liklihood of mutation by a factor of i\nfor cells with this mutation",
    "Ignore Growth Inhibit (-g)\nCells with this mutation have a 1/g chance of killing off neighbors\nto make room for mitosis",
    0
  };

  for(int i=0; descriptions[i]; ++i)
  {
    wxSizer * subsizer = new wxBoxSizer(wxHORIZONTAL);
    m_text_controls.push_back(new wxTextCtrl(this, -1, wxString::Format(_T("%d"), *param(i))));
    // The following line did not work for the Mac, so wxString::Format above.
    // *m_text_controls.back() << *param(i);
    subsizer->Add(m_text_controls.back(), 0, wxALL, 4);
    subsizer->Add(new wxStaticText(this, -1, descriptions[i]), 0, wxALL, 4);
    sizer->Add(subsizer, 0, wxGROW);
    sizer->Add(new wxStaticLine(this, wxLI_HORIZONTAL), 0, wxGROW);
  }

  wxSizer * subsizer = new wxBoxSizer(wxHORIZONTAL);
  subsizer->Add(new wxButton(this, wxID_OK, "OK"), 0, wxALL, 4);
  subsizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 4);
  sizer->Add(subsizer, 0, wxGROW);

  SetAutoLayout(true);
  SetSizer(sizer);
  sizer->Fit(this);
}

int * Tissue_Parameters_Dialog::param(int i)
{
  switch(i)
  {
  case 0: return &m_params.m_mutation_rate;
  case 1: return &m_params.m_random_seed_seed;
  case 2: return &m_params.m_random_apoptosis;
  case 3: return &m_params.m_telomere_length;
  case 4: return &m_params.m_evade_apoptosis;
  case 5: return &m_params.m_genetic_instability;
  case 6: return &m_params.m_ignore_growth_inhibit;
  default: assert(0);
  }
  return 0;
}

void Tissue_Parameters_Dialog::ok(wxCommandEvent &)
{
  for(int i=0; i < m_text_controls.size(); ++i)
    *param(i) = atoi(m_text_controls[i]->GetValue());
  EndModal(true);
}

void Tissue_Parameters_Dialog::cancel(wxCommandEvent &)
{
  EndModal(false);
}

