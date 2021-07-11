#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include "renderer.h"
#include "sand.h"
#include <stdbool.h>

// globals
SimData sim;
bool mouseDown = 0;
float cellPixelWidth = 1.0f * WINDOW_WIDTH / WIDTH; // should probs be macro
int mouseX = WINDOW_WIDTH / 2; // default cursor to this position
int mouseY = WINDOW_WIDTH / 2;


// Simulation initialisation function. Should return initial "canvas" state
float * Init() {
  sim.cells = malloc(sizeof(Cell) * NUM_CELLS);
  // floats for RGBA vals of each cell
  sim.cellColors = malloc(sizeof(float[4]) * NUM_CELLS);

  float * cellColors = sim.cellColors;
  for (int i = 0; i < NUM_CELLS; i++) {
    // arbitrarily make the first 1/3 of cells be dirt
    if (i < NUM_CELLS / 3) {
      setCell(&sim, 0, xFromIdx(i), yFromIdx(i), DIRT, 0);
    } else {
      setCell(&sim, 0, xFromIdx(i), yFromIdx(i), AIR, 0);
    }
  }
  return cellColors;
}

// Render function is called by renderer.c every frame
float * Render(long tick) {
  if (mouseDown) {
    // square brush of size "size" 
    int size = 30;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        // currently no selection tool, defaults to DIRT brush
        setCell(&sim, 0, mouseX + i, mouseY + j, DIRT, (rand() % 2) * 2 - 1);
      }
    }
  }
  float * cellColors = sim.cellColors;
  for (int i = 0; i < NUM_CELLS; i++) {
    // for now just update each cell unconditionally on every tick
    if (tick % 1 == 0) {
      updateCell(&sim, tick, xFromIdx(i), yFromIdx(i));
    }
  }
  return cellColors;
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
