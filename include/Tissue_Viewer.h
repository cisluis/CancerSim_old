//
//  Tissue_Viewer.hpp
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


#ifndef TISSUEVIEWERHFILE
#define TISSUEVIEWERHFILE

#include "View_3d.h"
#include "History_View.h"
#include "GLUI_Window.h"
#include "Tissue.h"

class Tissue_Viewer;

class Tissue_Viewer_Holder
{
public:
  virtual const CancerSim::Palette & tissue_viewer_palette() = 0;
  virtual void tissue_viewer_clone(const Tissue_Viewer *) = 0;
  virtual void tissue_viewer_close(Tissue_Viewer *) = 0;
};

class Tissue_Viewer : public GLUI_Window, public View_3d_Holder, public History_View_Holder
{
public:

  Tissue_Viewer();
  ~Tissue_Viewer();
  void create(Tissue_Viewer_Holder *);

  // command handlers
  void render();
  void run();
  void update();

  void reset();

  // implement the GLUI_Window interface
  virtual void button_callback(GLUI_Button*);
  virtual void idle_callback();

  // implement the View_3d_Holder interface
  virtual void view_3d_rotation(View_3d *);

  // make the parameter a copy of *this
  void clone(Tissue_Viewer *) const;
  
protected:

  // enables and disables controls as necessary.  This is
  // called by update() but ocasionally called separately.
  void enable_controls();

  void jump_to_time();

  bool m_running;

  // the GLUI library has an irritating habit of causing extra
  // redraws and momentarily enabling disabled controls when
  // they are repeatedly disabled (or enabled).  Furthermore,
  // the api does not support querying a control to determine
  // whether it is enabled.  This map is used to keep track.
  // It is accessed *only* by the 'enable' function below.
  typedef std::map<GLUI_Control*, bool> Enable_Map;
  Enable_Map m_enable;
  
  // enable (true) or disable (false) the control.  If the
  // control wasn't already in the map, it is added.
  void enable(GLUI_Control*, bool);

  GLUI_Button * m_run_button;
  GLUI_Button * m_reset_button;
  GLUI_Button * m_close_button;
  GLUI_Button * m_view_3d_button;
  GLUI_Button * m_clone_button;

  GLUI_EditText * m_max_cells;

  GLUI_Button * m_jump_time_button;
  GLUI_EditText * m_time_edit;
  int m_jump_to_time;

  std::vector<GLUI_EditText*> m_mutation_significance;

  // the number of events processed since the
  // last update.
  unsigned int m_outdated;

  Tissue m_tissue;

  void add_view_3d();
  typedef std::vector<View_3d*> View_3ds;
  View_3ds m_view_3d;

  History_View m_history_view;

  Tissue_Viewer_Holder * m_holder;
};

#endif
