CC=g++
CFLAGS=`freetype-config --cflags`
LDFLAGS=`freetype-config --libs`

render_text: render_text.o Font.o Bitmap.o Glyph.o
	$(CC) $(CFLAGS) $(LDFLAGS) -g -Wall -std=c++11 -o render_text
clean:
	rm -f *~ *.o
