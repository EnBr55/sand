#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sand.h"
#include "cells.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void velocityUpdate(SimData * sim, int x, int y, long tick) {
    Cell * this = getCell(sim, x, y);
    if (this->vel.x == 0 && this->vel.y == 0) {
      return;
    }
    int x2 = x + round(this->vel.x);
    int y2 = y + round(this->vel.y);

    // we draw a line function. If our line is purely in 1d, then 
    // it's no longer a function and we need a different method to update
    if (x2 == x || y2 == y) {
      lineUpdate(sim, x, y, x2, y2, tick);
      return;
    }

    float g = gradient(x, y, x2, y2);
    int xdiff = abs(x2 - x);
    // get sign of direction of movement
    int xdir = (x2 - x) / xdiff;

    // for each move in the x direction, compute y value using dy/dx
    int pathX, pathY;
    int oldX = x;
    int oldY = y;

    // save old cell params
    Cell oldCell;
    oldCell.type = this->type;
    oldCell.vel.x = this->vel.x;
    oldCell.vel.y = this->vel.y;

    Cell * target;
    for (int i = 1; i <= xdiff; i++) {
        pathX = x + i*xdir;
        pathY = y + floor(i * g);
        target = getCell(sim, pathX, pathY);
        if (target != NULL && target->type == AIR) {
          // currently no selection tool, defaults to DIRT brush
          setCell(sim, tick, pathX, pathY, oldCell.vel.x, oldCell.vel.y, oldCell.type);
          setCell(sim, tick, oldX, oldY, 0, 0, AIR);
          oldX = pathX;
          oldY = pathY;
        } else {
          break;
        }
    }
}

void lineUpdate(SimData * sim, int x, int y, int x2, int y2, long tick) {
    Cell * this = getCell(sim, x, y);
    int xdiff = abs(x2 - x);
    int ydiff = abs(y2 - y);
    // 0 if there's no xdiff, 1 otherwise
    int xFlag = MIN(xdiff, 1);
    int yFlag = MIN(ydiff, 1);

    int pathX, pathY;
    int oldX = x;
    int oldY = y;
    Cell * target;

    Cell oldCell;
    oldCell.type = this->type;
    oldCell.vel.x = this->vel.x;
    oldCell.vel.y = this->vel.y;

    
    // either xdiff or ydiff is assumed to be 0
    for (int i = 1; i <= MAX(xdiff, ydiff); i++) {
      pathX = x + i*xFlag;
      pathY = y - i*yFlag;
      target = getCell(sim, pathX, pathY);
      if (target != NULL && target->type == AIR) {
        // currently no selection tool, defaults to DIRT brush
        //printf("setting cell with oldX: %d, oldY: %d, newX: %d, newY: %d, type: %d\n", oldX, oldY, pathX, pathY, oldCell.type);
        setCell(sim, tick, pathX, pathY, oldCell.vel.x, oldCell.vel.y, oldCell.type);
        //setCell(sim, 0, 90, 90, 0, 0, DIRT);
        setCell(sim, tick, oldX, oldY, 0, 0, AIR);
        oldX = pathX;
        oldY = pathY;
      } else {
        break;
      }
    }
}

void updateAir(SimData * sim, long tick, int x, int y) {
}

void updateDirt(SimData * sim, long tick, int x, int y) {
    Cell * this = getCell(sim, x, y);
    Cell * below = getCell(sim, x, y - 1);
    // first check what's below us.
    if (below != NULL && below->type == AIR) {
      // if it's just air, increase downwards velocity
      this->vel.y += GRAVITY;
    } else if (below != NULL && below-> type != AIR) {
      // if there's something below, stop moving down
      this->vel.y = 0;
      // if we can't go straight down try move (x - 1, x, x + 1) at random
      int xDir = (rand() % 3) - 1;
      Cell * target = getCell(sim, x + xDir, y - 1);
      if (target != NULL && target->type == AIR) {
        setCell(sim, tick, x + xDir, y - 1, this->vel.x, this->vel.y, DIRT);
        setCell(sim, tick, x, y, 0, 0, AIR);
      }
    }
    // finally, use new velocity to update position again
    velocityUpdate(sim, x, y, tick);
}

void updateWater(SimData * sim, long tick, int x, int y) {
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
      target = getCell(sim, x + xDir, y);
    }
    // update position
    if (target != NULL && target->type == AIR) {
      Cell * this = getCell(sim, x, y);
      setCell(sim, tick,
          x + xDir,
          y + (moveSideways ? 0 : -1),
          this->vel.x,
          this->vel.y,
          WATER);
      setCell(sim, tick, x, y, 0, 0, AIR);
    } else {
      // if everything else fails, flip direction of movement for next tick ?
    }
}
