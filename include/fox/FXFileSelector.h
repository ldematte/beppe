/********************************************************************************
*                                                                               *
*                  F i l e   S e l e c t i o n   W i d g e t                    *
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
* $Id: FXFileSelector.h,v 1.38 2002/02/26 17:20:12 fox Exp $                 *
********************************************************************************/
#ifndef FXFILESELECTOR_H
#define FXFILESELECTOR_H

#ifndef FXPACKER_H
#include "FXPacker.h"
#endif


class FXFileList;
class FXTextField;
class FXComboBox;
class FXDirBox;
class FXButton;
class FXIcon;
class FXMenuPane;
class FXCheckButton;


/// File selection modes
enum {
  SELECTFILE_ANY,             /// A single file, existing or not (to save to)
  SELECTFILE_EXISTING,        /// An existing file (to load)
  SELECTFILE_MULTIPLE,        /// Multiple existing files
  SELECTFILE_MULTIPLE_ALL,    /// Multiple existing files or directories
  SELECTFILE_DIRECTORY        /// Existing directory
  };


/// File selection widget
class FXAPI FXFileSelector : public FXPacker {
  FXDECLARE(FXFileSelector)
protected:
  FXFileList    *filebox;         // File list widget
  FXTextField   *filename;        // File name entry field
  FXComboBox    *filefilter;      // Combobox for pattern list
  FXMenuPane    *bookmarks;       // Menu for bookmarks
  FXCheckButton *readonly;        // Open file as read only
  FXDirBox      *dirbox;          // Directory hierarchy list
  FXButton      *accept;          // Accept button
  FXButton      *cancel;          // Cancel button
  FXIcon        *updiricon;       // Up directory icon
  FXIcon        *newdiricon;      // New directory icon
  FXIcon        *listicon;        // List mode icon
  FXIcon        *detailicon;      // Detail mode icon
  FXIcon        *iconsicon;       // Icon mode icon
  FXIcon        *homeicon;        // Go home icon
  FXIcon        *workicon;        // Go home icon
  FXIcon        *shownicon;       // Files shown icon
  FXIcon        *hiddenicon;      // Files hidden icon
  FXIcon        *markicon;        // Book mark icon
  FXIcon        *clearicon;       // Book clear icon
  FXIcon        *deleteicon;      // Delete file icon
  FXIcon        *moveicon;        // Rename file icon
  FXIcon        *copyicon;        // Copy file icon
  FXIcon        *linkicon;        // Link file icon
  FXRecentFiles  mrufiles;        // Recently visited places
  FXuint         selectmode;      // Select mode
protected:
  FXFileSelector(){}
  static FXString patternFromText(const FXString& pattern);
  static FXString extensionFromPattern(const FXString& pattern);
private:
  FXFileSelector(const FXFileSelector&);
  FXFileSelector &operator=(const FXFileSelector&);
public:
  long onCmdAccept(FXObject*,FXSelector,void*);
  long onCmdFilter(FXObject*,FXSelector,void*);
  long onCmdItemDblClicked(FXObject*,FXSelector,void*);
  long onCmdItemSelected(FXObject*,FXSelector,void*);
  long onCmdItemDeselected(FXObject*,FXSelector,void*);
  long onCmdDirectoryUp(FXObject*,FXSelector,void*);
  long onUpdDirectoryUp(FXObject*,FXSelector,void*);
  long onCmdDirTree(FXObject*,FXSelector,void*);
  long onCmdHome(FXObject*,FXSelector,void*);
  long onCmdWork(FXObject*,FXSelector,void*);
  long onCmdBookmark(FXObject*,FXSelector,void*);
  long onCmdVisit(FXObject*,FXSelector,void*);
  long onCmdNew(FXObject*,FXSelector,void*);
  long onUpdNew(FXObject*,FXSelector,void*);
  long onCmdMove(FXObject*,FXSelector,void*);
  long onCmdCopy(FXObject*,FXSelector,void*);
  long onCmdLink(FXObject*,FXSelector,void*);
  long onCmdDelete(FXObject*,FXSelector,void*);
  long onUpdSelected(FXObject*,FXSelector,void*);
  long onPopupMenu(FXObject*,FXSelector,void*);
public:
  enum {
    ID_FILEFILTER=FXPacker::ID_LAST,
    ID_ACCEPT,
    ID_FILELIST,
    ID_DIRECTORY_UP,
    ID_DIRTREE,
    ID_HOME,
    ID_WORK,
    ID_BOOKMARK,
    ID_VISIT,
    ID_NEW,
    ID_DELETE,
    ID_MOVE,
    ID_COPY,
    ID_LINK,
    ID_LAST
    };
public:

  /// Constructor
  FXFileSelector(FXComposite *p,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Return a pointer to the "Accept" button
  FXButton *acceptButton() const { return accept; }

  /// Return a pointer to the "Cancel" button
  FXButton *cancelButton() const { return cancel; }

  /// Change file name
  void setFilename(const FXString& path);

  /// Return file name, if any
  FXString getFilename() const;

  /**
  * Return array of strings containing the selected file names, terminated
  * by an empty string.  This string array must be freed using delete [].
  * If no files were selected, a NULL is returned.
  */
  FXString* getFilenames() const;

  /// Change file pattern
  void setPattern(const FXString& ptrn);

  /// Return file pattern
  FXString getPattern() const;

  /**
  * Change the list of file patterns shown in the file dialog.
  * Each pattern comprises an optional name, followed by a pattern in
  * parentheses.  The patterns are separated by newlines.
  * For example,
  *
  *  "*\n*.cpp,*.cc\n*.hpp,*.hh,*.h"
  *
  * and
  *
  *  "All Files (*)\nC++ Sources (*.cpp,*.cc)\nC++ Headers (*.hpp,*.hh,*.h)"
  *
  * will set the same three patterns, but the former shows no pattern names.
  */
  void setPatternList(const FXString& patterns);

  /**
  * Set list of patterns as name,pattern pairs.
  * The list should be terminated with a final NULL string.
  * (DEPRECATED)
  */
  void setPatternList(const FXchar **ptrns);

  /// Return list of patterns
  FXString getPatternList() const;

  /**
  * After setting the list of patterns, this call will
  * initially select pattern n as the active one.
  */
  void setCurrentPattern(FXint n);

  /// Return current pattern number
  FXint getCurrentPattern() const;

  /// Get pattern text for given pattern number
  FXString getPatternText(FXint patno) const;

  /// Change pattern text for pattern number
  void setPatternText(FXint patno,const FXString& text);

  /// Change directory
  void setDirectory(const FXString& path);

  /// Return directory
  FXString getDirectory() const;

  /// Set the inter-item spacing (in pixels)
  void setItemSpace(FXint s);

  /// Return the inter-item spacing (in pixels)
  FXint getItemSpace() const;

  /// Change file list style
  void setFileBoxStyle(FXuint style);

  /// Return file list style
  FXuint getFileBoxStyle() const;

  /// Change file selection mode
  void setSelectMode(FXuint mode);

  /// Return file selection mode
  FXuint getSelectMode() const { return selectmode; }

  /// Show readonly button
  void showReadOnly(FXbool show);

  /// Return TRUE if readonly is shown
  FXbool shownReadOnly() const;

  /// Set initial state of readonly button
  void setReadOnly(FXbool state);

  /// Get readonly state
  FXbool getReadOnly() const;

  /// Save object to a stream
  virtual void save(FXStream& store) const;

  /// Load object from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXFileSelector();
  };


#endif
