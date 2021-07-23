#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include "renderer.h"
#include "sand.h"
#include <stdbool.h>
#include <math.h>

#define BRUSH_SIZE 10

// globals
SimData sim;
bool mouseDown = 0;
float cellPixelWidth = 1.0f * WINDOW_WIDTH / WIDTH; // should probs be macro
int mouseX = WINDOW_WIDTH / 2; // default cursor to this position
int mouseY = WINDOW_WIDTH / 2;

void drawLine(int x1, int y1, int x2, int y2);

// Simulation initialisation function. Should return initial "canvas" state
float * Init() {
  sim.cells = malloc(sizeof(Cell) * NUM_CELLS);
  // floats for RGBA vals of each cell
  sim.cellColors = malloc(sizeof(float[4]) * NUM_CELLS);

  float * cellColors = sim.cellColors;
  for (int i = 0; i < NUM_CELLS; i++) {
    // arbitrarily make the first 1/3 of cells be dirt
    if (i < NUM_CELLS / 3) {
      setCell(&sim, 0, xFromIdx(i), yFromIdx(i), 0, 0, DIRT);
    } else {
      setCell(&sim, 0, xFromIdx(i), yFromIdx(i), 0, 0, AIR);
    }
  }
  return cellColors;
}

// Render function is called by renderer.c every frame
float * Render(long tick) {
  if (mouseDown) {
    // basic square brush 
    for (int i = 0; i < BRUSH_SIZE; i++) {
      for (int j = 0; j < BRUSH_SIZE; j++) {
        if (getCell(&sim, mouseX + i, mouseY + j)) {
          // currently no selection tool, defaults to DIRT brush
          setCell(&sim, 0, mouseX + i, mouseY + j, 0, 0, WATER);
        }
      }
    }
  }
  float * cellColors = sim.cellColors;
  //drawLine(10, 50, 90, 58);
  for (int i = 0; i < NUM_CELLS; i++) {
    // for now just update each cell unconditionally on every tick
    if (tick % 1 == 0) {
      updateCell(&sim, tick, xFromIdx(i), yFromIdx(i));
    }
  }
  return cellColors;
}

void drawLine(int x1, int y1, int x2, int y2) {
    float g = gradient(x1, y1, x2, y2);
    int xdiff = abs(x2 - x1);
    // get sign of direction of movement
    int xdir = (x2 - x1) / xdiff;

    // for each move in the x direction, compute y value using dy/dx
    int x, y;
    for (int i = 0; i < xdiff; i++) {
        x = x1 + i*xdir;
        y = y1 + floor(i * g);
        //printf("g: %f, xdiff: %d, xdir: %d, x: %d, y: %d\n", g, xdiff, xdir, x, y);
        // NOT CALLING ??
        if (getCell(&sim, x, y)) {
          // currently no selection tool, defaults to DIRT brush
          setCell(&sim, 0, x, y, 0, 0, DIRT);
        }
    }
}

void mouseMoved(int x, int y) {
  int normalizedX = x / cellPixelWidth;
  // normalize y such that (0, 0) is the bottom left of the screen
  int normalizedY = WIDTH - (y / cellPixelWidth) - 1;
  mouseX = normalizedX;
  mouseY = normalizedY;
}

// this is called both on the start and end of a mouse button being clicked
void mouseClicked(int button, int state, int x, int y) {
  mouseDown = !state;
}

void CleanupSim() {
  free(sim.cellColors);
  free(sim.cells);
}

int main(int argc, char* argv[]) {
  Initialize(argc, argv);
  glutMainLoop();
  return 0;
}
