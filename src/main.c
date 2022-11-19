#define _POSIX_C_SOURCE 199309L

#define _DEBUG false
#define DEBUG_TEXT_PUTS "\x1B[34mDEBUG\x1B[0m"

#define MAX_PATH 4096

#define DEFAULT_FRAME_TIME 0.07
#define DEFAULT_FIT_OPTION "fit"

#ifndef _COLORS_
  #define _COLORS_
    /* FOREGROUND */
    #define RST "\x1B[0m"

    /* Normal colors */
    #define KBLK  "\x1B[30m" //black
    #define KRED  "\x1B[31m" //red
    #define KGRN  "\x1B[32m" //green
    #define KYEL  "\x1B[33m" //yellow
    #define KBLU  "\x1B[34m" //blue
    #define KMAG  "\x1B[35m" //magenta
    #define KCYN  "\x1B[36m" //cyan
    #define KWHT  "\x1B[37m" //white

    /* Bright colors */
    #define KBBLK "\x1B[90m" //bright black(gray)
    #define KBRED "\x1B[91m" //bright red
    #define KBGRN "\x1B[92m" //bright green
    #define KBYEL "\x1B[93m" //bright yellow
    #define KBBLU "\x1B[94m" //bright blue
    #define KBMAG "\x1B[95m" //bright magenta
    #define KBCYN "\x1B[96m" //bright cyan
    #define KBWHT "\x1B[97m" //bright white

    /* misc */
    #define BOLD  "\x1B[1m"  //bold
    #define UNDL  "\x1B[4m"  //underline
  #endif  /* _COLORS_ */
/* end_ifndef _COLORS_ */

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
#include <math.h>
#include <ctype.h>

// Basic constants
const char author[] = "TheRealOne78";
const char authorMail[] = "bajcsielias78@gmail.com";
const char ver[] = "0.0.2";

const char appendDefaultConfPath[] = "/.config/xawp/xawp.conf"; /* This path will be concatenated with HOME envar */

/* Basic config/argument variables */
bool DEBUG = _DEBUG;     /* If true, print debug info */

char pathConf[MAX_PATH]; /* path to images directory, from configuration file */
char pathArg[MAX_PATH];  /* path to images directory, from user argument (not -c) */

char confPath[MAX_PATH]; /* path to configuration file */

unsigned imgCount;       /* number of images */

char **imgPath;          /* pointers to paths of images, from configuration file */

double frameTime = DEFAULT_FRAME_TIME;     /* time between frames */ /* The time set is the default one */

bool isArgConf = false;                    /* If true, the configuration file from argument will be used */

bool hasArgTime = false;                   /* If true, time from user argument will be used */
bool hasArgDir = false;                    /* If true, the directory will be used from user argument */

bool usingStaticWallpaper = false;         /* If true, XAWP will run only once to set a static wallpaper */
bool hasArgStaticWallpaper = false;        /* If true, it will be used the Static Wallpaper from user argument */

bool hasArgFit = false;                    /* If true, the fit option from user argument will be used - Order 0 */
bool hasConfFit = false;                   /* If true, the fit option from configuration file will be used - Order 1 */
char defaultFitOpt[] = DEFAULT_FIT_OPTION; /* Default Fit Option - Order 2 */
char *fitOpt;                              /* The final fit option */

/* Miscellaneous variables */
bool hasCurrentDir = false;            /* If true, the directory containing images has a current directory file: ./ */
bool hasParentDir = false;             /* If true, the directory containing images has a parent directory file: ../ */

typedef struct {
  Window root;
  Pixmap pixmap;
  Imlib_Context *render_context;
  int width, height;
} Monitor;

void help(void);
void version(void);

void getImgCount(char str[][MAX_PATH]);
void getImgPath(char str[][MAX_PATH], int choice);
static int compare_fun (const void *p, const void *q);
void freeUsingPath(void);
void setRootAtoms(Display *display, Monitor *monitor);
void ImFit(Imlib_Image *image[]);


