#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "renderer.h"

int main(int argc, char* argv[]) {
  Initialize(argc, argv);

  glutMainLoop();
  
  exit(EXIT_SUCCESS);
}

