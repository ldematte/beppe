/********************************************************************************
*                                                                               *
*                        S p i n   B u t t o n   W i d g e t                    *
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
* $Id: FXSpinner.h,v 1.26 2002/01/18 22:42:54 jeroen Exp $                      *
********************************************************************************/
#ifndef FXSPINNER_H
#define FXSPINNER_H

#ifndef FXPACKER_H
#include "FXPacker.h"
#endif



// Spinner Options
enum {
  SPIN_NORMAL  =  0,                // Normal, non-cyclic
  SPIN_CYCLIC  =  0x00020000,       // Cyclic spinner
  SPIN_NOTEXT  =  0x00040000,       // No text visible
  SPIN_NOMAX   =  0x00080000,       // Spin all the way up to infinity
  SPIN_NOMIN   =  0x00100000        // Spin all the way down to -infinity
  };


class FXTextField;
class FXArrowButton;


/// Spinner control
class FXAPI FXSpinner : public FXPacker {
  FXDECLARE(FXSpinner)
protected:
  FXTextField   *textField;         // Text field
  FXArrowButton *upButton;          // The up button
  FXArrowButton *downButton;        // The down button
  FXint          range[2];          // Reported data range
  FXint          incr;              // Increment
  FXint          pos;               // Current position
protected:
  FXSpinner();
  virtual void layout();
private:
  FXSpinner(const FXSpinner&);
  FXSpinner& operator=(const FXSpinner&);
public:
  long onUpdIncrement(FXObject*,FXSelector,void*);
  long onCmdIncrement(FXObject*,FXSelector,void*);
  long onUpdDecrement(FXObject*,FXSelector,void*);
  long onCmdDecrement(FXObject*,FXSelector,void*);
  long onCmdEntry(FXObject*,FXSelector,void*);
  long onChgEntry(FXObject*,FXSelector,void*);
  long onKeyPress(FXObject*,FXSelector,void*);
  long onKeyRelease(FXObject*,FXSelector,void*);
  long onCmdSetValue(FXObject*,FXSelector,void*);
  long onCmdSetIntValue(FXObject*,FXSelector,void*);
  long onCmdGetIntValue(FXObject*,FXSelector,void*);
  long onCmdSetIntRange(FXObject*,FXSelector,void*);
  long onCmdGetIntRange(FXObject*,FXSelector,void*);
public:
  enum{
    ID_INCREMENT=FXPacker::ID_LAST,
    ID_DECREMENT,
    ID_ENTRY,
    ID_LAST
    };
public:

  /// Construct a spinner
  FXSpinner(FXComposite *p,FXint cols,FXObject *tgt=NULL,FXSelector sel=0,FXuint opts=SPIN_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);

  /// Create server-side resources
  virtual void create();

  /// Disable spinner
  virtual void disable();

  /// Enable spinner
  virtual void enable();

  /// Return default width
  virtual FXint getDefaultWidth();

  /// Return default height
  virtual FXint getDefaultHeight();

  /// Increment spinner
  void increment();

  /// Decrement spinner
  void decrement();

  /// Return TRUE if in cyclic mode
  FXbool isCyclic() const;

  /// Set to cyclic mode, i.e. wrap around at maximum/minimum
  void setCyclic(FXbool cyclic);

  /// Return TRUE if text is visible
  FXbool isTextVisible() const;

  /// Set text visible flag
  void setTextVisible(FXbool shown);

  /// Change current value
  virtual void setValue(FXint value);

  /// Return current value
  FXint getValue() const { return pos; }

  /// Change the spinner's range
  void setRange(FXint lo,FXint hi);

  /// Get the spinner's current range
  void getRange(FXint& lo,FXint& hi) const { lo=range[0]; hi=range[1]; }

  /// Change spinner increment
  void setIncrement(FXint increment);

  /// Return spinner increment
  FXint getIncrement() const { return incr; }

  /// Set the text font
  void setFont(FXFont *fnt);

  /// Get the text font
  FXFont *getFont() const;

  /// Set the status line help text for this spinner
  void setHelpText(const FXString& text);

  /// Get the status line help text for this spinner
  FXString getHelpText() const;

  /// Set the tool tip message for this spinner
  void setTipText(const FXString& text);

  /// Get the tool tip message for this spinner
  FXString getTipText() const;

  /// Change spinner style
  void setSpinnerStyle(FXuint style);

  /// Return current spinner style
  FXuint getSpinnerStyle() const;

  /// Allow editing of the text field
  void setEditable(FXbool edit=TRUE);

  /// Return TRUE if text field is editable
  FXbool isEditable() const;

  /// Save spinner to a stream
  virtual void save(FXStream& store) const;

  /// Load spinner from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXSpinner();
  };

#endif
