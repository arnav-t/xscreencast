#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

int main()
{
	Display *disp = XOpenDisplay(NULL);
	Screen *scr = XDefaultScreenOfDisplay(disp);
	printf("Width: %d, Height: %d\n", WidthOfScreen(scr), HeightOfScreen(scr));
	return 0;
}