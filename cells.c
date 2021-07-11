#include <stdlib.h>
#include <stdio.h>
#include "sand.h"
#include "cells.h"

void updateAir(SimData * sim, long tick, int x, int y) {
}

void updateDirt(SimData * sim, long tick, int x, int y) {
    Cell * below = getCell(sim, x, y - 1);
    if (below != NULL && below->type == AIR) {
      setCell(sim, tick, x, y - 1, DIRT);
      setCell(sim, tick, x, y, AIR);
    } else {
      // can go either (x - 1, x, x + 1) at random
      int randDir = (rand() % 3) - 1;
      below = getCell(sim, x + randDir, y - 1);
      if (below != NULL && below->type == AIR) {
        setCell(sim, tick, x + randDir, y - 1, DIRT);
        setCell(sim, tick, x, y, AIR);
      }
    }
}

void updateWater(SimData * sim, long tick, int x, int y) {
    int idx = idxFromCoord(x, y);
    printf("dir: %d\n", sim->cells[idx].dir);
    Cell * below = getCell(sim, x, y - 1);
    if (below != NULL && below->type == AIR) {
      setCell(sim, tick, x, y - 1, WATER);
      setCell(sim, tick, x, y, AIR);
    } else {
      // can go either (x - 1, x + 1) at random
      int randDir = (rand() % 2)*2 - 1;
      below = getCell(sim, x + randDir, y - 1);
      if (below != NULL && below->type == AIR) {
        setCell(sim, tick, x + randDir, y - 1, WATER);
        setCell(sim, tick, x, y, AIR);
      } else {
        Cell * side = getCell(sim, x + sim->cells[idx].dir, y);
        if (side != NULL && side->type == AIR) {
          setCell(sim, tick, x + sim->cells[idx].dir, y, WATER);
          setCell(sim, tick, x, y, AIR);
        } else {
          flipDirection(sim, x, y);
        }
      }
    }
}
