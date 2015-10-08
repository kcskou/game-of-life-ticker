CC=gcc
CFLAGS=`freetype-config --cflags`
LDFLAGS=`freetype-config --libs`

render_text: render_text.o
	$(CC) -o render_text render_text.o $(CFLAGS) $(LDFLAGS)
