#ifndef SAND_H_INCLUDED
#define SAND_H_INCLUDED

#define AIR 0
#define DIRT 1
#define WATER 2

int idxFromCoord(int x, int y);
int xFromIdx(int i);
int yFromIdx(int i);

typedef struct {
  float r;
  float g;
  float b;
  float a;
} Color;

typedef struct {
  int x;
  int y;
} Position;

typedef struct {
  int type;
  Position pos;
  char dir;
  long lastUpdate;
} Cell;

typedef struct {
  float * cellColors;
  Cell * cells;
} SimData;


Cell * getCell(SimData * sim, int x, int y);
void setCell(SimData * sim, long tick, int x, int y, int type, char dir);
void updateCell(SimData * sim, long tick, int x, int y);
void setColor(float * colors, int idx, float r, float g, float b, float a);

void flipDirection(SimData * sim, int x, int y);

#endif
