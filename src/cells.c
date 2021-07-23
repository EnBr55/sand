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

    int xdiff = abs(x2 - x);
    int ydiff = abs(y2 - y);
    // 0 if there's no xdiff, 1 otherwise
    int xSign = 0;
    int ySign = 0;

    if (xdiff != 0) {
      xSign = (x2 - x) / xdiff;
    }
    if (ydiff != 0) {
      ySign = (y2 - y) / ydiff;
    }

    // we draw a line function. If our line is purely in 1d, then 
    // it's no longer a function and we need a different method to update
    if (x2 == x || y2 == y) {
      lineUpdate(sim, x, y, x2, y2, tick);
      return;
    }

    float g = gradient(x, y, x2, y2);

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
        pathX = x + i*xSign;
        pathY = y + floor(i * abs(g))*ySign;
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
    int xSign = 0;
    int ySign = 0;
    if (xdiff != 0) {
      xSign = (x2 - x) / xdiff;
    }
    if (ydiff != 0) {
      ySign = (y2 - y) / ydiff;
    }

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
      pathX = x + i*xSign;
      pathY = y + i*ySign;
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
        // invert x velocity
        setCell(sim, tick, oldX, oldY, oldCell.vel.x * -1.0f, oldCell.vel.y, oldCell.type);
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
    } else if (below != NULL && below->type != AIR) {
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
    Cell * this = getCell(sim, x, y);
    Cell * below = getCell(sim, x, y - 1);
    // first check what's below us.
    if (below != NULL && below->type == AIR) {
      // if it's just air, increase downwards velocity
      this->vel.y += GRAVITY;
      // if it has been 2 ticks since this was updated, lose all x velocity
      if (this->lastUpdate < (tick - 2)) {
        this->vel.x = 0;
        //printf("aa\n");
      }
    } else if (below != NULL && below->type != AIR) {
      this->vel.y = 0;
      // if it's something other than air, try move down and left/right
      int xDir = (rand() % 2) * 2 - 1;
      Cell * target = getCell(sim, x + xDir, y - 1);
      if (target != NULL && target->type == AIR) {
        setCell(sim, tick,
            x + xDir,
            y - 1,
            this->vel.x + xDir,
            this->vel.y,
            WATER);
        setCell(sim, tick, x, y, 0, 0, AIR);
      } else {
        if (this->vel.x < 0.9) {
          this->vel.x = xDir;
        }
      }
    }
    velocityUpdate(sim, x, y, tick);
}
