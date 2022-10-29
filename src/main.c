#define _POSIX_C_SOURCE 199309L

#define _DEBUG true
#define DEBUG_TEXT_PUTS "\x1B[34mDEBUG\x1B[0m"

#define MAX_PATH 4096

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

// Basic constants
const char author[] = "TheRealOne78";
const char authorMail[] = "bajcsielias78@gmail.com";
const char ver[] = "0.0.1";

const char appendDefaultConfPath[] = "/.config/xawp/xawp.conf"; /* This path will be concatenated with HOME envar */

/* Basic config/argument variables */
bool DEBUG = _DEBUG;     /* If true, print debug info */

char pathConf[MAX_PATH]; /* path to images directory, from configuration file */
char pathArg[MAX_PATH];  /* path to images directory, from user argument (not -c) */

char confPath[MAX_PATH]; /* path to configuration file */

unsigned imgCount;       /* number of images */

char **pConfPath;        /* pointers to paths of images, from configuration file */
char **pArgPath;         /* pointers to paths of images, from user argument (not -c) */
char ***pUsingPath = &pConfPath; /* pointer to the using paths - TO BE REMOVED **TODO** */

double timeConf;         /* time between frames, from configuration file */
double timeArg;          /* time between frames, from user argument (not -c) */

bool isArgConf = false;  /* If true, the configuration file from argument will be used */

bool hasArgTime = false; /* If true, time from user argument will be used */
bool hasArgDir = false;  /* If true, the directory will be used from user argument */

/* Miscellaneous variables */
bool hasCurrentDir = false; /* If true, the directory containing images has a current directory file: ./ */
bool hasParentDir = false;  /* If true, the directory containing images has a parent directory file: ../ */

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
void ImFit(char *sampleImg, int fitOpt);


int main(int argc, char *argv[]) {
  char configTime[6];
  configTime[0] = '\0';

  // Do stuff with the arguments
  static struct option long_options [] = {
    { "help"     , no_argument      , NULL , 'h' },
    { "time"     , required_argument, NULL , 't' },
    { "version"  , no_argument      , NULL , 'v' },
    { "debug"    , no_argument      , NULL , 'D' },
    { "fit"      , required_argument, NULL , 'f' }, // Not implemented yet - This is a feature that XAWP will fit the photo based on user's requirements
    { "directory", required_argument, NULL , 'd' }, // Not implemented yet - This will make the user prompt photos after the --directory/-d option
    { "config"   , required_argument, NULL , 'c' }, // Not implemented yet - This will make the make the user prompt another config file than the default one
    { NULL       , 0                , NULL , 0   }
  };

  while(1) {
    int c = getopt_long(argc, argv, "ht:vDf:d:c:", long_options, NULL);
    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c) {
      case 'D':
        DEBUG = !DEBUG;
        if(DEBUG==true)
          fprintf(stdout, "Enabled debug\n");
        break;

      case 'h':
        help();
        exit(0);
        break;

      case 't':
        snprintf(configTime, sizeof(configTime), "%s", optarg);
        timeArg = atof(configTime);
        hasArgTime = true;
        break;

      case 'f':
        printf("Fit is not implemented yet, skipping...\n");
        //TODO: implement this fit
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
  /* print XAWP color logo in ASCII art. Remove a single backslash '\' to see the real logo */
  printf("\n"
    KYEL" /$$   /$$"  KRED"  /$$$$$$ "  KMAG" /$$      /$$"  KBCYN" /$$$$$$$ "  RST"\n"
    KYEL"| $$  / $$"  KRED" /$$__  $$"  KMAG"| $$  /$ | $$"  KBCYN"| $$__  $$"  RST"\n"
    KYEL"|  $$/ $$/"  KRED"| $$  \\ $$"  KMAG"| $$ /$$$| $$"  KBCYN"| $$  \\ $$"  RST"\n"
    KYEL" \\  $$$$/ "  KRED"| $$$$$$$$"  KMAG"| $$/$$ $$ $$"  KBCYN"| $$$$$$$/"  RST"\n"
    KYEL"  >$$  $$ "  KRED"| $$__  $$"  KMAG"| $$$$_  $$$$"  KBCYN"| $$____/ "  RST"\n"
    KYEL" /$$/\\  $$"  KRED"| $$  | $$"  KMAG"| $$$/ \\  $$$"  KBCYN"| $$      "  RST"\n"
    KYEL"| $$  \\ $$"  KRED"| $$  | $$"  KMAG"| $$/   \\  $$"  KBCYN"| $$      "  RST"\n"
    KYEL"|__/  |__/"  KRED"|__/  |__/"  KMAG"|__/     \\__/"  KBCYN"|__/      "  RST"\n"
    KBWHT "X11 Animated Wallpaper Player v%s\n\n" RST, ver
  );
  /* ASCII art generated from patorjk.com/software/taag
   *
   * #-> bigmoney-ne : by nathan bloomfield (xzovik@gmail.com)
   * #-> based on art from the legendary MAKEMONEYFAST chain letter
   * #->
   * #-> History:
   * #->   5-30-2007 : first version (required characters only)
   * #->
   * #-> (end comments)                                           */

  double flt;
  int bln;

  config_t cfg;
  config_setting_t *setting;
  const char *str;
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

  if(hasArgDir) {
    getImgCount(&pathArg);
    getImgPath(&pathArg, 1);
  }
  else if(!hasArgDir && config_lookup_string(&cfg, "path", &str)) {
    strcpy(pathConf, str);
    getImgCount(&pathConf); //conf=0, arg=1
    getImgPath(&pathConf, 0);  //conf=0, arg=1
  }
  else
    fprintf(stderr, "No 'path' setting in configuration file.\n");

  if(!hasArgTime && config_lookup_float(&cfg, "time", &flt)) {
    timeConf = flt;
    if(DEBUG)
      fprintf(stdout, DEBUG_TEXT_PUTS": timeConf: %f\n", timeConf);
  }
  else
    fprintf(stderr, "No 'time' setting in configuration file.\n");

  if(config_lookup_bool(&cfg, "debug", &bln))
    DEBUG = bln;
  else
    fprintf(stderr, "No 'debug' setting in configuration file.\n");

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
  for(int temp = 0; temp < imgCount - fileOffset; temp++) {
    char tempImgPath[MAX_PATH]; /* To concatenate dir path + image name */
    images[temp] = imlib_load_image((*pUsingPath)[(fileOffset+temp)]);
    if(DEBUG)
      fprintf(stdout, DEBUG_TEXT_PUTS": Imlib loaded %s\n", (*pUsingPath)[(fileOffset+temp)]);
  }
  if(DEBUG)
    fprintf(stdout, "\n");
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
    fprintf(stdout, DEBUG_TEXT_PUTS": Starting render loop");

  struct timespec timeout;
  timeout.tv_sec  = 0;
  timeout.tv_nsec = 70000000;

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

      nanosleep(&timeout, NULL);
    }
  }
}

