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

#ifndef __DIR_CHECKER_H__
# define __DIR_CHECKER_H__

/* Get the maximum path size based of
 * different operating systems. */
#ifndef PATH_MAX
  #ifdef __linux__
    #include <linux/limits.h>
  #elif BSD
    #include <limits.h>
  #elif __APPLE__
    #include <limits.h>
  #endif
#endif

void formatPath(char *path, char formattedPath[PATH_MAX]);
void verifyDirPath(char path[PATH_MAX]);

#endif
