//
//  reviewGUI.cpp
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


#include "reviewGUI.h"
#include "wx/statline.h"
#include "wx/choice.h"
#include <stdio.h>
#include <iostream>

class Review_Window : public wxFrame
{
public:
  Review_Window();
  void button_pressed(wxEvent & e);
  void die(wxCloseEvent&);
  void source_choice(wxCommandEvent&);
  void cancersim(wxCommandEvent&);
protected:
  FILE * m_file;
  wxChoice * m_source_choice;
  wxChoice * m_seed_choice;
  DECLARE_EVENT_TABLE()
};

#define ID_MAX_RADIOBUTTON  (wxID_HIGHEST+100)
#define ID_SOURCE_CHOICE    (wxID_HIGHEST+101)
#define ID_SEED_CHOICE    (wxID_HIGHEST+102)
#define ID_CANCERSIM    (wxID_HIGHEST+103)

BEGIN_EVENT_TABLE(Review_Window, wxFrame)
  EVT_CUSTOM_RANGE(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxID_HIGHEST, ID_MAX_RADIOBUTTON, Review_Window::button_pressed)
  EVT_CLOSE(Review_Window::die)
  EVT_CHOICE(ID_SOURCE_CHOICE, Review_Window::source_choice)
  EVT_BUTTON(ID_CANCERSIM, Review_Window::cancersim)
END_EVENT_TABLE()

IMPLEMENT_APP(reviewGUI)

bool reviewGUI::OnInit()
{
  Review_Window * window = new Review_Window;
  window->Show(true);
  return true;
}

Review_Window::Review_Window() :
  wxFrame(0, -1, "CancerSim Review", wxDefaultPosition, wxDefaultSize),
  m_file(0)
{
  m_file = popen("./review", "w");
  if(m_file == 0)
  {
    std::cerr << "Failed to open the 'review' program.  Exiting..." << std::endl;
    exit(-1);
  }

  wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

  m_source_choice = new wxChoice(this, ID_SOURCE_CHOICE);
  m_source_choice->Append("Num Cells", (void*) "xextract_num_cells");
  m_source_choice->Append("(No Mutations)", (void*) "xextract_num_with_mutation 0");
  m_source_choice->Append("Genetic Instability", (void*) "xextract_num_with_mutation 1");
  m_source_choice->Append("Ignore Growth Inhibit", (void*) "xextract_num_with_mutation 2");
  m_source_choice->Append("Evade Apoptosis", (void*) "xextract_num_with_mutation 3");
  m_source_choice->Append("Limitless Replication", (void*) "xextract_num_with_mutation 4");
  m_source_choice->Append("Sustained Angiogenesis", (void*) "xextract_num_with_mutation 5");
  m_source_choice->Append("Self Growth", (void*) "xextract_num_with_mutation 6");
  m_source_choice->SetSelection(0);
  fprintf(m_file, "l%s\n", (const char*) m_source_choice->GetStringSelection());
  fflush(m_file);
  sizer->Add(m_source_choice, 0, wxGROW);

  char * setting_names[8][4] = {
    "Grid Size", "27000", "125000", "1000000",
    "Genetic Instability", "10", "100", "1000",
    "Ignore Growth Inhibit", "2", "10", "100",
    "Telomere Length", "25", "100", "200",
    "Mutation Rate", "10000", "100000", "1000000",
    "Evade Apoptosis", "5", "20", 0,
    "Random Apoptosis", "100", "1000", "2000",
    0
  };

  for(int i=0; setting_names[i][0]; ++i)
  {
    sizer->Add(new wxStaticLine(this, wxLI_HORIZONTAL), 0, wxGROW);
    sizer->Add(new wxStaticText(this, -1, setting_names[i][0]), 0, wxLEFT, 5);
    wxSizer * subsizer = new wxBoxSizer(wxHORIZONTAL);
    for(int j=1; j < 4 && setting_names[i][j]; ++j)
    {
      int style = (j==1 ? wxRB_GROUP : 0);
      wxRadioButton * button = new wxRadioButton(this, wxID_HIGHEST+i*3+(j-1), setting_names[i][j], wxDefaultPosition, wxDefaultSize, style);
      subsizer->Add(button, 0, wxLEFT, 5);
      button->SetValue(j-1 == 1);
    }
    sizer->Add(subsizer);
  }

  sizer->Add(new wxStaticLine(this, wxLI_HORIZONTAL), 0, wxGROW);
  wxSizer * subsizer = new wxBoxSizer(wxHORIZONTAL);
  subsizer->Add(new wxButton(this, ID_CANCERSIM, "CancerSim"), 0);
  m_seed_choice = new wxChoice(this, ID_SEED_CHOICE);
  m_seed_choice->Append("Seed = 1");
  m_seed_choice->Append("Seed = 2");
  m_seed_choice->Append("Seed = 3");
  m_seed_choice->SetSelection(0);
  subsizer->Add(m_seed_choice, 1, wxLEFT, 4);
  sizer->Add(subsizer, 0, wxGROW);

  SetAutoLayout(true);
  SetSizer(sizer);
  sizer->Fit(this);
}

void Review_Window::button_pressed(wxEvent & event)
{
  int id = event.GetId()-wxID_HIGHEST;
  char param_names[] = { 'n', 'i', 'g', 't', 'm', 'e', 'a', 'r' };
  fprintf(m_file, "%c%d\n", param_names[id/3], id%3);
  fflush(m_file);
}

void Review_Window::die(wxCloseEvent&)
{
  fprintf(m_file, "q\n");
  fflush(m_file);
  fclose(m_file);
  m_file=0;

  Destroy();
}

void Review_Window::source_choice(wxCommandEvent & evt)
{
//  fprintf(m_file, "%s\n", (const char*) m_source_choice->GetStringSelection(), (const char*) evt.GetClientData());
  fprintf(m_file, "l%s\n%s\n", (const char*) m_source_choice->GetStringSelection(), (const char*) evt.GetClientData());
  fflush(m_file);
}

void Review_Window::cancersim(wxCommandEvent & evt)
{
  fprintf(m_file, "c%d\n", m_seed_choice->GetSelection());
  fflush(m_file);
}

