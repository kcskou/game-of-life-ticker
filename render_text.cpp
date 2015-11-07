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

  Pattern* weekender = Pattern::from_rle("patterns/weekender.rle");
  std::cout << "print weekender:" << std::endl;
  weekender->bitmap()->print();
  weekender->save_as_rle("patterns/myweekender.rle");
  delete weekender;

  Pattern* ticker = Pattern::from_rle("patterns/TickerEXTemplate.rle");
  ticker->save_as_rle("patterns/myticker.rle");
  delete ticker;

  return 0;
}
