/********************************************************************************
*                                                                               *
*                               F o n t   O b j e c t                           *
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
* $Id: FXFont.h,v 1.23 2002/01/18 22:42:53 jeroen Exp $                         *
********************************************************************************/
#ifndef FXFONT_H
#define FXFONT_H

#ifndef FXID_H
#include "FXId.h"
#endif



/// Font style hints which influence the matcher
enum FXFontHint {
  FONTPITCH_DEFAULT    = 0,         /// Default pitch
  FONTPITCH_FIXED      = 1,         /// Fixed pitch, mono-spaced
  FONTPITCH_VARIABLE   = 2,         /// Variable pitch, proportional spacing
  FONTHINT_DONTCARE    = 0,         /// Don't care which font
  FONTHINT_DECORATIVE  = 4,         /// Fancy fonts
  FONTHINT_MODERN      = 8,         /// Monospace typewriter font
  FONTHINT_ROMAN       = 16,        /// Variable width times-like font, serif
  FONTHINT_SCRIPT      = 32,        /// Script or cursive
  FONTHINT_SWISS       = 64,        /// Helvetica/swiss type font, sans-serif
  FONTHINT_SYSTEM      = 128,       /// System font
  FONTHINT_X11         = 256,       /// X11 Font string
  FONTHINT_SCALABLE    = 512,       /// Scalable fonts
  FONTHINT_POLYMORPHIC = 1024       /// Polymorphic fonts
  };


/// Font slant
enum FXFontSlant {
  FONTSLANT_DONTCARE        = 0,    /// Don't care about slant
  FONTSLANT_REGULAR         = 1,    /// Regular straight up
  FONTSLANT_ITALIC          = 2,    /// Italics
  FONTSLANT_OBLIQUE         = 3,    /// Oblique slant
  FONTSLANT_REVERSE_ITALIC  = 4,    /// Reversed italic
  FONTSLANT_REVERSE_OBLIQUE = 5     /// Reversed oblique
  };


/// Font character set encoding
enum FXFontEncoding {
  FONTENCODING_DEFAULT      = 0,                        /// Don't care character encoding
  FONTENCODING_ISO_8859_1   = 1,
  FONTENCODING_ISO_8859_2   = 2,
  FONTENCODING_ISO_8859_3   = 3,
  FONTENCODING_ISO_8859_4   = 4,
  FONTENCODING_ISO_8859_5   = 5,                        /// Cyrillic (almost obsolete)
  FONTENCODING_ISO_8859_6   = 6,
  FONTENCODING_ISO_8859_7   = 7,
  FONTENCODING_ISO_8859_8   = 8,
  FONTENCODING_ISO_8859_9   = 9,
  FONTENCODING_ISO_8859_10  = 10,
  FONTENCODING_ISO_8859_11  = 11,
  FONTENCODING_ISO_8859_13  = 13,
  FONTENCODING_ISO_8859_14  = 14,
  FONTENCODING_ISO_8859_15  = 15,
  FONTENCODING_ISO_8859_16  = 16,
  FONTENCODING_KOI8         = 17,
  FONTENCODING_KOI8_R       = 18,                       /// Russian
  FONTENCODING_KOI8_U       = 19,                       /// Ukrainian
  FONTENCODING_KOI8_UNIFIED = 20,

  FONTENCODING_LATIN1      = FONTENCODING_ISO_8859_1,   /// Latin 1 (West European)
  FONTENCODING_LATIN2      = FONTENCODING_ISO_8859_2,   /// Latin 2 (East European)
  FONTENCODING_LATIN3      = FONTENCODING_ISO_8859_3,   /// Latin 3 (South European)
  FONTENCODING_LATIN4      = FONTENCODING_ISO_8859_4,   /// Latin 4 (North European)
  FONTENCODING_LATIN5      = FONTENCODING_ISO_8859_9,   /// Latin 5 (Turkish)
  FONTENCODING_LATIN6      = FONTENCODING_ISO_8859_10,  /// Latin 6 (Nordic)
  FONTENCODING_LATIN7      = FONTENCODING_ISO_8859_13,  /// Latin 7 (Baltic Rim)
  FONTENCODING_LATIN8      = FONTENCODING_ISO_8859_14,  /// Latin 8 (Celtic)
  FONTENCODING_LATIN9      = FONTENCODING_ISO_8859_15,  /// Latin 9 AKA Latin 0
  FONTENCODING_LATIN10     = FONTENCODING_ISO_8859_16,  /// Latin 10

