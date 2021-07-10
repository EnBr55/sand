#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glew.c"
#include "renderer.h"

#define WIDTH 2
#define NUM_CELLS (WIDTH * WIDTH)

int
  CurrentWidth = 800,
  CurrentHeight = 600,
  WindowHandle = 0;

float cellColors[NUM_CELLS][4];
float cellTranslations[NUM_CELLS][2];

unsigned FrameCount = 0;

// declare global GLEW ids
GLuint
  VertexShaderId,
  FragmentShaderId,
  ProgramId,
  VaoId,
  VboId,
  IndexBufferId,
  colorVBO,
  instanceVBO;
    
// GLSL vertex shader -- where GLSL = "OpenGL Shading Language"
const GLchar* VertexShader =
{
  "#version 400\n"\

  "layout(location=0) in vec2 in_Position;\n"\
  "layout(location=1) in vec4 in_Color;\n"\
  "layout(location=2) in vec2 in_Offset;\n"\
  "out vec4 ex_Color;\n"\

  "void main(void)\n"\
  "{\n"\
  "  gl_Position = vec4(in_Position + in_Offset, 0.0, 1.0);\n"\
  "  ex_Color = in_Color;\n"\
  "}\n"
};

// GLSL fragment shader
const GLchar* FragmentShader =
{
  "#version 400\n"\

  "in vec4 ex_Color;\n"\
  "out vec4 out_Color;\n"\

  "void main(void)\n"\
  "{\n"\
  "  out_Color = ex_Color;\n"\
  "}\n"
};

int idxFromCoord(int x, int y, int width) {
  return x + width*y;
}

int xFromIdx(int i, int width) {
  return i % width;
}

int yFromIdx(int i, int width) {
  return i / width;
}

void Initialize(int argc, char* argv[]) {
  GLenum GlewInitResult;

  InitWindow(argc, argv);

  GlewInitResult = glewInit();
  if (GlewInitResult != GLEW_OK) {
    fprintf(stderr, "ERROR: %s\n", glewGetErrorString(GlewInitResult));
    exit(EXIT_FAILURE);
  }
  
  fprintf(
    stdout,
    "INFO: OpenGL Version: %s\n",
    glGetString(GL_VERSION)
  );

  // initialize translations
  // NOTE TO FUTURE BEN:
    // normalised coords for shaders operate in [-1, 1] ...
    // but we're indexing with only positive (x, y)
  int i = 0;
  for (int x = -WIDTH; x < WIDTH; x+=2) {
    for (int y = -WIDTH; y < WIDTH; y+=2) {
      cellTranslations[i][0] = (float)x / WIDTH + +0.10f;
      cellTranslations[i][1] = (float)y / WIDTH + 0.10f;

      cellColors[i][0] = (float)i / NUM_CELLS;
      cellColors[i][1] = 1;
      cellColors[i][2] = 1;
      cellColors[i++][3] = 1;
    }
  }

  CreateShaders();
  CreateVBO();
  glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
}

void InitWindow(int argc, char* argv[]) {
  glutInit(&argc, argv);
  
  glutInitContextVersion(4, 0);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutIdleFunc(IdleFunction);
  glutTimerFunc(0, TimerFunction, 0);

  glutSetOption(
    GLUT_ACTION_ON_WINDOW_CLOSE,
    GLUT_ACTION_GLUTMAINLOOP_RETURNS
  );
  
  glutInitWindowSize(CurrentWidth, CurrentHeight);

  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

  if(WindowHandle < 1) {
    fprintf(
      stderr,
      "ERROR: Could not create a new rendering window.\n"
    );
    exit(EXIT_FAILURE);
  }

  glutReshapeFunc(ResizeFunction);
  glutDisplayFunc(RenderFunction);
}

void ResizeFunction(int Width, int Height) {
  CurrentWidth = Width;
  CurrentHeight = Height;
  glViewport(0, 0, CurrentWidth, CurrentHeight);
}

void RenderFunction(void) {
  ++FrameCount;
  // clear buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  if (FrameCount % 1000 == 0) {
    printf("Framecount: %d\n", FrameCount);
    printf("THING\n");
    cellColors[0][1] += 0.1f;
    cellColors[1][1] += 0.1f;
    cellColors[2][1] += 0.1f;
    cellColors[3][1] += 0.1f;
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float[4]) * NUM_CELLS, &cellColors[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NUM_CELLS);

  glutSwapBuffers();
  glutPostRedisplay();
}

void IdleFunction(void) {
  glutPostRedisplay();
}

