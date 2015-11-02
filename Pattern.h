#ifndef PATTERN_H_
#define PATTERN_H_

#include <string>

#include "Bitmap.h"

class Pattern {
 public:
  Pattern(Bitmap*);
  Pattern(Bitmap*, std::string);
  ~Pattern();

  static Pattern* from_rle(std::string);
  void save_as_rle(std::string);

  Bitmap* bitmap() const;
  unsigned int width() const;
  unsigned int height() const;

 private:
  static void parse_rle_item(std::string, Bitmap*);

  unsigned int width_;
  unsigned int height_;
  Bitmap* bitmap_;
  std::string rule_;
};

#endif  // PATTERN_H_
