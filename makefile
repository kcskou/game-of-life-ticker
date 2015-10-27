CC=g++
CFLAGS=`freetype-config --cflags` -g -Wall -std=c++11
LDFLAGS=`freetype-config --libs`

OBJECTS=render_text.o Font.o Glyph.o Bitmap.o 

all: render_text

render_text: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
 
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< $(LDFLAGS)

clean:
	rm -f *~ *.o render_text
