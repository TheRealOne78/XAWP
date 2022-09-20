#!/bin/bash

# Check for root
if [[ "$EUID" != 0 ]]; then
	echo "Please run this script with super user permission!"
	exit 1
fi

# Basic global variables
PKG_MGR=""          # The package manager that will install the dependencies
WILL_INSTALL=false  # Check if anything will be installed, else skip

# List of dependencies
# Common Linux: gcc make imlib2 libx11 libconfig
# Debian:       gcc make libimlib2-dev libx11-dev libconfig-dev
# RHEL:         gcc make imlib2-devel libX11-devel libconfig-devel
# BSD:          gcc gmake imlib2 libx11 libconfig"

# Checking existing dependencies
if [ ! -x "$(command -v gcc)" ]; then
	printf "[i] gcc not detected, adding it in the dependencies install queue\n"
	DEPENDENCIES="$DEPENDENCIES gcc"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES gcc"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES gcc"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES gcc"
	WILL_INSTALL=true
fi

if [ ! -x "$(command -v make)" ]; then
	printf "[i] make not detected, adding it in the dependencies install queue\n"
	DEPENDENCIES="$DEPENDENCIES make"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES make"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES make"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES gmake"
	WILL_INSTALL=true
fi

if [ ! -f "/usr/include/Imlib2.h" ]; then
	printf "[i] imlib2 not detected, adding it in the dependencies install queue\n"
	DEPENDENCIES="$DEPENDENCIES imlib2"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES libimlib2-dev"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES imlib2-devel"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES imlib2"
	WILL_INSTALL=true
fi

if [ ! -d "/usr/include/X11" ]; then
	printf "[i] libx11 not detected, adding it in the dependencies install queue\n"
	DEPENDENCIES="$DEPENDENCIES libx11"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES libx11-dev"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES libx11-devel"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES libx11"
	WILL_INSTALL=true
fi

if [ ! -f "/usr/include/libconfig.h" ]; then
	printf "[i] libconfig not detected, adding it in the dependencies install queue\n"
	DEPENDENCIES="$DEPENDENCIES libconfig"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES libconfig-dev"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES libconfig-devel"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES libconfig"
	WILL_INSTALL=true
fi

# Choosing the right package manager
## Linux distributions
### Debian
if [ $WILL_INSTALL == true ]; then
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
[e] Please install$DEPENDENCIES manually and run this file again to compile and install XAWP in your system!\n"
    exit 1

  ##BSD Family
  ### FreeBSD
  elif [ -x "$(command -v pkg)" ]; then
  	printf "[i] pkg package manager detected\n"
  	PKG_MGR="pkg"

  ### OpenBSD
  elif [ -x "$(command -v pkg_add)" ]; then
  	printf "[i] pkg_add detected\n"
  	printf "[w] If you are using OpenBSD, you might not have a good desktop experience because of OpenBSD's lack of desktop support.\n"
    PKG_MGR="pkg_add"

  ### NetBSD
  elif [ -x "$(command -v pkgin)" ]; then
  	printf "[i] pkgin package manager detected\n"
  	PKG_MGR="pkgin"

  ### No package manager
  else
    printf "[e] A valid package manager was not found!\n\
  [e] Please install$DEPENDENCIES manually and run this file again to compile and install XAWP in your system!\n"
    exit 1
  fi
  printf "[i] $PKG_MGR will be used to install the required dependencies\n"

  if [ $PKG_MGR == "apt-get" ]; then
  	$PKG_MGR install $DEB_DEPENDENCIES
  	if [ $? -eq 0 ]; then
  	  printf "[i] Packages installed successfully\n"
  	else
  		printf "[e] Error: $PKG_MGR returned with exit code $?, aborting!\n"
  		exit 1
  	fi

  elif [ $PKG_MGR == "pacman" ]; then
  	$PKG_MGR -Sy $DEPENDENCIES
  	if [ $? -eq 0 ]; then
  	  printf "[i] Packages installed successfully\n"
  	else
  		printf "[e] Error: $PKG_MGR returned with exit code $?, aborting!\n"
  		exit 1
  	fi

  elif [ $PKG_MGR == "dnf" ]; then
  	$PKG_MGR install $DEPENDENCIES
  	if [ $? -eq 0 ]; then
  	  printf "[i] Packages installed successfully\n"
  	else
  		printf "[e] Error: $PKG_MGR returned with exit code $?, aborting!\n"
  		exit 1
  	fi

  elif [ $PKG_MGR == "emerge" ]; then
  	$PKG_MGR $DEPENDENCIES
  	if [ $? -eq 0 ]; then
  	  printf "[i] Packages installed successfully\n"
  	else
  		printf "[e] Error: $PKG_MGR returned with exit code $?, aborting!\n"
  		exit 1
  	fi

  elif [ $PKG_MGR == "pkg" ]; then
  	$PKG_MGR $BSD_DEPENDENCIES
  	if [ $? -eq 0 ]; then
  	  printf "[i] Packages installed successfully\n"
  	else
  		printf "[e] Error: $PKG_MGR returned with exit code $?, aborting!\n"
  		exit 1
  	fi

  elif [ $PKG_MGR == "pkg_add" ]; then
  	$PKG_MGR $BSD_DEPENDENCIES
  	if [ $? -eq 0 ]; then
  	  printf "[i] Packages installed successfully\n"
  	else
  		printf "[e] Error: $PKG_MGR returned with exit code $?, aborting!\n"
  		exit 1
  	fi

  elif [ $PKG_MGR == "pkgin" ]; then
  	$PKG_MGR $BSD_DEPENDENCIES
  	if [ $? -eq 0 ]; then
  	  printf "[i] Packages installed successfully\n"
  	else
  		printf "[e] Error: $PKG_MGR returned with exit code $?, aborting!\n"
  		exit 1
  	fi
  fi
else
	printf "[i] Nothing to install, skipping.\n"
fi
