#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "HTTPServer.h"

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

void saveImage(Display *disp, char file[], int downscale)
{
	// Get default screen for the display
	Screen *scr = XDefaultScreenOfDisplay(disp);

	// Get the screen resolution
	int w, h;
	w = WidthOfScreen(scr);
	h = HeightOfScreen(scr);

	// Initialize image of screen
	printf("Initializing image...\n");
	XImage *img = XGetImage(disp, RootWindow(disp, DefaultScreen(disp)), 0, 0, w, h, AllPlanes, XYPixmap);

	// Allocate memory for image
	printf("Allocating memory...\n");
	unsigned char *pixels = (unsigned char *)malloc(3*(w/downscale)*(h/downscale)*sizeof(unsigned char));

	// Save colors into memory
	printf("Generating color matrix...\n");
	XColor color;
	Colormap colormap = DefaultColormap(disp, DefaultScreen(disp));
	for(int y = 0; y < h/downscale; ++y)
	{
		for(int x = 0; x < w/downscale; ++x)
		{
			// Query color
			color.pixel = XGetPixel(img, x*downscale, y*downscale);
			XQueryColor(disp, colormap, &color);

			// Save color
			*(pixels + 3*(y*w/downscale + x%(w/downscale))) = color.red/256;
			*(pixels + 3*(y*w/downscale + x%(w/downscale)) + 1) = color.green/256;
			*(pixels + 3*(y*w/downscale + x%(w/downscale)) + 2) = color.blue/256;
		}
	}

	// Free memory
	XFree(img);

	// Save image
	printf("Writing image...\n");
	stbi_write_jpg(file, w/downscale, h/downscale, 3, pixels, 0);

	// Free pixels
	free(pixels);
}

int main(int argc, char *argv[])
{
	server(atoi(argv[1]));
	/*clock_t start = clock();

	// Connect to X server
	Display *disp = XOpenDisplay(NULL);

	printf("Including stb_image_write.h...\n");
	printf("Saving as %s...\n", argv[1]);
	saveImage(disp, argv[1], atoi(argv[2]));
	printf("Saved screenshot successfully.\n");

	// Close connection to X server
	XCloseDisplay(disp);

	double execTime = (double)(clock() - start)/CLOCKS_PER_SEC;
	printf("Process completed in %.2lf s\n", execTime);*/

	return 0;
}