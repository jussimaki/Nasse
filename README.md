# Nasse
The original code for a platformer game, finished in 2010, compiles in VC++ 6, needs

fmod.dll version 3.7.5.0
and SDL.dll version 1.2.11.0

to run.

The current code does not actually start the game in its normal gameplay mode,
but in the level editor mode.

If I remember correctly, the game can be started in the normal mode by
commenting the lines 19 and 20, and then uncommenting the lines 23 and 24 in the file
main.cpp.

The git repository should include all the resource files (graphics, sounds etc.) to run the game,
but no executables or dll files are included.

If the code is to be compiled on a modern visual c++ compiler, some changes must be made for it to compile.

There are also some tools to convert resource files, for example .bmp picture files, into the game's own format,
but they are not yet included in the repository.
