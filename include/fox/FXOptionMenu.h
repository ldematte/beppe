/********************************************************************************
*                                                                               *
*                             O p t i o n   M e n u                             *
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
* $Id: FXOptionMenu.h,v 1.14 2002/01/18 22:42:54 jeroen Exp $                   *
********************************************************************************/
#ifndef FXOPTIONMENU_H
#define FXOPTIONMENU_H

#ifndef FXLABEL_H
#include "FXLabel.h"
#endif



class FXPopup;


/// Option Menu Button
class FXAPI FXOption : public FXLabel {
  FXDECLARE(FXOption)
protected:
  FXOption(){}
private:
  FXOption(const FXOption&);
  FXOption &operator=(const FXOption&);
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onEnter(FXObject*,FXSelector,void*);
  long onLeave(FXObject*,FXSelector,void*);
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  long onKeyPress(FXObject*,FXSelector,void*);
  long onKeyRelease(FXObject*,FXSelector,void*);
  long onHotKeyPress(FXObject*,FXSelector,void*);
  long onHotKeyRelease(FXObject*,FXSelector,void*);
public:

  /// Constructor
  FXOption(FXComposite* p,const FXString& text,FXIcon* ic=NULL,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=JUSTIFY_NORMAL|ICON_BEFORE_TEXT|MENUBUTTON_DOWN,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);

  /// Return default width
  virtual FXint getDefaultWidth();

  /// Return default height
  virtual FXint getDefaultHeight();

  /// Returns true because a menu button can receive focus
  virtual FXbool canFocus() const;

  /// Set focus to this window
  virtual void setFocus();

  /// Remove the focus from this window
  virtual void killFocus();

  /// Destructor
  virtual ~FXOption();
  };



/// Option Menu
class FXAPI FXOptionMenu : public FXLabel {
  FXDECLARE(FXOptionMenu)
protected:
  FXPopup  *pane;
  FXOption *current;
protected:
  FXOptionMenu(){}
  virtual void layout();
private:
  FXOptionMenu(const FXOptionMenu&);
  FXOptionMenu &operator=(const FXOptionMenu&);
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  long onFocusIn(FXObject*,FXSelector,void*);
  long onFocusOut(FXObject*,FXSelector,void*);
  long onMotion(FXObject*,FXSelector,void*);
  long onKeyPress(FXObject*,FXSelector,void*);
  long onKeyRelease(FXObject*,FXSelector,void*);
  long onCmdPost(FXObject*,FXSelector,void*);
  long onCmdUnpost(FXObject*,FXSelector,void*);
  long onQueryHelp(FXObject*,FXSelector,void*);
  long onQueryTip(FXObject*,FXSelector,void*);
  long onCmdSetValue(FXObject*,FXSelector,void*);
  long onCmdSetIntValue(FXObject*,FXSelector,void*);
  long onCmdGetIntValue(FXObject*,FXSelector,void*);
public:

  /// Constructor
  FXOptionMenu(FXComposite* p,FXPopup* pup=NULL,FXuint opts=JUSTIFY_NORMAL|ICON_BEFORE_TEXT,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);

  /// Create server-side resources
  virtual void create();

  /// Detach server-side resources
  virtual void detach();

  /// Delete server-side resources
  virtual void destroy();

  /// Remove the focus from this window
  virtual void killFocus();

  /// Return default width
  virtual FXint getDefaultWidth();

  /// Return default height
  virtual FXint getDefaultHeight();

  /// Return TRUE if the position is logically in the pane
  virtual FXbool contains(FXint parentx,FXint parenty) const;

  /// Set the current option
  void setCurrent(FXOption *win);

  /// Return the current option
  FXOption* getCurrent() const { return current; }

  /// Set the current option number
  void setCurrentNo(FXint no);

  /// Get the current option number
  FXint getCurrentNo() const;

  /// Set the pane which will be popped up
  void setPopup(FXPopup *pup);

  /// Return the pane which is poppup up
  FXPopup* getPopup() const { return pane; }

  /// Returns true because a option menu can receive focus
  virtual FXbool canFocus() const;

  /// Return TRUE if popped up
  FXbool isPopped() const;

  /// Save option menu to a stream
  virtual void save(FXStream& store) const;

  /// Load option menu from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXOptionMenu();
  };


#endif
