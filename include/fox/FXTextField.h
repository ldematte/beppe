/********************************************************************************
*                                                                               *
*                         T e x t   F i e l d   W i d g e t                     *
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
* $Id: FXTextField.h,v 1.34 2002/01/18 22:42:55 jeroen Exp $                    *
********************************************************************************/
#ifndef FXTEXTFIELD_H
#define FXTEXTFIELD_H

#ifndef FXFRAME_H
#include "FXFrame.h"
#endif



struct FXTimer;


/// Textfield styles
enum {
  TEXTFIELD_PASSWD     = 0x00800000,      /// Password mode
  TEXTFIELD_INTEGER    = 0x01000000,      /// Integer mode
  TEXTFIELD_REAL       = 0x02000000,      /// Real mode
  TEXTFIELD_READONLY   = 0x04000000,      /// NOT editable
  TEXTFIELD_ENTER_ONLY = 0x08000000,      /// Only callback when enter hit
  TEXTFIELD_LIMITED    = 0x10000000,      /// Limit entry to given number of columns
  TEXTFIELD_OVERSTRIKE = 0x20000000,      /// Overstrike mode
  TEXTFIELD_NORMAL     = FRAME_SUNKEN|FRAME_THICK
  };


/**
* A text field is a single-line text entry widget.
* The text field widget supports clipboard for cut-and-paste
* operations.
* Text input may be constrained to a certain format; the built-in
* capabilities support integer and real number entry constraints;
* additional constraints on the input may be implemented by intercepting
* the SEL_VERIFY message; a custom handler should examine the tentative
* input string passed as type const FXchar* in the message data, and return
* a value of "0" if the new input is accepted.
* During text entry, the text field sends a SEL_CHANGED message to its target,
* with the message data set to the current text value of type const FXchar*.
* When the text is accepted by hitting ENTER, the SEL_COMMAND message is sent.
*/
class FXAPI FXTextField : public FXFrame {
  FXDECLARE(FXTextField)
protected:
  FXString     contents;                  // Edited text
  FXFont      *font;                      // Text font
  FXColor      textColor;                 // Text color
  FXColor      selbackColor;              // Selected background color
  FXColor      seltextColor;              // Selected text color
  FXint        cursor;                    // Cursor position
  FXint        anchor;                    // Anchor position
  FXTimer     *blinker;                   // Blink timer
  FXint        columns;                   // Number of columns visible
  FXint        shift;                     // Shift amount
  FXString     clipped;                   // Clipped text
  FXString     help;                      // Help string
  FXString     tip;                       // Tooltip
protected:
  FXTextField();
  FXint index(FXint x) const;
  FXint coord(FXint i) const;
  virtual void layout();
  void drawCursor(FXuint state);
  void drawTextRange(FXDCWindow& dc,FXint fm,FXint to);
  void drawTextFragment(FXDCWindow& dc,FXint x,FXint y,FXint fm,FXint to);
  void drawPWDTextFragment(FXDCWindow& dc,FXint x,FXint y,FXint fm,FXint to);
private:
  FXTextField(const FXTextField&);
  FXTextField& operator=(const FXTextField&);
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onKeyPress(FXObject*,FXSelector,void*);
  long onKeyRelease(FXObject*,FXSelector,void*);
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  long onMiddleBtnPress(FXObject*,FXSelector,void*);
  long onMiddleBtnRelease(FXObject*,FXSelector,void*);
  long onVerify(FXObject*,FXSelector,void*);
  long onMotion(FXObject*,FXSelector,void*);
  long onSelectionLost(FXObject*,FXSelector,void*);
  long onSelectionGained(FXObject*,FXSelector,void*);
  long onSelectionRequest(FXObject*,FXSelector,void* ptr);
  long onClipboardLost(FXObject*,FXSelector,void*);
  long onClipboardGained(FXObject*,FXSelector,void*);
  long onClipboardRequest(FXObject*,FXSelector,void*);
  long onFocusSelf(FXObject*,FXSelector,void*);
  long onFocusIn(FXObject*,FXSelector,void*);
  long onFocusOut(FXObject*,FXSelector,void*);
  long onBlink(FXObject*,FXSelector,void*);
  long onAutoScroll(FXObject*,FXSelector,void*);
  long onQueryHelp(FXObject*,FXSelector,void*);
  long onQueryTip(FXObject*,FXSelector,void*);
  long onCmdSetValue(FXObject*,FXSelector,void*);
  long onCmdSetIntValue(FXObject*,FXSelector,void*);
  long onCmdSetRealValue(FXObject*,FXSelector,void*);
  long onCmdSetStringValue(FXObject*,FXSelector,void*);
  long onCmdGetIntValue(FXObject*,FXSelector,void*);
  long onCmdGetRealValue(FXObject*,FXSelector,void*);
  long onCmdGetStringValue(FXObject*,FXSelector,void*);
  long onCmdCursorHome(FXObject*,FXSelector,void*);
  long onCmdCursorEnd(FXObject*,FXSelector,void*);
  long onCmdCursorRight(FXObject*,FXSelector,void*);
  long onCmdCursorLeft(FXObject*,FXSelector,void*);
  long onCmdMark(FXObject*,FXSelector,void*);
  long onCmdExtend(FXObject*,FXSelector,void*);
  long onCmdSelectAll(FXObject*,FXSelector,void*);
  long onCmdDeselectAll(FXObject*,FXSelector,void*);
  long onCmdCutSel(FXObject*,FXSelector,void*);
  long onCmdCopySel(FXObject*,FXSelector,void*);
  long onCmdPasteSel(FXObject*,FXSelector,void*);
  long onCmdDeleteSel(FXObject*,FXSelector,void*);
  long onCmdOverstString(FXObject*,FXSelector,void*);
  long onCmdInsertString(FXObject*,FXSelector,void*);
  long onCmdBackspace(FXObject*,FXSelector,void*);
  long onCmdDelete(FXObject*,FXSelector,void*);
  long onCmdToggleEditable(FXObject*,FXSelector,void*);
  long onUpdToggleEditable(FXObject*,FXSelector,void*);
  long onCmdToggleOverstrike(FXObject*,FXSelector,void*);
  long onUpdToggleOverstrike(FXObject*,FXSelector,void*);
public:
  enum{
    ID_CURSOR_HOME=FXFrame::ID_LAST,
    ID_CURSOR_END,
    ID_CURSOR_RIGHT,
    ID_CURSOR_LEFT,
    ID_MARK,
    ID_EXTEND,
    ID_SELECT_ALL,
    ID_DESELECT_ALL,
    ID_CUT_SEL,
    ID_COPY_SEL,
    ID_PASTE_SEL,
    ID_DELETE_SEL,
    ID_OVERST_STRING,
    ID_INSERT_STRING,
    ID_BACKSPACE,
    ID_DELETE,
    ID_TOGGLE_EDITABLE,
    ID_TOGGLE_OVERSTRIKE,
    ID_BLINK,
    ID_LAST
    };
public:

