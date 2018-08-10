#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

void getPixel(Display *disp, int x, int y, XColor *color)
{
	// Initialize image
	XImage *img = XGetImage(disp, RootWindow(disp, DefaultScreen(disp)), x, y, 1, 1, AllPlanes, XYPixmap);
	
	// Get pixel value
	color->pixel = XGetPixel(img, x, y);
	
	// Free memory
	XFree(img);

	// Get color
	XQueryColor(disp, DefaultColormap(disp, DefaultScreen(disp)), color);
}

int main(int argc, char *argv[])
{
	// Connect to X server
	Display *disp = XOpenDisplay(NULL);

	// Get default screen for the display
	Screen *scr = XDefaultScreenOfDisplay(disp);

	printf("Width: %d, Height: %d\n", WidthOfScreen(scr), HeightOfScreen(scr));
	XColor color;
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	printf("Getting pixel (%d, %d)\n", x, y);
	getPixel(disp, x, y, &color);
	printf("rgb(%d, %d, %d)\n", color.red/256, color.green/256, color.blue/256);

	// Close connection to X server
	XCloseDisplay(disp);

	return 0;
}