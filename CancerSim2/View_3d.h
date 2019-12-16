//
//  View_3d.hpp
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


#ifndef VIEW3DHFILE
#define VIEW3DHFILE

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include "Palette.h"
#include "Multi_Grid_View.h"
#include "Connect_Adjacent_Points.h"
#include "Trackball.h"
#include "Grid_Vertex_Selection.h"
#include "Zoom.h"

class View_3d;
class Tissue;

// defines the callback interface for an owner of a 3d view
class View_3d_Holder
{
public:
  virtual void view_3d_rotation(View_3d * view) { };
};

class View_3d : public wxGLCanvas
{
  DECLARE_EVENT_TABLE()
public:
  View_3d(wxWindow * parent);
  virtual ~View_3d();

  void create(const Tissue *, View_3d_Holder *);

  virtual void render();

  void update();
  void set_palette(const CancerSim::Palette& p) { m_cell_view.set_palette(p); };
  const CancerSim::Palette & palette() const { return m_cell_view.palette(); };
  void copy_view_angle(const View_3d &);

  void OnPaint(wxPaintEvent&);
  void OnSize(wxSizeEvent&);
  void OnEraseBackground(wxEraseEvent&);
  void OnMouseEvent(wxMouseEvent &);

protected:
  const Tissue * m_tissue;
  Multi_Grid_View m_cell_view;
  Connect_Adjacent_Points m_capillary_view;
  Trackball m_trackball;
  Grid_Vertex_Selection m_cell_selection;
  Zoom m_zoom;
  View_3d_Holder * m_holder;
  std::vector<bool> m_cell_visibility;
  int m_mouse_y_offset;
  int m_mouse_x;
  int m_mouse_y;
  int m_width;
  int m_height;
  
  bool some_cells_are_visible() const;
  bool m_first_time;
};

#endif
