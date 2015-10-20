CC=g++
CFLAGS=`freetype-config --cflags`
LDFLAGS=`freetype-config --libs`

render_text: render_text.cpp
	$(CC) -g -Wall -std=c++11 -o render_text render_text.cpp $(CFLAGS) $(LDFLAGS)
