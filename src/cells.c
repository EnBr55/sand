#include <stdlib.h>
#include <stdio.h>
#include "sand.h"
#include "cells.h"

void updateAir(SimData * sim, long tick, int x, int y) {
}

void updateDirt(SimData * sim, long tick, int x, int y) {
    // first try to move straight down
    int xDir = 0;
    Cell * target = getCell(sim, x + xDir, y - 1);
    if (target == NULL || target->type != AIR) {
      // if we can't go straight down try move (x - 1, x, x + 1) at random
      xDir = (rand() % 3) - 1;
      target = getCell(sim, x + xDir, y - 1);
    }
    if (target != NULL && target->type == AIR) {
      setCell(sim, tick, x + xDir, y - 1, DIRT, 0);
      setCell(sim, tick, x, y, AIR, 0);
    }
}

void updateWater(SimData * sim, long tick, int x, int y) {
    int idx = idxFromCoord(x, y);
    // get current direction of water cell
    int dir = sim->cells[idx].dir;
    //printf("dir: %d\n", dir);
    // first try to move straight down
    char moveSideways = 0;
    int xDir = 0;
    Cell * target = getCell(sim, x, y - 1);
    if (target == NULL || target->type != AIR) {
      // if we can't go straight down try move (x - 1, x + 1) at random
      xDir = (rand() % 2) * 2 - 1;
      target = getCell(sim, x + xDir, y - 1);
    }
    // if still can't go straight down, try sideways
    if (target == NULL || target->type != AIR) {
      moveSideways = 1;
      target = getCell(sim, x + 1, y);
      dir = 1;
    } else if (target == NULL || target->type != AIR) {
      moveSideways = 1;
      target = getCell(sim, x - 1, y);
      dir = -1;
    }
    // update position
    if (target != NULL && target->type == AIR) {
      setCell(sim, tick,
          x + (moveSideways ? dir : xDir),
          y + (moveSideways ? 0 : -1),
          WATER,
          dir);
      setCell(sim, tick, x, y, AIR, 0);
    } else {
      // if everything else fails, flip direction of movement for next tick
      flipDirection(sim, x, y);
    }
}
