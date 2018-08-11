CC = gcc
LIBS = -lX11

xscreencast: xscreencast.o stb_image_write.h.gch
	$(CC) xscreencast.o -o xscreencast $(LIBS)

xscreencast.o: xscreencast.c stb_image_write.h.gch
	$(CC) -c xscreencast.c $(LIBS)

stb_image_write.h.gch: stb_image_write.h
	$(CC) -c stb_image_write.h $(LIBS)

clean:
	rm *.o *.h.gch