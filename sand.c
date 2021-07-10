#include <stdlib.h>
#include <stdio.h>
#include "renderer.h"
#include "sand.h"

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

void setCell(SimData * sim, int x, int y, int type) {
  int idx = idxFromCoord(x, y);
  sim->cells[idx].type = type;
  sim->cells[idx].pos.x = x;
  sim->cells[idx].pos.y = y;
  if (type == DIRT) {
    setColor(sim->cellColors, idx, 0.3f, 0.1f, 0.1f, 1.0f);
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

void updateCell(SimData * sim, int x, int y) {
  int idx = idxFromCoord(x, y);
  int type = sim->cells[idx].type;
  if (type == DIRT) {
    Cell * below = getCell(sim, x, y - 1);
    if (below != NULL && below->type == AIR) {
      setCell(sim, x, y - 1, DIRT);
      setCell(sim, x, y, AIR);
    } else {
      Cell * belowLeft = getCell(sim, x - 1, y - 1);
      if (belowLeft != NULL && belowLeft->type == AIR) {
        setCell(sim, x - 1, y - 1, DIRT);
        setCell(sim, x, y, AIR);
      } else {
        Cell * belowRight = getCell(sim, x + 1, y - 1);
        if (belowRight != NULL && belowRight->type == AIR) {
          setCell(sim, x + 1, y - 1, DIRT);
          setCell(sim, x, y, AIR);
        }
      }
    }
  }
}
