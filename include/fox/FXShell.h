/********************************************************************************
*                                                                               *
*                     S h e l l   W i n d o w   W i d g e t                     *
*                                                                               *
*********************************************************************************
* Copyright (C) 1997,2002 by Jeroen van der Zijp.   All Rights Reserved.        *
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
* $Id: FXShell.h,v 1.23 2002/01/18 22:42:54 jeroen Exp $                        *
********************************************************************************/
#ifndef FXSHELL_H
#define FXSHELL_H

#ifndef FXCOMPOSITE_H
#include "FXComposite.h"
#endif



/// A child of the Root window
class FXAPI FXShell : public FXComposite {
  FXDECLARE(FXShell)
protected:
  FXShell(){}
  FXShell(FXApp* a,FXuint opts,FXint x,FXint y,FXint w,FXint h);
  FXShell(FXWindow* own,FXuint opts,FXint x,FXint y,FXint w,FXint h);
private:
  FXShell(const FXShell&);
  FXShell &operator=(const FXShell&);
public:
  long onConfigure(FXObject*,FXSelector,void*);
  long onKeyPress(FXObject*,FXSelector,void*);
  long onKeyRelease(FXObject*,FXSelector,void*);
  long onFocusNext(FXObject*,FXSelector,void*);
  long onFocusPrev(FXObject*,FXSelector,void*);
public:

  /// Create server-side resources
  virtual void create();

  /// Mark this window's layout as dirty
  virtual void recalc();

  /// Move the focus to this window
  virtual void setFocus();

  /// Remove the focus from this window
  virtual void killFocus();

  /// Destroy shell
  virtual ~FXShell();
  };


#endif
