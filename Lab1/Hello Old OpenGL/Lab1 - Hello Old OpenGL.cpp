// Lab1 - Hello Old OpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h> // necesare pentru citirea shader-elor

#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h>


// include the freeglut Library file
#pragma comment (lib, "freeglut.lib")
void InitGLUT(int argc, char* argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowPosition(100, 100); // pozitia initiala a ferestrei
   glutInitWindowSize(1000, 700); //dimensiunile ferestrei
}
void Initialize(void)
{
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // culoarea de fond a ecranului
}
void RenderFunction(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
      glPointSize(20.0);
      glBegin(GL_POINTS);
      // primul varf
      glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
      glVertex4f(-0.8f, -0.8f, 0.0f, 1.0f);
      // al doilea varf
      glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
      glVertex4f(0.0f, 0.8f, 0.0f, 1.0f);
         // al treilea varf
         glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
         glVertex4f(0.8f, -0.8f, 0.0f, 1.0f);
      glEnd();
   glFlush();
}
int main(int argc, char* argv[])
{
   InitGLUT(argc, argv);
   glutCreateWindow("Primul triunghi - OpenGL <<vechi>>"); // titlul ferestrei

   printf("OpenGL version supported by this platform: (%s) \n", glGetString(GL_VERSION));
   printf("GLSL version supported by this platform: (%s) \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

   Initialize();
   glutDisplayFunc(RenderFunction);
   glutMainLoop();
   return 0;
}

