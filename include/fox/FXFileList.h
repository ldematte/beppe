/********************************************************************************
*                                                                               *
*                        F i l e    L i s t   W i d g e t                       *
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
* $Id: FXFileList.h,v 1.33 2002/01/18 22:42:53 jeroen Exp $                     *
********************************************************************************/
#ifndef FXFILELIST_H
#define FXFILELIST_H

#ifndef FXICONLIST_H
#include "FXIconList.h"
#endif



struct FXTimer;
struct FXFileAssoc;
class FXFileDict;
class FXFileList;
class FXIcon;


// File List options
enum {
  FILELIST_SHOWHIDDEN   = 0x04000000, /// Show hidden files or directories
  FILELIST_SHOWDIRS     = 0x08000000, /// Show only directories
  FILELIST_NO_OWN_ASSOC = 0x10000000  /// Do not create associations for files
  };



/// File item
class FXAPI FXFileItem : public FXIconItem {
  FXDECLARE(FXFileItem)
  friend class FXFileList;
protected:
  FXFileAssoc  *assoc;                      /// File association record
  unsigned long size;                       /// File size
  FXTime        date;                       /// File time
protected:
  FXFileItem():assoc(NULL),size(0),date(0){}
protected:
  enum{
    FOLDER     = 64,                        /// Directory item
    EXECUTABLE = 128,                       /// Executable item
    SYMLINK    = 256,                       /// Symbolic linked item
    CHARDEV    = 512,                       /// Character special item
    BLOCKDEV   = 1024,                      /// Block special item
    FIFO       = 2048,                      /// FIFO item
    SOCK       = 4096                       /// Socket item
    };
public:
  /// Constructor
  FXFileItem(const FXString& text,FXIcon* bi=NULL,FXIcon* mi=NULL,void* ptr=NULL):FXIconItem(text,bi,mi,ptr),assoc(NULL),size(0),date(0){}

  /// Return true if this is a file item
  FXbool isFile() const { return (state&(FOLDER|BLOCKDEV|CHARDEV|FIFO|SOCK))==0; }

  /// Return true if this is a directory item
  FXbool isDirectory() const { return (state&FOLDER)!=0; }

  /// Return true if this is an executable item
  FXbool isExecutable() const { return (state&EXECUTABLE)!=0; }

  /// Return true if this is a symbolic link item
  FXbool isSymlink() const { return (state&SYMLINK)!=0; }

  /// Return true if this is a character device item
  FXbool isChardev() const { return (state&CHARDEV)!=0; }

  /// Return true if this is a block device item
  FXbool isBlockdev() const { return (state&BLOCKDEV)!=0; }

  /// Return true if this is an FIFO item
  FXbool isFifo() const { return (state&FIFO)!=0; }

  /// Return true if this is a socket
  FXbool isSocket() const { return (state&SOCK)!=0; }

  /// Return the file-association object for this item
  FXFileAssoc* getAssoc() const { return assoc; }

  /// Return the file size for this item
  unsigned long getSize() const { return size; }

  /// Return the date for this item
  FXTime getDate() const { return date; }
  };


