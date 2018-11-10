#include "stdafx.h"
#include "Lab5Engine.h"

#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#include <glfw3.h>

#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

void Lab5Engine::Init()
{
   // indexurile piramidei
   unsigned int Indices[] = {
      0,1,3,
      1,2,3,
      0,2,3,
      0,1,2
   };

   // varfurile piramidei
   GLfloat Vertices[] = {
      -0.8f, -0.8f, 0.0f,
      0.0f, -0.8f, 0.8f,
      0.8f, -0.8f, 0.0f,
      0.0f,  0.8f, 0.0f
   };

   GLfloat Colors[] = {
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
   };

   std::vector<glm::vec3> positions;
   for (int i = 0; i < SIZEOF_ARRAY(Vertices); i += 3) {
      positions.push_back(glm::vec3(Vertices[i + 0], Vertices[i + 1], Vertices[i + 2]));
   }

   std::vector<unsigned int> indices;
   for (int i = 0; i < SIZEOF_ARRAY(Indices); ++i) {
      indices.push_back(Indices[i]);
   }

   spSceneManager->AddMesh("SimplePyramid", positions, indices);
}

void Lab5Engine::Update(float deltaTimeSeconds)
{
   // sets the clear color for the color buffer
   glClearColor(0, 0, 0, 1);

   // clears the color buffer (using the previously set color) and depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // render the object
   const Mesh* pPyramid = spSceneManager->GetMesh("SimplePyramid");
   if (pPyramid != nullptr) {
      spSceneManager->RenderMesh(*pPyramid, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.0f));
   }
}