#!/bin/sh

dependencies="gcc make libimlib2 libx11 libconfig"
deb-dependencies="gcc make libimlib2-dev libx11-dev libconfig-dev"

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
	apt-get install -y $deb-dependencies
	
# Arch
else if [ -x "$(command -v pacman)" ]; then
	printf "apt-get detected\nInstalling dependencies...\n"
	pacman -S $dependencies

# RHEL
else if [ -x "$(command -v rpm)" ]; then
	printf "apt-get detected\nInstalling dependencies...\n"
	rpm -i $dependencies

# Gentoo
else if [ -x "$(command -v emerge)" ]; then
	printf "Gentoo distribution detected!\n \
	Automatic package instalation with portage may lead to package conflicts. Please install $dependencies by hand"

##BSD Family
# FreeBSD
else if [ -x "$(command -v pkg)" ]; then
	printf "pkg detected\nInstalling dependencies...\n"
	pkg install -y $dependencies

# OpenBSD
else if [ -x "$(command -v pkg_add)" ]; then
	printf "pkg detected\nInstalling dependencies...\n"
	pkg_add $dependencies
	
# NetBSD
else if [ -x "$(command -v pkgin)" ]; then
	printf "pkg detected\nInstalling dependencies...\n"
	pkgin -y install $dependencies
	
fi

# Compile
make -($nproc)
printf "Compiled! Please run \"make install\" as super user to install in your system.\n"
exit 0
