#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glew.c"
#include "renderer.h"

int
  CurrentWidth = 800,
  CurrentHeight = 600,
  WindowHandle = 0;

unsigned FrameCount = 0;

// declare global GLEW ids
GLuint
  VertexShaderId,
  FragmentShaderId,
  ProgramId,
  VaoId,
  VboId,
  ColorBufferId;
    
// GLSL vertex shader -- where GLSL = "OpenGL Shading Language"
const GLchar* VertexShader =
{
  "#version 400\n"\

  "layout(location=0) in vec4 in_Position;\n"\
  "layout(location=1) in vec4 in_Color;\n"\
  "out vec4 ex_Color;\n"\

  "void main(void)\n"\
  "{\n"\
  "  gl_Position = in_Position;\n"\
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

  glDrawArrays(GL_TRIANGLES, 0, 3);

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
  DestroyShaders();
  DestroyVBO();
}

// create buffer objects and define buffer contents
void CreateVBO(void) {
  // triangle vertices
  GLfloat Vertices[] = {
     -0.8f, -0.8f, 0.0f, 1.0f,
     0.0f,  0.8f, 0.0f, 1.0f,
     0.8f, -0.8f, 0.0f, 1.0f
  };
  GLfloat Colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f
  };

  GLenum ErrorCheckValue = glGetError();

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
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &ColorBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR)
  {
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
  glDeleteBuffers(1, &ColorBufferId);
  glDeleteBuffers(1, &VboId);

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

// another function from openglbook i dont fully understand yet
void CreateShaders(void)
{
  GLenum ErrorCheckValue = glGetError();

  VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
  glCompileShader(VertexShaderId);

  FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
  glCompileShader(FragmentShaderId);

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
