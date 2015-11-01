#ifndef BITMAP_H_
#define BITMAP_H_

class Bitmap {
 public:
  Bitmap(unsigned int, unsigned int);
  Bitmap(unsigned int, unsigned int, unsigned char*);
  ~Bitmap();

  void bit_blit(Bitmap*, int, int);
  bool set_bits(unsigned int);
  bool clear_bits(unsigned int);
  void print();

  unsigned int width() const;
  unsigned int height() const;
  unsigned int cursor() const;

 private:
  bool write_bits(unsigned int, bool);

  unsigned int width_;
  unsigned int height_;
  unsigned int cursor_;
  unsigned char* pixels_;
};

#endif  // BITMAP_H_