  FONTENCODING_USASCII     = FONTENCODING_ISO_8859_1,   /// Latin 1
  FONTENCODING_WESTEUROPE  = FONTENCODING_ISO_8859_1,   /// Latin 1 (West European)
  FONTENCODING_EASTEUROPE  = FONTENCODING_ISO_8859_2,   /// Latin 2 (East European)
  FONTENCODING_SOUTHEUROPE = FONTENCODING_ISO_8859_3,   /// Latin 3 (South European)
  FONTENCODING_NORTHEUROPE = FONTENCODING_ISO_8859_4,   /// Latin 4 (North European)
  FONTENCODING_CYRILLIC    = FONTENCODING_ISO_8859_5,   /// Cyrillic
  FONTENCODING_RUSSIAN     = FONTENCODING_KOI8,         /// Cyrillic
  FONTENCODING_ARABIC      = FONTENCODING_ISO_8859_6,   /// Arabic
  FONTENCODING_GREEK       = FONTENCODING_ISO_8859_7,   /// Greek
  FONTENCODING_HEBREW      = FONTENCODING_ISO_8859_8,   /// Hebrew
  FONTENCODING_TURKISH     = FONTENCODING_ISO_8859_9,   /// Latin 5 (Turkish)
  FONTENCODING_NORDIC      = FONTENCODING_ISO_8859_10,  /// Latin 6 (Nordic)
  FONTENCODING_THAI        = FONTENCODING_ISO_8859_11,  /// Thai
  FONTENCODING_BALTIC      = FONTENCODING_ISO_8859_13,  /// Latin 7 (Baltic Rim)
  FONTENCODING_CELTIC      = FONTENCODING_ISO_8859_14   /// Latin 8 (Celtic)
  };


/// Font weight
enum FXFontWeight {
  FONTWEIGHT_DONTCARE   = 0,        /// Don't care about weight
  FONTWEIGHT_THIN       = 100,      /// Thin
  FONTWEIGHT_EXTRALIGHT = 200,      /// Extra light
  FONTWEIGHT_LIGHT      = 300,      /// Light
  FONTWEIGHT_NORMAL     = 400,      /// Normal or regular weight
  FONTWEIGHT_REGULAR    = 400,      /// Normal or regular weight
  FONTWEIGHT_MEDIUM     = 500,      /// Medium bold face
  FONTWEIGHT_DEMIBOLD   = 600,      /// Demi bold face
  FONTWEIGHT_BOLD       = 700,      /// Bold face
  FONTWEIGHT_EXTRABOLD  = 800,      /// Extra
  FONTWEIGHT_HEAVY      = 900,      /// Heavy
  FONTWEIGHT_BLACK      = 900       /// Black
  };


/// Font relative setwidth
enum FXFontSetWidth {
  FONTSETWIDTH_DONTCARE       = 0,    /// Don't care about set width
  FONTSETWIDTH_ULTRACONDENSED = 10,   /// Ultra condensed printing
  FONTSETWIDTH_EXTRACONDENSED = 20,   /// Extra condensed
  FONTSETWIDTH_CONDENSED      = 30,   /// Condensed
  FONTSETWIDTH_NARROW         = 30,   /// Narrow
  FONTSETWIDTH_COMPRESSED     = 30,   /// Compressed
  FONTSETWIDTH_SEMICONDENSED  = 40,   /// Semi-condensed
  FONTSETWIDTH_MEDIUM         = 50,   /// Medium printing
  FONTSETWIDTH_NORMAL         = 50,   /// Normal printing
  FONTSETWIDTH_REGULAR        = 50,   /// Regulat printing
  FONTSETWIDTH_SEMIEXPANDED   = 60,   /// Semi expanded
  FONTSETWIDTH_EXPANDED       = 70,   /// Expanded
  FONTSETWIDTH_WIDE           = 80,   /// Wide
  FONTSETWIDTH_EXTRAEXPANDED  = 80,   /// Extra expanded
  FONTSETWIDTH_ULTRAEXPANDED  = 90    /// Ultra expanded
  };


/// Font style
struct FXFontDesc {
  FXchar          face[48];                 /// Face name
  FXuint          size;                     /// Size in deci-points
  FXuint          weight;                   /// Weight [light, normal, bold, ...]
  FXuint          slant;                    /// Slant [normal, italic, oblique, ...]
  FXuint          encoding;                 /// Encoding of character set
  FXuint          setwidth;                 /// Set width [normal, condensed, expanded, ...]
  FXuint          flags;                    /// Flags
  };


