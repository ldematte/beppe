/********************************************************************************
*                                                                               *
*                          I C O   I m a g e   O b j e c t                      *
*                                                                               *
*********************************************************************************
* Copyright (C) 2001,2002 by Janusz Ganczarski.   All Rights Reserved.          *
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
* $Id: FXICOImage.h,v 1.5 2002/01/18 22:46:41 jeroen Exp $                      *
********************************************************************************/
#ifndef FXICOIMAGE_H
#define FXICOIMAGE_H

#ifndef FXIMAGE_H
#include "FXImage.h"
#endif


///  ICO graphics file
class FXAPI FXICOImage : public FXImage {
  FXDECLARE(FXICOImage)
protected:
  FXICOImage(){}
private:
  FXICOImage(const FXICOImage&);
  FXICOImage &operator=(const FXICOImage&);
public:

  /// Construct image from memory stream formatted in Microsoft ICO format
  FXICOImage(FXApp* a,const void *pix=NULL,FXuint opts=0,FXint w=1,FXint h=1);

  /// Save pixels into stream in ICO file
  virtual void savePixels(FXStream& store) const;

  /// Load pixels from stream in bitmap format
  virtual void loadPixels(FXStream& store);

  /// Destroy image
  virtual ~FXICOImage();
  };


/// Load a ICO file from a stream
extern FXAPI FXbool fxloadICO(FXStream& store,FXuchar*& data,FXColor& transp,FXint& width,FXint& height);


/// Save a ICO file to a stream
extern FXAPI FXbool fxsaveICO(FXStream& store,const FXuchar *data,FXColor transp,FXint width,FXint height);


#endif