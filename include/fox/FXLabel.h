/********************************************************************************
*                                                                               *
*                            L a b e l   W i d g e t                            *
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
* $Id: FXLabel.h,v 1.15 2002/01/18 22:42:53 jeroen Exp $                        *
********************************************************************************/
#ifndef FXLABEL_H
#define FXLABEL_H

#ifndef FXFRAME_H
#include "FXFrame.h"
#endif



/// Justification modes
enum {
  JUSTIFY_NORMAL       = 0,			      /// Default justification is centered text
  JUSTIFY_CENTER_X     = 0,			      /// Text is centered horizontally
  JUSTIFY_LEFT         = 0x00008000,		      /// Text is left-justified
  JUSTIFY_RIGHT        = 0x00010000,		      /// Text is right-justified
  JUSTIFY_HZ_APART     = JUSTIFY_LEFT|JUSTIFY_RIGHT,  /// Combination of JUSTIFY_LEFT & JUSTIFY_RIGHT
  JUSTIFY_CENTER_Y     = 0,			      /// Text is centered vertically
  JUSTIFY_TOP          = 0x00020000,		      /// Text is aligned with label top
  JUSTIFY_BOTTOM       = 0x00040000,		      /// Text is aligned with label bottom
  JUSTIFY_VT_APART     = JUSTIFY_TOP|JUSTIFY_BOTTOM   /// Combination of JUSTIFY_TOP & JUSTIFY_BOTTOM
  };



/// Relationship options for icon-labels
enum {
  ICON_UNDER_TEXT      = 0,			      /// Icon appears under text
  ICON_AFTER_TEXT      = 0x00080000,		      /// Icon appears after text (to its right)
  ICON_BEFORE_TEXT     = 0x00100000,		      /// Icon appears before text (to its left)
  ICON_ABOVE_TEXT      = 0x00200000,		      /// Icon appears above text
  ICON_BELOW_TEXT      = 0x00400000,		      /// Icon appears below text
  TEXT_OVER_ICON       = ICON_UNDER_TEXT,	      /// Same as ICON_UNDER_TEXT
  TEXT_AFTER_ICON      = ICON_BEFORE_TEXT,	      /// Same as ICON_BEFORE_TEXT
  TEXT_BEFORE_ICON     = ICON_AFTER_TEXT,	      /// Same as ICON_AFTER_TEXT
  TEXT_ABOVE_ICON      = ICON_BELOW_TEXT,	      /// Same as ICON_BELOW_TEXT
  TEXT_BELOW_ICON      = ICON_ABOVE_TEXT	      /// Same as ICON_ABOVE_TEXT
  };


/// Normal way to show label
enum {
  LABEL_NORMAL         = JUSTIFY_NORMAL|ICON_BEFORE_TEXT
  };


class FXIcon;
class FXFont;


/**
* A label widget can be used to place a text and/or icon for
* explanation purposes.  The text label may have an optional tooltip
* and/or help string.
*/
class FXAPI FXLabel : public FXFrame {
  FXDECLARE(FXLabel)
protected:
  FXString label;
  FXIcon*  icon;
  FXFont*  font;
  FXHotKey hotkey;
  FXint    hotoff;
  FXColor  textColor;
  FXString tip;
  FXString help;
protected:
  FXLabel();
  FXint labelHeight(const FXString& text) const;
  FXint labelWidth(const FXString& text) const;
  void drawLabel(FXDCWindow& dc,const FXString& text,FXint hot,FXint tx,FXint ty,FXint tw,FXint th);
  void just_x(FXint& tx,FXint& ix,FXint tw,FXint iw);
  void just_y(FXint& ty,FXint& iy,FXint th,FXint ih);
private:
  FXLabel(const FXLabel&);
  FXLabel &operator=(const FXLabel&);
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onHotKeyPress(FXObject*,FXSelector,void*);
  long onHotKeyRelease(FXObject*,FXSelector,void*);
  long onCmdGetStringValue(FXObject*,FXSelector,void*);
  long onCmdSetStringValue(FXObject*,FXSelector,void*);
  long onQueryHelp(FXObject*,FXSelector,void*);
  long onQueryTip(FXObject*,FXSelector,void*);
public:

  /// Construct label with given text and icon
  FXLabel(FXComposite* p,const FXString& text,FXIcon* ic=0,FXuint opts=LABEL_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);

  /// Create server-side resources
  virtual void create();

  /// Detach server-side resources
  virtual void detach();

  /// Enable the window
  virtual void enable();

  /// Disable the window
  virtual void disable();

  /// Return default width
  virtual FXint getDefaultWidth();

  /// Return default height
  virtual FXint getDefaultHeight();

  /// Set the text for this label
  void setText(const FXString& text);

  /// Get the text for this label
  FXString getText() const { return label; }

  /// Set the icon for this label
  void setIcon(FXIcon* ic);

  /// Get the icon for this label
  FXIcon* getIcon() const { return icon; }

  /// Set the text font
  void setFont(FXFont *fnt);

  /// Get the text font
  FXFont* getFont() const { return font; }

  /// Get the current text color
  FXColor getTextColor() const { return textColor; }

  /// Set the current text color
  void setTextColor(FXColor clr);

  /// Set the current text-justification mode.
  void setJustify(FXuint mode);

  /// Get the current text-justification mode.
  FXuint getJustify() const;

  /// Set the current icon position
  void setIconPosition(FXuint mode);

  /// Get the current icon position
  FXuint getIconPosition() const;

  /// Set the status line help text for this label
  void setHelpText(const FXString& text);

  /// Get the status line help text for this label
  FXString getHelpText() const { return help; }

  /// Set the tool tip message for this label
  void setTipText(const FXString&  text);

  /// Get the tool tip message for this label
  FXString getTipText() const { return tip; }

  /// Save label to a stream
  virtual void save(FXStream& store) const;

  /// Load label from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXLabel();
  };



#endif
