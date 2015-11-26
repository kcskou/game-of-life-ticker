#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#include "Pattern.h"
#include "Rle_Writer.h"

Pattern::Pattern(Bitmap* bitmap)
    : width_{bitmap->width()},
      height_{bitmap->height()},
      bitmap_{bitmap} {}

Pattern::Pattern(Bitmap* bitmap, std::string rule)
    : width_{bitmap->width()},
      height_{bitmap->height()},
      bitmap_{bitmap},
      rule_{rule} {}

Pattern::~Pattern() { delete bitmap_; }

Pattern* Pattern::from_rle(std::string file) {
  Bitmap* bitmap;
  unsigned int width;
  unsigned int height;
  std::string rule;

  bool found_header = false;
  bool done = false;
  std::ifstream in(file.c_str());
  std::string line;

  std::regex re_x(R"(x\s*=\s*(\d+))");
  std::regex re_y(R"(y\s*=\s*(\d+))");
  std::regex re_rule(R"(rule\s*=\s*([BS]?\d+/[BS]?\d+))");
  std::regex re_item(R"(((\d+)?[bo\$]))");
  std::smatch match;

  while (std::getline(in, line) && !done) {
    size_t non_whitespace = line.find_first_not_of(" \t\n\v\f\r");
    if (non_whitespace != std::string::npos && line[non_whitespace] != '#') {
      if (found_header) {
        if (line.find("!") != std::string::npos) { done = true; }
        while (std::regex_search(line, match, re_item)) {
          Pattern::parse_rle_item(match[1].str(), bitmap);
          line = match.suffix().str();
        }
      } else {
        if (std::regex_search(line, match, re_x)) {
          width = static_cast<unsigned int>(std::stoul(match[1].str()));
        }
        if (std::regex_search(line, match, re_y)) {
          height = static_cast<unsigned int>(std::stoul(match[1].str()));
        }
        if (std::regex_search(line, match, re_rule)) {
          rule = match[1].str();
        }
        if (width && height) {
          bitmap = new Bitmap(width, height);
          found_header = true;
        } else {
          std::cerr << "missing or illegal RLE file header" << std::endl;
          exit(1);
        }
      }
    }
  }
  Pattern* p = new Pattern(bitmap, rule);
  return p;
}

void Pattern::save_as_rle(std::string outfile) {
  std::ofstream rle_file(outfile.c_str());
  if (rle_file.is_open()) {
    std::stringstream ss;
    ss << "x = " << width_ << ", " << "y = " << height_;
    Rle_Writer writer{rle_file};
    if (!rule_.empty()) {
      ss << ", " << "rule = " << rule_;
    }
    ss << std::endl;
    
    unsigned char* pixels =  bitmap_->pixels();
    unsigned int num_eol = 1;
    for (unsigned int row = 0; row < height_; row++) {
      unsigned int run_count = 1;
      for (unsigned int col = 0; col < width_ - 1; col++) {
        unsigned char current_bit = pixels[row * width_ + col];
        unsigned char next_bit = pixels[row * width_ + col + 1];
        if (current_bit != next_bit) {
            if (run_count > 1) { ss << run_count; }
            ss << (current_bit ? 'o' : 'b');
            run_count = 1;
        } else {
            run_count++;
        }
      }
      unsigned char last_bit = pixels[(row + 1) * width_ - 1];
      if (last_bit) {
        // line ends with 1, print the live cells 
        if (run_count > 1) { ss << run_count; }
        ss << 'o';
        num_eol = 1;
      } else if (run_count == width_ && row != 0) {
        // empty line, move back cursor and update run_count of $
        unsigned int move_back = (num_eol == 1) ? 1 : num_digits(num_eol) + 1;
        long pos = ss.tellp();
        ss.seekp(pos - move_back);
        ss << ++num_eol;
      } else {
        // not empty line, reset num_eol
        num_eol = 1;
      }
      if (row != height_ - 1) {
          // not the last row, print $
          ss << '$'; 
      }
    }
    ss << '!';
    writer.write(ss);
  } else {
    std::cerr << "Unable to open file to save pattern." << std::endl;
    exit(1);
  }
}

Bitmap* Pattern::bitmap() const { return bitmap_; }
unsigned int Pattern::width() const { return width_; }
unsigned int Pattern::height() const { return height_; }

void Pattern::parse_rle_item(std::string item, Bitmap* bitmap) {
  std::regex re_split_item(R"((\d+)?([bo\$]))");
  std::smatch match;
  while (std::regex_search(item, match, re_split_item)) {
    unsigned int run_count = match[1].length() ? std::stoul(match[1].str()): 1;
    std::string tag = match[2].str();
    if (tag == "$") {
      unsigned int offset = bitmap->cursor() % bitmap->width();
      unsigned int bits_to_pad = offset ? bitmap->width() - offset : 0;
      if (!bitmap->clear_bits(bits_to_pad)) {
        std::cerr << "error: trying to edit a completed bitmap" << std::endl;
        exit(1);
      }
      if (run_count > 1) {
        if (!bitmap->clear_bits((run_count - 1) * bitmap->width())) {
          std::cerr << "error: trying to edit a completed bitmap" << std::endl;
          exit(1);
        }
      }
    } else if (tag == "o") {
      if (!bitmap->set_bits(run_count)) {
        std::cerr << "error: trying to edit a completed bitmap" << std::endl;
        exit(1);
      }
    } else if (tag == "b") {
      if (!bitmap->clear_bits(run_count)) {
        std::cerr << "error: trying to edit a completed bitmap" << std::endl;
        exit(1);
      }
    }
    item = match.suffix().str();
  }
}

unsigned int Pattern::num_digits(unsigned int num) {
  unsigned int digits = 0;
  if (num < 0) digits = 1;
  while (num) {
    num /= 10;
    digits++;
  }
  return digits;
}
