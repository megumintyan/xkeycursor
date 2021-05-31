#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include "config.h"


int main()
{
	Display *display;

	if ((display = XOpenDisplay(NULL)) == NULL) {
		fprintf(stderr, "Error: Can't open display: %s\n",
		        getenv("DISPLAY"));
		return EXIT_FAILURE;
	}

	Window window = XDefaultRootWindow(display);

	XGrabKeyboard(display, window, False, GrabModeAsync, GrabModeAsync, CurrentTime);
	XEvent e;
	int x11_fd;
	fd_set in_fds;
	struct timeval tv;

	x11_fd = ConnectionNumber(display);

	while(1) {
		FD_ZERO(&in_fds);
		FD_SET(x11_fd, &in_fds);

		tv.tv_sec = 0;
		tv.tv_usec = 10000;

		int num_ready_fds = select(x11_fd + 1, &in_fds, NULL, NULL, &tv);

		while(XPending(display))
			XNextEvent(display, &e);

		switch (e.type) {
		case KeyPress:
			if (XLookupKeysym(&e.xkey, 0) == RIGHT)
				XWarpPointer(display, None, None, 0, 0, 0, 0, HSPEED, 0);
			if (XLookupKeysym(&e.xkey, 0) == LEFT)
				XWarpPointer(display, None, None, 0, 0, 0, 0, -1*HSPEED, 0);
			if (XLookupKeysym(&e.xkey, 0) == DOWN)
				XWarpPointer(display, None, None, 0, 0, 0, 0, 0, VSPEED);
			if (XLookupKeysym(&e.xkey, 0) == UP)
				XWarpPointer(display, None, None, 0, 0, 0, 0, 0, -1*VSPEED);
			if (XLookupKeysym(&e.xkey, 0) == LBUTTON) {
				XTestFakeButtonEvent(display, Button1, True, CurrentTime);
				usleep(100000);
				XTestFakeButtonEvent(display, Button1, False, CurrentTime);
			}
			if (XLookupKeysym(&e.xkey, 0) == RBUTTON) {
				XTestFakeButtonEvent(display, Button3, True, CurrentTime);
				usleep(100000);
				XTestFakeButtonEvent(display, Button3, False, CurrentTime);
			}
			if (XLookupKeysym(&e.xkey, 0) == QUIT) {
				XCloseDisplay(display);
				return 0;
			}
		}

	}

	XCloseDisplay(display);
}
