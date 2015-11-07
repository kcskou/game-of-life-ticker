#ifndef RLE_WRITER_H_
#define RLE_WRITER_H_

#include <fstream>
#include <sstream>
#include <string>

class Rle_Writer {
 public:
  Rle_Writer(std::ofstream&);
  ~Rle_Writer();

  void write(std::stringstream&);

 private:
  const unsigned int kCharLimit = 70;
  std::ofstream& outfile_;
  std::string buffer_;
  unsigned int num_char_;
};

#endif  // RLE_WRITER_H_
