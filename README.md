# XAWP
XAWP - X11 Animated Wallpaper Player - A program that plays animated wallapers in pure X11

XAWP is based on AlecsFerra's animated_wallpaper.c <a href="https://gist.github.com/AlecsFerra/ef1cc008990319f3b676eb2d8aa89903">github gist</a>.
This is an X11 alternative for <a href="https://github.com/glouw/paperview">glouw/paperview</a> that does not rely on SDL2.
Why use XAWP instead of paperview? You don't need SDL2 to run this, it can work in compositors and it's just a little more lightweight.

Why not just use AlecsFerra's animated_wallpaper.c?
XAWP can be installed more easily and can be configured directly in $HOME/.config/XAWP/XAWP.conf instead of modifying the source code line by line. For example, instead of adding the path of the wallpaper files in the source code, the user can add the path of folder containing the animated wallpaper. It can also be noted that every user has it's own configuration.
