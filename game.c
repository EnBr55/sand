#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include "renderer.h"
#include "sand.h"
#include <stdbool.h>

SimData sim;
bool mouseDown = 0;
float cellPixelWidth = 1.0f * WINDOW_WIDTH / WIDTH;
int mouseX = WINDOW_WIDTH / 2;
int mouseY = WINDOW_WIDTH / 2;

float * Init() {
  sim.cells = malloc(sizeof(Cell) * NUM_CELLS);
  // floats for RGBA vals of each cell
  sim.cellColors = malloc(sizeof(float[4]) * NUM_CELLS);

  float * cellColors = sim.cellColors;
  for (int i = 0; i < NUM_CELLS; i++) {
    if (i < NUM_CELLS / 3) {
      setCell(&sim, 0, xFromIdx(i), yFromIdx(i), DIRT, 0);
    } else {
      setCell(&sim, 0, xFromIdx(i), yFromIdx(i), AIR, 0);
    }
  }
  //for (int i = 400; i < 590; i += 3) {
    //setCell(&sim, 500, i, DIRT);
  //} 
  return cellColors;
}

float * Render(long tick) {
  if (mouseDown) {
    int size = 90;
    for (int i = 0; i < size; i++) {
      for (int j = 0; i < size; i++) {
        setCell(&sim, 0, mouseX + i, mouseY + j, DIRT, (rand() % 2) * 2 - 1);
      }
    }
  }
  float * cellColors = sim.cellColors;
  for (int i = 0; i < NUM_CELLS; i++) {
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

void mouseClicked(int button, int state, int x, int y) {
  mouseDown = !state;
}

void CleanupSim() {
  free(sim.cellColors);
}


int main(int argc, char* argv[]) {
  Initialize(argc, argv);
  glutMainLoop();
  return 0;
}