  /// Construct text field wide enough to display ncols columns
  FXTextField(FXComposite* p,FXint ncols,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=TEXTFIELD_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);

  /// Create server-side resources
  virtual void create();

  /// Enable text field
  virtual void enable();

  /// Disable text field
  virtual void disable();

  /// Return default width
  virtual FXint getDefaultWidth();

  /// Return default height
  virtual FXint getDefaultHeight();

  /// Yes, text field may receive focus
  virtual FXbool canFocus() const;

  /// Move the focus to this window
  virtual void setFocus();

  /// Remove the focus from this window
  virtual void killFocus();

  /// Return TRUE if text field may be edited
  FXbool isEditable() const;

  /// Change text field editability
  void setEditable(FXbool edit=TRUE);

  /// Set cursor position
  void setCursorPos(FXint pos);

  /// Return cursor position
  FXint getCursorPos() const { return cursor; }

  /// Change anchor position
  void setAnchorPos(FXint pos);

  /// Return anchor position
  FXint getAnchorPos() const { return anchor; }

  /// Set the text for this label
  void setText(const FXString& text);

  /// Get the text for this label
  FXString getText() const { return contents; }

  /// Set the text font
  void setFont(FXFont* fnt);

  /// Get the text font
  FXFont* getFont() const { return font; }

  /// Change text color
  void setTextColor(FXColor clr);

  /// Return text color
  FXColor getTextColor() const { return textColor; }

  /// Change selected background color
  void setSelBackColor(FXColor clr);

  /// Return selected background color
  FXColor getSelBackColor() const { return selbackColor; }

  /// Change selected text color
  void setSelTextColor(FXColor clr);

  /// Return selected text color
  FXColor getSelTextColor() const { return seltextColor; }

  /// Change width of text field in terms of number of columns * `m'
  void setNumColumns(FXint cols);

  /// Return number of columns
  FXint getNumColumns() const { return columns; }

  /// Change text justification mode
  void setJustify(FXuint mode);

  /// Return text justification mode
  FXuint getJustify() const;

  /// Set the status line help text for this label
  void setHelpText(const FXString& text);

  /// Get the status line help text for this label
  FXString getHelpText() const { return help; }

  /// Set the tool tip message for this text field
  void setTipText(const FXString& text);

  /// Get the tool tip message for this text field
  FXString getTipText() const { return tip; }

  /// Change text style
  void setTextStyle(FXuint style);

  /// Return text style
  FXuint getTextStyle() const;

  /// Select all text
  FXbool selectAll();

  /// Select len characters starting at given position pos
  FXbool setSelection(FXint pos,FXint len);

  /// Extend the selection from the anchor to the given position
  FXbool extendSelection(FXint pos);

  /// Unselect the text
  FXbool killSelection();

  /// Return TRUE if position pos is selected
  FXbool isPosSelected(FXint pos) const;

  /// Return TRUE if position is fully visible
  FXbool isPosVisible(FXint pos) const;

  /// Scroll text to make the given position visible
  void makePositionVisible(FXint pos);

  /// Save text field to a stream
  virtual void save(FXStream& store) const;

  /// Load text field from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXTextField();
  };


#endif
