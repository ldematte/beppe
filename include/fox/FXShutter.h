/********************************************************************************
*                                                                               *
*                 S h u t t e r   C o n t a i n e r   W i d g e t               *
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
* $Id: FXShutter.h,v 1.15 2002/01/18 22:42:54 jeroen Exp $                      *
********************************************************************************/
#ifndef FXSHUTTER_H
#define FXSHUTTER_H

#ifndef FXVERTICALFRAME_H
#include "FXVerticalFrame.h"
#endif



struct FXTimer;
class FXShutter;
class FXButton;
class FXScrollWindow;
class FXShutterItem;


/// Shutter Control
class FXAPI FXShutterItem : public FXVerticalFrame {
  FXDECLARE(FXShutterItem)
  friend class FXShutter;
protected:
  FXButton         *button;
  FXScrollWindow   *scrollWindow;
  FXVerticalFrame  *content;
protected:
  FXShutterItem(){}
private:
  FXShutterItem(const FXShutterItem&);
  FXShutterItem &operator=(const FXShutterItem&);
public:
  long onFocusUp(FXObject*,FXSelector,void*);
  long onFocusDown(FXObject*,FXSelector,void*);
  long onCmdButton(FXObject*,FXSelector,void*);
public:
  enum{
    ID_SHUTTERITEM_BUTTON=FXVerticalFrame::ID_LAST,
    ID_LAST
    };
public:
  /// Constructor
  FXShutterItem(FXShutter *p,const FXString& text=NULL,FXIcon* icon=NULL,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_SPACING,FXint pr=DEFAULT_SPACING,FXint pt=DEFAULT_SPACING,FXint pb=DEFAULT_SPACING,FXint hs=DEFAULT_SPACING,FXint vs=DEFAULT_SPACING);

  /// Return a pointer to the button for this item
  FXButton* getButton() const { return button; }

  /// Return a pointer to the contents for this item
  FXVerticalFrame* getContent() const { return content; }

  /// Set the status line help text for this item
  void setHelpText(const FXString& text);

  /// Get the status line help text for this item
  FXString getHelpText() const;

  /// Set the tool tip message for this item
  void setTipText(const FXString&  text);

  /// Get the tool tip message for this item
  FXString getTipText() const;

  /// Destructor
  virtual ~FXShutterItem();
  };


/// Shutter Control
class FXAPI FXShutter : public FXVerticalFrame {
  FXDECLARE(FXShutter)
  friend class FXShutterItem;
protected:
  FXint          current;               // Item currently open
  FXint          closing;               // Item closing down
  FXTimer       *timer;                 // Timer for animation
  FXint          heightIncrement;       // Height delta
  FXint          closingHeight;         // Closing items current height
  FXbool         closingHadScrollbar;   // Closing item had a scroll bar
protected:
  FXShutter(){}
  virtual void layout();
private:
  FXShutter(const FXShutter&);
  FXShutter &operator=(const FXShutter&);
public:
  long onFocusUp(FXObject*,FXSelector,void*);
  long onFocusDown(FXObject*,FXSelector,void*);
  long onTimeout(FXObject*,FXSelector,void*);
  long onOpenItem(FXObject*,FXSelector,void*);
  long onCmdSetValue(FXObject*,FXSelector,void*);
  long onCmdSetIntValue(FXObject*,FXSelector,void*);
  long onCmdGetIntValue(FXObject*,FXSelector,void*);
  long onCmdOpen(FXObject*,FXSelector,void*);
  long onUpdOpen(FXObject*,FXSelector,void*);
public:
  enum{
    ID_SHUTTER_TIMEOUT=FXVerticalFrame::ID_LAST,
    ID_OPEN_SHUTTERITEM,
    ID_OPEN_FIRST,
    ID_OPEN_LAST=ID_OPEN_FIRST+1000,
    ID_LAST
    };
public:
  /// Constructor
  FXShutter(FXComposite *p,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_SPACING,FXint pr=DEFAULT_SPACING,FXint pt=DEFAULT_SPACING,FXint pb=DEFAULT_SPACING,FXint hs=DEFAULT_SPACING,FXint vs=DEFAULT_SPACING);

  /// Set the currently displayed item (panel = 0, 1, 2, ..., npanels-1)
  virtual void setCurrent(FXint panel);

  /// Return the index of the currently displayed item
  FXint getCurrent() const { return current; }

  /// Destructor
  virtual ~FXShutter();
  };


#endif
