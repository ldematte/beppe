/********************************************************************************
*                                                                               *
*                       S t a t u s b a r   W i d g e t                         *
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
* $Id: FXStatusbar.h,v 1.13 2002/01/18 22:42:54 jeroen Exp $                    *
********************************************************************************/
#ifndef FXSTATUSBAR_H
#define FXSTATUSBAR_H

#ifndef FXHORIZONTALFRAME_H
#include "FXHorizontalFrame.h"
#endif



/// Statusbar options
enum {
  STATUSBAR_WITH_DRAGCORNER = 0x00020000    /// Causes the DragCorner to be shown
  };


class FXDragCorner;
class FXStatusline;


/// Status bar
class FXAPI FXStatusbar : public FXHorizontalFrame {
  FXDECLARE(FXStatusbar)
protected:
  FXDragCorner *corner;
  FXStatusline *status;
protected:
  FXStatusbar(){}
  virtual void layout();
private:
  FXStatusbar(const FXStatusbar&);
  FXStatusbar& operator=(const FXStatusbar&);
public:

  /// Construct status bar with or without a drag corner
  FXStatusbar(FXComposite* p,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=3,FXint pr=3,FXint pt=2,FXint pb=2,FXint hs=4,FXint vs=0);

  /// Return default width
  virtual FXint getDefaultWidth();

  /// Return default height
  virtual FXint getDefaultHeight();

  /// Show or hide the drag corner
  void setCornerStyle(FXbool withcorner=TRUE);

  /// Return TRUE if drag corner shown
  FXbool getCornerStyle() const;

  /// Acess the status line widget
  FXStatusline *getStatusline() const { return status; }

  /// Access the drag corner widget
  FXDragCorner *getDragCorner() const { return corner; }

  /// Save status bar to a stream
  virtual void save(FXStream& store) const;

  /// Load status bar from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXStatusbar();
  };


#endif
