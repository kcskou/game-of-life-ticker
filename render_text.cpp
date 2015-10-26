#include <iostream>
#include <cstdlib>
#include <string>
#include "Font.h"
#include "Bitmap.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " text-to-render" << std::endl;
    exit(1);
  }

  Font* font = new Font("arial.ttf", 24);
  Bitmap* bitmap = font->render(argv[1]);
  bitmap->print(); // FIXME override operator << to print to cout
  delete bitmap;
  delete font;
  return 0;
}
