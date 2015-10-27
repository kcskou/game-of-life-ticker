#include <iostream>
#include <cstdlib>
#include "Bitmap.h"

Bitmap::Bitmap(unsigned int width, unsigned int height)
    : width_{width},
      height_{height} {
  pixels_ = (unsigned char*) calloc(width * height, sizeof *pixels_);
} 

Bitmap::Bitmap(unsigned int width, unsigned int height, unsigned char *pixels) 
    : width_{width},
      height_{height},
      pixels_{pixels} {}

Bitmap::~Bitmap() { delete pixels_; }

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

void Bitmap::print() {
  unsigned int i, j;
  for (i = 0; i < height_; i++) {
    for (j = 0; j < width_; j++) {
      std::cout << (pixels_[(i * width_) + j] == 0 ? '.' : '#');
    }
    std::cout << std::endl;
  }
}

unsigned int Bitmap::width() const { return width_; }
unsigned int Bitmap::height() const { return height_; }
