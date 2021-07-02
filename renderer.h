#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#define WINDOW_TITLE_PREFIX "Ben Game"

typedef struct {
  float pos[4];
  float rgba[4];
} Vertex;

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

#endif
