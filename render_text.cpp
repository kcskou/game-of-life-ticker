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
  // Pattern* glider = Pattern::from_rle("patterns/glider.rle");
  // std::cout << "print glider:" << std::endl;
  // glider->bitmap()->print();
  // delete glider;

  // Pattern* loaf = Pattern::from_rle("patterns/loaf.rle");
  // std::cout << "print loaf:" << std::endl;
  // loaf->bitmap()->print();
  // delete loaf;

  // Pattern* weekender = Pattern::from_rle("patterns/weekender.rle");
  // std::cout << "print weekender:" << std::endl;
  // weekender->bitmap()->print();
  // weekender->save_as_rle("patterns/myweekender.rle");
  // delete weekender;

  // Pattern* empty = Pattern::from_rle("patterns/empty.rle");
  // empty->bitmap()->print();
  // delete empty;

  Bitmap* zebra = new Bitmap(4, 6);
  int n = 3;
  while (n--) {
    zebra->set_bits(2);
  }
  zebra->clear_bits(4);
  zebra->clear_bits(4);
  zebra->set_bits(4);
  Pattern* test = new Pattern(zebra);
  test->bitmap()->print();
  test->save_as_rle("patterns/test.rle");
  delete test;


  return 0;
}
