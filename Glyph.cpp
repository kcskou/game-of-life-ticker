#include "Glyph.h"
#include <algorithm>
#include <ft2build.h>
#include FT_FREETYPE_H

Glyph::Glyph(FT_GlyphSlot slot)
    : bitmap{unpack_mono_bitmap(slot->bitmap)},
      width{slot->bitmap.width},
      height{slot->bitmap.rows},
      ascent{std::max(0, (int) slot->metrics.horiBearingY / 64)},
      descent{std::max(0, (int) (slot->metrics.height / 64) - ascent)},
      advance{slot->advance.x / 64} {}

Glyph::~Glyph() { delete bitmap; }

Bitmap* Glyph::get_bitmap() { return bitmap; }
int Glyph::get_width() { return width; }
int Glyph::get_height() { return height; }
int Glyph::get_ascent() { return ascent; }
int Glyph::get_descent() { return descent; }
int Glyph::get_advance() { return advance; }

Bitmap* Glyph::unpack_mono_bitmap(FT_Bitmap src) {
  unsigned char* dst_buffer = 
      (unsigned char*) calloc(src.width * src.rows, sizeof *dst_buffer);
  int i, j, k;
  int num_bits_done, dst_start_index, bits_to_unpack;
  char byte, bit;
  for (i = 0; i < (int) src.rows; i++) {
    for (j = 0; j < src.pitch; j++) {
      byte = src.buffer[i * src.pitch + j];
      num_bits_done = j * 8;
      dst_start_index = i * src.width + num_bits_done;
      bits_to_unpack = std::min((int) src.width - num_bits_done, 8);
      for ( k = 0; k < bits_to_unpack; k++ ) {
        bit = byte & (1 << (7 - k));
        dst_buffer[dst_start_index + k] = bit ? 1 : 0;
      }
    }
  }
  Bitmap* bitmap = new Bitmap(src.width, src.rows, dst_buffer);
  return bitmap;
}

