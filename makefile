CXXFLAGS=-g -Wall -std=c++11 `freetype-config --cflags` 
LDLIBS=`freetype-config --libs`

TARGETS=render_text print_rle pattern_to_rle
OBJECTS=Font.o Glyph.o Bitmap.o Pattern.o Rle_Writer.o

all: $(TARGETS) 

render_text: render_text.o $(OBJECTS)
	$(CXX) $^ $(LDLIBS) -o $@ 

print_rle: print_rle.o $(OBJECTS)
	$(CXX) $^ $(LDLIBS) -o $@ 

pattern_to_rle: pattern_to_rle.o $(OBJECTS)
	$(CXX) $^ $(LDLIBS) -o $@ 
  
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< 

clean:
	rm -f *~ *.o $(TARGETS)