void TimerFunction(int Value) {
  if (Value != 0) {
    char * TempString = (char *) malloc(512 + strlen(WINDOW_TITLE_PREFIX));
    sprintf(TempString, "%s: %d Frames Per Second @ %d x %d",
        WINDOW_TITLE_PREFIX, FrameCount*4, CurrentWidth, CurrentHeight);
    glutSetWindowTitle(TempString);
    free(TempString);
  }
  FrameCount = 0;
  glutTimerFunc(250, TimerFunction, 1);
  glutCloseFunc(Cleanup);
}

// cleanup function to be passed to glut
void Cleanup(void) {
    printf("Cleaning up\n");
  DestroyShaders();
  DestroyVBO();
}

// create buffer objects and define buffer contents
void CreateVBO(void) {
  // Square vertices
  Vertex Vertices[] = {
    // Top left { {pos}, {color} }
    { { -0.1f, 0.1f } },
    // Bottom left
    { { -0.1f, -0.1f }  },
    // Top right
    { { 0.1f, 0.1f } },
    // Top right
    { { 0.1f, 0.1f } },
    // Bottom right
    { { 0.1f, -0.1f } },
    // Bottom left
    { { -0.1f, -0.1f } },
  };

  GLenum ErrorCheckValue = glGetError();
  const size_t BufferSize = sizeof(Vertices);
  const size_t VertexSize = sizeof(Vertices[0]);

  // generates vertex array objects in GPU's memory and stores
  // their identifiers in VaoId (only 1 identifier in this case)
  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);

  // generate buffers in GPU's memory
  glGenBuffers(1, &VboId);
  // bind the generated buffer (so when we reference target
  // buffer, it's this one)
  glBindBuffer(GL_ARRAY_BUFFER, VboId);
  // copy vertex data over to buffer (GL_ARRAY_BUFFER refers to target)
  // static indicates that "Vertices" will not be modified after upload
  glBufferData(GL_ARRAY_BUFFER, BufferSize, Vertices, GL_STATIC_DRAW);

  // define data type for vertex positions and colors
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
  glEnableVertexAttribArray(0);
  glGenBuffers(1, &IndexBufferId);

  // buffer object for color data
  glGenBuffers(1, &colorVBO);
  glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float[4]) * NUM_CELLS, &cellColors[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // set attribute pointer for translations
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // update content of vertex attribute 1 when we render new instance
  glVertexAttribDivisor(1, 1);

  // buffer object for translation data
  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float[2]) * NUM_CELLS, &cellTranslations[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // set attribute pointer for translations
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // update content of vertex attribute 2 when we render new instance
  glVertexAttribDivisor(2, 1);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR) {
    fprintf(
      stderr,
      "ERROR: Could not create a VBO: %s \n",
      gluErrorString(ErrorCheckValue)
    );

    exit(-1);
  }

}

void DestroyVBO(void) {
  GLenum ErrorCheckValue = glGetError();

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &VboId);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &IndexBufferId);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &VaoId);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR)
  {
    fprintf(
      stderr,
      "ERROR: Could not destroy the VBO: %s \n",
      gluErrorString(ErrorCheckValue)
    );

    exit(-1);
  }
}

// compiles and links shader programs
void CreateShaders(void)
{
  GLenum ErrorCheckValue = glGetError();

  // create shader object of type GL_VERTEX_SHADER and get its id
  VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  // load shader source code and associate with id from above
  glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
  // compile shader to shader with id from above
  glCompileShader(VertexShaderId);

  // repeat process for fragment shader
  FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
  glCompileShader(FragmentShaderId);

  // create opengl program and link compiled shaders
  ProgramId = glCreateProgram();
  glAttachShader(ProgramId, VertexShaderId);
  glAttachShader(ProgramId, FragmentShaderId);
  glLinkProgram(ProgramId);
  glUseProgram(ProgramId);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR)
  {
    fprintf(
      stderr,
      "ERROR: Could not create the shaders: %s \n",
      gluErrorString(ErrorCheckValue)
    );

    exit(-1);
  }
}

void DestroyShaders(void)
{
  GLenum ErrorCheckValue = glGetError();

  glUseProgram(0);

  glDetachShader(ProgramId, VertexShaderId);
  glDetachShader(ProgramId, FragmentShaderId);

  glDeleteShader(FragmentShaderId);
  glDeleteShader(VertexShaderId);

  glDeleteProgram(ProgramId);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR)
  {
    fprintf(
      stderr,
      "ERROR: Could not destroy the shaders: %s \n",
      gluErrorString(ErrorCheckValue)
    );

    exit(-1);
  }
}
