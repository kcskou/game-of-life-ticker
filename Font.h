#ifndef FONT_H
#define FONT_H

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Bitmap.h" 
#include "Glyph.h"

struct Dimension {
  int width;
  int height;
  int baseline;
};

class Font {
 public:
  Font(std::string, int);
  ~Font();

  Bitmap* render(std::string);

 private:
  Dimension get_dimension(std::string);
  Glyph* render_glyph(char);
  long kerning_offset(char, char);
  void print_error(std::string, int);

  FT_Library lib_;
  FT_Face face_;
};

#endif
