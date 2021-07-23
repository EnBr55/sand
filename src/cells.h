#ifndef CELLS_H_INCLUDED
#define CELLS_H_INCLUDED
#include "sand.h"

#define GRAVITY -0.4f

void updateAir(SimData * sim, long tick, int x, int y);
void updateDirt(SimData * sim, long tick, int x, int y);
void updateWater(SimData * sim, long tick, int x, int y);

void velocityUpdate(SimData * sim, int x, int y, long tick);
void lineUpdate(SimData * sim, int x, int y, int x2, int y2, long tick);

#endif
