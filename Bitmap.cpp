#include <iostream>
#include <cstdlib>
#include "Bitmap.h"

Bitmap::Bitmap(unsigned int width, unsigned int height)
    : m_width{width},
      m_height{height} {
  m_pixels = (unsigned char*) calloc(width * height, sizeof *m_pixels);
} 

Bitmap::Bitmap(unsigned int width, unsigned int height, unsigned char *pixels) 
    : m_width{width},
      m_height{height},
      m_pixels{pixels} {}

Bitmap::~Bitmap() { delete m_pixels; }

void Bitmap::bit_blit(Bitmap* src, int x, int y) {
  unsigned int i, j;
  int src_index = 0;
  int row_offset = m_width - src->m_width;
  int dst_index = y * m_width + x;
  for (i = 0; i < src->m_height; i++) {
    for (j = 0; j < src->m_width; j++) {
      m_pixels[dst_index++] |= src->m_pixels[src_index++];
    }
    dst_index += row_offset;
  }
}

void Bitmap::print() {
  unsigned int i, j;
  for (i = 0; i < m_height; i++) {
    for (j = 0; j < m_width; j++) {
      std::cout << (m_pixels[(i * m_width) + j] == 0 ? '.' : '#');
    }
    std::cout << std::endl;
  }
}
