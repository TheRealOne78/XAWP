#!/bin/bash

DEPENDENCIES="gcc make libimlib2 libx11 libconfig"
DEB_DEPENDENCIES="gcc make libimlib2-dev libx11-dev libconfig-dev"

# Check for root
if [[ "$EUID" != 0 ]]; then
	echo "Please run this script with super user permission!"
	exit 1
fi
    
# Install Dependencies

##Linux distributions
# Debian
if [ -x "$(command -v apt-get)" ]; then
	printf "apt-get detected\nInstalling dependencies...\n"
	apt-get install -y $DEB_DEPENDENCIES
	
# Arch
else if [ -x "$(command -v pacman)" ]; then
	printf "apt-get detected\nInstalling dependencies...\n"
	pacman -S $DEPENDENCIES

# RHEL
else if [ -x "$(command -v rpm)" ]; then
	printf "apt-get detected\nInstalling dependencies...\n"
	rpm -i $DEPENDENCIES

# Gentoo
else if [ -x "$(command -v emerge)" ]; then
	printf "Gentoo distribution detected!\n \
	Automatic package instalation with portage may lead to package conflicts. Please install $DEPENDENCIES by hand"

##BSD Family
# FreeBSD
else if [ -x "$(command -v pkg)" ]; then
	printf "pkg detected\nInstalling dependencies...\n"
	pkg install -y $DEPENDENCIES

# OpenBSD
else if [ -x "$(command -v pkg_add)" ]; then
	printf "pkg detected\nInstalling dependencies...\n"
	pkg_add $DEPENDENCIES
	
# NetBSD
else if [ -x "$(command -v pkgin)" ]; then
	printf "pkg detected\nInstalling dependencies...\n"
	pkgin -y install $DEPENDENCIES
	
fi

# Compile
make -$(nproc)
printf "Compiled! Please run \"make install\" as super user to install in your system.\n"
exit 0
