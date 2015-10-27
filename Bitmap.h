#ifndef BITMAP_H
#define BITMAP_H

class Bitmap {
 public:
  Bitmap(unsigned int, unsigned int);
  Bitmap(unsigned int, unsigned int, unsigned char*);
  ~Bitmap();
        
  void bit_blit(Bitmap*, int, int);
  void print();

  unsigned int width() const;
  unsigned int height() const;

 private:
  unsigned int width_;
  unsigned int height_;
  unsigned char* pixels_;
};

#endif
