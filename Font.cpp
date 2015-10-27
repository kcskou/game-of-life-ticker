#include <algorithm>
#include <iostream>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.h"

Font::Font(std::string filename, int size) {
  FT_Error error;
  error = FT_Init_FreeType(&lib_);
  if (error) print_error("freetype init error", error);
    
  error = FT_New_Face(lib_, filename.c_str(), 0, &face_);
  if (error) print_error("new face error", error);

  error = FT_Set_Char_Size(face_, size * 64, 0, 72, 0);
  if (error) print_error("set char size error", error);
}

Font::~Font() {
  FT_Done_Face(face_);
  FT_Done_FreeType(lib_);
}

Bitmap* Font::render(std::string text) {
  Dimension dim = get_dimension(text);

  int y;
  int x = 0;
  char prev_char = 0;
  Bitmap* bitmap = new Bitmap(dim.width, dim.height);
  Glyph* glyph;

  for (char ch : text) {
    glyph = render_glyph(ch);
    x += kerning_offset(prev_char, ch);
    y = dim.height - dim.baseline - glyph->ascent();
    bitmap->bit_blit(glyph->bitmap(), x, y);
    x += glyph->advance();
    prev_char = ch;
    delete glyph;
  }
  return bitmap;
}

Dimension Font::get_dimension(std::string text) {
  long kerning_x;
  long target_width = 0;
  long max_ascent = 0;
  long max_descent = 0;
  char prev_char = 0;

  Glyph* glyph;
  Dimension dim;

  for (char ch : text) {
    glyph = render_glyph(ch);
    max_ascent = std::max(glyph->ascent(), max_ascent);
    max_descent = std::max(glyph->descent(), max_descent);
    kerning_x = kerning_offset(prev_char, ch);
    target_width += std::max(glyph->advance() + kerning_x,
                             static_cast<long>(glyph->width()));
    prev_char = ch;
    delete glyph;
  }

  dim.width = target_width;
  dim.height = max_ascent + max_descent;
  dim.baseline = max_descent;
  return dim;
}

Glyph* Font::render_glyph(char ch) {
  FT_Error error;
  FT_UInt glyph_index;
  glyph_index = FT_Get_Char_Index(face_, ch);
  error = FT_Load_Glyph(face_, glyph_index, 
                        FT_LOAD_RENDER | FT_LOAD_TARGET_MONO);
  if (error) print_error("load glyph error", error);
  Glyph* glyph = new Glyph(face_->glyph);
  return glyph;
}

long Font::kerning_offset(char prev, char curr) {
  if (prev == 0) {
    return 0;
  } else {
    FT_Vector kerning;
    FT_UInt prev_glyph_index = FT_Get_Char_Index(face_, prev);
    FT_UInt curr_glyph_index = FT_Get_Char_Index(face_, curr);
    FT_Get_Kerning(face_, prev_glyph_index, curr_glyph_index,
            FT_KERNING_UNFITTED, &kerning);
    return kerning.x / 64;
  }
}

void Font::print_error(std::string message, int error) {
    std::cerr << message << ": " << error << std::endl;
    exit( 1 );
}