/// File List object
class FXAPI FXFileList : public FXIconList {
  FXDECLARE(FXFileList)
protected:
  FXString     directory;             // Current directory
  FXString     orgdirectory;          // Original directory
  FXString     dropdirectory;         // Drop directory
  FXDragAction dropaction;            // Drop action
  FXString     dragfiles;             // Dragged files
  FXFileDict  *associations;          // Association table
  FXString     pattern;               // Pattern of file names
  FXuint       matchmode;             // File wildcard match mode
  FXTime       timestamp;             // Time when last refreshed
  FXTimer     *refreshtimer;          // Refresh timer
  FXTimer     *opentimer;             // Open up folder when hovering
  FXIcon      *big_folder;            // Big folder icon
  FXIcon      *mini_folder;           // Mini folder icon
  FXIcon      *big_doc;               // Big document icon
  FXIcon      *mini_doc;              // Mini document icon
  FXIcon      *big_app;               // Big application icon
  FXIcon      *mini_app;              // Mini application icon
protected:
  FXFileList();
  virtual FXIconItem *createItem(const FXString& text,FXIcon *big,FXIcon* mini,void* ptr);
  void listDirectory();
private:
  FXFileList(const FXFileList&);
  FXFileList &operator=(const FXFileList&);
public:
  long onRefreshTimer(FXObject*,FXSelector,void*);
  long onOpenTimer(FXObject*,FXSelector,void*);
  long onDNDEnter(FXObject*,FXSelector,void*);
  long onDNDLeave(FXObject*,FXSelector,void*);
  long onDNDMotion(FXObject*,FXSelector,void*);
  long onDNDDrop(FXObject*,FXSelector,void*);
  long onDNDRequest(FXObject*,FXSelector,void*);
  long onBeginDrag(FXObject*,FXSelector,void*);
  long onEndDrag(FXObject*,FXSelector,void*);
  long onDragged(FXObject*,FXSelector,void*);
  long onCmdSetValue(FXObject*,FXSelector,void*);
  long onCmdGetStringValue(FXObject*,FXSelector,void*);
  long onCmdSetStringValue(FXObject*,FXSelector,void*);
  long onCmdDirectoryUp(FXObject*,FXSelector,void*);
  long onUpdDirectoryUp(FXObject*,FXSelector,void*);
  long onCmdSortByName(FXObject*,FXSelector,void*);
  long onUpdSortByName(FXObject*,FXSelector,void*);
  long onCmdSortByType(FXObject*,FXSelector,void*);
  long onUpdSortByType(FXObject*,FXSelector,void*);
  long onCmdSortBySize(FXObject*,FXSelector,void*);
  long onUpdSortBySize(FXObject*,FXSelector,void*);
  long onCmdSortByTime(FXObject*,FXSelector,void*);
  long onUpdSortByTime(FXObject*,FXSelector,void*);
  long onCmdSortByUser(FXObject*,FXSelector,void*);
  long onUpdSortByUser(FXObject*,FXSelector,void*);
  long onCmdSortByGroup(FXObject*,FXSelector,void*);
  long onUpdSortByGroup(FXObject*,FXSelector,void*);
  long onCmdSortReverse(FXObject*,FXSelector,void*);
  long onUpdSortReverse(FXObject*,FXSelector,void*);
  long onCmdSetPattern(FXObject*,FXSelector,void*);
  long onUpdSetPattern(FXObject*,FXSelector,void*);
  long onCmdSetDirectory(FXObject*,FXSelector,void*);
  long onUpdSetDirectory(FXObject*,FXSelector,void*);
  long onCmdToggleHidden(FXObject*,FXSelector,void*);
  long onUpdToggleHidden(FXObject*,FXSelector,void*);
  long onCmdShowHidden(FXObject*,FXSelector,void*);
  long onUpdShowHidden(FXObject*,FXSelector,void*);
  long onCmdHideHidden(FXObject*,FXSelector,void*);
  long onUpdHideHidden(FXObject*,FXSelector,void*);
  long onCmdHeader(FXObject*,FXSelector,void*);
  long onUpdHeader(FXObject*,FXSelector,void*);
public:
  static FXint cmpFName(const FXIconItem* pa,const FXIconItem* pb);
  static FXint cmpRName(const FXIconItem* pa,const FXIconItem* pb);
  static FXint cmpFType(const FXIconItem* pa,const FXIconItem* pb);
  static FXint cmpRType(const FXIconItem* pa,const FXIconItem* pb);
  static FXint cmpFSize(const FXIconItem* pa,const FXIconItem* pb);
  static FXint cmpRSize(const FXIconItem* pa,const FXIconItem* pb);
  static FXint cmpFTime(const FXIconItem* pa,const FXIconItem* pb);
  static FXint cmpRTime(const FXIconItem* pa,const FXIconItem* pb);
  static FXint cmpFUser(const FXIconItem* pa,const FXIconItem* pb);
  static FXint cmpRUser(const FXIconItem* pa,const FXIconItem* pb);
  static FXint cmpFGroup(const FXIconItem* pa,const FXIconItem* pb);
  static FXint cmpRGroup(const FXIconItem* pa,const FXIconItem* pb);
public:
  enum {
    ID_SORT_BY_NAME=FXIconList::ID_LAST,
    ID_SORT_BY_TYPE,
    ID_SORT_BY_SIZE,
    ID_SORT_BY_TIME,
    ID_SORT_BY_USER,
    ID_SORT_BY_GROUP,
    ID_SORT_REVERSE,
    ID_DIRECTORY_UP,
    ID_SET_PATTERN,
    ID_SET_DIRECTORY,
    ID_SHOW_HIDDEN,
    ID_HIDE_HIDDEN,
    ID_TOGGLE_HIDDEN,
    ID_REFRESHTIMER,
    ID_OPENTIMER,
    ID_LAST
    };
public:

  /// Construct a file list
  FXFileList(FXComposite *p,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Create server-side resources
  virtual void create();

  /// Detach server-side resources
  virtual void detach();

  /// Destroy server-side resources
  virtual void destroy();

  /// Set current file
  void setCurrentFile(const FXString& file);

  /// Return current file
  FXString getCurrentFile() const;

  /// Set current directory
  void setDirectory(const FXString& path);

  /// Return current directory
  FXString getDirectory() const { return directory; }

  /// Change wildcard matching pattern
  void setPattern(const FXString& ptrn);

  /// Return wildcard pattern
  FXString getPattern() const { return pattern; }

  /// Return TRUE if item is a directory
  FXbool isItemDirectory(FXint index) const;

  /// Return TRUE if item is a file
  FXbool isItemFile(FXint index) const;

  /// Return TRUE if item is executable
  FXbool isItemExecutable(FXint index) const;

  /// Return name of item at index
  FXString getItemFilename(FXint index) const;

  /// Return full pathname of item at index
  FXString getItemPathname(FXint index) const;

  /// Return file association of item
  FXFileAssoc* getItemAssoc(FXint index) const;

  /// Return wildcard matching mode
  FXuint getMatchMode() const { return matchmode; }

  /// Change wildcard matching mode
  void setMatchMode(FXuint mode);

  /// Return TRUE if showing hidden files
  FXbool showHiddenFiles() const;

  /// Show or hide hidden files
  void showHiddenFiles(FXbool showing);

  /// Return TRUE if showing directories only
  FXbool showOnlyDirectories() const;

  /// Show directories only
  void showOnlyDirectories(FXbool shown);

  /// Change file associations
  void setAssociations(FXFileDict* assoc);

  /// Return file associations
  FXFileDict* getAssociations() const { return associations; }

  /// Save to stream
  virtual void save(FXStream& store) const;

  /// Load from stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXFileList();
  };


#endif
