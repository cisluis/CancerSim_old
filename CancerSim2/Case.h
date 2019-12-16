//
//  Case.h
//  CancerSim2
//
//  Created by Luis Cisneros on 12/13/19.
//  Copyright © 2019 Arizona Cancer Evolution Center. All rights reserved.
//
//     Case.h
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


#ifndef CASEHFILE
#define CASEHFILE

#include "wx/wx.h"
#include "Dual_3d_View.h"
#include "History_View.h"
#include "Tissue.h"
#include "wx/frame.h"
#include "wx/button.h"

class Case;

class Case_Holder
{
public:
  virtual void case_holder_duplicate(const Case *) = 0;
};

class Case : public wxFrame
{
  DECLARE_EVENT_TABLE()
public:

  Case(Case_Holder *, const wxString& title, const wxPoint& pos, const wxSize& size);
  virtual ~Case();

  void create(int argc, char * argv[]);
  void set_palette(const CancerSim::Palette &);
  
  //void OnSashPositionChange(int newSashPosition);
  void OnSashPositionChange(wxSplitterEvent&);
  bool Show(bool show = TRUE);
  void OnSize(wxSizeEvent&);
  void OnIdle(wxIdleEvent&);
  void OnRunButton(wxCommandEvent &);
  void OnSingleStepButton(wxCommandEvent &);
  void OnDuplicateButton(wxCommandEvent &);
  void OnResetButton(wxCommandEvent &);
  void OnJumpTimeButton(wxCommandEvent &);
  void OnDualViewButton(wxCommandEvent &);
  void OnScreenshotButton(wxCommandEvent &);
  void OnEditSettingsButton(wxCommandEvent &);
  void OnEditRedrawFrequency(wxCommandEvent &);
  void clone(Case *) const;

private:
  void run();
  void update();

  Case_Holder * m_holder;
  Tissue m_tissue;
  Dual_3d_View * m_3d_views;
  History_View * m_history_view;
  wxButton * m_run_button;
  wxButton * m_single_step_button;
  wxButton * m_duplicate_button;
  wxButton * m_reset_button;
  wxButton * m_jump_time_button;
  wxButton * m_dual_view_button;
  wxButton * m_screenshot_button;
  wxButton * m_redraw_frequency_button;
  bool m_running;
  int m_jump_to_time;
  int m_redraw_frequency;
};

#endif
