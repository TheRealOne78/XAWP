#define _POSIX_C_SOURCE 199309L

#include <Imlib2.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <getopt.h>

bool DEBUG = false;
const char author[] = "TheRealOne78";
const char authorMail[] = "bajcsielias78@gmail.com";
const char ver[] = "1.0";

float timeArg;

typedef struct {
  Window root;
  Pixmap pixmap;
  Imlib_Context *render_context;
  int width, height;
} Monitor;

void help() {
    printf("Usage: [options] images_path"							"\n"
        "Options:"										"\n"
        "-h, --help \t Output this help list and exit"						"\n"
	"-t, --time \t Set the time XAWP needs to wait between the change of images: --time seconds.milliseconds\n"
	"-v, --version \t Output version information and license and exit"			"\n"
	"\nNote that XAWP uses a lot of resources like RAM and CPU!\n"				"\n"
	);
}

void version() {
	printf("XAMP version ");printf(ver);
	printf(											"\n"
	"Copyright (C) 2022 TheRealOne78"							"\n"
	"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>."	"\n"
	"This is free software: you are free to change and redistribute it."			"\n"
	"There is NO WARRANTY, to the extent permitted by law.\n"				"\n"
	);
}

void setRootAtoms(Display *display, Monitor *monitor) {
  Atom atom_root, atom_eroot, type;
  unsigned char *data_root, *data_eroot;
  int format;
  unsigned long length, after;

  atom_root = XInternAtom(display, "_XROOTMAP_ID", True);
  atom_eroot = XInternAtom(display, "ESETROOT_PMAP_ID", True);

  // doing this to clean up after old background
  if (atom_root != None && atom_eroot != None) {
    XGetWindowProperty(display, monitor->root, atom_root, 0L, 1L, False,
                       AnyPropertyType, &type, &format, &length, &after,
                       &data_root);   
     
    if (type == XA_PIXMAP) {       
      XGetWindowProperty(display, monitor->root, atom_eroot, 0L, 1L, False,
                         AnyPropertyType, &type, &format, &length, &after,
                         &data_eroot);          
              
      if (data_root && data_eroot && type == XA_PIXMAP &&
          *((Pixmap *)data_root) == *((Pixmap *)data_eroot))
        XKillClient(display, *((Pixmap *)data_root));
    }                     
  }                       
                         
  atom_root = XInternAtom(display, "_XROOTPMAP_ID", False);
  atom_eroot = XInternAtom(display, "ESETROOT_PMAP_ID", False);
                              
  // setting new background atoms                       
  XChangeProperty(display, monitor->root, atom_root, XA_PIXMAP, 32,
                  PropModeReplace, (unsigned char *)&monitor->pixmap, 1);
  XChangeProperty(display, monitor->root, atom_eroot, XA_PIXMAP, 32,
                  PropModeReplace, (unsigned char *)&monitor->pixmap, 1);
}

int main(int argc, char **argv[]) {
	int noimgArgs = 1;
	if (argc<=1)
	{
		help();
		exit(1);
	}
 
	char configTime[6];
	configTime[0] = '\0';

	static struct option long_options [] = {
	{ "help",	no_argument,	NULL,	'h' },
	{ "time",	required_argument,NULL,	't' },
	{ "version",	no_argument,	NULL,	'v' },
	{ "debug",	optional_argument,NULL,	'D' },
	{ NULL,		0,		NULL,	0   }
	};

	while(1){
		int c = getopt_long(argc, argv, "ht:vD", long_options, NULL);
		/* Detect the end of the options. */
        	if (c == -1)
		break;

		switch (c)
        	{
			case 'D':
				DEBUG=true;
				noimgArgs=noimgArgs+1;
				break;

			case 'h':
				help();
				exit(0);
				break;

			case 't':
				snprintf(configTime, sizeof(configTime), "%s", optarg);
				noimgArgs=noimgArgs + 2;
				break;

			case 'v':
				version();
				exit(0);
				break;

			case '?':
				/* No need to print and error message because getopt_long did that already. */
				exit(1);
				break;

			default:
				abort();
        	}
	}

	float time = atof(configTime);

if (DEBUG==true)
  fprintf(stdout, "Loading images");

	Imlib_Image images[argc-noimgArgs];
	for (int imgCount=0; imgCount <= argc-noimgArgs-1; imgCount++){
		images[imgCount] = imlib_load_image(argv[imgCount+noimgArgs]);
	}
	int images_count = argc-1;
	//printf(argv[0]);printf(argv[1]);printf(argv[2]);printf(argv[3]);
	//exit(1);

if(DEBUG==true)
  fprintf(stdout, "Loading monitors\n");

  Display *display = XOpenDisplay(NULL);
  if (!display) {
    fprintf(stderr, "Could not open XDisplay\n");
    exit(42);
  }

  const int screen_count = ScreenCount(display);
if(DEBUG==true)
  fprintf(stdout, "Found %d screens\n", screen_count);

  Monitor *monitors = malloc(sizeof(Monitor) * screen_count);
  for (int current_screen = 0; current_screen < screen_count;
       ++current_screen) {
if(DEBUG==true)
    fprintf(stdout, "Running screen %d\n", current_screen);

    const int width = DisplayWidth(display, current_screen);
    const int height = DisplayHeight(display, current_screen);
    const int depth = DefaultDepth(display, current_screen);
    Visual *vis = DefaultVisual(display, current_screen);
    const int cm = DefaultColormap(display, current_screen);

if(DEBUG==true){
    fprintf(stdout, "Screen %d: width: %d, height: %d, depth: %d\n",
            current_screen, width, height, depth);
}
    Window root = RootWindow(display, current_screen);
    Pixmap pixmap = XCreatePixmap(display, root, width, height, depth);

    monitors[current_screen].width = width;
    monitors[current_screen].height = height;
    monitors[current_screen].root = root;
    monitors[current_screen].pixmap = pixmap;
    monitors[current_screen].render_context = imlib_context_new();
    imlib_context_push(monitors[current_screen].render_context);
    imlib_context_set_display(display);
    imlib_context_set_visual(vis);
    imlib_context_set_colormap(cm);
    imlib_context_set_drawable(pixmap);
    imlib_context_set_color_range(imlib_create_color_range());
    imlib_context_pop();
  }

if(DEBUG==true)
  fprintf(stdout, "Loaded %d screens\n", screen_count);

if(DEBUG==true)
  fprintf(stdout, "Starting render loop");

  struct timespec timeout;
  timeout.tv_sec = 0;
  timeout.tv_nsec = 70000000;
while (1==1){
  for (int cycle = 0; cycle < 62; ++cycle) {
    Imlib_Image current = images[cycle % images_count];
    for (int monitor = 0; monitor < screen_count; ++monitor) {
      Monitor *c_monitor = &monitors[monitor];
      imlib_context_push(c_monitor->render_context);
      imlib_context_set_dither(1);
      imlib_context_set_blend(1);
      imlib_context_set_image(current);

      imlib_render_image_on_drawable(0, 0);

      setRootAtoms(display, c_monitor);
      XKillClient(display, AllTemporary);
      XSetCloseDownMode(display, RetainTemporary);
      XSetWindowBackgroundPixmap(display, c_monitor->root, c_monitor->pixmap);
      XClearWindow(display, c_monitor->root);
      XFlush(display);
      XSync(display, False);
      imlib_context_pop();
    }
   
    nanosleep(&timeout, NULL);
  }
 }

}

