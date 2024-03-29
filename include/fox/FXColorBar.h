/********************************************************************************
*                                                                               *
*                         C o l o r B a r   W i d g e t                         *
*                                                                               *
*********************************************************************************
* Copyright (C) 2001,2002 by Jeroen van der Zijp.   All Rights Reserved.        *
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
* $Id: FXColorBar.h,v 1.6 2002/01/18 22:42:51 jeroen Exp $                      *
********************************************************************************/
#ifndef FXCOLORBAR_H
#define FXCOLORBAR_H

#ifndef FXFRAME_H
#include "FXFrame.h"
#endif



/// Color bar orientation
enum {
  COLORBAR_HORIZONTAL = 0,            /// Color bar shown horizontally
  COLORBAR_VERTICAL   = 0x00008000    /// Color bar shown vertically
  };


class FXImage;


/**
* A color bar is a widget which controls the brightness (value) of a
* color by means of the hue, saturation, value specification system.
*/
class FXAPI FXColorBar : public FXFrame {
  FXDECLARE(FXColorBar)
protected:
  FXImage  *bar;          // Intensity bar
  FXfloat   hsv[3];       // Hue, saturation, value
  FXString  tip;          // Tooltip value
  FXString  help;         // Help value
protected:
  FXColorBar();
  void updatebar();
  virtual void layout();
private:
  FXColorBar(const FXColorBar&);
  FXColorBar &operator=(const FXColorBar&);
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  long onMotion(FXObject*,FXSelector,void*);
  long onQueryHelp(FXObject*,FXSelector,void*);
  long onQueryTip(FXObject*,FXSelector,void*);
public:

  /// Construct color well with initial color clr
  FXColorBar(FXComposite* p,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=FRAME_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);

  /// Create server-side resources
  virtual void create();

  /// Detach server-side resources
  virtual void detach();

  /// Return default width
  virtual FXint getDefaultWidth();

  /// Return default height
  virtual FXint getDefaultHeight();

  /// Change hue
  void setHue(FXfloat h);

  /// Return hue
  FXfloat getHue() const { return hsv[0]; }

  /// Change saturation
  void setSat(FXfloat s);

  /// Return saturation
  FXfloat getSat() const { return hsv[1]; }

  /// Change value
  void setVal(FXfloat v);

  /// Return value
  FXfloat getVal() const { return hsv[2]; }

  /// Change the color bar style
  FXuint getBarStyle() const;

  /// Get the color bar style
  void setBarStyle(FXuint style);

  /// Set status line help text for this color well
  void setHelpText(const FXString& text){ help=text; }

  /// Get status line help text for this color well
  FXString getHelpText() const { return help; }

  /// Set tool tip message for this color well
  void setTipText(const FXString& text){ tip=text; }

  /// Get tool tip message for this color well
  FXString getTipText() const { return tip; }

  /// Save color well to a stream
  virtual void save(FXStream& store) const;

  /// Load color well from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXColorBar();
  };


#endif