void help(void) {
  printf(                                                                               "\n"
         "XAWP - X11 Animated Wallpaper Player"                                         "\n"
         "Play animated wallpapers in X11 by passing XAWP a directory containing the"   "\n"
         "pictures frames wanted to be displayed."                                      "\n"
                                                                                        "\n"
         "Usage: [options] images_path"                                                 "\n"
                                                                                        "\n"
         "Options:"                                                                     "\n"
         "-h, --help \t Output this help list and exit"                                 "\n"
         "-t, --time \t Set the time XAWP needs to wait between the"                    "\n"
         "\t\t change of images: --time seconds.milliseconds"                           "\n"
         "-v, --version \t Output version information and license and exit"             "\n"
         "-D, --debug \t Output the debug log"                                          "\n"
                                                                                        "\n"
         "Note that XAWP uses a lot of resources like RAM and CPU!"                     "\n"
                                                                                        "\n"
        );
}

void version(void) {
  printf("XAWP version %s" /* version number */                                         "\n"
                                                                                        "\n"
         "Copyright (C) 2022 TheRealOne78"                                              "\n"
         "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
         "This is free software: you are free to change and redistribute it."           "\n"
         "There is NO WARRANTY, to the extent permitted by law."                        "\n"
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

  char ***pImgPath;

  switch(choice) {
    case 0:
      pImgPath = &pConfPath;
      if(DEBUG)
        fprintf(stdout, DEBUG_TEXT_PUTS": Using \"%s\" from configuration file\n", *str);
      break;
    case 1:
      pImgPath = &pArgPath;
      if(DEBUG)
        fprintf(stdout, DEBUG_TEXT_PUTS": Using user arguments for configurations\n", *str);
      break;
    default:
      fprintf(stderr, "Something went wrong: In function getImgPath(), choice is %d\n", choice);
      exit(EXIT_FAILURE);
      break;
  }
  *pImgPath = (char**)malloc(imgCount * sizeof(char*));

  DIR *d;
  struct dirent *dir;

  d = opendir(*str);
  int temp = 0;
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      (*pImgPath)[temp] = (char*)malloc(MAX_PATH * sizeof(char));
      strcpy((*pImgPath)[temp], *str);
      strcat((*pImgPath)[temp++], dir->d_name);
    }
    closedir(d);
  }
    /* readdir() dumps mixed files, so qsort will sort alphabetically */
    qsort(*pImgPath, imgCount, sizeof((*pImgPath)[0]), compare_fun);

    /* Prints all the selected files */
    if(DEBUG) {
      fprintf(stdout, "\n"DEBUG_TEXT_PUTS": Selected files:\n");
      for(int i = 0; i < imgCount; i++)
        fprintf(stdout, "  | File %d: %s\n", i, (*pImgPath)[i]);
      fprintf  (stdout, "  | **End**\n\n");
    }

    /* Now check if there are any "." and ".." files in path
     * in order to know where the actual images start     */
    char tempImgPath1dot[MAX_PATH];
    strcpy(tempImgPath1dot, *str);
    strcat(tempImgPath1dot, ".");
    if(strcmp((*pImgPath)[0], tempImgPath1dot) == 0) {
      hasCurrentDir = true;
      if(DEBUG)
        fprintf(stdout, DEBUG_TEXT_PUTS": \"%s\" has current directory file, skipping it.\n", *str);
    }
    else
      hasCurrentDir = false;

    char tempImgPath2dot[MAX_PATH];
    strcpy(tempImgPath2dot, *str);
    strcat(tempImgPath2dot, "..");
    if(strcmp((*pImgPath)[1], tempImgPath2dot) == 0) {
      hasParentDir = true;
      if(DEBUG)
        fprintf(stdout, DEBUG_TEXT_PUTS": \"%s\" has parent directory file, skipping it.\n", *str);
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
              (*pUsingPath)[temp], temp);
    free((*pUsingPath)[temp]);
  }
    if(DEBUG)
      fprintf(stdout, DEBUG_TEXT_PUTS": Unallocated dynamic array of images at address "KGRN"%p"RST"\n\n", (*pUsingPath));
    free((*pUsingPath));
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

