/********************************************************************************
*                                                                               *
*                     D i r e c t o r y   L i s t   W i d g e t                 *
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
* $Id: FXDirList.h,v 1.30 2002/01/18 22:42:52 jeroen Exp $                      *
********************************************************************************/
#ifndef FXDIRLIST_H
#define FXDIRLIST_H

#ifndef FXTREELIST_H
#include "FXTreeList.h"
#endif



struct FXTimer;
struct FXFileAssoc;
class  FXFileDict;
class  FXIcon;
class FXDirList;


/// Directory List options
enum {
  DIRLIST_SHOWFILES     = 0x08000000, /// Show files as well as directories
  DIRLIST_SHOWHIDDEN    = 0x10000000, /// Show hidden files or directories
  DIRLIST_NO_OWN_ASSOC  = 0x20000000  /// Do not create associations for files
  };


/// Directory item
class FXAPI FXDirItem : public FXTreeItem {
  FXDECLARE(FXDirItem)
  friend class FXDirList;
protected:
  FXDirItem    *iprev;
  FXDirItem    *inext;
  FXDirItem    *list;
  FXFileAssoc  *assoc;
  unsigned long size;
  FXTime        date;
protected:
  FXDirItem():iprev(NULL),inext(NULL),list(NULL),assoc(NULL),date(0){}
protected:
  enum {
    FOLDER      = 512,                // Directory item
    EXECUTABLE  = 1024,               // Executable item
    SYMLINK     = 2048,               // Symbolic linked item
    CHARDEV     = 4096,               // Character special item
    BLOCKDEV    = 8192,               // Block special item
    FIFO        = 16384,              // FIFO item
    SOCK        = 32768               // Socket item
    };
public:
  /// Constructor
  FXDirItem(const FXString& text,FXIcon* oi=NULL,FXIcon* ci=NULL,void* ptr=NULL):FXTreeItem(text,oi,ci,ptr),iprev(NULL),inext(NULL),list(NULL),assoc(NULL),size(0),date(0){}
  FXbool isDirectory() const { return (state&FOLDER)!=0; }
  FXbool isExecutable() const { return (state&EXECUTABLE)!=0; }
  FXbool isSymlink() const { return (state&SYMLINK)!=0; }
  FXbool isChardev() const { return (state&CHARDEV)!=0; }
  FXbool isBlockdev() const { return (state&BLOCKDEV)!=0; }
  FXbool isFifo() const { return (state&FIFO)!=0; }
  FXbool isSocket() const { return (state&SOCK)!=0; }
  FXFileAssoc* getAssoc() const { return assoc; }
  unsigned long getSize() const { return size; }
  FXTime getDate() const { return date; }
  };


