#include <stdlib.h>
#include <stdio.h>
#include "renderer.h"
#include "sand.h"
#include "cells.h"

int idxFromCoord(int x, int y) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= WIDTH) {
    return -1;
  }
  return x + WIDTH*y;
}

int xFromIdx(int i) {
  return (i % WIDTH);
}

int yFromIdx(int i) {
  return (i / WIDTH);
}

Cell * getCell(SimData * sim, int x, int y) {
  int idx = idxFromCoord(x, y);
  if (idx < 0) {
    return NULL;
  }
  return sim->cells + idx;
}

void setCell(SimData * sim, long tick, int x, int y, int type, char dir) {
  int idx = idxFromCoord(x, y);
  sim->cells[idx].type = type;
  sim->cells[idx].pos.x = x;
  sim->cells[idx].pos.y = y;
  sim->cells[idx].lastUpdate = tick;
  sim->cells[idx].dir = dir;
  if (type == DIRT) {
    setColor(sim->cellColors, idx, 0.3f, 0.1f, 0.1f, 1.0f);
  }
  if (type == AIR) {
    setColor(sim->cellColors, idx, 0.2f, 0.2f, 0.9f, 1.0f);
  }
  if (type == WATER) {
    setColor(sim->cellColors, idx, 0.0f, 0.2f, 0.7f, 1.0f);
  }
}

void flipDirection(SimData * sim, int x, int y) {
  int idx = idxFromCoord(x, y);
  //printf("direction BEFORE flip: %d\n", sim->cells[idx].dir);
  if (sim->cells[idx].dir == 1) {
    sim->cells[idx].dir = -1;
  } else {
    sim->cells[idx].dir = 1;
  }
  //printf("direction after flip: %d\n", sim->cells[idx].dir);
}

void setColor(float * colors, int idx, float r, float g, float b, float a) {
    colors[idx*RGBA_OFFSET + 0] = r;
    colors[idx*RGBA_OFFSET + 1] = g;
    colors[idx*RGBA_OFFSET + 2] = b;
    colors[idx*RGBA_OFFSET + 3] = a;
}

void updateCell(SimData * sim, long tick, int x, int y) {
  int idx = idxFromCoord(x, y);
  if (sim->cells[idx].lastUpdate == tick) {
    return;
  }
  sim->cells[idx].lastUpdate = tick;

  int type = sim->cells[idx].type;
  switch(type) {
    case AIR:
      updateAir(sim, tick, x, y);
      break;
    case DIRT:
      updateDirt(sim, tick, x, y);
      break;
    case WATER:
      updateWater(sim, tick, x, y);
      break;
  }
}
