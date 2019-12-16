//
//  Dual_3d_View.cpp
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


#include "Dual_3d_View.h"
#include "gl_save_backbuffer_as_bmp.h"

Dual_3d_View::Dual_3d_View(wxWindow * parent)
  : wxSplitterWindow(parent,-1),
    m_a(0),
    m_b(0)
{

}

void Dual_3d_View::create(const Tissue * tissue)
{
  m_tissue = tissue;
  m_a = new View_3d(this);
  m_a->create(m_tissue, this);
  m_a->update();
  Initialize(m_a);
  Layout();
  SetSashPosition(80);
}

void Dual_3d_View::update()
{
  m_a->update();
  if(m_b) m_b->update();
}

void Dual_3d_View::view_3d_rotation(View_3d * view)
{
  if(view == m_a)
  {
    if(m_b)
      m_b->copy_view_angle(*m_a);
  }
  else if(view == m_b)
    m_a->copy_view_angle(*m_b);
  else
    assert(false);
}

bool Dual_3d_View::is_split() const
{
  return m_b;
}

void Dual_3d_View::toggle_split()
{
  if(is_split())
  {
    Unsplit();
    delete m_b;
    m_b = 0;
  }
  else
  {
    m_b = new View_3d(this);
    m_b->create(m_tissue, this);
    m_b->set_palette(m_a->palette());
    SplitVertically(m_a, m_b);
    m_b->update();
    m_b->copy_view_angle(*m_a);
  }
}

void Dual_3d_View::set_palette(const CancerSim::Palette & p)
{
  m_a->set_palette(p);
  if(m_b)
    m_b->set_palette(p);
}

const char * Dual_3d_View::screenshot(const char * filename)
{
  m_a->SetCurrent();
  return gl_save_backbuffer_as_bmp(filename);
}
