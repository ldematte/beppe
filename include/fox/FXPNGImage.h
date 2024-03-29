/********************************************************************************
*                                                                               *
*                          P N G   I m a g e   O b j e c t                      *
*                                                                               *
*********************************************************************************
* Copyright (C) 1999,2002 by Jeroen van der Zijp.   All Rights Reserved.        *
*********************************************************************************
* This library is free software; you can redistribute it and/or                 *
* modify it under the terms of the GNU Lesser General Public                    *
* License as published by the Free Software Foundation; either                  *
* version 2.1 of the License, or (at your option) any later version.            *
*                                                                               *
* This library is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
* Lesser General Public License for more details.                               *
*                                                                               *
* You should have received a copy of the GNU Lesser General Public              *
* License along with this library; if not, write to the Free Software           *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.    *
*********************************************************************************
* $Id: FXPNGImage.h,v 1.8 2002/01/18 22:42:54 jeroen Exp $                      *
********************************************************************************/
#ifndef FXPNGIMAGE_H
#define FXPNGIMAGE_H

#ifndef FXIMAGE_H
#include "FXImage.h"
#endif


/// Portable Network Graphics (PNG) Image class
class FXAPI FXPNGImage : public FXImage {
  FXDECLARE(FXPNGImage)
protected:
  FXPNGImage(){}
private:
  FXPNGImage(const FXPNGImage&);
  FXPNGImage &operator=(const FXPNGImage&);
public:

  /// Construct an image from memory stream formatted in PNG format
  FXPNGImage(FXApp *a,const void *pix=NULL,FXuint opts=0,FXint w=1,FXint h=1);

  /// Save pixels into stream in PNG format
  virtual void loadPixels(FXStream& store);

  /// Load pixels from stream in PNG format
  virtual void savePixels(FXStream& store) const;

  /// Destroy
  virtual ~FXPNGImage();
  };


/// Load a png from a stream
extern FXAPI FXbool fxloadPNG(FXStream& store,FXuchar*& data,FXColor& transp,FXint& width,FXint& height);


/// Save a png to a stream
extern FXAPI FXbool fxsavePNG(FXStream& store,const FXuchar* data,FXColor transp,FXint width,FXint height);


#endif