/// Font class
class FXAPI FXFont : public FXId {
  FXDECLARE(FXFont)
protected:
  FXString  name;                 // Name of the font
  FXuint    size;                 // Font size (points*10)
  FXuint    weight;               // Font weight
  FXuint    slant;                // Font slant
  FXuint    encoding;             // Character set encoding
  FXuint    setwidth;             // Relative setwidth
  FXuint    hints;                // Matching hints
  void     *font;                 // Info about the font
private:
#ifdef WIN32
  FXID      dc;                   // Dummy
#endif
protected:
  FXFont();
#ifndef WIN32
  char* findbestfont(char *fontname) const;
  const char* fallbackfont() const;
  char* findmatch(char *fontname,const char* family) const;
#endif
private:
  FXFont(const FXFont&);
  FXFont &operator=(const FXFont&);
public:

  /// Construct font from font description
  FXFont(FXApp* a,const FXFontDesc& fontdesc);

  /// Construct a font with given face name, size in points(pixels), weight, slant, character set encoding, setwidth, and hints
  FXFont(FXApp* a,const FXString& face,FXuint sz,FXuint wt=FONTWEIGHT_NORMAL,FXuint sl=FONTSLANT_REGULAR,FXuint enc=FONTENCODING_DEFAULT,FXuint setw=FONTSETWIDTH_DONTCARE,FXuint h=0);

  /// Construct a font with given X11 font string
  FXFont(FXApp* a,const FXString& nm);

  /// Create the font
  virtual void create();

  /// Detach the font
  virtual void detach();

  /// Destroy the font
  virtual void destroy();

  /// Get face name
  FXString getName() const { return name; }

  /// Get size in deci-points
  FXuint getSize() const { return size; }

  /// Get font weight
  FXuint getWeight() const { return weight; }

  /// Get slant
  FXuint getSlant() const { return slant; }

  /// Get character set encoding
  FXuint getEncoding() const { return encoding; }

  /// Get setwidth
  FXuint getSetWidth() const { return setwidth; }

  /// Get hints
  FXuint getHints() const { return hints; }

  /// Get font description
  void getFontDesc(FXFontDesc& fontdesc) const;

  /// Change font description
  void setFontDesc(const FXFontDesc& fontdesc);

  /// Find out if the font is monotype or proportional
  FXbool isFontMono() const;

  /// See if font has glyph for ch
  FXbool hasChar(FXint ch) const;

  /// Get first character glyph in font
  FXint getMinChar() const;

  /// Get last character glyph in font
  FXint getMaxChar() const;

  /// Left bearing
  FXint leftBearing(FXchar ch) const;

  /// Right bearing
  FXint rightBearing(FXchar ch) const;

  /// Width of widest character in font
  FXint getFontWidth() const;

  /// Height of highest character in font
  FXint getFontHeight() const;

  /// Ascent from baseline
  FXint getFontAscent() const;

  /// Descent from baseline
  FXint getFontDescent() const;

  /// Get font leading [that is lead-ing as in Pb!]
  FXint getFontLeading() const;

  /// Get font line spacing
  FXint getFontSpacing() const;

  /// Calculate width of given text in this font
  FXint getTextWidth(const FXchar *text,FXuint n) const;

  /// Calculate height of given text in this font
  FXint getTextHeight(const FXchar *text,FXuint n) const;

  /// List all fonts matching hints
  static FXbool listFonts(FXFontDesc*& fonts,FXuint& numfonts,const FXString& face,FXuint wt=FONTWEIGHT_DONTCARE,FXuint sl=FONTSLANT_DONTCARE,FXuint sw=FONTSETWIDTH_DONTCARE,FXuint en=FONTENCODING_DEFAULT,FXuint h=0);

  /// Save font data into stream
  virtual void save(FXStream& store) const;

  /// Load font data from stream
  virtual void load(FXStream& store);

  /// Destroy font
  virtual ~FXFont();
  };



/// Parse font description from a string
extern FXAPI FXbool fxparsefontdesc(FXFontDesc& fontdesc,const FXchar* string);

/// Unparse font description into a string
extern FXAPI FXbool fxunparsefontdesc(FXchar *string,const FXFontDesc& fontdesc);


#endif
