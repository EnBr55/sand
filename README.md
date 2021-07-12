# sand

My own simple OpenGL rendering library + a falling-sand-style simulation implementing it.

The renderer is fully contained in `renderer.c` with some function signatures outlined in `renderer.h` that a client is expected to implement (such as initialization and rendering). The main idea here was to only use old stuff with as few layers of abstraction as possible to maximise what I could squeeze out of my GPU (still very much in progress). I'm using FreeGLUT for compatibility and GLEW for OpenGL extension-loading as the only non-standard libraries included. This is also my first project with anything OpenGL-related so there are probably lot of weird things I'm doing in the code.

## Installation

### Linux (and maybe others if you're lucky)

Just `git clone` this repository and then `cd` into the directory before running `make`. The program can be launched from `./build/game`.

**Note**: based on your hardware, there are some variables you might want to change. In the `renderer.h` file, the constant `WIDTH` defines the number of horizontal cells in the simulation, so there will be `WIDTH^2` total cells rendered each frame. In `game.c`, the `BRUSH_SIZE` constant defines a square region of this size in which cells will be filled while around the cursor while the left mouse button is down.

## Defining your own program using the rendering library

todo (but also it should be very obvious from the code if you're interested enough)
