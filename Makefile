libs = -lX11
cc = gcc

xscreencast: xscreencast.o
	$(cc) xscreencast.o -o xscreencast $(libs) 

xscreencast.o: xscreencast.c
	$(cc) -c xscreencast.c $(libs)

clean:
	rm *.o