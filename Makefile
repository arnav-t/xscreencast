CC = gcc
LIBS = -lX11


xscreencast: xscreencast.o
	$(CC) xscreencast.o -o xscreencast $(LIBS) 

xscreencast.o: xscreencast.c
	$(CC) -c xscreencast.c $(LIBS)

clean:
	rm *.o