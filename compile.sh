#!/bin/bash

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
ENDCOLOR="\e[0m"

INFO="["$BLUE"i"$ENDCOLOR"]"
WARN="["$YELLOW"w"$ENDCOLOR"]"
ERR="["$RED"e"$ENDCOLOR"]"

# Compiling
printf "$INFO Compiling with $(nproc) threads...\n"
## BSD family
if [ -x "$(command -v pkg)" ] || [ -x "$(command -v pkg_add)" ] || [ -x "$(command -v pkgin)" ]; then
  gmake -j$(nproc)
## Linux
else
  make -j$(nproc)
fi
EXIT_CODE=$?

if [[ $EXIT_CODE -eq 0 ]]; then
  printf "\n"$INFO" Compiling successful!\n"
  exit $EXIT_CODE

else
  printf "\n"$ERR" Compiling unsuccessful! Please check the debug info from above for more information\n"
  printf "$INFO Exiting...\n"
  exit $EXIT_CODE
fi
