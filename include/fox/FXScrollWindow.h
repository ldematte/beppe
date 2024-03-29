/********************************************************************************
*                                                                               *
*                     S c r o l l W i n d o w   W i d g e t                     *
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
* $Id: FXScrollWindow.h,v 1.13 2002/01/18 22:42:54 jeroen Exp $                 *
********************************************************************************/
#ifndef FXSCROLLWINDOW_H
#define FXSCROLLWINDOW_H

#ifndef FXSCROLLAREA_H
#include "FXScrollArea.h"
#endif



/**
* The scroll window widget scrolls an arbitrary child window.
* Use the scroll window when parts of the user interface itself
* need to be scrolled, for example when applications need to run
* on small screens.
*/
class FXAPI FXScrollWindow : public FXScrollArea {
  FXDECLARE(FXScrollWindow)
protected:
  FXScrollWindow(){}
  virtual void layout();
private:
  FXScrollWindow(const FXScrollWindow&);
  FXScrollWindow &operator=(const FXScrollWindow&);
public:
  long onKeyPress(FXObject*,FXSelector,void*);
  long onKeyRelease(FXObject*,FXSelector,void*);
  long onFocusSelf(FXObject*,FXSelector,void*);
public:

  /// Construct a scroll window
  FXScrollWindow(FXComposite* p,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Return a pointer to the contents window
  FXWindow* contentWindow() const;

  /// Return the width of the contents
  virtual FXint getContentWidth();

  /// Return the height of the contents
  virtual FXint getContentHeight();

  /// Move contents to the specified position
  virtual void moveContents(FXint x,FXint y);
  };


#endif
