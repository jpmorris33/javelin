# javelin

This is a port of my late 1990s platform game, Javelin, to modern systems.
The initial target is Linux, but it can probably be adapted to windows
with minimal hassle.

The 80286 assembler has been rewritten in C, and the display backend is now
using SDL2, though for simplicity's sake the original engine is largely the
same, and converts the 320x200x256 game screen to an SDL surface only for
the final output stage.
In future I may try different approaches.

As of this writing I haven't included the game's data files, as it contains
third-party music files which I can't just re-license to BSD.  I may later
upload a stripped-down datafile with the music missing or replaced so people
can play with it.

Note that this is very old code!  I would not write a game engine like this
now, and it's probably chock full of security holes.  I may consider refactoring
it eventually for the 20th anniversary release, but for now my main objective
is just to get it fully operational.  To be sure, it is playable, given the
datafile from the original DOS release.

