//
//  Tissue_Viewer.cpp
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


#include <glui.h>
#include "Tissue_Viewer.h"
#include <memory>

Tissue_Viewer::Tissue_Viewer() :
  m_running(false),
  m_run_button(0),
  m_reset_button(0),
  m_close_button(0),
  m_view_3d_button(0),
  m_clone_button(0),
  m_outdated(0),
  m_holder(0),
  m_jump_time_button(0),
  m_time_edit(0),
  m_jump_to_time(0)
{

}

Tissue_Viewer::~Tissue_Viewer()
{
  for(View_3ds::iterator i = m_view_3d.begin(); i != m_view_3d.end(); ++i)
    delete *i;
}

void Tissue_Viewer::create(Tissue_Viewer_Holder * holder)
{
  m_holder = holder;

  //// create glui windows
  GLUI_Window::create("CancerGrid");
  m_run_button = add_button("Run");
  m_run_button->set_alignment(GLUI_ALIGN_LEFT);

  m_clone_button = add_button("Duplicate");
  m_clone_button->set_alignment(GLUI_ALIGN_LEFT);

  m_reset_button = add_button("Reset");
  m_reset_button->set_alignment(GLUI_ALIGN_LEFT);

  m_close_button = add_button("Close");
  m_close_button->set_alignment(GLUI_ALIGN_LEFT);

  m_max_cells = m_window->add_edittext("Max Cells", GLUI_EDITTEXT_INT);
  m_max_cells->set_alignment(GLUI_ALIGN_LEFT);
  m_max_cells->set_int_val(100000);
  m_max_cells->set_int_limits(10, 100000000);

  GLUI_Panel * mutation_significance = m_window->add_panel("Mutation Significance");
  for(int i=0; i < m_tissue.num_mutation_types(); ++i)
  {
    const Tissue::Mutation m = m_tissue.ith_mutation_type(i);
    GLUI_EditText * et = m_window->add_edittext_to_panel(mutation_significance, (char*)m_tissue.mutation_name(m), GLUI_EDITTEXT_INT);
    et->set_alignment(GLUI_ALIGN_RIGHT);
    et->set_int_val(m_tissue.significance(m));
    m_mutation_significance.push_back(et);
  }

  GLUI_Panel * time_panel = m_window->add_panel("Time");
  time_panel->set_alignment(GLUI_ALIGN_LEFT);
  m_time_edit = m_window->add_edittext_to_panel(time_panel, "Sim Time", GLUI_EDITTEXT_INT);
  m_time_edit->set_alignment(GLUI_ALIGN_LEFT);
  m_time_edit->set_int_limits(0, 100000000);
  m_jump_time_button = add_button_to_panel(time_panel, "Jump");
  m_jump_time_button->set_alignment(GLUI_ALIGN_LEFT);

  m_view_3d_button = add_button("3d View");
  m_view_3d_button->set_alignment(GLUI_ALIGN_LEFT);

  m_history_view.create(&m_tissue, this, 300, 100, "History");
  m_history_view.set_palette(m_holder->tissue_viewer_palette());

  reset();

  add_view_3d();

  update();
};



void Tissue_Viewer::button_callback(GLUI_Button * btn)
{
  if(btn == m_run_button)
    run();
  else if(btn == m_reset_button)
  {
    reset();
    update();
  }
  else if(btn == m_view_3d_button)
    add_view_3d();
  else if(btn == m_jump_time_button)
    jump_to_time();
  else if(btn == m_clone_button)
    m_holder->tissue_viewer_clone(this);
  else if(btn == m_close_button)
    m_holder->tissue_viewer_close(this);
}

void Tissue_Viewer::reset()
{
  if(m_running)
    run();
  assert(!m_running);

  int max_cells = m_max_cells->get_int_val();
  max_cells = (int) pow(max_cells+1, 1/3.0);

  m_tissue.create(max_cells, max_cells, max_cells);
  m_history_view.discard();
  m_history_view.sample();
}

void Tissue_Viewer::enable(GLUI_Control * ctl, bool b)
{
  Enable_Map::iterator iter = m_enable.find(ctl);
  
  if(iter == m_enable.end())
    iter = m_enable.insert(Enable_Map::value_type(ctl,!b)).first;
  
  if(iter->second != b)
  {
    iter->second = b;
    if(b)
      ctl->enable();
    else
      ctl->disable();
  }
  else
    ; // the point of this function is *not* to take action unless necessary.
}

void Tissue_Viewer::enable_controls()
{
  enable(m_max_cells, !m_running);
  for(int i=0; i < m_mutation_significance.size(); ++i)
    enable(m_mutation_significance[i], !m_running);
  enable(m_time_edit, !m_running);
  enable(m_jump_time_button, !m_running);
  enable(m_clone_button, !m_running);
}

void Tissue_Viewer::update()
{
  enable_controls();
  m_outdated=0;
  for(int i=0; i < m_view_3d.size(); ++i)
    m_view_3d[i]->update();
  
  m_history_view.set_window();
  m_history_view.render();

  m_time_edit->set_int_val(m_tissue.time());
}

void Tissue_Viewer::run()
{
  if(!m_running)
  {
    for(int i=0; i < m_tissue.num_mutation_types(); ++i)
      m_tissue.set_significance(m_tissue.ith_mutation_type(i), m_mutation_significance[i]->get_int_val());
    int max_cells = m_max_cells->get_int_val();
    max_cells = (int) pow(max_cells+1, 1/3.0);
    if(max_cells != m_tissue.ni() ||
       max_cells != m_tissue.nj() ||
       max_cells != m_tissue.nk() )
    {
      reset();
      update();
    }
    m_run_button->set_name("Stop");
    enable_idle_callback();
    m_running = true;
    enable_controls();
  }
  else
  {
    m_jump_to_time = 0;
    m_run_button->set_name("Run");
    disable_idle_callback();
    m_running = false;
    update();
  }
}

void Tissue_Viewer::idle_callback()
{
  int i;

  if(m_running && m_jump_to_time && m_jump_to_time == m_tissue.time())
  {
    run(); // we've reached the time; this toggles (stops) the run.
    return;
  }

  if(m_running)
  {
    if(m_tissue.run_to_time(m_tissue.time()+1) > 0)
      m_history_view.sample();
    if(m_tissue.time() % 100 == 0 && m_jump_to_time == 0)
      update();
  }
}

void Tissue_Viewer::view_3d_rotation(View_3d * view)
{
  for(View_3ds::iterator i=m_view_3d.begin(); i != m_view_3d.end(); ++i)
    if(*i != view)
      (*i)->copy_view_angle(*view);
}

void Tissue_Viewer::add_view_3d()
{
  View_3d * v = new View_3d();
  v->create(&m_tissue, this, 300,300,"3d View");
  v->set_palette(m_holder->tissue_viewer_palette());
  v->update();
  if(m_view_3d.size())
    v->copy_view_angle(*m_view_3d[0]);
  m_view_3d.push_back(v);
}

void Tissue_Viewer::jump_to_time()
{
  int time = m_time_edit->get_int_val();

  // since the simulation can only run forwards,
  // we must ensure the time is less than or
  // equal to the jump time.
  if(m_tissue.time() > time)
    reset();

  m_jump_to_time = time;
  run();
}

void Tissue_Viewer::clone(Tissue_Viewer * tv) const
{
  m_tissue.clone(&tv->m_tissue);
  m_history_view.clone(&tv->m_history_view);
}
