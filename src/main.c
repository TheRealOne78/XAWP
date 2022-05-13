//#define _POSIX_C_SOURCE 199309L

#include <Imlib2.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

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
	"-v, --version \t Output version information and exit\n"				"\n"
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


void setTime(int argc; char **argv[]){
//TODO: make a filter that gives the options from agruments and get the time option in order to process it
	for(int tempCycleVar=1; tempCycleVar <= argc-2; tempCycleVar++)
	{
		if(argv[tempCycleVar] == "-t" || argv[tempCycleVar] == "--time"){
		timeArg = argv[tempCycleVar+1];
		break;
		}
	}
}

int main(int argc, char **argv[]) {
	if (argc<=1)
	{
		help();
		exit(1);
	}
 
	static struct option long_options [] = {
	{ "help",	optional_argument,NULL,	'h' },
	{ "time",	no_argument,	NULL,	't' },
	{ "version",	no_argument,	NULL,	'v' },
	{ NULL,		0,		NULL,	0   }
	};
int indexptr;
	while(1){
		int c = getopt_long(argc, argv, "htv", long_options, NULL);

        	if (c == -1)
		break;

		switch (c)
        	{
			case 'h':
				help();
				exit(0);
				break;

			case 't':
				setTime(argv);
				break;

			case 'v':
				version();
				exit(0);
				break;

			case '?':
				/* No need to print and error message because getopt_long did that already. */
				exit(1);
				break;
        	}
	}

#ifdef DEBUG
  fprintf(stdout, "Loading images");
#endif

	Imlib_Image images[argc-1];
	for (int imgCount=0; imgCount <= argc-2; imgCount++){
		images[imgCount] = imlib_load_image(argv[imgCount+1]);
	}
	int images_count = argc-1;

#ifdef DEBUG
  fprintf(stdout, "Loading monitors\n");
#endif

  Display *display = XOpenDisplay(NULL);
  if (!display) {
    fprintf(stderr, "Could not open XDisplay\n");
    exit(42);
  }

  const int screen_count = ScreenCount(display);
#ifdef DEBUG
  fprintf(stdout, "Found %d screens\n", screen_count);
#endif

  Monitor *monitors = malloc(sizeof(Monitor) * screen_count);
  for (int current_screen = 0; current_screen < screen_count;
       ++current_screen) {
#ifdef DEBUG
    fprintf(stdout, "Running screen %d\n", current_screen);
#endif

    const int width = DisplayWidth(display, current_screen);
    const int height = DisplayHeight(display, current_screen);
    const int depth = DefaultDepth(display, current_screen);
    Visual *vis = DefaultVisual(display, current_screen);
    const int cm = DefaultColormap(display, current_screen);

#ifdef DEBUG
    fprintf(stdout, "Screen %d: width: %d, height: %d, depth: %d\n",
            current_screen, width, height, depth);
#endif

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

#ifdef DEBUG
  fprintf(stdout, "Loaded %d screens\n", screen_count);
#endif

#ifdef DEBUG
  fprintf(stdout, "Starting render loop");
#endif

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