void ImFit(char *sampleImg, int fitOpt) {
  /* This function is responsible for fitting the image when rendering depending
   * in user's arguments.
   *
   * This function gets passed 2 variables, a char pointer to the first image and
   * a int which tells how the image should fit
   * For example, if the int is 0 or NULL it will fit fullscreen or if fitOpt is 2,
   * the image will be rendered in the center of the screen.
   *
   * Right now, the available fit options are the following[0-6]:
   *
   *  ─────┬────────────────────
   *   Opt │ Fit name
   *  ─────┼────────────────────
   *   0   │ Fullscreen
   *       │
   *   1   │ Fullscreen Cropped
   *       │
   *   2   │ Centered
   *       │
   *   3   │ Left-up Corner
   *       │
   *   4   │ Left-down Corner
   *       │
   *   5   │ Right-down Corner
   *       │
   *   6   │ Right-up Corner
   *
   * The image passed through the char pointer will be analized for determining it's
   * width and height.
   *
   * The reason this function analyzes only the first image is because analyzing every
   * single image would be redundant, loosing a lot of time and CPU free usage. XAWP
   * is not supposed to run different width and height images, just the original coalesce
   * There is an exception however where all the images will suffer modifications       */

  /* This function is still a TODO and this is it's pseudo-code:
   * int fitOptLimit = 6;
   * int imWidth = Imlib_get_image_width(*sampleImg);
   * int imHeight = Imlib_get_image_height(*sampleImg);
   * switch(fitOpt) {
   *   case 0:
   *     Scale image based on width and height;
   *     return position;
   *     break;
   *   case 1:
   *     pass images to crop function image based on width and height since the images
   *     will suffer modifications;
   *     return position;
   *     break;
   *   case 2:
   *     Determine the center of the image and XScreen's width and height and then
   *     determine where the position should start from with that info;
   *     break;
   *   case 3:
   *     Return the position as 0,0 since no modifications are needed;
   *     break;
   *   case 4:
   *     Determine XScreen's height;
   *     Return position as (Xscreen height - img height),0;
   *     break;
   *   case 5:
   *     Determine XScreen's width and height
   *     Return position as (XScreen height - img height),(XScreen width - img width);
   *     break;
   *   case 6:
   *     Determine XScreen's width and height;
   *     Return position as 0,(XScreen width - img width);
   *     break;
   *   default:
   *     fprintf(stderr, "Fatal error! fitOpt is %d and should be [0-%d].\n"
   *                     "Please make sure fit is configured correctly\n",
   *             fitOpt, fitOptLimit);
   *     exit(EXIT_FAILURE);
   *     break;
   * }                                                                                */
}
