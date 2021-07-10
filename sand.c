#include <stdlib.h>
#include <stdio.h>
#include "renderer.h"
#include "sand.h"

int idxFromCoord(int x, int y) {
  return x + WIDTH*y;
}

int xFromIdx(int i) {
  return (i % WIDTH);
}

int yFromIdx(int i) {
  return (i / WIDTH);
}

Cell * getCell(int x, int y) {
  return NULL;
}

void setCell(SimData * sim, int x, int y, int type) {
  int idx = idxFromCoord(x, y);
  sim->cells[idx].type = type;
  if (type == DIRT) {
    setColor(sim->cellColors, idx, 0.3f, 0.4f, 0.3f, 1.0f);
  }
  if (type == AIR) {
    setColor(sim->cellColors, idx, 0.2f, 0.2f, 0.9f, 1.0f);
  }
}

void setColor(float * colors, int idx, float r, float g, float b, float a) {
    colors[idx*RGBA_OFFSET + 0] = r;
    colors[idx*RGBA_OFFSET + 1] = g;
    colors[idx*RGBA_OFFSET + 2] = b;
    colors[idx*RGBA_OFFSET + 3] = a;
}
