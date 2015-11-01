#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "Pattern.h"

Pattern::Pattern(Bitmap* bitmap) : bitmap_{bitmap} {}

Pattern::Pattern(Bitmap* bitmap, std::string rule)
    : bitmap_{bitmap},
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
          width = std::stoul(match[1].str());
        }
        if (std::regex_search(line, match, re_y)) {
          height = std::stoul(match[1].str());
        }
        if (std::regex_search(line, match, re_rule)) {
          rule = match[1].str();
          std::cout << "rule: " << rule << std::endl;
        }
        if (width && height) {
          std::cout << "width: " << width << std::endl;
          std::cout << "height: " << height << std::endl;
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

Bitmap* Pattern::bitmap() const { return bitmap_; }
unsigned int Pattern::width() const { return bitmap_->width(); }
unsigned int Pattern::height() const { return bitmap_->height(); }

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
