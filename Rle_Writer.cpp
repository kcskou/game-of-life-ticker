#include <fstream>
#include <string>
#include "Rle_Writer.h"

Rle_Writer::Rle_Writer(std::ofstream& outfile)
    : outfile_{outfile},
      num_char_{0} {}

Rle_Writer::~Rle_Writer() { outfile_.close(); }

void Rle_Writer::write(std::stringstream& in) {
  char ch = '\0';
  while (in.get(ch)) {
    if (++num_char_ == kCharLimit) {
      outfile_ << std::endl << buffer_;
      num_char_ = buffer_.length();
      buffer_.clear();
    } else if (ch == 'o' || ch == 'b' || ch == '$') {
      buffer_ += ch;
      outfile_ << buffer_;
      buffer_.clear();
      continue;
    }
    buffer_ += ch;
  }
  outfile_ << buffer_;
  in.str(std::string());
  in.clear();
}
