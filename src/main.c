#define _POSIX_C_SOURCE 199309L

#include <Imlib2.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <getopt.h>
#include <libconfig.h>
#include <dirent.h>
#include <string.h>

// Basic constants
const char author[] = "TheRealOne78";
const char authorMail[] = "bajcsielias78@gmail.com";
const char ver[] = "0.1";

const unsigned int maxPathLenght = 4096;
const char appendConfPath[] = "/.config/xawp/xawp.conf"; // The path that will be concatenated with HOME envar

// Basic config/arguments
bool DEBUG = true;
char pathConf[4096];
char pathArg[4096];
unsigned confImgCount;
unsigned argImgCount;
char **prConfPath; // pointer to row Configuration Paths
char **prArgPath;  // pointer to row Argument Paths
double timeConf;
double timeArg;

// Misc
int imgCount = 0;
bool hasCurrentDir = false;
bool hasParentDir = false;
bool isConfConf = false;
bool isArgConf = false;

typedef struct {
  Window root;
  Pixmap pixmap;
  Imlib_Context *render_context;
  int width, height;
} Monitor;

void help() {
 printf(                                                                                "\n"
        "XAWP - X11 Animated Wallpaper Player"                                          "\n"
        "Play animated wallpapers in X11 by passing XAWP a directory containing the"    "\n"
	      "pictures frames wanted to be displayed."                                       "\n"
	                                                                                      "\n"
        "Usage: [options] images_path\n"						                                    "\n"
        "Options:"									                                                    "\n"
        "-h, --help \t Output this help list and exit"					                        "\n"
	      "-t, --time \t Set the time XAWP needs to wait between the"			                "\n"
	      "\t\t change of images: --time seconds.milliseconds"				                    "\n"
	      "-v, --version \t Output version information and license and exit"		          "\n"
	      "-D, --debug \t Output the debug log"						                                "\n"
	      "\nNote that XAWP uses a lot of resources like RAM and CPU!\n"			            "\n"
	     );
}

void version() {
  printf("XAWP version "); printf(ver);
	printf(										                                                            "\n"
	       "Copyright (C) 2022 TheRealOne78"						                                  "\n"
	       "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
	       "This is free software: you are free to change and redistribute it."		        "\n"
	       "There is NO WARRANTY, to the extent permitted by law.\n"		                 	"\n"
	      );
}

void getImgCount(char *str, int argORcount) {
  /* This function gets the string of the directory where the images exist and
   * counts every image. argORcout is to know if pImgCount should point to
   * the argument img count variable or configuration count variable.       */
  unsigned *pImgCount;

  if(argORcount == 0)
    pImgCount = &confImgCount;
  else if(argORcount == 1)
    pImgCount = &argImgCount;
  else {
    printf("Something went wrong: getImgCount argORcount is %d", argORcount);
    exit(EXIT_FAILURE);
  }
  *pImgCount = 0;

  DIR *d;
  struct dirent *dir;
  int i = 0;

  d = opendir(str);
  if (d)
  {
     while ((dir = readdir(d)) != NULL)
     {
       *pImgCount++;
     }
     closedir(d);
  }
}

static int compare_fun (const void *p, const void *q) {
  /* compare_fun() and some code from getImgPath() from
   * https://www.linuxquestions.org/questions/programming-9/how-to-list-and-sort-files-in-some-directory-by-the-names-on-linux-win-in-c-4175555160/
   * by NevemTeve - Thank you NevemTeve
   * This function is mandatory for qsort to be able to know
   * what approach to use to sort the images              */

  const char *l = p;
  const char *r = q;
  int cmp;

  cmp = strcmp(l, r);
  return cmp;
}

