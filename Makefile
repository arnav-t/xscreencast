CC = gcc
LIBS = -lX11
FLAGS = -std=c99

xscreencast: xscreencast.o stb_image_write.h.gch HTTPServer.h.gch
	$(CC) xscreencast.o -o xscreencast $(FLAGS) $(LIBS)

xscreencast.o: xscreencast.c stb_image_write.h.gch HTTPServer.h.gch
	$(CC) -c xscreencast.c $(FLAGS) $(LIBS)

HTTPServer.h.gch: HTTPServer.h
	$(CC) -c HTTPServer.h $(FLAGS) $(LIBS)

stb_image_write.h.gch: stb_image_write.h
	$(CC) -c stb_image_write.h $(FLAGS) $(LIBS)

clean:
	rm *.o *.h.gch

install: xscreencast
	cp xscreencast /usr/local/bin/xscreencast
	mkdir -p ~/.xscreencast
	cp home.html ~/.xscreencast/home.html

uninstall:
	rm /usr/local/bin/xscreencast
	rm -rf ~/.xscreencast