int main(int argc, char *argv[]) {
  char configTime[6];
  configTime[0] = '\0';

  // Do stuff with the arguments
  static struct option long_options [] = {
    { "help"                , no_argument      , NULL , 'h' },
    { "time"                , required_argument, NULL , 't' },
    { "version"             , no_argument      , NULL , 'v' },
    { "debug"               , no_argument      , NULL , 'D' },
    { "fit"                 , required_argument, NULL , 'f' }, // Not implemented yet - This is a feature that XAWP will fit the photo based on user's requirements
    { "directory"           , required_argument, NULL , 'd' },
    { "config"              , required_argument, NULL , 'c' },
    { "set-static-wallpaper", required_argument, NULL , 'S' },
    { NULL                  , 0                , NULL , 0   }
  };

  while(1) {
    int c = getopt_long(argc, argv, "ht:vDf:d:c:S:", long_options, NULL);
    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c) {
      case 'h':
        help();
        exit(0);
        break;

      case 't':
        snprintf(configTime, sizeof(configTime), "%s", optarg);
        frameTime = atof(configTime);
        hasArgTime = true;
        break;

      case 'v':
        version();
        exit(0);
        break;

      case 'D':
        DEBUG = !DEBUG;
        if(DEBUG)
          fprintf(stdout, DEBUG_TEXT_PUTS": Enabled debug\n");
        break;

      case 'f':
        printf("Fit is not implemented yet, skipping...\n");
        //TODO: implement this fit
        break;
        strcpy(fitOpt, optarg);
        hasArgFit = true;
        break;

      case 'd':
        strcpy(pathArg, optarg);
        getImgCount(&pathArg);
        getImgPath (&pathArg, 1);
        hasArgDir = true;
        break;

      case 'c':
        strcpy(confPath, optarg);
        isArgConf = true;
        break;

      case 'S':
        imgPath = (char**)malloc(1 * sizeof(char*));
        imgPath[0] = (char*)malloc(MAX_PATH * sizeof(char));
        strcpy(imgPath[0], optarg);
        imgCount++;
        usingStaticWallpaper = true;
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
  /* print XAWP color logo in ASCII art. Remove a single backslash '\' from each '\\'
   * to see the real logo                                                              */
  if(system("[[ -x \"$(command -v lolcat)\" ]]") == 0) {
    system("printf \" /\\$\\$   /\\$\\$  /\\$\\$\\$\\$\\$\\$  /\\$\\$      /\\$\\$ /\\$\\$\\$\\$\\$\\$\\$\\n| \\$\\$  / \\$\\$ /\\$\\$__  \\$\\$| \\$\\$  /\\$ | \\$\\$| \\$\\$__  \\$\\$\\n|  \\$\\$/ \\$\\$/| \\$\\$  \\ \\$\\$| \\$\\$ /\\$\\$\\$| \\$\\$| \\$\\$  \\ \\$\\$\\n \\  \\$\\$\\$\\$/ | \\$\\$\\$\\$\\$\\$\\$\\$| \\$\\$/\\$\\$ \\$\\$ \\$\\$| \\$\\$\\$\\$\\$\\$\\$\\n  >\\$\\$  \\$\\$ | \\$\\$__  \\$\\$| \\$\\$\\$\\$_  \\$\\$\\$\\$| \\$\\$____/\\n /\\$\\$/\\  \\$\\$| \\$\\$  | \\$\\$| \\$\\$\\$/ \\  \\$\\$\\$| \\$\\$      \\n| \\$\\$  \\ \\$\\$| \\$\\$  | \\$\\$| \\$\\$/   \\  \\$\\$| \\$\\$      \\n|__/  |__/|__/  |__/|__/     \\__/|__/      \\n\" | lolcat");
  }
  else {
    printf("\n" /* ==print logo== */
      KYEL" /$$   /$$"  KRED"  /$$$$$$ "  KMAG" /$$      /$$"  KBCYN" /$$$$$$$ "  RST"\n"
      KYEL"| $$  / $$"  KRED" /$$__  $$"  KMAG"| $$  /$ | $$"  KBCYN"| $$__  $$"  RST"\n"
      KYEL"|  $$/ $$/"  KRED"| $$  \\ $$"  KMAG"| $$ /$$$| $$"  KBCYN"| $$  \\ $$"  RST"\n"
      KYEL" \\  $$$$/ "  KRED"| $$$$$$$$"  KMAG"| $$/$$ $$ $$"  KBCYN"| $$$$$$$/"  RST"\n"
      KYEL"  >$$  $$ "  KRED"| $$__  $$"  KMAG"| $$$$_  $$$$"  KBCYN"| $$____/ "  RST"\n"
      KYEL" /$$/\\  $$"  KRED"| $$  | $$"  KMAG"| $$$/ \\  $$$"  KBCYN"| $$      "  RST"\n"
      KYEL"| $$  \\ $$"  KRED"| $$  | $$"  KMAG"| $$/   \\  $$"  KBCYN"| $$      "  RST"\n"
      KYEL"|__/  |__/"  KRED"|__/  |__/"  KMAG"|__/     \\__/"  KBCYN"|__/      "  RST"\n"
    );
  }
  printf(KBWHT "X11 Animated Wallpaper Player v%s\n\n" RST, ver);
  /* ASCII art generated from patorjk.com/software/taag
   *
   * #-> bigmoney-ne : by nathan bloomfield (xzovik@gmail.com)
   * #-> based on art from the legendary MAKEMONEYFAST chain letter
   * #->
   * #-> History:
   * #->   5-30-2007 : first version (required characters only)
   * #->
   * #-> (end comments)                                           */

  config_t cfg;
  config_setting_t *setting;
  const char *cfgStaticWallpaper;
  const char *cfgPath;
  double cfgTime;
  int cfgDebug;
  int cfgFit;
  config_init(&cfg);

  if(!isArgConf) {
    strcpy(confPath, getenv("HOME"));
    strcat(confPath, appendDefaultConfPath);
  }

  if(DEBUG)
    fprintf(stdout, DEBUG_TEXT_PUTS": configuration file path: \"%s\"\n", confPath);

  // Read the file. If there is an error, report it and exit.
  if(! config_read_file(&cfg, confPath)) {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
           config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return(EXIT_FAILURE);
  }

  if(config_lookup_bool(&cfg, "debug", &cfgDebug))
    DEBUG = cfgDebug;
  else
    fprintf(stderr, "No 'debug' setting in configuration file.\n");
  if(DEBUG)
    fprintf(stdout, DEBUG_TEXT_PUTS": Enabled debug\n");

  if(!hasArgStaticWallpaper && config_lookup_string(&cfg, "static-wallpaper", &cfgStaticWallpaper)){
    imgPath = (char**)malloc(1 * sizeof(char*));
    imgPath[0] = (char*)malloc(MAX_PATH * sizeof(char));
    strcpy(imgPath[0], cfgStaticWallpaper);
    imgCount++;
    usingStaticWallpaper = true;
  }

  if(hasArgDir && !usingStaticWallpaper) {
    getImgCount(&pathArg);
    getImgPath(&pathArg, 1);
  }
  else if(!hasArgDir && config_lookup_string(&cfg, "path", &cfgPath) && !usingStaticWallpaper) {
    strcpy(pathConf, cfgPath);
    getImgCount(&pathConf); //conf=0, arg=1
    getImgPath(&pathConf, 0);  //conf=0, arg=1
  }
  else
    fprintf(stderr, "No 'path' setting in configuration file.\n");

  if(!hasArgTime && config_lookup_float(&cfg, "time", &cfgTime) && !usingStaticWallpaper) {
    frameTime = cfgTime;
    if(DEBUG)
      fprintf(stdout, DEBUG_TEXT_PUTS": frameTime: %lf\n", frameTime);
  }
  else
    fprintf(stderr, "No 'time' setting in configuration file.\n");

  if(config_lookup_bool(&cfg, "fit", &cfgFit) && !hasArgFit && !usingStaticWallpaper) {
    hasConfFit = cfgFit;
  }

  config_destroy(&cfg);

  if(DEBUG)
    fprintf(stdout, DEBUG_TEXT_PUTS": Loading images\n");

  int fileOffset = 0;

  if(hasCurrentDir)
    fileOffset++;
  if(hasParentDir)
    fileOffset++;

  // Loading Images to ImLib
  Imlib_Image images[imgCount-fileOffset];
  if(!usingStaticWallpaper) {
    for(int temp = 0; temp < imgCount - fileOffset; temp++) {
      images[temp] = imlib_load_image(imgPath[(fileOffset+temp)]);
      if(DEBUG)
        fprintf(stdout, DEBUG_TEXT_PUTS": Imlib loaded %s\n", (imgPath)[(fileOffset+temp)]);
    }
    if(DEBUG)
      fprintf(stdout, "\n");
  }
  else {
    images[0] = imlib_load_image((imgPath)[0]);
    if(DEBUG)
      fprintf(stdout, DEBUG_TEXT_PUTS": Imlib loaded %s\n", (imgPath)[0]);
  }
  freeUsingPath();

  // Loading the monitors, counting them and getting the resolution
  if(DEBUG)
    fprintf(stdout, DEBUG_TEXT_PUTS": Loading monitors\n");

  Display *display = XOpenDisplay(NULL);
  if (!display) {
    fprintf(stderr, "Could not open XDisplay\n");
    exit(42);
  }

  const int screen_count = ScreenCount(display);
  if(DEBUG)
    fprintf(stdout, DEBUG_TEXT_PUTS": Found %d screens\n", screen_count);

  Monitor *monitors = malloc(sizeof(Monitor) * screen_count);
  for(int current_screen = 0; current_screen < screen_count;
      ++current_screen) {
    if(DEBUG)
      fprintf(stdout, DEBUG_TEXT_PUTS": Running screen %d\n", current_screen);

    const int width  = DisplayWidth(display, current_screen);
    const int height = DisplayHeight(display, current_screen);
    const int depth  = DefaultDepth(display, current_screen);
    Visual *vis      = DefaultVisual(display, current_screen);
    const int cm     = DefaultColormap(display, current_screen);

    if(DEBUG) {
      fprintf(stdout, DEBUG_TEXT_PUTS": Screen %d: width: %d, height: %d, depth: %d\n",
              current_screen, width, height, depth);
    }

    Window root = RootWindow(display, current_screen);
    Pixmap pixmap = XCreatePixmap(display, root, width, height, depth);

    monitors[current_screen].width          = width;
    monitors[current_screen].height         = height;
    monitors[current_screen].root           = root;
    monitors[current_screen].pixmap         = pixmap;
    monitors[current_screen].render_context = imlib_context_new();
    imlib_context_push(monitors[current_screen].render_context);
    imlib_context_set_display(display);
    imlib_context_set_visual(vis);
    imlib_context_set_colormap(cm);
    imlib_context_set_drawable(pixmap);
    imlib_context_set_color_range(imlib_create_color_range());
    imlib_context_pop();
  }
  if(DEBUG)
    fprintf(stdout, DEBUG_TEXT_PUTS": Loaded %d screens\n", screen_count);

  /* Rendering the images on the screens found at the
   * choosen time interval, forever                */
  if(DEBUG)
    fprintf(stdout, DEBUG_TEXT_PUTS": Starting render loop\n");

  struct timespec timeout;
  double time_nsec_raw = frameTime;
  if(time_nsec_raw > 1.0)
    time_nsec_raw -= floor(time_nsec_raw);
  timeout.tv_sec  = floor(frameTime);
  timeout.tv_nsec = time_nsec_raw * 1e9;

  while(1) {
    for(int cycle = 0; cycle < imgCount - fileOffset; ++cycle) {
      Imlib_Image current = images[cycle % imgCount];
      for(int monitor = 0; monitor < screen_count; ++monitor) {
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
      if(usingStaticWallpaper) {
        if(DEBUG)
          fprintf(stdout, DEBUG_TEXT_PUTS": Using static wallpaper detected, exiting...\n");
        exit(EXIT_SUCCESS);
      }

      nanosleep(&timeout, NULL);
    }
  }
}

void help(void) {
  printf(                                                                                 "\n"
         "XAWP - X11 Animated Wallpaper Player"                                           "\n"
         "Play animated wallpapers in X11 by passing XAWP a directory containing the"     "\n"
         "pictures frames wanted to be displayed."                                        "\n"
                                                                                          "\n"
         "Usage: xawp [options]"                                                          "\n"
                                                                                          "\n"
         "Options:"                                                                       "\n"
         "-h, --help\t\t\t"              "Output this help list and exit"                 "\n"
         "-t, --time\t\t\t"              "Set the time XAWP needs to wait between the"    "\n"
         "\t\t\t\t"                      "change of images: --time seconds.milliseconds"  "\n"
                                                                                          "\n"
         "-v, --version\t\t\t"           "Output version information and license and exit""\n"
         "-D, --debug\t\t\t"             "Output the debug log"                           "\n"
         "-d, --directory\t\t\t"         "Set directory containing animation frames:"     "\n"
         "\t\t\t\t"                      "--directory "KBWHT"/home/foo/wallgif/"RST       "\n"
                                                                                          "\n"
         "-c, --config\t\t\t"            "Set another configuration file than the default""\n"
         "\t\t\t\t"                 KBWHT"%s/.config/xawp/xawp.conf"RST" configuration file\n"
                                                                                          "\n"
         "-S, --set-static-wallpaper\t"  "Set a static wallpaper and exit"                "\n"
                                                                                          "\n"
         "Note that XAWP uses a lot of system resources like RAM and CPU!"                "\n"
                                                                                          "\n"
        , getenv("HOME"));
}

void version(void) {
  printf("XAWP version %s" /* version number */                                           "\n"
                                                                                          "\n"
         "Copyright (C) 2022 TheRealOne78"                                                "\n"
         "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.""\n"
         "This is free software: you are free to change and redistribute it."             "\n"
         "There is NO WARRANTY, to the extent permitted by law."                          "\n"
                                                                                          "\n"
        , ver);
}

void getImgCount(char str[][MAX_PATH]) {
  /* This function gets the string of the directory where the images exist and
   * counts every image. argORcout is to know if pImgCount should point to
   * the argument img count variable or configuration count variable.       */
  imgCount = 0;

  DIR *d;
  struct dirent *dir;
  int i = 0;
  d = opendir(*str);
  if(d) {
    while ((dir = readdir(d)) != NULL)
      imgCount++;
    closedir(d);
  }
}

static int compare_fun (const void *p, const void *q) {
  /* compare_fun() and some code from getImgPath() from
   * https://www.linuxquestions.org/questions/programming-9/how-to-list-and-sort-files-in-some-directory-by-the-names-on-linux-win-in-c-4175555160/
   * by NevemTeve - Thank you NevemTeve
   * This function is mandatory for qsort to be able to know
   * what approach to use to sort the images              */

  const char *l = *(const char**)p;
  const char *r = *(const char**)q;
  int cmp;

  cmp = strcmp(l, r);
  return cmp;
}

void getImgPath(char str[][MAX_PATH], int choice) {
  /* This function serves for saving the images paths from a
   * choosen directory to a dynamically allocated array of
   * pointers, pointers pointing to the string of path
   * I choosed this way because a normal initialized array
   * would use much more memory, Imlib uses a lot of memory
   * anyway.
   *
   * Just like getImgCount(), getImgPath() gets choice
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

  if(DEBUG) {
    switch(choice) {
      case 0:
        fprintf(stdout, DEBUG_TEXT_PUTS": Using \"%s\" from configuration file\n", *str);
        break;
      case 1:
        fprintf(stdout, DEBUG_TEXT_PUTS": Using user arguments for configurations\n", *str);
        break;
    }
  }
  imgPath = (char**)malloc(imgCount * sizeof(char*));

  DIR *d;
  struct dirent *dir;

  d = opendir(*str);
  int temp = 0;
  if (d) {
    bool notEndingWithSlash;
    if(*str[strlen(*str)] != '/') /* Check to see if path is not ending with '/' */
      notEndingWithSlash = true;
    while ((dir = readdir(d)) != NULL) {
      (imgPath)[temp] = (char*)malloc(MAX_PATH * sizeof(char));
      strcpy((imgPath)[temp], *str);
      if(notEndingWithSlash) /* Add "/" to frame path if the dir path doesn't end with "/" */
        strcat((imgPath)[temp], "/");
      strcat((imgPath)[temp++], dir->d_name);
    }
    closedir(d);
  }
    /* readdir() dumps mixed files, so qsort will sort alphabetically */
    qsort(imgPath, imgCount, sizeof((imgPath)[0]), compare_fun);

    /* Prints all the selected files */
    if(DEBUG) {
      fprintf(stdout, "\n"DEBUG_TEXT_PUTS": Selected files:\n");
      for(int i = 0; i < imgCount; i++)
        fprintf(stdout, "  | File %d: %s\n", i, (imgPath)[i]);
      fprintf  (stdout, "  | ** End of files **\n\n");
    }

    /* Now check if there are any "." and ".." files in path
     * in order to know where the actual images start     */
    char tempImgPath1dot[MAX_PATH];
    strcpy(tempImgPath1dot, *str);
    strcat(tempImgPath1dot, "/.");
    if(strcmp((imgPath)[0], tempImgPath1dot) == 0) {
      hasCurrentDir = true;
      if(DEBUG)
        fprintf(stdout, DEBUG_TEXT_PUTS": \"%s/\" has current directory file, skipping it.\n", *str);
    }
    else
      hasCurrentDir = false;

    char tempImgPath2dot[MAX_PATH];
    strcpy(tempImgPath2dot, *str);
    strcat(tempImgPath2dot, "/..");
    if(strcmp((imgPath)[1], tempImgPath2dot) == 0) {
      hasParentDir = true;
      if(DEBUG)
        fprintf(stdout, DEBUG_TEXT_PUTS": \"%s/\" has parent directory file, skipping it.\n", *str);
    }
    else
      hasParentDir = false;
}

void freeUsingPath(void) {
  /* As the name says, this function frees all the
   * allocated memory from the using image path */

  for(int temp; temp < imgCount; temp++) {
    if(DEBUG)
      fprintf(stdout, DEBUG_TEXT_PUTS": Unallocated address "KGRN"%p"RST" pointing to file index %d\n",
              (imgPath)[temp], temp);
    free((imgPath)[temp]);
  }
    if(DEBUG)
      fprintf(stdout, DEBUG_TEXT_PUTS": Unallocated dynamic array of images at address "KGRN"%p"RST"\n\n", (imgPath));
    free(imgPath);
}

void setRootAtoms(Display *display, Monitor *monitor) {
  /* This function should clear and load the images to
   * X11 screen.                                    */

  Atom atom_root, atom_eroot, type;
  unsigned char *data_root, *data_eroot;
  int format;
  unsigned long length, after;

  atom_root  = XInternAtom(display, "_XROOTMAP_ID"    , True);
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

  atom_root  = XInternAtom(display, "_XROOTPMAP_ID"   , False);
  atom_eroot = XInternAtom(display, "ESETROOT_PMAP_ID", False);

  // setting new background atoms
  XChangeProperty(display, monitor->root, atom_root, XA_PIXMAP, 32,
                  PropModeReplace, (unsigned char *)&monitor->pixmap, 1);
  XChangeProperty(display, monitor->root, atom_eroot, XA_PIXMAP, 32,
                  PropModeReplace, (unsigned char *)&monitor->pixmap, 1);
}

void ImFit(Imlib_Image *image[]) {
  /* This function is responsible for fitting the image when rendering depending
   * in user's arguments.
   *
   * This function gets passed a pointer to the Imlib images and uses a global char*
   * variable "fitOpt", which has the fit option.
   *
   * Right now, the available fit options are the following:
   *   * FULLSCREEN,
   *   * FULLSCREEN CROPPED,
   *   * CENTERED,
   *   * TOP-LEFT,
   *   * BOTTOM-LEFT,
   *   * BOTTOM-RIGHT,
   *   * TOP-RIGHT.
   * Despite these being written with uppercase letters, the fitOpt is not case
   * sensitive.
   *
   * ImFit() will also modify the loaded Imlib images based on fitOpt.
   */

  /* This function is still a TODO and this is it's pseudo-code: */
  int fitOptLimit = 6;
  //int imWidth   = Imlib_get_image_width(*sampleImg);
  //int imHeight  = Imlib_get_image_height(*sampleImg);

  for(int temp = 0; temp < strlen(fitOpt); temp++) /* Uppercase every char of fitOpt */
    fitOpt[temp] = toupper(fitOpt[temp]);

  if(strcmp(fitOpt, "FULLSCREEN") == 0) {
    //Scale image based on width and height;
    //return position;
  }

  else if(strcmp(fitOpt, "FULLSCREEN CROPPED") == 0) {
    //pass images to crop function image based on width and height since the images
    //will suffer modifications;
    //return position;
  }

  else if(strcmp(fitOpt, "CENTERED") == 0) {
    //Determine the center of the image and XScreen's width and height and then
    //determine where the position should start from with that info;
  }

  else if(strcmp(fitOpt, "TOP-LEFT") == 0) {
    //Return the position as 0,0 since no modifications are needed;
  }

  else if(strcmp(fitOpt, "BOTTOM-LEFT") == 0) {
    //Determine XScreen's height;
    //Return position as (Xscreen height - img height),0;
  }

  else if(strcmp(fitOpt, "BOTTOM-RIGHT") == 0) {
    //Determine XScreen's width and height
    //Return position as (XScreen height - img height),(XScreen width - img width);
  }

  else if(strcmp(fitOpt, "TOP-RIGHT") == 0) {
    //Determine XScreen's width and height;
    //Return position as 0,(XScreen width - img width);
  }

  else {
    fprintf(stderr, "Fatal error! %s is not a valid fit option!\n"
                    "Please make sure fit is configured correctly\n",
            fitOpt);
    exit(EXIT_FAILURE);
  }
}
