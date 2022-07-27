#!/bin/bash

#Install Dependencies

if [ -x "$(command -v apt-get)" ]; then
	printf "Apt detected\nInstalling dependencies\n"
	sudo apt-get install -y libimlib2-dev install libconfig-dev
else
	printf "Apt NOT detected\nInstall libimlib2-dev libconfig-dev manually\n"
	exit 1; fi

make
printf "Installed!\n"
exit 0
