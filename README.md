# sand

Super simple `OpenGL` rendering library + a falling-sand-style implementing it.

The renderer is fully contained in `renderer.c` with some function signatures outlined in `renderer.h` that a client is expected to implement (such as initialization). The main idea here was to only use old stuff with as few layers of abstraction as possible. I'm using FreeGLUT for compatibility and GLEW for OpenGL extension-loading as the only non-standard libraries included. This is also my first project with anything OpenGL-related so there are probably lot of weird things I'm doing.

## Installation

Just `git clone` the repository and then `cd` into the directory before running `make`. The program can be launched from `./build/game`.

## Defining your own program using the rendering library

todo (but also it should be very obvious from the code if you care enough)
