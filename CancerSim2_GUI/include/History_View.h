//
//  History_View.h
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


#ifndef HISTORYVIEWHFILE
#define HISTORYVIEWHFILE

#include "wx/wx.h"
#include "glcanvas.h"
#include "Tissue.h"
#include "Tissue_Genotype_History.h"
#include "Palette.h"
#include <math.h>

class History_View : public wxGLCanvas
{
  DECLARE_EVENT_TABLE()
public:
  History_View(wxWindow *);
  virtual ~History_View();
  void create(const Tissue *);
  virtual void render();
  void sample(); // take a sample of the Tissue passed into create()
  const Tissue_Genotype_History & history() const { return m_history; };
  void set_palette(const CancerSim::Palette & p);
  void discard(); // discard all data, starting fresh.
  void clone(History_View *) const; // make the parameter the same as *this

  void OnPaint(wxPaintEvent&);
  void OnSize(wxSizeEvent&);
  void OnEraseBackground(wxEraseEvent&);
  void OnMouseEvent(wxMouseEvent &);

protected:
  double map(int i) const { return m_log ? (i ? log(i) : 0) : i; };
  const Tissue * m_tissue;
  Tissue_Genotype_History m_history;
  CancerSim::Palette m_palette;
  bool m_log;

  void update_marker(int x, int y);
  Tissue::Time m_marker_time;
  Tissue::Genotype m_marker_genotype;
  int m_marker_count;

  bool m_lbutton_down;

  int m_border; // number of pixels around the edge not used.
  int m_height, m_width; // of the viewport.
};

#endif

