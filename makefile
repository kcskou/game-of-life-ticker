CXXFLAGS=-g -Wall -std=c++11 `freetype-config --cflags` 
LDLIBS=`freetype-config --libs`

OBJECTS=render_text.o Font.o Glyph.o Bitmap.o Pattern.o Rle_Writer.o

all: render_text

render_text: $(OBJECTS)
	$(CXX) $^ $(LDLIBS) -o $@ 
 
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< 

clean:
	rm -f *~ *.o render_text
