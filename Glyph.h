#ifndef GLYPH_H
#define GLYPH_H

#include "Bitmap.h"
#include <ft2build.h>
#include FT_FREETYPE_H
 
class Glyph {
 public:
  Glyph(FT_GlyphSlot);
  ~Glyph();
   
  Bitmap* bitmap() const;
  unsigned int width() const;
  unsigned int height() const;
  long ascent() const;
  long descent() const;
  long advance() const;

 private:
  Bitmap* unpack_mono_bitmap(FT_Bitmap);

  Bitmap* bitmap_;
  long ascent_;
  long descent_;
  long advance_;
};

#endif
