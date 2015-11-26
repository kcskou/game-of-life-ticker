# game-of-life-ticker
Customizable ticker in Conway's Game Of Life based on the "Golly Ticker".

### Dependencies
[FreeType 17.4.11](http://www.freetype.org/developer.html)

### Summary
This is a work in progress. However, you can still play around with a few functional pieces of the program I envisioned: 

- ./render_text takes a string and render the text into a bitmap.
- ./print_rle takes a Game of Life pattern file in RLE format, parse it and print out the bitmap.
- ./pattern_to_rle takes a RLE file, parse it, and save the bitmap as another RLE file.
