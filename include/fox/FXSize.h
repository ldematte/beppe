/********************************************************************************
*                                                                               *
*                               S i z e    C l a s s                            *
*                                                                               *
*********************************************************************************
* Copyright (C) 1994,2002 by Jeroen van der Zijp.   All Rights Reserved.        *
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
* $Id: FXSize.h,v 1.3 2002/01/18 22:42:54 jeroen Exp $                          *
********************************************************************************/
#ifndef FXSIZE_H
#define FXSIZE_H


/// Size
class FXAPI FXSize {
public:
  FXshort w;
  FXshort h;
public:

  /// Constructors
  FXSize(){ }
  FXSize(const FXSize& s):w(s.w),h(s.h){ }
  FXSize(FXshort ww,FXshort hh):w(ww),h(hh){ }

  /// Equality
  friend FXAPI FXbool operator==(const FXSize& s,const FXSize& t){ return s.w==t.w && s.h==t.h; }
  friend FXAPI FXbool operator!=(const FXSize& s,const FXSize& t){ return s.w!=t.w || s.h!=t.h; }

  /// Assignment
  FXSize& operator=(const FXSize& s){ w=s.w; h=s.h; return *this; }

  /// Assignment operators
  FXSize& operator+=(const FXSize& s){ w+=s.w; h+=s.h; return *this; }
  FXSize& operator-=(const FXSize& s){ w-=s.w; h-=s.h; return *this; }
  FXSize& operator*=(FXshort c){ w*=c; h*=c; return *this; }
  FXSize& operator/=(FXshort c){ w/=c; h/=c; return *this; }

  /// Negation
  FXSize operator-(){ return FXSize(-w,-h); }

  /// Other operators
  friend FXAPI FXSize operator+(const FXSize& s,const FXSize& t){ return FXSize(s.w+t.w,s.h+t.h); }
  friend FXAPI FXSize operator-(const FXSize& s,const FXSize& t){ return FXSize(s.w-t.w,s.h-t.h); }
  friend FXAPI FXSize operator*(const FXSize& s,FXshort c){ return FXSize(s.w*c,s.h*c); }
  friend FXAPI FXSize operator*(FXshort c,const FXSize& s){ return FXSize(c*s.w,c*s.h); }
  friend FXAPI FXSize operator/(const FXSize& s,FXshort c){ return FXSize(s.w/c,s.h/c); }
  friend FXAPI FXSize operator/(FXshort c,const FXSize& s){ return FXSize(c/s.w,c/s.h); }

  /// Save object to a stream
  friend FXAPI FXStream& operator<<(FXStream& store,const FXSize& s);

  /// Load object from a stream
  friend FXAPI FXStream& operator>>(FXStream& store,FXSize& s);
  };


#endif
