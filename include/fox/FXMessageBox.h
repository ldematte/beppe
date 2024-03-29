/********************************************************************************
*                                                                               *
*                         M e s s a g e   B o x e s                             *
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
* $Id: FXMessageBox.h,v 1.17 2002/01/18 22:42:54 jeroen Exp $                   *
********************************************************************************/
#ifndef FXMESSAGEBOX_H
#define FXMESSAGEBOX_H

#ifndef FXDIALOGBOX_H
#include "FXDialogBox.h"
#endif



// Message box buttons
enum {
  MBOX_OK               = 0x10000000,   /// Message box has a only an OK button
  MBOX_OK_CANCEL        = 0x20000000,   /// Message box has OK and CANCEL buttons
  MBOX_YES_NO           = 0x30000000,   /// Message box has YES and NO buttons
  MBOX_YES_NO_CANCEL    = 0x40000000,   /// Message box has YES, NO, and CANCEL buttons
  MBOX_QUIT_CANCEL      = 0x50000000,   /// Message box has QUIT and CANCEL buttons
  MBOX_QUIT_SAVE_CANCEL = 0x60000000    /// Message box has QUIT, SAVE, and CANCEL buttons
  };


// Return values
enum {
  MBOX_CLICKED_YES      = 1,            /// The YES button was clicked
  MBOX_CLICKED_NO       = 2,            /// The NO button was clicked
  MBOX_CLICKED_OK       = 3,            /// The OK button was clicked
  MBOX_CLICKED_CANCEL   = 4,            /// The CANCEL button was clicked
  MBOX_CLICKED_QUIT     = 5,            /// The QUIT button was clicked
  MBOX_CLICKED_SAVE     = 6             /// The SAVE button was clicked
  };


/// Message box
class FXAPI FXMessageBox : public FXDialogBox {
  FXDECLARE(FXMessageBox)
protected:
  FXMessageBox(){}
private:
  FXMessageBox(const FXMessageBox&);
  FXMessageBox &operator=(const FXMessageBox&);
  void initialize(const FXString& text,FXIcon* ic,FXuint whichbuttons);
public:
  long onCmdClicked(FXObject*,FXSelector,void*);
  long onCmdCancel(FXObject*,FXSelector,void*);
public:
  enum{
    ID_CLICKED_YES=FXDialogBox::ID_LAST,
    ID_CLICKED_NO,
    ID_CLICKED_OK,
    ID_CLICKED_CANCEL,
    ID_CLICKED_QUIT,
    ID_CLICKED_SAVE,
    ID_LAST
    };
public:

  /// Construct message box with given caption, icon, and message text
  FXMessageBox(FXWindow* owner,const FXString& caption,const FXString& text,FXIcon* ic=NULL,FXuint opts=0,FXint x=0,FXint y=0);

  /// Construct free floating message box with given caption, icon, and message text
  FXMessageBox(FXApp* a,const FXString& caption,const FXString& text,FXIcon* ic=NULL,FXuint opts=0,FXint x=0,FXint y=0);

  /**
  * Show a modal error message.
  * The text message may contain printf-tyle formatting commands.
  */
  static FXuint error(FXWindow* owner,FXuint opts,const char* caption,const char* message,...) FX_PRINTF(4,5) ;

  /**
  * Show a modal warning message
  * The text message may contain printf-tyle formatting commands.
  */
  static FXuint warning(FXWindow* owner,FXuint opts,const char* caption,const char* message,...) FX_PRINTF(4,5) ;

  /**
  * Show a modal question dialog
  * The text message may contain printf-tyle formatting commands.
  */
  static FXuint question(FXWindow* owner,FXuint opts,const char* caption,const char* message,...) FX_PRINTF(4,5) ;

  /**
  * Show a modal information dialog
  * The text message may contain printf-tyle formatting commands.
  */
  static FXuint information(FXWindow* owner,FXuint opts,const char* caption,const char* message,...) FX_PRINTF(4,5) ;

  };



#endif
