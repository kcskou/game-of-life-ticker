# game-of-life-ticker
Customizable ticker in Conway's Game Of Life based on the "Golly Ticker".

![The Original Golly Ticker](http://golly.sourceforge.net/ticker.gif)

### Dependencies
[FreeType 17.4.11](http://www.freetype.org/developer.html)

### Summary
As you can see above, the Golly Ticker pattern prints out "Golly" when it is run in the Game of Life. The idea is to write a generator that outputs a ticker that prints any user defined messages or images.

This is a work in progress. However, you can still play around with a few functional pieces of the program I envisioned: 

- ./render_text takes a string and render the text into a bitmap.
- ./print_rle takes a Game of Life pattern file in RLE format, parse it and print out the bitmap.
- ./pattern_to_rle takes a RLE file, parse it, and save the bitmap as another RLE file.

