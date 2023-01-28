/*
 * Copyright (C) 2023 TheRealOne78 <bajcsielias78@gmail.com>
 *
 * This file is part of the XAWP project
 *
 * XAWP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XAWP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XAWP. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __XAWP_H__
# define __XAWP_H__


#ifdef DEBUG
/* If DEBUG was defined somewhere else as false, redefine here */
# if DEBUG == false
/* If you want DEBUG as true, define it only here in A and B, unless you know
 * what you are doing. The values between A and B must be the same! */
#  define DEBUG false  /* A - MUST be the same as B! */
# endif
#else
# define DEBUG false  /* B - MUST be the same as A! */
#endif

/* The default wait time between frame changes */
#ifndef DEFAULT_FRAME_TIME
# define DEFAULT_FRAME_TIME 0.07
#endif

/* The fit option XAWP should use by default */
#ifndef DEFAULT_FIT_OPTION
# define DEFAULT_FIT_OPTION "fit"
#endif

/* The default XAWP config file */
#ifndef DEFAULT_CONFIG_FILE_PATH
# define DEFAULT_CONFIG_FILE_PATH "~/.config/xawp/xawp.conf"
#endif

typedef struct {
  Window root;
  Pixmap pixmap;
  Imlib_Context *render_context;
  int width, height;
} Monitor;

void help(void);
void version(void);

void term_handler(int signum);

void getImgCount(char str[PATH_MAX]);
void getImgPath(char str[PATH_MAX], int choice);
static int compare_fun(const void *p, const void *q);
void freeUsingPath(void);
void setRootAtoms(Display *display, Monitor *monitor);
void ImFit(Imlib_Image *image[]);

#endif
