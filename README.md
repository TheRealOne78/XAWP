# XAWP - X11 Animated Wallpaper Player 
X11 Animated Wallpaper Player - An X11 animated wallpaper player made to be easily configurable, be fast and work in compositors

XAWP is based on AlecsFerra's animated_wallpaper.c <a href="https://gist.github.com/AlecsFerra/ef1cc008990319f3b676eb2d8aa89903">github gist</a>.
This is an X11 alternative for <a href="https://github.com/glouw/paperview">glouw/paperview</a> which does not rely on SDL2.
Why use XAWP instead of paperview? You don't need SDL2 to run this, it can work in compositors and it should be faster.

Why not just use AlecsFerra's animated_wallpaper.c?

Unlike AlecsFerra's animated_wallpaper.c, XAWP can be installed more easily ~~and can be configured directly in $HOME/.config/XAWP/XAWP.conf instead of modifying the source code line by line.~~<br>
For example, instead of adding the path of the wallpaper files in the source code, the user can add the path of folder containing the animated wallpaper picture frames with the following options:

1. ~~by writing the directory into the configuration file~~
2. or by appending to the command an argument to the directory


TODO:

- [x] Add arguments
- [ ] Add configuration file
- [ ] Add fit
- [ ] Automate ImageMagick picture converting from gif to other formats with user prompt
- [ ] Documentation and populate this README.md file

Quick note!
In present, the code is not functional giving segmentation fault immediately after opening it. As there is no release yet, please use the stable branch https://github.com/TheRealOne78/XAWP/tree/stable. To compile use `make`, to install use `make install` as root and to use XAWP, append to the command the absolute directory + wildcard(\*) eg: `/home/foo/aniwall/*. It won't work with pictures that are not in order. Also make sure there isn't any other file in that directory.
