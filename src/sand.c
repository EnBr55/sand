#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "renderer.h"
#include "sand.h"
#include "cells.h"

// return pixel array index from x and y coordinates
int idxFromCoord(int x, int y) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= WIDTH) {
    return -1;
  }
  return x + WIDTH*y;
}

// return an x-coordinate from pixel array index
int xFromIdx(int i) {
  return (i % WIDTH);
}

// return a y-coordinate from pixel array index
int yFromIdx(int i) {
  return (i / WIDTH);
}

// try to return a cell at given coordinates, otherwise returning NULL
Cell * getCell(SimData * sim, int x, int y) {
  int idx = idxFromCoord(x, y);
  if (idx < 0) {
    return NULL;
  }
  return sim->cells + idx;
}

void setCell(SimData * sim, long tick, int x, int y, float vx, float vy, int type) {
  int idx = idxFromCoord(x, y);
  sim->cells[idx].type = type;
  sim->cells[idx].pos.x = x;
  sim->cells[idx].pos.y = y;
  sim->cells[idx].vel.x = vx;
  sim->cells[idx].vel.y = vy;
  sim->cells[idx].lastUpdate = tick;
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

float gradient(int x1, int y1, int x2, int y2) { 
    return ((float) (y2 - y1)) / ((float)(x2 - x1));
}

float distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

