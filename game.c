#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include "renderer.h"

SimData sim;

float * Init() {
  printf("Sim initializing...\n");
  // floats for RGBA vals of each cell
  sim.cellColors = malloc(sizeof(float[4]) * NUM_CELLS);
  float * cellColors = sim.cellColors;
  for (int i = 0; i < NUM_CELLS; i++) {
    cellColors[i*RGBA_OFFSET + 0] = 0.1f;
    cellColors[i*RGBA_OFFSET + 1] = 0.0f;
    cellColors[i*RGBA_OFFSET + 2] = 0 *(i * 1.0f) / NUM_CELLS + 0.0f;
    cellColors[i*RGBA_OFFSET + 3] = 1.0f;
  }
  return cellColors;
}

float * Render() {
  float * cellColors = sim.cellColors;
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
