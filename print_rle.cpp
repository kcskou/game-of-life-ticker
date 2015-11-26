#include <iostream>
#include <cstdlib>
#include <string>
#include "Font.h"
#include "Bitmap.h"
#include "Pattern.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " your-life-pattern.rle" << std::endl;
    exit(1);
  }
  std::string rle_file = argv[1];
  std::cout << "Parsing " << rle_file << "..." << std::endl;
  Pattern* pattern = Pattern::from_rle(rle_file);
  std::cout << "Printing " << rle_file << "..." << std::endl;
  pattern->bitmap()->print();
  delete pattern;
  return 0;
}
