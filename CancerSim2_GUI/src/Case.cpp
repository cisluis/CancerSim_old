//
//  Case.cpp
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


#include "Case.h"
#include <vector>
#include "wx/filedlg.h"
#include "wx/layout.h"
#include "wx/sashwin.h"
#include "Tissue_Parameters_Dialog.h"
#include "sys/time.h"


#undef Above
#undef Below

#define ID_0          10000
#define ID_RUN        (ID_0+0)
#define ID_DUPLICATE  (ID_0+1)
#define ID_RESET      (ID_0+2)
#define ID_JUMP_TIME  (ID_0+3)
#define ID_DUAL_VIEW  (ID_0+4)
#define ID_SCREENSHOT (ID_0+5)
#define ID_SINGLE_STEP   (ID_0+6)
#define ID_EDIT_SETTINGS (ID_0+7)
#define ID_REDRAW_FREQUENCY (ID_0+8)

BEGIN_EVENT_TABLE(Case, wxFrame)
  EVT_SPLITTER_SASH_POS_CHANGED(-1, Case::OnSashPositionChange)
  EVT_SIZE(Case::OnSize)
  EVT_IDLE(Case::OnIdle)
  EVT_BUTTON(ID_RUN, Case::OnRunButton)
  EVT_BUTTON(ID_SINGLE_STEP, Case::OnSingleStepButton)
  EVT_BUTTON(ID_DUPLICATE, Case::OnDuplicateButton)
  EVT_BUTTON(ID_RESET, Case::OnResetButton)
  EVT_BUTTON(ID_JUMP_TIME, Case::OnJumpTimeButton)
  EVT_BUTTON(ID_DUAL_VIEW, Case::OnDualViewButton)
  EVT_BUTTON(ID_SCREENSHOT, Case::OnScreenshotButton)
  EVT_BUTTON(ID_EDIT_SETTINGS, Case::OnEditSettingsButton)
  EVT_BUTTON(ID_REDRAW_FREQUENCY, Case::OnEditRedrawFrequency)
END_EVENT_TABLE()


Case::Case(Case_Holder * holder, const wxString & title, const wxPoint & pos, const wxSize & size) :
  wxFrame(0, 107, title, pos, size),
  m_3d_views(0),
  m_history_view(0),
  m_running(false),
  m_holder(holder),
  m_jump_to_time(0),
  m_redraw_frequency(100),
  m_run_button(new wxButton(this, ID_RUN, "Run")),
  m_single_step_button(new wxButton(this, ID_SINGLE_STEP, "Single Step")),
  m_reset_button(new wxButton(this, ID_RESET, "Reset")),
  m_duplicate_button(new wxButton(this, ID_DUPLICATE, "Duplicate")),
  m_jump_time_button(new wxButton(this, ID_JUMP_TIME, "Jump Time")),
  m_dual_view_button(new wxButton(this, ID_DUAL_VIEW, "Dual View")),
  m_screenshot_button(new wxButton(this, ID_SCREENSHOT, "Screenshot")),
  m_redraw_frequency_button(new wxButton(this, ID_REDRAW_FREQUENCY, "Redraw Freq"))
{
  CreateStatusBar();
  // lay out the screen elements.
  const int gap = 5;

  std::vector<wxButton*> buttons;
  buttons.push_back(m_run_button);
  buttons.push_back(m_single_step_button);
  buttons.push_back(m_duplicate_button);
  buttons.push_back(m_reset_button);
  buttons.push_back(m_jump_time_button);
  buttons.push_back(m_dual_view_button);
  buttons.push_back(m_screenshot_button);
  buttons.push_back(new wxButton(this, ID_EDIT_SETTINGS, "Settings"));
  buttons.push_back(m_redraw_frequency_button);

  for(int i=0; i < buttons.size(); ++i)
  {
    wxLayoutConstraints * c = new wxLayoutConstraints;
    c->width.AsIs();
    c->height.AsIs();
    c->left.Absolute(gap);
    if(i)
      c->top.Below(buttons[i-1], gap);
    else
      c->top.Absolute(gap);
    buttons[i]->SetConstraints(c);
  }
  wxSplitterWindow * splitter = new wxSplitterWindow(this,-1);
  m_history_view = new History_View(splitter);
  m_3d_views = new Dual_3d_View(splitter);
  
  wxLayoutConstraints * c = new wxLayoutConstraints;

  #ifdef __WXMAC__
  c->left.RightOf(buttons[0], gap + 30);
  #else
  c->left.RightOf(buttons[0], gap);
  #endif
  c->top.Absolute(gap);
  c->right.SameAs(this, wxRight, gap);
  c->bottom.SameAs(this, wxBottom, gap);
  splitter->SetConstraints(c);

  SetAutoLayout(TRUE);
  Layout();
  splitter->SplitHorizontally(m_3d_views, m_history_view);
}

Case::~Case()
{
}

void Case::create(int argc, char * argv[])
{
  m_tissue.create(argc, argv);

  m_3d_views->create(&m_tissue);
  m_history_view->create(&m_tissue);

  update();
}

// This is a real hack for OSX
void Case::OnSashPositionChange(wxSplitterEvent&){
//void Case::OnSashPositionChange(int newSashPosition){
  int w, h;
  GetSize(&w, &h);
  SetSize(w+1, h+1);
}

