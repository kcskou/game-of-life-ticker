#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Glyph.h"

Glyph::Glyph(FT_GlyphSlot slot)
    : bitmap_{unpack_mono_bitmap(slot->bitmap)},
      ascent_{std::max(static_cast<long>(0), slot->metrics.horiBearingY / 64)},
      descent_{std::max(static_cast<long>(0),
               (slot->metrics.height / 64) - ascent_)},
      advance_{slot->advance.x / 64} {}

Glyph::~Glyph() { delete bitmap_; }

Bitmap* Glyph::bitmap() const { return bitmap_; }
unsigned int Glyph::width() const { return bitmap_->width(); }
unsigned int Glyph::height() const { return bitmap_->height(); }
long Glyph::ascent() const { return ascent_; }
long Glyph::descent() const { return descent_; }
long Glyph::advance() const { return advance_; }

Bitmap* Glyph::unpack_mono_bitmap(FT_Bitmap src) {
  unsigned char* dst_buffer = new unsigned char[src.width * src.rows]();
  int i, j, k;
  int num_bits_done, dst_start_index, bits_to_unpack;
  char byte, bit;
  for (i = 0; i < static_cast<int>(src.rows); i++) {
    for (j = 0; j < src.pitch; j++) {
      byte = src.buffer[i * src.pitch + j];
      num_bits_done = j * 8;
      dst_start_index = i * src.width + num_bits_done;
      bits_to_unpack = std::min(static_cast<int>(src.width) - num_bits_done, 8);
      for ( k = 0; k < bits_to_unpack; k++ ) {
        bit = byte & (1 << (7 - k));
        dst_buffer[dst_start_index + k] = bit ? 1 : 0;
      }
    }
  }
  Bitmap* bitmap = new Bitmap(src.width, src.rows, dst_buffer);
  return bitmap;
}

