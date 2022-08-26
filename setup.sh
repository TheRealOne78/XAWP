#!/bin/bash

# Check for root
if [[ "$EUID" != 0 ]]; then
	echo "Please run this script with super user permission!"
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
make install
EXIT_CODE=$?
if [[ ! $EXIT_CODE -eq 0 ]]; then
  printf "[e] Couldn't install! Please check if XAWP compiled successfully\n"
	exit $EXIT_CODE
fi
