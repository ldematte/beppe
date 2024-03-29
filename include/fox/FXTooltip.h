/********************************************************************************
*                                                                               *
*                           T o o l t i p   W i d g e t                         *
*                                                                               *
*********************************************************************************
* Copyright (C) 1998,2002 by Jeroen van der Zijp.   All Rights Reserved.        *
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
* $Id: FXTooltip.h,v 1.16 2002/01/18 22:42:55 jeroen Exp $                      *
********************************************************************************/
#ifndef FXTOOLTIP_H
#define FXTOOLTIP_H

#ifndef FXSHELL_H
#include "FXShell.h"
#endif



struct FXTimer;
class FXFont;


/// Tooltip styles
enum {
  TOOLTIP_PERMANENT  = 0x00020000,      /// Tooltip stays up indefinitely
  TOOLTIP_VARIABLE   = 0x00040000,      /// Tooltip stays up variable time, depending on the length of the string
  TOOLTIP_NORMAL     = 0                /// Normal tooltip
  };


/// Hopefully Helpful Hint message
class FXAPI FXTooltip : public FXShell {
  FXDECLARE(FXTooltip)
protected:
  FXString  label;
  FXFont   *font;
  FXColor   textColor;
  FXTimer  *timer;
  FXbool    popped;
protected:
  FXTooltip();
  virtual FXbool doesOverrideRedirect() const;
  void place(FXint x,FXint y);
  void autoplace();
private:
  FXTooltip(const FXTooltip&);
  FXTooltip& operator=(const FXTooltip&);
#ifdef WIN32
  virtual const char* GetClass() const;
#endif
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onUpdate(FXObject*,FXSelector,void*);
  long onTipShow(FXObject*,FXSelector,void*);
  long onTipHide(FXObject*,FXSelector,void*);
  long onCmdGetStringValue(FXObject*,FXSelector,void*);
  long onCmdSetStringValue(FXObject*,FXSelector,void*);
public:
  enum {
    ID_TIP_SHOW=FXShell::ID_LAST,
    ID_TIP_HIDE,
    ID_LAST
    };
public:
  /// Construct a tool tip
  FXTooltip(FXApp* a,FXuint opts=TOOLTIP_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Create server-side resources
  virtual void create();

  /// Detach server-side resources
  virtual void detach();

  /// Display the tip
  virtual void show();

  /// Return default width
  virtual FXint getDefaultWidth();

  /// Return default height
  virtual FXint getDefaultHeight();

  /// Set the text for this tip
  void setText(const FXString& text);

  /// Get the text for this tip
  FXString getText() const { return label; }

  /// Set the tip text font
  void setFont(FXFont *fnt);

  /// Get the tip text font
  FXFont* getFont() const { return font; }

  /// Get the current tip text color
  FXColor getTextColor() const { return textColor; }

  /// Set the current tip text color
  void setTextColor(FXColor clr);

  virtual FXbool doesSaveUnder() const;

  /// Save tip to a stream
  virtual void save(FXStream& store) const;

  /// Load tip from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXTooltip();
  };


#endif
