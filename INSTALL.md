**NOTE**: This document refers only for installs made by the methods in this very document. XAWP developers are not responsible in any way for broken third party installs like unofficial distributions packages that are broken.

# Compiling and installing

## Automatically
The easiest and fastest method is by executing the following command:
```bash
root# bash ./setup.sh
```
This will ensure every dependency will be installed, the compiling and installing to the system.<br>
If manual compiling and installing is the wanted option, the text below will explain how.

## Compiling

### Installing the dependencies
The easiest approach is by running the `configure.sh` script:<br>

```bash
root# bash ./configure.sh
```
This bash script will automatically check and install all dependencies via the package manager the system is using.
The following package managers are available:
 - apt-get   (Debian, Ubuntu, Pop!_OS, Mint, etc.)
 - pacman    (Arch, Artix, Arco, Manjaro, etc.)
 - dnf       (Fedora)
 - pkg       (FreeBSD, (probaly Termux as well))
 - pkg_add   (OpenBSD)
 - pkgin     (NetBSD)
If the system the user wants to compile XAWP does not have any of these package managers, the next approach may be the solution. Gentoo Linux and CentOS Linux are some examples of distributions that are not compatible for automatic dependencies install.<br>

The manual approach is by selecting the packages and install them manually.
The dependencies XAWP needs for compiling and executing are the following:
 - gcc
 - make      (for Linux based systems)
 - gmake     (for BSD based systems)
 - imlib2    (dev)
 - libx11    (dev)
 - libconfig (dev)
**NOTE**: From system to system, the packages may have different name!

### Automatic compiling
The following command will be used for automatic compiling:
```bash
foo$ bash ./compile.sh
```
This method will use by default every thread available in the selected device, however the XAWP source code is not big and it should compile fast in modern computers.

### Manual compiling
The only command needed for compiling in Linux based distributions is `make` and for the BSD Family is `gmake`. It can be executed as following:
```bash
#Linux:
foo$ make -j$(nproc)
#BSD:
foo$ gmake -j$(nproc)
```
Of course, this command needs to be executed in the same directory with the `Makefile` file, otherwise the command will fail.
`-j$(nproc)` is the an argument specifing to use all the threads available in the system. Using less threads is quite simple, $(nproc) being modified with the number of CPU threads make will use for compiling. Eg: `make -j2 # will only use 2 CPU threads`

### Installing
Installing is easy as pie, the only command needed is the following:
```bash
root# make install
```

However, there is another manual option that requires copying by the final build to the system and making it executable:
```bash
root# cp ./build/xawp /usr/bin/ \# Copies the build to /usr/bin
root# chmod +x /usr/bin/xawp    \# Changes the permissions of the file to be executable
```

# Uninstalling
Uninstalling the compiled file can be made by running the following command:
```bash
root# make uninstall
```

If the user wants to uninstall manually or xawp was installed in another directory the follwing command will be executed: 
```bash
root# rm -f /usr/bin/xawp # removes xawp from the system
                          # change /usr/bin/xawp with the xawp path if it differs
```
