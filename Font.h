#ifndef FONT_H
#define FONT_H

#include <string>
#include "Bitmap.h" 
#include "Glyph.h"
#include <ft2build.h>

struct Dimension {
  int width, height, baseline;
};

class Font {
 public:
  Font(std::string, int);
  ~Font();

  Bitmap* render(std::string);

 private:
  Dimension get_dimension(std::string);
  Glyph* render_glyph(char);
  int kerning_offset(char, char);
  void handle_error(std::string, int);

  FT_Library lib;
  FT_Face face;
};

#endif
