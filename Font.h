#ifndef FONT_H_
#define FONT_H_

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Bitmap.h"
#include "Glyph.h"

struct Dimension {
  long width;
  long height;
  long baseline;
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

#endif  // FONT_H_
