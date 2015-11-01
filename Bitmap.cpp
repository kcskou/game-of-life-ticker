#include <cstdlib>
#include <iostream>

#include "Bitmap.h"

Bitmap::Bitmap(unsigned int width, unsigned int height)
    : width_{width},
      height_{height} {
  pixels_ = new unsigned char[width * height]();
}

Bitmap::Bitmap(unsigned int width, unsigned int height, unsigned char* pixels)
    : width_{width},
      height_{height},
      pixels_{pixels} {}

Bitmap::~Bitmap() { delete[] pixels_; }

void Bitmap::bit_blit(Bitmap* src, int x, int y) {
  unsigned int i, j;
  int src_index = 0;
  int row_offset = width_ - src->width_;
  int dst_index = y * width_ + x;
  for (i = 0; i < src->height_; i++) {
    for (j = 0; j < src->width_; j++) {
      pixels_[dst_index++] |= src->pixels_[src_index++];
    }
    dst_index += row_offset;
  }
}

bool Bitmap::set_bits(unsigned int num_bits) {
  return write_bits(num_bits, true);
}

bool Bitmap::clear_bits(unsigned int num_bits) {
  return write_bits(num_bits, false);
}

void Bitmap::print() {
  unsigned int i, j;
  for (i = 0; i < height_; i++) {
    for (j = 0; j < width_; j++) {
      std::cout << (pixels_[(i * width_) + j] == 0 ? '.' : '#');
    }
    std::cout << std::endl;
  }
}

bool Bitmap::write_bits(unsigned int num_bits, bool set_bits) {
  unsigned char bit_value = set_bits ? 1 : 0;
  while (num_bits--) {
    if (cursor_ >= width_ * height_) { return false; }
    pixels_[cursor_++] = bit_value;
  }
  return true;
}

unsigned int Bitmap::width() const { return width_; }
unsigned int Bitmap::height() const { return height_; }
unsigned int Bitmap::cursor() const { return cursor_; }
