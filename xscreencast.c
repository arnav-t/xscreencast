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
	// Get the screen resolution
	int w, h;
	w = WidthOfScreen(XDefaultScreenOfDisplay(disp));
	h = HeightOfScreen(XDefaultScreenOfDisplay(disp));

	// Initialize image of screen
	if(verbose)
		printf("Initializing image... ");
	XImage *img = XGetImage(disp, RootWindow(disp, DefaultScreen(disp)), 0, 0, w, h, AllPlanes, ZPixmap);
	if(verbose)
		printf("[done]\n");

	// Allocate memory for image
	if(verbose)
		printf("Allocating memory... ");
	unsigned char *pixels = (unsigned char *)malloc(3*w*h*sizeof(unsigned char));
	if(verbose)
		printf("[done]\n");

	// Save colors into memory
	if(verbose)
		printf("Generating color matrix... ");
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
	if(verbose)
		printf("[done]\n");

	// Free memory
	XFree(img);
	free(imgPtr);
	
	// Save image
	if(verbose)
		printf("Writing image... ");
	stbi_write_jpg(file, w, h, 3, pixels, 0);
	if(verbose)
		printf("[done]\n");

	// Free pixels
	free(pixels);
}

int main(int argc, char *argv[])
{
	// Initialize delay
	double minDelay = 0.5;
	double delay;

	// Check command line arguments
	int verbose = 0;
	int opt;
	while((opt = getopt(argc, argv, "vm")) != -1) 
	{
		switch(opt)
		{
			case 'v':
				verbose = 1;
				break;
			case 'm':
				minDelay = 0.1;
				break;
			default:
				fprintf(stderr, "Usage: xscreencast [OPTION]... [PORT]\n\nOptions:\n\t-v\t\tVerbose output\n\t-m\t\tMaximum framerate\n");
				exit(EXIT_FAILURE);
		}
	}	
	int port = atoi(argv[argc - 1]);
	printf("Using port %d.\n", port);

	// Connect to X server
	Display *disp = XOpenDisplay(NULL);
	clock_t start;
	while(1)
	{
		// Record start time
		start = clock();

		// Save current frame
		saveImage(disp, IMAGE, verbose);

		// Record frame time
		delay = (double)(clock() - start)/CLOCKS_PER_SEC;
		if(verbose)
			printf("Frame completed in %.2lf s\n", delay);

		// Launch server
		server(port, delay + minDelay, verbose);
	}

	// Close connection to X server
	XCloseDisplay(disp);

	return 0;
}