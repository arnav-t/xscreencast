#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>


int main()
{
	Display *d = XOpenDisplay(0);
	printf("Still good in the hood\n");
	return 0;
}