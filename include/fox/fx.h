/********************************************************************************
*                                                                               *
*                   M a i n   F O X   I n c l u d e   F i l e                   *
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
* $Id: fx.h,v 1.49 2002/01/18 22:42:55 jeroen Exp $                             *
********************************************************************************/
#ifndef FX_H
#define FX_H

// Basic includes
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

// FOX defines
#include "fxver.h"
#include "fxdefs.h"

// FOX classes
#include "FXException.h"
#include "FXStream.h"
#include "FXString.h"
#include "FXSize.h"
#include "FXPoint.h"
#include "FXRectangle.h"
#include "FXObject.h"
#include "FXDelegator.h"
#include "FXDict.h"
#include "FXFile.h"
#include "FXURL.h"
#include "FXStringDict.h"
#include "FXSettings.h"
#include "FXRegistry.h"
#include "FXObjectList.h"
#include "FXAccelTable.h"
#include "FXRecentFiles.h"
#include "FXCharset.h"
#include "FXApp.h"
#include "FXId.h"
#include "FXVisual.h"
#include "FXFont.h"
#include "FXCursor.h"
#include "FXGIFCursor.h"
#include "FXDrawable.h"
#include "FXBitmap.h"
#include "FXImage.h"
#include "FXGIFImage.h"
#include "FXBMPImage.h"
#include "FXXPMImage.h"
#include "FXPCXImage.h"
#include "FXTGAImage.h"
#include "FXIcon.h"
#include "FXGIFIcon.h"
#include "FXBMPIcon.h"
#include "FXXPMIcon.h"
#include "FXPCXIcon.h"
#include "FXTGAIcon.h"
#include "FXRegion.h"
#include "FXDC.h"
#include "FXDCWindow.h"
#include "FXDCPrint.h"
#include "FXFileDict.h"
#include "FXWindow.h"
#include "FXFrame.h"
#include "FXSeparator.h"
#include "FXLabel.h"
#include "FXDial.h"
#include "FXColorBar.h"
#include "FXColorWell.h"
#include "FXColorWheel.h"
#include "FXTextField.h"
#include "FXButton.h"
#include "FXPicker.h"
#include "FXToggleButton.h"
#include "FXCheckButton.h"
#include "FXRadioButton.h"
#include "FXArrowButton.h"
#include "FXMenuButton.h"
#include "FXComposite.h"
#include "FXPacker.h"
#include "FXHorizontalFrame.h"
#include "FXVerticalFrame.h"
#include "FXMatrix.h"
#include "FXSpinner.h"
#include "FXRootWindow.h"
#include "FXCanvas.h"
#include "FXGroupBox.h"
#include "FXShell.h"
#include "FXTooltip.h"
#include "FXPopup.h"
#include "FXTopWindow.h"
#include "FXDialogBox.h"
#include "FXMainWindow.h"
#include "FXMenuPane.h"
#include "FXMenuCaption.h"
#include "FXMenuSeparator.h"
#include "FXMenuTitle.h"
#include "FXMenuCascade.h"
#include "FXMenuCommand.h"
#include "FXToolbar.h"
#include "FXMenubar.h"
#include "FXOptionMenu.h"
#include "FXSwitcher.h"
#include "FXTabBar.h"
#include "FXTabBook.h"
#include "FXTabItem.h"
#include "FXScrollbar.h"
#include "FXScrollArea.h"
#include "FXScrollWindow.h"
#include "FXList.h"
#include "FXComboBox.h"
#include "FXListBox.h"
#include "FXTreeList.h"
#include "FXTreeListBox.h"
#include "FXImageView.h"
#include "FXTable.h"
#include "FXDragCorner.h"
#include "FXStatusbar.h"
#include "FXStatusline.h"
#include "FXMessageBox.h"
#include "FXDirList.h"
#include "FXSlider.h"
#include "FXSplitter.h"
#include "FX4Splitter.h"
#include "FXHeader.h"
#include "FXShutter.h"
#include "FXIconList.h"
#include "FXFileList.h"
#include "FXDirBox.h"
#include "FXDriveBox.h"
#include "FXDirSelector.h"
#include "FXDirDialog.h"
#include "FXFileSelector.h"
#include "FXFileDialog.h"
#include "FXColorSelector.h"
#include "FXColorDialog.h"
#include "FXFontSelector.h"
#include "FXFontDialog.h"
#include "FXUndoList.h"
#include "FXText.h"
#include "FXDataTarget.h"
#include "FXProgressBar.h"
#include "FXReplaceDialog.h"
#include "FXSearchDialog.h"
#include "FXInputDialog.h"
#include "FXProgressDialog.h"
#include "FXMDIButton.h"
#include "FXMDIClient.h"
#include "FXMDIChild.h"
#include "FXDocument.h"
#include "FXToolbarGrip.h"
#include "FXToolbarShell.h"
#include "FXToolbarTab.h"
#include "FXPrintDialog.h"
#include "FXDebugTarget.h"

#endif
