#ifndef CELLS_H_INCLUDED
#define CELLS_H_INCLUDED
#include "sand.h"

void updateAir(SimData * sim, long tick, int x, int y);
void updateDirt(SimData * sim, long tick, int x, int y);
void updateWater(SimData * sim, long tick, int x, int y);

#endif
