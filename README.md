<p align="center">
  <img src="./icons/logo_gradient_v2.svg" width="50%">
</p>

# X11 Animated Wallpaper Player 

X11 Animated Wallpaper Player - An X11 animated wallpaper player made to be easily configurable, be fast and work in compositors

XAWP is based on AlecsFerra's animated_wallpaper.c [github gist](https://gist.github.com/AlecsFerra/ef1cc008990319f3b676eb2d8aa89903).<br>
This is an X11 alternative for [glouw/paperview](https://github.com/glouw/paperview) which does not rely on use of SDL2.

### Why use XAWP instead of paperview?

You don't need SDL2 to run this, it can work in compositors and it _should_ be faster.

### Why not just use AlecsFerra's animated_wallpaper.c?

Unlike AlecsFerra's animated_wallpaper.c, XAWP can be installed more easily and can be configured directly in ~/.config/XAWP/XAWP.conf instead of modifying the source code line by line.<br>
For example, instead of adding the path of the wallpaper files in the source code, the user can add the path of folder containing the animated wallpaper picture frames with the following options:

1. By writing the directory into the configuration file
2. By appending to the command an argument to the directory

## Dependencies:

* bash
* gcc
* make (gmake for BSD)
* imlib2
* libx11
* libconfig
* lolcat (optional)

## TODO:

* [x] Add arguments
* [x] Add configuration file
* [ ] Add fit
* [ ] Automate ImageMagick picture converting from gif to other formats with user prompt
* [ ] Add major desktop environments support
* [ ] Documentation and populate this README.md file and wiki

## Wiki

Get help from the [wiki](https://github.com/TheRealOne78/XAWP/wiki)!

## Quick note!

In present time, XAWP is still buggy and it's advised to use the [stable version](https://github.com/TheRealOne78/XAWP/tree/stable) if you are not developing for XAWP.
