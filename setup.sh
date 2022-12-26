#!/bin/bash

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
ENDCOLOR="\e[0m"

INFO="["$BLUE"i"$ENDCOLOR"]"
WARN="["$YELLOW"w"$ENDCOLOR"]"
ERR="["$RED"e"$ENDCOLOR"]"

# Check for root
if [[ "$EUID" != 0 ]]; then
  printf "$ERR Please run this script with super user permission!\n"
  exit $EUID
fi

# Install Dependencies
bash ./configure.sh
EXIT_CODE=$?
if [[ ! $EXIT_CODE -eq 0 ]]; then
  exit $EXIT_CODE;
fi

# Compile
bash ./compile.sh
EXIT_CODE=$?
if [[ ! $EXIT_CODE -eq 0 ]]; then
  exit $EXIT_CODE
fi

# Install
## BSD family
if [ -x "$(command -v pkg)" ] || [ -x "$(command -v pkg_add)" ] || [ -x "$(command -v pkgin)" ]; then
  gmake install
## Linux
else
  make install
fi
EXIT_CODE=$?
if [[ ! $EXIT_CODE -eq 0 ]]; then
  printf "$ERR Couldn't install! Please check if XAWP compiled successfully\n"
  exit $EXIT_CODE
fi

# Clean
## BSD family
if [ -x "$(command -v pkg)" ] || [ -x "$(command -v pkg_add)" ] || [ -x "$(command -v pkgin)" ]; then
  gmake clean
## Linux
else
  make clean
fi
EXIT_CODE=$?
if [[ ! $EXIT_CODE -eq 0 ]]; then
  printf "$ERR Couldn't clean up\n"
  exit $EXIT_CODE
fi
