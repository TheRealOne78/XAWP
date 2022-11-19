<p align="center">
  <img src="https://i.imgur.com/DxCHBP5.png" width="50%">
</p>

# X11 Animated Wallpaper Player 
X11 Animated Wallpaper Player - An X11 animated wallpaper player made to be easily configurable, be fast and work in compositors

XAWP is based on AlecsFerra's animated_wallpaper.c <a href="https://gist.github.com/AlecsFerra/ef1cc008990319f3b676eb2d8aa89903">github gist</a>.
This is an X11 alternative for <a href="https://github.com/glouw/paperview">glouw/paperview</a> which does not rely on SDL2.
Why use XAWP instead of paperview? You don't need SDL2 to run this, it can work in compositors and it should be faster.

Why not just use AlecsFerra's animated_wallpaper.c?

Unlike AlecsFerra's animated_wallpaper.c, XAWP can be installed more easily and can be configured directly in $HOME/.config/XAWP/XAWP.conf instead of modifying the source code line by line.<br>
For example, instead of adding the path of the wallpaper files in the source code, the user can add the path of folder containing the animated wallpaper picture frames with the following options:

1. by writing the directory into the configuration file
2. or by appending to the command an argument to the directory

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
* [ ] Documentation and populate this README.md file and wiki

## Wiki
Get help from the [wiki](https://github.com/TheRealOne78/XAWP/wiki)!

## Quick note!
In present, xawp is still buggy and it's advised to use the code from the [stable branch](https://github.com/TheRealOne78/XAWP/tree/stable)