/// Directory tree List
class FXAPI FXDirList : public FXTreeList {
  FXDECLARE(FXDirList)
protected:
  FXFileDict   *associations;         // Association table
  FXString      dropdirectory;        // Drop directory
  FXDragAction  dropaction;           // Drop action
  FXString      dragfiles;            // Dragged files
  FXString      pattern;              // Pattern of file names
  FXuint        matchmode;            // File wildcard match mode
  FXTimer      *refresh;              // Refresh timer
  FXIcon       *closed_folder;        // Closed folder icon
  FXIcon       *open_folder;          // Open folder icon
  FXIcon       *mini_doc;             // Document icon
  FXIcon       *mini_app;             // Application icon
  FXIcon       *cdromicon;
  FXIcon       *harddiskicon;
  FXIcon       *networkicon;
  FXIcon       *floppyicon;
  FXIcon       *zipdiskicon;
  FXuint        counter;              // Refresh counter
protected:
  FXDirList();
  virtual FXTreeItem* createItem(const FXString& text,FXIcon* oi,FXIcon* ci,void* ptr);
  FXchar *getpath(const FXTreeItem* item,FXchar* pathname) const;
  FXTreeItem* getitem(FXchar* pathname);
  FXbool scanRootDir(FXbool relist);
  FXbool scanSubDir(FXDirItem *par,FXchar *pathname,FXbool relist);
  FXbool listSubDir(FXDirItem *par,FXchar *pathname);
  FXbool listRoots();
private:
  FXDirList(const FXDirList&);
  FXDirList &operator=(const FXDirList&);
public:
  long onRefresh(FXObject*,FXSelector,void*);
  long onBeginDrag(FXObject*,FXSelector,void*);
  long onEndDrag(FXObject*,FXSelector,void*);
  long onDragged(FXObject*,FXSelector,void*);
  long onDNDEnter(FXObject*,FXSelector,void*);
  long onDNDLeave(FXObject*,FXSelector,void*);
  long onDNDMotion(FXObject*,FXSelector,void*);
  long onDNDDrop(FXObject*,FXSelector,void*);
  long onDNDRequest(FXObject*,FXSelector,void*);
  long onOpened(FXObject*,FXSelector,void*);
  long onClosed(FXObject*,FXSelector,void*);
  long onExpanded(FXObject*,FXSelector,void*);
  long onCollapsed(FXObject*,FXSelector,void*);
  long onCmdSetValue(FXObject*,FXSelector,void*);
  long onCmdSetStringValue(FXObject*,FXSelector,void*);
  long onCmdGetStringValue(FXObject*,FXSelector,void*);
  long onCmdToggleHidden(FXObject*,FXSelector,void*);
  long onUpdToggleHidden(FXObject*,FXSelector,void*);
  long onCmdShowHidden(FXObject*,FXSelector,void*);
  long onUpdShowHidden(FXObject*,FXSelector,void*);
  long onCmdHideHidden(FXObject*,FXSelector,void*);
  long onUpdHideHidden(FXObject*,FXSelector,void*);
  long onCmdToggleFiles(FXObject*,FXSelector,void*);
  long onUpdToggleFiles(FXObject*,FXSelector,void*);
  long onCmdShowFiles(FXObject*,FXSelector,void*);
  long onUpdShowFiles(FXObject*,FXSelector,void*);
  long onCmdHideFiles(FXObject*,FXSelector,void*);
  long onUpdHideFiles(FXObject*,FXSelector,void*);
  long onCmdSetPattern(FXObject*,FXSelector,void*);
  long onUpdSetPattern(FXObject*,FXSelector,void*);
  long onCmdSortReverse(FXObject*,FXSelector,void*);
  long onUpdSortReverse(FXObject*,FXSelector,void*);
public:
  static FXint cmpFName(const FXTreeItem* a,const FXTreeItem* b);
  static FXint cmpRName(const FXTreeItem* pa,const FXTreeItem* pb);
public:
  enum {
    ID_REFRESH=FXTreeList::ID_LAST,
    ID_SHOW_FILES,
    ID_HIDE_FILES,
    ID_TOGGLE_FILES,
    ID_SHOW_HIDDEN,
    ID_HIDE_HIDDEN,
    ID_TOGGLE_HIDDEN,
    ID_SET_PATTERN,
    ID_SORT_REVERSE,
    ID_LAST
    };
public:

  /// Construct a directory list
  FXDirList(FXComposite *p,FXint nvis,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Create server-side resources
  virtual void create();

  /// Detach server-side resources
  virtual void detach();

  /// Destroy server-side resources
  virtual void destroy();

  /// Return TRUE if item is a directory
  FXbool isItemDirectory(const FXTreeItem* item) const;

  /// Return TRUE if item is a file
  FXbool isItemFile(const FXTreeItem* item) const;

  /// Return TRUE if item is executable
  FXbool isItemExecutable(const FXTreeItem* item) const;

  /// Set current file
  void setCurrentFile(const FXString& file);

  /// Return current file
  FXString getCurrentFile() const;

  /// Set current directory
  void setDirectory(const FXString& path);

  /// Return current directory
  FXString getDirectory() const;

  /// Return name of item
  FXString getItemFilename(const FXTreeItem* item) const;

  /// Return full pathname of item
  FXString getItemPathname(const FXTreeItem* item) const;

  /// Change wildcard matching pattern
  void setPattern(const FXString& ptrn);

  /// Return wildcard pattern
  FXString getPattern() const { return pattern; }

  /// Return wildcard matching mode
  FXuint getMatchMode() const { return matchmode; }

  /// Change wildcard matching mode
  void setMatchMode(FXuint mode);

  /// Return TRUE if showing files as well as directories
  FXbool showFiles() const;

  /// Show or hide normal files
  void showFiles(FXbool showing);

  /// Return TRUE if showing hidden files and directories
  FXbool showHiddenFiles() const;

  /// Show or hide hidden files and directories
  void showHiddenFiles(FXbool showing);

  /// Change file associations
  void setAssociations(FXFileDict* assoc);

  /// Return file associations
  FXFileDict* getAssociations() const { return associations; }

  /// Save to stream
  virtual void save(FXStream& store) const;

  /// Load from stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXDirList();
  };


#endif
