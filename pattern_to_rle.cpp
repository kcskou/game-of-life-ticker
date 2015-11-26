#include <iostream>
#include <cstdlib>
#include <string>
#include "Font.h"
#include "Bitmap.h"
#include "Pattern.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " input-life-pattern.rle "
      << " output-life-pattern.rle" << std::endl;
    exit(1);
  }
  std::string input_rle_file = argv[1];
  std::string output_rle_file = argv[2];
  std::cout << "Parsing " << input_rle_file << "..." << std::endl;
  Pattern* pattern = Pattern::from_rle(input_rle_file);
  std::cout << "Saving " << output_rle_file << "..." << std::endl;
  pattern->save_as_rle(output_rle_file);
  std::cout << "Done!" << std::endl;
  delete pattern;
  return 0;
}
