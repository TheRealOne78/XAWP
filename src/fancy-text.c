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

#include <stdio.h>
#include <stdlib.h>

#include "fancy-text.h"
#include "info.h"

void puts_logo() {
  /* Outputs XAWP ascii logo with lolcat, a tool to
   * color text on terminal with gradient.
   * Not a hard dependency */
  if(system("[[ $(command -v lolcat) ]]") == 0) {
    if(system("printf \" /\\$\\$   /\\$\\$  /\\$\\$\\$\\$\\$\\$  /\\$\\$      /\\$\\$ /\\$\\$\\$\\$\\$\\$\\$\\n| \\$\\$  / \\$\\$ /\\$\\$__  \\$\\$| \\$\\$  /\\$ | \\$\\$| \\$\\$__  \\$\\$\\n|  \\$\\$/ \\$\\$/| \\$\\$  \\ \\$\\$| \\$\\$ /\\$\\$\\$| \\$\\$| \\$\\$  \\ \\$\\$\\n \\  \\$\\$\\$\\$/ | \\$\\$\\$\\$\\$\\$\\$\\$| \\$\\$/\\$\\$ \\$\\$ \\$\\$| \\$\\$\\$\\$\\$\\$\\$\\n  >\\$\\$  \\$\\$ | \\$\\$__  \\$\\$| \\$\\$\\$\\$_  \\$\\$\\$\\$| \\$\\$____/\\n /\\$\\$/\\  \\$\\$| \\$\\$  | \\$\\$| \\$\\$\\$/ \\  \\$\\$\\$| \\$\\$      \\n| \\$\\$  \\ \\$\\$| \\$\\$  | \\$\\$| \\$\\$/   \\  \\$\\$| \\$\\$      \\n|__/  |__/|__/  |__/|__/     \\__/|__/      \\n\" | lolcat") != 0) {
      fprintf(stderr, ERR_TEXT_PUTS"Error: The system() call did not return a non-zero exit code. This error can be ignored.");
    }
  }

  /* Outputs XAWP ascii logo without gradient */
  else {
    printf("\n" /* print logo */
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

  /* After printing the logo, print the title and version as well */
  printf(KBWHT "X11 Animated Wallpaper Player v%s\n\n" RST, VERSION);


  /* ASCII art:
   *
   *  /$$   /$$  /$$$$$$  /$$      /$$ /$$$$$$$
   * | $$  / $$ /$$__  $$| $$  /$ | $$| $$__  $$
   * |  $$/ $$/| $$  \ $$| $$ /$$$| $$| $$  \ $$
   *  \  $$$$/ | $$$$$$$$| $$/$$ $$ $$| $$$$$$$/
   *   >$$  $$ | $$__  $$| $$$$_  $$$$| $$____/
   *  /$$/\  $$| $$  | $$| $$$/ \  $$$| $$
   * | $$  \ $$| $$  | $$| $$/   \  $$| $$
   * |__/  |__/|__/  |__/|__/     \__/|__/
   *
   * ASCII art generated from patorjk.com/software/taag
   *
   * #-> bigmoney-ne : by nathan bloomfield (xzovik@gmail.com)
   * #-> based on art from the legendary MAKEMONEYFAST chain letter
   * #->
   * #-> History:
   * #->   5-30-2007 : first version (required characters only)
   * #->
   * #-> (end comments)                                           */
}
