/*
 * Copyright (C) 2023 TheRealOne78 <bajcsielias78@gmail.com>
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

/* Basic programming*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

/* XAWP created header files */
#include "fancy-text.h"
#include "dir-checker.h"

void formatPath(char *path, char formattedPath[PATH_MAX]) {

  /*
   * This function checks if the first character is a '~'.
   * If so, this function will replace the '~' with a propper
   * "/home/user/" path.
   *
   * Thanks to OpenAI's ChatGPT for all the help!
   */

  if(path[0] == '~') {
    char *homeDir = getenv("HOME");
    if(homeDir != NULL) {
      size_t homeDir_len = strlen(homeDir);
      size_t path_len = strlen(path);
       if(homeDir_len + path_len < PATH_MAX) {
         strcpy(formattedPath, homeDir);
         strcat(formattedPath, path+1);
      }
    }
    else {
      fprintf(stderr, ERR_TEXT_PUTS"You are homeless. Seriously, there is no HOME variable: %x\n", homeDir[0]);
    }
  }
  else {
    strcpy(formattedPath, path);
  }
}

void verifyDirPath(char path[PATH_MAX]) {

  /*
   * This function checks and creates directories for the
   * taget directory through an iteration, like `mkdir -p`
   * does.
   *
   * Thanks to OpenAI's ChatGPT for all the help!
   */

  if (access(path, F_OK) == 0) {
    /* Default config dir exists */
    return;
  }
  else {
    char tmpStr[PATH_MAX];
    char *ppath = NULL;
    size_t len;

    /* Copy path to tmpStr */
    snprintf(tmpStr, sizeof(tmpStr), "%s", path);
    len = strlen(tmpStr);

    /* If the path ends with '/', replace it with a NULL terminator */
    if(tmpStr[len - 1] == '/')
      tmpStr[len - 1] = '\0';

    /* Iterate over all characters.
     *
     * If there is a '/', temporarily replace it with a NULL terminator,
     * create the directory and replace back the '/'.
     */
    for(ppath = tmpStr + 1; *ppath; ppath++) {
      if(*ppath == '/') {
        *ppath = '\0';
        if(mkdir(tmpStr, S_IRWXU) != 0) {
          if(errno != EEXIST) {
            fprintf(stderr, ERR_TEXT_PUTS"Error creating directory '%s': %s\n", tmpStr, strerror(errno));
            return;
          }
        }
        *ppath = '/';
      }
    }

    /* Finally, create the wanted target directory */
    if(mkdir(tmpStr, S_IRWXU) != 0) {
      if(errno != EEXIST) {
        fprintf(stderr, ERR_TEXT_PUTS"Error creating directory '%s': %s\n", tmpStr, strerror(errno));
        return;
      }
    }
  }
}