// This is a real hack for OSX
bool Case::Show(bool show)
{
  wxPoint p = GetPosition();
  Move(0,0); Move(p);
  return wxFrame::Show(show);
}

// This is needed for OSX
void Case::OnSize(wxSizeEvent& event)
{
  wxFrame::OnSize(event);
  Show();
}

void Case::OnIdle(wxIdleEvent & event)
{
  //int i;

  if(m_running == false)
    return;

  const long checks_per_second = 2;

  timeval quitting_time;
  gettimeofday(&quitting_time, 0);
  quitting_time.tv_usec += 1000000 / checks_per_second;

  // normalize the quitting time.
  quitting_time.tv_sec += quitting_time.tv_usec / 1000000;
  quitting_time.tv_usec = quitting_time.tv_usec % 1000000;

  timeval now;

  int n = 0;

  while(1)
  {
    ++n;
    gettimeofday(&now, 0);
    if(now.tv_sec > quitting_time.tv_sec ||
       (now.tv_sec == quitting_time.tv_sec && now.tv_usec >= quitting_time.tv_usec))
      break;

    if(m_jump_to_time && m_jump_to_time == m_tissue.time())
    {
      run(); // we've reached the time; this toggles (stops) the run.
      break;
    }

    if(m_tissue.run_to_time(m_tissue.time()+1) == false)
    {
      run();
      break;
    }
    else
    {
      m_history_view->sample();
      if(m_tissue.time() % m_redraw_frequency == 0 && m_jump_to_time == 0)
    update();
    }
  }

  wxWakeUpIdle();
//  cout << n << endl;
}

void Case::run()
{
  if(!m_running)
  {
    std::cout << m_tissue << std::endl;
    m_run_button->SetLabel("Stop");
    m_running = true;
  }
  else
  {
    m_jump_to_time = 0;
    m_run_button->SetLabel("Run");
    m_running = false;
    std::cout << m_tissue.num_cells_created() << " cells have been created" << std::endl;
    update();
  }
}

void Case::update()
{
  
  char buf[200];
  sprintf(buf, "Sim Time = %d", m_tissue.time());
  SetStatusText(buf);
//  cout << buf << endl;

  m_3d_views->update();
  m_history_view->Refresh(FALSE);
}

void Case::OnRunButton(wxCommandEvent & event)
{
  run();
}

void Case::OnSingleStepButton(wxCommandEvent & event)
{
  m_jump_to_time = m_tissue.time()+1;
  run();
}

void Case::set_palette(const CancerSim::Palette & p)
{
  m_3d_views->set_palette(p);
  m_history_view->set_palette(p);
}

void Case::clone(Case * c) const
{
  m_tissue.clone(&c->m_tissue);
  m_history_view->clone(c->m_history_view);
  c->update();
}

void Case::OnDuplicateButton(wxCommandEvent &)
{
  if(m_running)
    run(); // stop
  m_holder->case_holder_duplicate(this);
}

void Case::OnResetButton(wxCommandEvent &)
{
  char buf[100];
  sprintf(buf, "%d", m_tissue.ni()*m_tissue.nj()*m_tissue.nk());
  wxTextEntryDialog dlg(this, "Max Number Of Cells", "Resize", buf);
  if(dlg.ShowModal()==wxID_OK)
  {
    int n = atoi(dlg.GetValue());
    if(n > 10)
    {
      Tissue_Parameters params = m_tissue.params();
      params.m_num_cell_locations = n;
      m_tissue.set_params(params);
      m_tissue.create(0, 0);
      m_history_view->discard();
      m_history_view->sample();
      update();
    }
  }
}

void Case::OnJumpTimeButton(wxCommandEvent &)
{
  char buf[100];
  sprintf(buf, "%d", m_tissue.time());
  wxTextEntryDialog dlg(this, "Jump To New Sim Time", "Jump Time", buf);
  if(dlg.ShowModal()==wxID_OK)
  {
    int n = atoi(dlg.GetValue());
    if(n < m_tissue.time())
    {
      m_tissue.create(0, 0);
      m_history_view->discard();
      m_history_view->sample();
      update();
    }
    m_jump_to_time = n;
    run();
  }
}

void Case::OnDualViewButton(wxCommandEvent &)
{
  m_3d_views->toggle_split();
  m_dual_view_button->SetLabel(m_3d_views->is_split() ? "Single View" : "Dual View");
  Show();
}

void Case::OnScreenshotButton(wxCommandEvent &)
{
  wxFileDialog dlg(this, "Save As .BMP In File...", "", "", "", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
  if(wxID_OK == dlg.ShowModal())
  {
    const char * error = m_3d_views->screenshot(dlg.GetPath());
    if(error)
      wxMessageDialog(this, error, "Error Saving Screenshot", wxOK).ShowModal();
  }
}

void Case::OnEditSettingsButton(wxCommandEvent &)
{
  Tissue_Parameters_Dialog dlg(this, m_tissue.params());
  if(dlg.ShowModal())
    m_tissue.set_params(dlg.m_params);
}

void Case::OnEditRedrawFrequency(wxCommandEvent &)
{
  char buf[100];
  sprintf(buf, "%d", m_redraw_frequency);
  wxTextEntryDialog dlg(this, "Redraw the display during a run every n timesteps:", "Redraw Frequency", buf);
  if(dlg.ShowModal()==wxID_OK)
    m_redraw_frequency = atoi(dlg.GetValue());
}
