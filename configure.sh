#!/bin/bash

RED="\e[31m"
YELLOW="\e[33m"
BLUE="\e[34m"
ENDCOLOR="\e[0m"

INFO="["$BLUE"i"$ENDCOLOR"]"
WARN="["$YELLOW"w"$ENDCOLOR"]"
ERR="["$RED"e"$ENDCOLOR"]"

# Check for root
if [[ "$EUID" != 0 ]]; then
  printf "$ERR Please run this script with super user permission!\n"
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
  printf "$INFO gcc not detected, adding it in the dependencies install queue\n"
  DEPENDENCIES="$DEPENDENCIES gcc"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES gcc"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES gcc"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES gcc"
  WILL_INSTALL=true
fi

if [ ! -x "$(command -v make)" ]; then
  printf "$INFO make not detected, adding it in the dependencies install queue\n"
  DEPENDENCIES="$DEPENDENCIES make"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES make"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES make"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES gmake"
  WILL_INSTALL=true
fi

if [ ! -f "/usr/include/Imlib2.h" ]; then
  printf "$INFO imlib2 not detected, adding it in the dependencies install queue\n"
  DEPENDENCIES="$DEPENDENCIES imlib2"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES libimlib2-dev"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES imlib2-devel"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES imlib2"
  WILL_INSTALL=true
fi

if [ ! -d "/usr/include/X11" ]; then
  printf "$INFO libx11 not detected, adding it in the dependencies install queue\n"
  DEPENDENCIES="$DEPENDENCIES libx11"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES libx11-dev"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES libx11-devel"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES libx11"
  WILL_INSTALL=true
fi

if [ ! -f "/usr/include/libconfig.h" ]; then
  printf "$INFO libconfig not detected, adding it in the dependencies install queue\n"
  DEPENDENCIES="$DEPENDENCIES libconfig"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES libconfig-dev"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES libconfig-devel"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES libconfig"
  WILL_INSTALL=true
fi

# Choosing the right package manager
## Linux distributions
if [ $WILL_INSTALL == true ]; then
  ### Debian
  if [ -x "$(command -v apt-get)" ]; then
    printf "$INFO apt-get package manager detected\n"
    PKG_MGR="apt-get"

  ### Arch
  elif [ -x "$(command -v pacman)" ]; then
    printf "$INFO pacman package manager detected\n"
    PKG_MGR="pacman"

  ### Fedora
  elif [ -x "$(command -v dnf)" ]; then
    printf "$INFO dnf package manager detected\n"
    PKG_MGR="dnf"

  ### CentOS - imlib2 lib/devel does not exist
  #elif [ -x "$(command -v yum)" ]; then
  #  printf "$IFNO yum package manager detected\n"
  #  PKG_MGR="yum"

  ### Gentoo
  elif [ -x "$(command -v emerge)" ]; then
    printf "$ERR Portage detected!\n\
$ERR Automatic package instalation with portage may lead to package conflicts.\n\
$ERR Please install$DEPENDENCIES manually and run this file again to compile and install XAWP in your system!\n\
$ERR To install these dependencies, try \"sudo emerge --ask $DEPENDENCIES\".\n
$ERR For more info, see https://wiki.gentoo.org/wiki/Emerge and https://wiki.gentoo.org/wiki/Portage\n"
    exit 1

  ## BSD Family
  ### FreeBSD
  elif [ -x "$(command -v pkg)" ]; then
    printf "$INFO pkg package manager detected\n"
    PKG_MGR="pkg"

  ### OpenBSD
  elif [ -x "$(command -v pkg_add)" ]; then
    printf "$INFO pkg_add detected\n"
    printf "$WARN If you are using OpenBSD, you might not have a good desktop experience because of OpenBSD's lack of desktop support.\n"
    PKG_MGR="pkg_add"

  ### NetBSD
  elif [ -x "$(command -v pkgin)" ]; then
    printf "$INFO pkgin package manager detected\n"
    PKG_MGR="pkgin"

  ### No package manager
  else
    printf "$ERR A valid package manager was not found!\n\
  $ERR Please install$DEPENDENCIES manually and run this file again to compile and install XAWP in your system!\n"
    exit 1
  fi
  printf "$INFO $PKG_MGR will be used to install the required dependencies\n"
  # Install dependencies
  ## Debian
  if [ $PKG_MGR == "apt-get" ]; then
    $PKG_MGR install -y $DEB_DEPENDENCIES
  ## Arch
  elif [ $PKG_MGR == "pacman" ]; then
    $PKG_MGR -Sy --noconfirm $DEPENDENCIES
  ## RedHat
  elif [ $PKG_MGR == "dnf" ]; then
    $PKG_MGR install -y $DEPENDENCIES
  ## FreeBSD
  elif [ $PKG_MGR == "pkg" ]; then
    $PKG_MGR install -y $BSD_DEPENDENCIES
  ## OpenBSD
  elif [ $PKG_MGR == "pkg_add" ]; then
    $PKG_MGR $BSD_DEPENDENCIES
  ## NetBSD
  elif [ $PKG_MGR == "pkgin" ]; then
    $PKG_MGR -y install $BSD_DEPENDENCIES
  fi
  if [ $? -eq 0 ]; then
    printf "$INFO Packages installed successfully\n"
  else
    printf "$ERR Error: $PKG_MGR returned with exit code $?, aborting!\n"
    exit 1
  fi

else
  printf "$INFO Nothing to install, skipping configure.sh\n"
fi
