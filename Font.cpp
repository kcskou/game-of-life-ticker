#include "Font.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <ft2build.h>
#include FT_FREETYPE_H

Font::Font(std::string filename, int size) {
  FT_Error error;
  error = FT_Init_FreeType(&lib);
  if (error) handle_error("library init error", error);
    
  error = FT_New_Face(lib, filename.c_str(), 0, &face);
  if (error) handle_error("new face error", error);

  error = FT_Set_Char_Size(face, size * 64, 0, 72, 0);
  if (error) handle_error("set char size error", error);
}

Font::~Font() {
  FT_Done_Face(face);
  FT_Done_FreeType(lib);
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
    y = dim.height - dim.baseline - glyph->get_ascent();
    bitmap->bit_blit(glyph->get_bitmap(), x, y);
    x += glyph->get_advance();
    prev_char = ch;
  }
  return bitmap;
}

Dimension Font::get_dimension(std::string text) {
  int kerning_x;
  int target_width = 0;
  int max_ascent = 0;
  int max_descent = 0;
  char prev_char = 0;

  Glyph* glyph;
  Dimension dim;

  for (char ch : text) {
    glyph = render_glyph(ch);
    max_ascent = std::max(glyph->get_ascent(), max_ascent);
    max_descent = std::max(glyph->get_descent(), max_descent);
    kerning_x = kerning_offset(prev_char, ch);
    target_width += std::max(glyph->get_advance() + kerning_x, glyph->get_width());
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
  glyph_index = FT_Get_Char_Index(face, ch);
  error = FT_Load_Glyph(face, glyph_index, 
                        FT_LOAD_RENDER | FT_LOAD_TARGET_MONO);
  if (error) handle_error("load glyph error", error);
  Glyph* glyph = new Glyph(face->glyph);
  return glyph;
}

int Font::kerning_offset(char prev, char curr) {
  if (prev == 0) {
    return 0;
  } else {
    FT_UInt prev_glyph_index = FT_Get_Char_Index(face, prev);
    FT_UInt curr_glyph_index = FT_Get_Char_Index(face, curr);
    FT_Vector* kerning = new FT_Vector();
    FT_Get_Kerning(face, prev_glyph_index, curr_glyph_index,
            FT_KERNING_UNFITTED, kerning);
    int kerning_x = kerning->x / 64;
    delete kerning;
    return kerning_x;
  }
}

void Font::handle_error(std::string message, int error) {
    std::cout << message << ": " << error << std::endl;
    exit( 1 );
}

