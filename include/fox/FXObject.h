/********************************************************************************
*                                                                               *
*                         T o p l e v el   O b j e c t                          *
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
* $Id: FXObject.h,v 1.16 2002/01/18 22:42:54 jeroen Exp $                       *
********************************************************************************/
#ifndef FXOBJECT_H
#define FXOBJECT_H


// Minimum and maximum keys
#define MINKEY    0
#define MAXKEY    65535


// Minimum and maximum types
#define MINTYPE   0
#define MAXTYPE   65535


// Association key
typedef FXuint FXSelector;


// Forward
class FXObject;



/// Describes a FOX object
struct FXAPI FXMetaClass {
  const FXchar       *className;
  FXObject*         (*manufacture)();
  const FXMetaClass  *baseClass;
  const void         *assoc;
  FXuint              nassocs;
  FXuint              assocsz;
  FXuint              namelen;

  /// Check if metaclass is subclass of some other metaclass
  FXbool isSubClassOf(const FXMetaClass* metaclass) const;

  /// Make instance of some object
  FXObject* makeInstance() const;

  /// Ask class name
  const FXchar* getClassName() const { return className; }

  /// Ask base class
  const FXMetaClass* getBaseClass() const { return baseClass; }

  /// Find metaclass object
  static const FXMetaClass* getMetaClassFromName(const FXchar* name);

  /// Search message map
  const void* search(FXSelector key) const;
  };


// Pre-runtime initializer
class FXAPI __FXMETACLASSINITIALIZER__ {
public:
  __FXMETACLASSINITIALIZER__(const FXMetaClass* meta);
  };


/// Macro to set up class declaration
#define FXDECLARE(classname) \
  public: \
   struct FXMapEntry { FXSelector keylo; FXSelector keyhi; long (classname::* func)(FXObject*,FXSelector,void*); }; \
   static const FXMetaClass metaClass; \
   static FXObject* manufacture(); \
   virtual long handle(FXObject* sender,FXSelector sel,void* ptr); \
   virtual const FXMetaClass* getMetaClass() const { return &metaClass; } \
   friend FXStream& operator<<(FXStream& store,const classname* obj){return store.saveObject((FXObjectPtr)(obj));} \
   friend FXStream& operator>>(FXStream& store,classname*& obj){return store.loadObject((FXObjectPtr&)(obj));} \
  private:


/// Macro to set up class implementation
#define FXIMPLEMENT(classname,baseclassname,mapping,nmappings) \
  FXObject* classname::manufacture(){return new classname;} \
  const FXMetaClass classname::metaClass={#classname,classname::manufacture,&baseclassname::metaClass,mapping,nmappings,sizeof(classname::FXMapEntry),sizeof(#classname)}; \
  __FXMETACLASSINITIALIZER__ classname##Initializer(&classname::metaClass); \
  long classname::handle(FXObject* sender,FXSelector sel,void* ptr){ \
    const FXMapEntry* me=(const FXMapEntry*)metaClass.search(sel); \
    return me ? (this->* me->func)(sender,sel,ptr) : baseclassname::handle(sender,sel,ptr); \
    }


/// Macro to set up abstract class declaration
#define FXDECLARE_ABSTRACT(classname) \
  public: \
   struct FXMapEntry { FXSelector keylo; FXSelector keyhi; long (classname::* func)(FXObject*,FXSelector,void*); }; \
   static const FXMetaClass metaClass; \
   virtual long handle(FXObject* sender,FXSelector sel,void* ptr); \
   virtual const FXMetaClass* getMetaClass() const { return &metaClass; } \
   friend FXStream& operator<<(FXStream& store,const classname* obj){return store.saveObject((FXObjectPtr)(obj));} \
   friend FXStream& operator>>(FXStream& store,classname*& obj){return store.loadObject((FXObjectPtr&)(obj));} \
  private:


/// Macro to set up abstract class implementation
#define FXIMPLEMENT_ABSTRACT(classname,baseclassname,mapping,nmappings) \
  const FXMetaClass classname::metaClass={#classname,NULL,&baseclassname::metaClass,mapping,nmappings,sizeof(classname::FXMapEntry),sizeof(#classname)}; \
  __FXMETACLASSINITIALIZER__ classname##Initializer(&classname::metaClass); \
  long classname::handle(FXObject* sender,FXSelector sel,void* ptr){ \
    const FXMapEntry* me=(const FXMapEntry*)metaClass.search(sel); \
    return me ? (this->* me->func)(sender,sel,ptr) : baseclassname::handle(sender,sel,ptr); \
    }


/// MetaClass of a class
#define FXMETACLASS(classname) (&classname::metaClass)


/// Set up map type
#define FXDEFMAP(classname) static const classname::FXMapEntry

/// Define range of function types
#define FXMAPTYPES(typelo,typehi,func) {MKUINT(MINKEY,typelo),MKUINT(MAXKEY,typehi),&func}

/// Define range of function types
#define FXMAPTYPE(type,func) {MKUINT(MINKEY,type),MKUINT(MAXKEY,type),&func}

/// Define range of functions
#define FXMAPFUNCS(type,keylo,keyhi,func) {MKUINT(keylo,type),MKUINT(keyhi,type),&func}

/// Define one function
#define FXMAPFUNC(type,key,func) {MKUINT(key,type),MKUINT(key,type),&func}


/// Base of all FOX object
class FXAPI FXObject {
  FXDECLARE(FXObject)
public:

  /// Called for unhandled messages
  virtual long onDefault(FXObject*,FXSelector,void*);

public:

  /// Get class name of some object
  const FXchar* getClassName() const;

  /// Check if object is member of metaclass
  FXbool isMemberOf(const FXMetaClass* metaclass) const;

  /// Save object to stream
  virtual void save(FXStream& store) const;

  /// Load object from stream
  virtual void load(FXStream& store);

  /// Virtual destructor
  virtual ~FXObject();
  };


#endif
