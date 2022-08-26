#!/bin/bash

#Compiling
printf "[i] Compiling with $(nproc) threads..."
if [ -x "$(command -v pkg)" ]; then
	gmake -j$(nproc)
# OpenBSD
elif [ -x "$(command -v pkg_add)" ]; then
	gmake -j$(nproc)
# NetBSD
elif [ -x "$(command -v pkgin)" ]; then
	gmake -j$(nproc)
# Linux distributions
else
  make -j$(nproc)
fi
EXIT_CODE=$?

if [[ $EXIT_CODE -eq 0 ]]; then
	printf "\n[i] Compiling successful!\n"
  exit $EXIT_CODE

else
	printf "\n[e] Compiling unsuccessful! Please check the debug info from above for more information\n"
	printf "[i] Exiting...\n"
	exit $EXIT_CODE
fi
