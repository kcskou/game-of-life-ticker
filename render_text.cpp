#include <iostream>
#include <cstdlib>
#include <string>
#include "Font.h"
#include "Bitmap.h"
#include "Pattern.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " text-to-render" << std::endl;
    exit(1);
  }

  Font* font = new Font("arial.ttf", 24);
  Bitmap* bitmap = font->render(argv[1]);
  bitmap->print();
  delete bitmap;
  delete font;

  // testing rle parser
  Pattern* glider = Pattern::from_rle("glider.rle");
  Pattern* loaf = Pattern::from_rle("loaf.rle");
  Pattern* weekender = Pattern::from_rle("weekender.rle");
  std::cout << "print glider:" << std::endl;
  glider->bitmap()->print();
  std::cout << "print loaf:" << std::endl;
  loaf->bitmap()->print();
  std::cout << "print weekender:" << std::endl;
  weekender->bitmap()->print();
  delete glider;
  delete loaf;
  delete weekender;

  return 0;
}
