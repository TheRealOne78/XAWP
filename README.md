<img src="https://i.imgur.com/DxCHBP5.png" width="50%">

# X11 Animated Wallpaper Player 
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
In present, the code is not functional (it doesn't give segmentation fault, config should be 80% implemented but imlib loading images is not implemented at all in this commit). As there is no release yet, please use the stable branch https://github.com/TheRealOne78/XAWP/tree/stable.

<br><br>Installation:
```
chmod +x setup.sh
./setup.sh
```
<br>It won't work with pictures that are not in order, so always be sure all the frames have all the digits. You won't have to correct the frame generation numbering scheme after auto-ImageMagick generator will be integrated.
<br>Example:
```
pic001.png  pic002.png  pic003.png ...
pic010.png  pic011.png  pic012.png ...
    .           .           .
    .           .           .
    .           .           .
pic100.png  pic101.png  pic100.png ...
```
<br>Also make sure there isn't any other file in that directory.
