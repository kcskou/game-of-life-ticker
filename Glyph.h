#ifndef GLYPH_H
#define GLYPH_H

#include "Bitmap.h"
#include <ft2build.h>
#include FT_FREETYPE_H
 
class Glyph {
 public:
  Glyph(FT_GlyphSlot);
  ~Glyph();
   
  Bitmap* get_bitmap();
  int get_width();
  int get_height();
  int get_ascent();
  int get_descent();
  int get_advance();

 private:
  Bitmap* unpack_mono_bitmap(FT_Bitmap);

  Bitmap* bitmap;
  int width;
  int height;
  int ascent;
  int descent;
  int advance;
};

#endif
