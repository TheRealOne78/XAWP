#!/bin/bash

# Check for root
if [[ "$EUID" != 0 ]]; then
	echo "Please run this script with super user permission!"
	exit 1
fi

# Basic global variables

TOTAL_DEPS=5 # Constant for all dependencies
ALL_DEPS=0 # To check if it matches TOTAL_DEPS after checking dependencies
PKG_MGR="" # The package manager that will install the dependencies

# List of dependencies
# common: gcc make imlib2 libx11 libconfig
# debian: gcc make libimlib2-dev libx11-dev libconfig-dev
# deb:    gcc make imlib2-devel libX11-devel libconfig-devel
# BSD:    gcc gmake imlib2 libx11 libconfig"

# Checking existing dependencies
if [ ! -x "$(command -v gcc)" ]; then
	printf "[i] gcc not detected, adding it in the dependencies install queue"
	DEPENDENCIES="$DEPENDENCIES gcc"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES gcc"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES gcc"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES gcc"
fi

if [ ! -x "$(command -v make)" ]; then
	printf "[i] gcc not detected, adding it in the dependencies install queue"
	DEPENDENCIES="$DEPENDENCIES make"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES make"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES make"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES gmake"
fi

# Choosing the right package manager
## Linux distributions
### Debian
if [ -x "$(command -v apt-get)" ]; then
	printf "[i] apt-get package manager detected\n"
  PKG_MGR="apt-get"

### Arch
elif [ -x "$(command -v pacman)" ]; then
	printf "[i] pacman package manager detected\n"
	PKG_MGR="pacman"

### Fedora
elif [ -x "$(command -v dnf)" ]; then
	printf "[i] dnf package manager detected\n"
	PKG_MGR="dnf"

### CentOS - imlib2 lib/devel does not exist
#elif [ -x "$(command -v yum)" ]; then
# printf "yum package manager detected\n"
#	PKG_MGR="yum"

### Gentoo
elif [ -x "$(command -v emerge)" ]; then
	printf "[e] Portage detected!\n\
[e] Automatic package instalation with portage may lead to package conflicts.\n\
[e] Please install $DEPS manually and run this file again to compile and install XAWP in your system!\n"
  exit 1

##BSD Family
### FreeBSD
elif [ -x "$(command -v pkg)" ]; then
	printf "[i] pkg package manager detected\n"
	PKG_MGR="pkg"

### OpenBSD
elif [ -x "$(command -v pkg_add)" ]; then
	printf "[i] pkg_add detected\n"
	printf "[w] If you are using OpenBSD, you might not have a good desktop experience because of OpenBSD's lack of desktop support."
  PKG_MGR="pkg_add"

### NetBSD
elif [ -x "$(command -v pkgin)" ]; then
	printf "[i] pkgin package manager detected\n"
	PKG_MGR="pkgin"

else
  printf "[e] A valid package manager was not found!\n\
[e] Please install $DEPS manually and run this file again to compile and install XAWP in your system!\n"
  exit 1
fi
printf "[i] $PKG_MGR will be used to install the required dependencies"
