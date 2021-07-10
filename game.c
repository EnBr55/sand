#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include "renderer.h"
#include "sand.h"

SimData sim;

float * Init() {
  sim.cells = malloc(sizeof(Cell) * NUM_CELLS);
  // floats for RGBA vals of each cell
  sim.cellColors = malloc(sizeof(float[4]) * NUM_CELLS);

  float * cellColors = sim.cellColors;
  for (int i = 0; i < NUM_CELLS; i++) {
    if (i < NUM_CELLS / 3) {
      setCell(&sim, xFromIdx(i), yFromIdx(i), DIRT);
    } else {
      setCell(&sim, xFromIdx(i), yFromIdx(i), AIR);
    }
  }
  for (int i = 400; i < 590; i += 3) {
    setCell(&sim, 500, i, DIRT);
  } 
  return cellColors;
}

float * Render(long tick) {
  float * cellColors = sim.cellColors;
  for (int i = 0; i < NUM_CELLS; i++) {
    if (tick % 1 == 0) {
      updateCell(&sim, xFromIdx(i), yFromIdx(i));
    }
  }
  return cellColors;
}

void CleanupSim() {
  free(sim.cellColors);
}

int main(int argc, char* argv[]) {
  Initialize(argc, argv);
  glutMainLoop();
  return 0;
}
