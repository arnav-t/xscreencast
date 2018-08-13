#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
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

void saveImage(Display *disp, char file[], int verbose)
{
	// Get default screen for the display
	Screen *scr = XDefaultScreenOfDisplay(disp);

	// Get the screen resolution
	int w, h;
	w = WidthOfScreen(scr);
	h = HeightOfScreen(scr);

	// Initialize image of screen
	if(verbose)
		printf("Initializing image...\n");
	XImage *img = XGetImage(disp, RootWindow(disp, DefaultScreen(disp)), 0, 0, w, h, AllPlanes, ZPixmap);

	// Allocate memory for image
	if(verbose)
		printf("Allocating memory...\n");
	unsigned char *pixels = (unsigned char *)malloc(3*w*h*sizeof(unsigned char));

	// Save colors into memory
	if(verbose)
		printf("Generating color matrix...\n");
	unsigned char *imgPtr = img->data;
	for(int y = 0; y < h; ++y)
	{
		for(int x = 0; x < w; ++x)
		{
			// Save color
			*(pixels + 3*(y*w + x%w)) = *(imgPtr + 4*(y*w + x%w) + 2); 
			*(pixels + 3*(y*w + x%w) + 1) = *(imgPtr + 4*(y*w + x%w) + 1); 
			*(pixels + 3*(y*w + x%w) + 2) = *(imgPtr + 4*(y*w + x%w) + 0);
		}
	}

	// Free memory
	XFree(img);
	
	// Save image
	if(verbose)
		printf("Writing image...\n");
	stbi_write_jpg(file, w, h, 3, pixels, 0);

	// Free pixels
	free(pixels);
}

int main(int argc, char *argv[])
{
	// Initialize delay
	double delay = 1;

	// Check command line arguments
	int verbose = 0;
	int opt;
	while((opt = getopt(argc, argv, "v")) != -1) 
	{
		switch(opt)
		{
			case 'v':
				verbose = 1;
				break;
			default:
				fprintf(stderr, "Usage: xscreencast [-v] [port]\n");
				exit(EXIT_FAILURE);
		}
	}	
	int port = atoi(argv[argc - 1]);
	printf("Using port %d.\n", port);

	// Connect to X server
	Display *disp = XOpenDisplay(NULL);

	while(1)
	{
		clock_t start = clock();

		if(verbose)	
			printf("Saving as %s...\n", IMAGE);
		saveImage(disp, IMAGE, verbose);
		if(verbose)
			printf("Saved screenshot successfully.\n");

		// Launch server
		server(port, delay, verbose);

		delay = (double)(clock() - start)/CLOCKS_PER_SEC + 0.1;
		if(verbose)
			printf("Frame completed in %.2lf s\n", delay);
	}

	// Close connection to X server
	XCloseDisplay(disp);

	return 0;
}