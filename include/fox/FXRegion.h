/********************************************************************************
*                                                                               *
*                      C l i p p i n g   R e g i o n                            *
*                                                                               *
*********************************************************************************
* Copyright (C) 2000,2002 by Jeroen van der Zijp.   All Rights Reserved.        *
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
* $Id: FXRegion.h,v 1.6 2002/01/18 22:42:54 jeroen Exp $                        *
********************************************************************************/
#ifndef FXREGION_H
#define FXREGION_H


/// Region
class FXAPI FXRegion {
  friend class FXDC;
  friend class FXDCWindow;
private:
  void *region;
public:

  /// Construct new empty region
  FXRegion();

  /// Construct new region copied from region r
  FXRegion(const FXRegion& r);

  /// Construct new region set to given rectangle
  FXRegion(FXint x,FXint y,FXint w,FXint h);

  /// Assign region r to this one
  FXRegion &operator=(const FXRegion& r);

  /// Return TRUE if region is empty
  FXbool empty() const;

  /// Return TRUE if region contains point
  FXbool contains(FXint x,FXint y) const;

  /// Return TRUE if region contains rectangle
  FXbool contains(FXint x,FXint y,FXint w,FXint h) const;

  /// Return bounding box
  void bounds(FXRectangle& r) const;

  /// Offset region by dx,dy
  FXRegion& offset(FXint dx,FXint dy);

  /// Union region r with this one
  FXRegion& operator+=(const FXRegion& r);

  /// Intersect region r with this one
  FXRegion& operator*=(const FXRegion& r);

  /// Substract region r from this one
  FXRegion& operator-=(const FXRegion& r);

  /// Xor region r with this one
  FXRegion& operator^=(const FXRegion& r);

  /// Union of region r1 and region r2
  friend FXAPI FXRegion operator+(const FXRegion& r1,const FXRegion& r2);

  /// Intersection of region r1 and region r2
  friend FXAPI FXRegion operator*(const FXRegion& r1,const FXRegion& r2);

  /// Substract region r2 from region r1
  friend FXAPI FXRegion operator-(const FXRegion& r1,const FXRegion& r2);

  /// Xor of region r1 and region r2
  friend FXAPI FXRegion operator^(const FXRegion& r1,const FXRegion& r2);

  /// Return TRUE if region equal to this one
  friend FXAPI FXbool operator==(const FXRegion& r1,const FXRegion& r2);

  /// Return TRUE if region not equal to this one
  friend FXAPI FXbool operator!=(const FXRegion& r1,const FXRegion& r2);

  /// Destroy region
 ~FXRegion();
  };


#endif
