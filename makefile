CC=gcc
CFLAGS=`freetype-config --cflags`
LDFLAGS=`freetype-config --libs`

render_text: render_text.c
	$(CC) -g -Wall -o render_text render_text.c $(CFLAGS) $(LDFLAGS)
