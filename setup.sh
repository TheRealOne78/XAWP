#!/bin/bash

DEPENDENCIES="gcc make imlib2 libx11 libconfig"
DEB_DEPENDENCIES="gcc make libimlib2-dev libx11-dev libconfig-dev"
RPM_DEPENDENCIES="gcc make imlib2-devel libX11 libconfig"
BSD_DEPENDENCIES="gcc gmake imlib2 libx11 libconfig"
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
elif [ -x "$(command -v pacman)" ]; then
	printf "pacman detected\nInstalling dependencies...\n"
	pacman -S --noconfirm $DEPENDENCIES

# Fedora
elif [ -x "$(command -v dnf)" ]; then
	printf "dnf detected\nInstalling dependencies...\n"
	dnf install -y $RPM_DEPENDENCIES

# CentOS - imlib2 lib/devel does not exist
#elif [ -x "$(command -v yum)" ]; then
# printf "yum detected\nInstalling dependencies...\n"
#	yum install -y $RPM_DEPENDENCIES

# Gentoo
elif [ -x "$(command -v emerge)" ]; then
	printf "portage detected!\n\
Automatic package instalation with portage may lead to package conflicts.\n\
Please install $DEPENDENCIES manually and run as root \"make -j\$(nproc); make install\" to compile and install in your system!\n"
  exit 1

##BSD Family
# FreeBSD
elif [ -x "$(command -v pkg)" ]; then
	printf "pkg detected\nInstalling dependencies...\n"
	pkg install -y $BSD_DEPENDENCIES

# OpenBSD
elif [ -x "$(command -v pkg_add)" ]; then
	printf "pkg_add detected\nInstalling dependencies...\n"
	pkg_add -U $BSD_DEPENDENCIES

# NetBSD
elif [ -x "$(command -v pkgin)" ]; then
	printf "pkgin detected\nInstalling dependencies...\n"
	pkgin -y install $BSD_DEPENDENCIES

else
printf "A valid package manager was not found! Please install $DEPENDENCIES manually and run as root \"make -j\$(nproc); make install\"" to compile and install in your system!\n""
fi

# Compile
# FreeBSD
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
	printf "Compiled! Please run \"make install\" as super user to install in your system.\n"

else
	printf "Something went wrong!"
	exit $EXIT_CODE
fi
