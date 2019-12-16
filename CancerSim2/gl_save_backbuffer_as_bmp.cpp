//
//  gl_save_backbuffer_as_bmp.cpp
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


#ifdef __WXMAC__
#     include <OpenGL/gl.h>
#else
#     include <GL/gl.h>
#endif

#include <fstream>
#include <sys/types.h>
#include <vector>
#include <iostream>

struct BITMAPFILEHEADER
{
  char bfType[2];
  int32_t bfSize;
  int16_t bfReserved1;
  int16_t bfReserved2;
  int32_t bfOffBits;
};
 
struct BITMAPINFOHEADER
{
  int32_t biSize;
  int32_t biWidth;
  int32_t biHeight;
  int16_t biPlanes;
  int16_t biBitCount;
  int32_t biCompression;
  int32_t biSizeImage;
  int32_t biXPelsPerMeter;
  int32_t biYPelsPerMeter;
  int32_t biClrUsed;
  int32_t biClrImportant;
};


// The original c code was posted to usenet comp.graphics.api.opengl
// by Jongwhan Lee (leejw51@hitel.net) on 2000/06/12

const char * gl_save_backbuffer_as_bmp(const char* filename)
{
  glReadBuffer(GL_BACK); // designate buffer to read

  int vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);

  std::cout << "width: " << vp[2] << std::endl;

  const int width=vp[2];
  const int height=vp[3];
  const int width_bytes = ((width*3-1)/4+1)*4;
  const int size=width_bytes*height;

  std::vector<char> buffer(size);

  glReadPixels(0,0, width, height, GL_RGB, GL_UNSIGNED_BYTE, &buffer[0]);

  // swap Red, Blue values..
  for(int row = 0; row < height; ++row)
  {
    char * begin = &buffer[0]+row*width_bytes;
    char * end = begin + width*3;
    for(; begin != end; begin += 3)
      std::swap(begin[0], begin[2]);
  }

  std::fstream file(filename, std::ios::out);
  if(file.fail())
    return "Failed to open file for output";

  // write header

  BITMAPINFOHEADER bi;
  // set up info header
  bi.biSize  = sizeof(bi);
  bi.biWidth  = width;
  bi.biHeight   = height;
  bi.biPlanes   = 1;
  bi.biBitCount  = 24;
  bi.biCompression = 0;
  bi.biSizeImage  = 0;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrUsed  = 0;
  bi.biClrImportant = 0;
  
  // set up file header
  BITMAPFILEHEADER hdr;
  hdr.bfType[0]='B';
  hdr.bfType[1]='M';
  hdr.bfSize  =  14 + sizeof(bi) + size;
  hdr.bfReserved1  = 0;
  hdr.bfReserved2  = 0;
  hdr.bfOffBits  = 14 + bi.biSize;
  
  // write to disk
  file.write((char*)&hdr, 2);
  file.write((char*)&hdr.bfSize, 12);
  file.write((char*)&bi, sizeof(bi));
  file.write((char*)&buffer[0], buffer.size());

  if(file.fail())
    return "Failed to write the file";

  return 0;
}

//  #include <iostream>
//  void width_bytes(int width)
//  {
//    cout << width << ' ' << (((width-1)/4+1)*4) << ' ' << (width*3%4 ? (width*3/8+1)*8 : width*3) << endl;
//  }

//  int main(int argc, char * argv[])
//  {
//    for(int i=0; i < 50; ++i)
//      width_bytes(i);
//  }