void getImgPath(char *str[maxPathLenght], int argORcount) {
  /* This function serves for saving the images paths from a
   * choosen directory to a dynamically allocated array of
   * pointers, pointers pointing to the string of path
   * I choosed this way because a normal initialized array
   * would use much more memory, Imlib uses a lot of memory
   * anyway.
   *
   * Just like getImgCount(), getImgPath() gets argORcount
   * which is used to know where pImgCount and pImgPath
   * should point to: argument or configuration file
   *
   * readdir() mixes up the files order, so qsort is used
   *
   * Most partitions have their first files as . and ..,
   * current directory and previous directory respectively
   * which shouldn't be part of the image loading, so if
   * statements are used to know if the first 1-2 files
   * are or not . and ..                                  */

  DIR *d;
  struct dirent *dir;
  unsigned *pImgCount;
  char ***pImgPath;

  if(argORcount == 0) {
    pImgCount = &confImgCount;
    pImgPath = &prConfPath;
  }
  else if(argORcount == 1) {
    pImgCount = &argImgCount;
    pImgPath = &prArgPath;
  }
  else {
    printf("Something went wrong: getImgPath argORcount is %d", argORcount);
    exit(EXIT_FAILURE);
  }
  *pImgPath = (char*)malloc( *pImgCount * sizeof( char* ));

  d = opendir(*str);
  int i = 0;
  if (d)
  {
    while ((dir = readdir(d)) != NULL) {
      *pImgPath[i]=(char*)malloc(maxPathLenght * sizeof(char));
      strcpy(*pImgPath[i], dir->d_name);
      i++;
    }
    closedir(d);
  }
    //readdir() dumps mixed files, so will order with qsort()
    qsort(pImgPath, imgCount, sizeof pImgPath[0], compare_fun);

    /* Now check if there are any "." and ".." files in path
     * in order to know where the actual images start     */
    if(*pImgPath[0] == ".")
      hasCurrentDir = true;
    else
      hasCurrentDir = false;
    if(*pImgPath[1] == "..")
      hasParentDir = true;
    else
      hasParentDir = false;
}

void setRootAtoms(Display *display, Monitor *monitor) {
  /* This function should clear and load the images to
   * X11 screen.                                    */

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
  //TODO: DON'T FORGET TO free()!!!
  char configTime[6];
  configTime[0] = '\0';

  // Do stuff with the arguments
  static struct option long_options [] = {
  	{ "help",    no_argument,       NULL,	'h' },
  	{ "time",    required_argument, NULL,	't' },
  	{ "version", no_argument,     	NULL,	'v' },
  	{ "debug",   no_argument,       NULL,	'D' },
	  { "fit",     required_argument, NULL, 'f' }, // Not implemented yet - This is a feature that XAWP will fit the photo based on user's requirements
	  { "directory",required_argument,NULL, 'd' }, // Not implemented yet - This will make the user prompt photos after the --directory/-d option
	  { "config",  required_argument, NULL, 'c' }, // Not implemented yet - This will make the make the user prompt another config file than the default one
  	{ NULL,	     0,		              NULL,	0   }
  };

  while(1){
    int c = getopt_long(argc, argv, "ht:vDf:d:c:", long_options, NULL);
    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c){
      case 'D':
      	DEBUG = !DEBUG;
      	break;

      case 'h':
      	help();
      	exit(0);
      	break;

      case 't':
      	snprintf(configTime, sizeof(configTime), "%s", optarg);
      	timeArg = atof(configTime);
        break;

      case 'f':
        printf("Fit is not implemented yet, skipping...\n");
	      //TODO: implement this fit
        break;

      case 'd':
	      strcpy(pathArg, optarg);
        break;

      case 'c':
        printf("Custom configuration file is not implemented yet, skipping...\n");
        //TODO: implement this custom config file
    	  break;

      case 'v':
      	version();
      	exit(0);
      	break;

      case '?':
      	/* No need to print and error message because
      	   getopt_long did that already. */
      	exit(1);
      	break;

      default:
      	abort();
    }
  }

  config_t cfg;
  config_setting_t *setting;
  char *str;
  double flt;
  int bln;
  config_init(&cfg);

  // Read the file. If there is an error, report it and exit.
  char confPath[maxPathLenght]; strcpy(confPath, getenv("HOME")); strcat(confPath, appendConfPath);
  if(DEBUG == true)
    printf("DEBUG: xawp.conf path: %s\n", confPath);
  if(! config_read_file(&cfg, confPath)) {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
           config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return(EXIT_FAILURE);
  }

  if(config_lookup_string(&cfg, "path", &str)) {
    strcpy(pathConf, str);
    getImgCount(&pathConf, 0); //conf=0, arg=1
    getImgPath(&pathConf, 0);  //conf=0, arg=1
  }
  else {
    fprintf(stderr, "No 'path' setting in configuration file.\n");
  }

  if(config_lookup_float(&cfg, "time", &flt)) {
    timeConf = flt;
    if(DEBUG == true)
      printf("DEBUG: timeConf: %f\n", timeConf);
  }
  else {
    fprintf(stderr, "No 'time' setting in configuration file.\n");
  }

  if(config_lookup_bool(&cfg, "debug", &bln)) {
    DEBUG = bln;
  }
  else {
    fprintf(stderr, "No 'debug' setting in configuration file.\n");
  }
  config_destroy(&cfg);


