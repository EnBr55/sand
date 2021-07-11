#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#define WINDOW_TITLE_PREFIX "Sand"
#define RGBA_OFFSET 4

// SIM CONSTANTS

#define WIDTH 600
#define NUM_CELLS (WIDTH * WIDTH)
#define CELL_WIDTH (1.0f * WIDTH / NUM_CELLS)

typedef struct {
  float pos[2];
  float rgba[4];
} Vertex;


int ColorIdxFromCoord(int x, int y);
int ColorXFromIdx(int i);
int ColorYFromIdx(int i);

// Implemented by renderer
void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);
void TimerFunction(int);
void IdleFunction(void);
void Cleanup(void);
void CreateVBO(void);
void DestroyVBO(void);
void CreateShaders(void);
void DestroyShaders(void);

// Should be implemented by client

// .: Expected to return pointer to RGBA vals (float[NUM_CELLS][4]) :.
// Called once by renderer after GLEW initialization
float * Init(void);
// Called every frame
float * Render(long tick);
void CleanupSim(void);

#endif