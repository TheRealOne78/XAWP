#!/bin/bash

printf "[i] Compiling with $(nproc) threads..."
make -j$(nproc)
EXIT_CODE=$?
if [[ $EXIT_CODE -eq 0 ]]; then
	printf "\n[i] Compiling successful! Install XAWP with \"make install\" as root.\n"
  exit $EXIT_CODE

else
	printf "\n[e] Compiling unsuccessful! Please check the debug info from above for more information\n"
	printf "[i] Exiting...\n"
	exit $EXIT_CODE
fi