//  if(isArgConf == true) {
//    getImgCount(&pathArg, 1); // conf=0, arg=1
//    pArgPath = &argPath;
//  }

  if(DEBUG==true)
    fprintf(stdout, "DEBUG: Loading images\n");

//  char **path;
//  if(isArgConf == true)
//    path = *pArgPath;
//  else if(isConfConf == true)
//    path = &confPath;
//  else {
//    fprintf(stderr, "No valid path settings! Please check your configuration file and arguments.\n");
//    help();
//    exit(1);
//  }

  int fileOffset = 0;
  if(hasCurrentDir == true)
    fileOffset++;
  if(hasParentDir == true)
    fileOffset++;

  // Loading Images to ImLib
  Imlib_Image images[imgCount-fileOffset];
  for(int i = 0; fileOffset + i < imgCount; i++){
  //images[i] = *path[fileOffset+i];
  //printf("%s\n", *path[fileOffset+i]);
  }
  /* -- Old Imlib_Image loading, please delete this after the new implementation --
  Imlib_Image images[argc-noimgArgs];

  for (int imgCount=0; imgCount <= argc-noimgArgs-1; imgCount++){
    images[imgCount] = imlib_load_image(argv[imgCount+noimgArgs]);
  }
  int images_count = argc-1; */
  //TODO: DON'T FORGET TO free()!!!!
  // Loading the monitors, counting them and getting the resolution
  if(DEBUG==true)
    fprintf(stdout, " DEBUG: Loading monitors\n");

  Display *display = XOpenDisplay(NULL);
  if (!display) {
    fprintf(stderr, "Could not open XDisplay\n");
    exit(42);
  }

  const int screen_count = ScreenCount(display);
  if(DEBUG==true)
    fprintf(stdout, "DEBUG: Found %d screens\n", screen_count);

  Monitor *monitors = malloc(sizeof(Monitor) * screen_count);
  for (int current_screen = 0; current_screen < screen_count;
       ++current_screen) {
  if(DEBUG==true)
    fprintf(stdout, "DEBUG: Running screen %d\n", current_screen);

    const int width = DisplayWidth(display, current_screen);
    const int height = DisplayHeight(display, current_screen);
    const int depth = DefaultDepth(display, current_screen);
    Visual *vis = DefaultVisual(display, current_screen);
    const int cm = DefaultColormap(display, current_screen);

  if(DEBUG==true){
    fprintf(stdout, "DEBUG: Screen %d: width: %d, height: %d, depth: %d\n",
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
    fprintf(stdout, "DEBUG: Loaded %d screens\n", screen_count);

  /* Rendering the images on the screens found at the
   * choosen time interval, forever                */
  if(DEBUG==true)
    fprintf(stdout, "DEBUG: Starting render loop");

  struct timespec timeout;
  timeout.tv_sec = 0;
  timeout.tv_nsec = 70000000;

  while (1) {
    for (int cycle = 0; cycle < 10; ++cycle) {
      Imlib_Image current = images[cycle % imgCount];
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
