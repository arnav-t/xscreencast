#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void getPixel(Display *disp, int x, int y, XColor *color)
{
	// Initialize 1x1 image
	XImage *img = XGetImage(disp, RootWindow(disp, DefaultScreen(disp)), x, y, 1, 1, AllPlanes, XYPixmap);
	
	// Get pixel value
	color->pixel = XGetPixel(img, 0, 0);
	
	// Free memory
	XFree(img);

	// Get color
	XQueryColor(disp, DefaultColormap(disp, DefaultScreen(disp)), color);
}

void saveImage(Display *disp, char file[])
{
	// Get default screen for the display
	Screen *scr = XDefaultScreenOfDisplay(disp);

	// Get the screen resolution
	int w, h;
	w = WidthOfScreen(scr);
	h = HeightOfScreen(scr);

	// Initialize image of screen
	XImage *img = XGetImage(disp, RootWindow(disp, DefaultScreen(disp)), 0, 0, w, h, AllPlanes, XYPixmap);

	// Allocate memory for image
	unsigned char *pixels = (unsigned char *)malloc(3*w*h*sizeof(unsigned char));

	// Save colors into memory
	for(int x = 0; x < w; ++x)
	{
		for(int y = 0; y < h; ++y)
		{
			// Query color
			XColor color;
			color.pixel = XGetPixel(img, x, y);
			XQueryColor(disp, DefaultColormap(disp, DefaultScreen(disp)), &color);

			// Save color
			*(pixels + 3*(y*w + x%w)) = color.red/256;
			*(pixels + 3*(y*w + x%w) + 1) = color.green/256;
			*(pixels + 3*(y*w + x%w) + 2) = color.blue/256;
		}
	}

	// Free memory
	XFree(img);

	// Save image
	stbi_write_jpg(file, w, h, 3, pixels, 100);	

	// Free pixels
	free(pixels);
}

int main(int argc, char *argv[])
{
	// Connect to X server
	Display *disp = XOpenDisplay(NULL);

	printf("Including stb_image_write.h...\n");
	printf("Saving as %s...\n", argv[1]);
	saveImage(disp, argv[1]);
	printf("Saved screenshot successfully.\n");

	// Close connection to X server
	XCloseDisplay(disp);

	return 0;
}