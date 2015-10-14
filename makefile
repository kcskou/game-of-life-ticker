CC=g++
CFLAGS=`freetype-config --cflags`
LDFLAGS=`freetype-config --libs`

render_text: render_text.cpp
	$(CC) -g -Wall -o render_text render_text.cpp $(CFLAGS) $(LDFLAGS)
