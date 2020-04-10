// src: https://gitlab.freedesktop.org/xorg/xserver/issues/876
#include <X11/Xlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	Display* dpy = XOpenDisplay(NULL);

	unsigned int width = 700;
	unsigned int height = 500;

	Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, width, height, 2, 0, 0xffffff);
	XMapRaised(dpy, w);

	XGCValues gcattr;
	gcattr.foreground = 0;
	GC gc = XCreateGC(dpy, w, GCForeground, &gcattr);
	XFlush(dpy);

	sleep(1);

	printf("about to grab\n");

	XGrabKeyboard(
		dpy, w, 
		/* owner events */ True, 
		/* pointer_mode */ GrabModeSync,
		/* keyboard_mode */ GrabModeSync,
		CurrentTime
	);
	XFlush(dpy);

	printf("grabbed\n");

	for (unsigned i = 0; i < width; i++) {
		XDrawLine(dpy, w, gc, i, 0, width-i, height);
		XFlush(dpy);
		usleep( 2*1000000 / width );
	}

	XUngrabKeyboard(dpy, CurrentTime);
	XFlush(dpy);
	printf("ungrabbed\n");

	XCloseDisplay(dpy);

	return 0;
}
