#ifndef BITMAP_H
#define BITMAP_H

class Bitmap {
 public:
  Bitmap(unsigned int, unsigned int);
  Bitmap(unsigned int, unsigned int, unsigned char*);
  ~Bitmap();
        
  void bit_blit(Bitmap*, int, int);
  void print();

 private:
  unsigned int m_width;
  unsigned int m_height;
  unsigned char *m_pixels;
};

#endif
