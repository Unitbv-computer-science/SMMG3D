// Lab1 - Hello New OpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <chrono>
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h> 
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#include <glfw3.h>

#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Camera.h"
#include "WGLRenderTarget.h"

// include the freeglut Library file
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "Engine3D.lib")
#pragma comment (lib, "glfw3dll.lib")

GLuint VaoId, VboId, IboId, ColorBufferId, VertexShaderId, FragmentShaderId, ProgramId;
GLuint ModelLocation, ViewLocation, ProjLocation;
bool bWireframe = false;
bool bPespectiveProjection = true;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


// Shader-ul de varfuri / Vertex shader (este privit ca un sir de caractere)
const GLchar* VertexShader =
{
	"#version 400\n"\
	"layout(location=0) in vec4 in_Position;\n"\
	"layout(location=1) in vec4 in_Color;\n"\
	"out vec4 ex_Color;\n"\
	"uniform mat4 Model;\n"\
   "uniform mat4 View;\n"\
   "uniform mat4 Proj;\n"\
	"void main()\n"\
	"{\n"\
	"  gl_Position = Model * in_Position;\n"\
	"  ex_Color = in_Color;\n"\
	"}\n"
};
// Shader-ul de fragment / Fragment shader (este privit ca un sir de caractere)
const GLchar* FragmentShader =
{
	"#version 400\n"\
	"in vec4 ex_Color;\n"\
	"out vec4 out_Color;\n"\
	"void main()\n"\
	"{\n"\
	"  out_Color = ex_Color;\n"\
	"}\n"
};

void CreateVBO()
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
		-0.8f, -0.8f, 0.0f, 1.0f,
		 0.0f, -0.8f, 0.8f, 1.0f,
		 0.8f, -0.8f, 0.0f, 1.0f,
		 0.0f,  0.8f, 0.0f, 1.0f
	};

	// culorile, ca atribute ale varfurilor
	GLfloat Colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};
	// se creeaza un buffer nou
	glGenBuffers(1, &VboId);
	// este setat ca buffer curent
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	// punctele sunt "copiate" in bufferul curent
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	// se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	// se activeaza lucrul cu atribute; atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	// 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer, pentru culoare
	glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	// atributul 1 =  culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer pentru indexuri
	glGenBuffers(1, &IboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void DestroyVBO()
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}
void CreateShaders()
{
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

	GLint Success = 0;
	GLchar ErrorLog[1024] = {0};

	glGetProgramiv(ProgramId, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ProgramId, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ProgramId);
	glGetProgramiv(ProgramId, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ProgramId, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

   glUseProgram(ProgramId);
	ModelLocation = glGetUniformLocation(ProgramId, "Model");
   ViewLocation = glGetUniformLocation(ProgramId, "View");
   ProjLocation = glGetUniformLocation(ProgramId, "Proj");
}
void DestroyShaders()
{
	glUseProgram(0);

	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);

	glDeleteProgram(ProgramId);
}

void Initialize()
{
	CreateVBO();
	CreateShaders();
}

void RenderScene(Camera* pCamera)
{
   static float triangleOffset = 0.0f;
   const float triangleIncrement = 0.001f;

   triangleOffset += triangleIncrement;

   if (bWireframe) {
      glPolygonMode(GL_FRONT, GL_LINE);
      glDisable(GL_DEPTH_TEST);
   } else {
      glPolygonMode(GL_FRONT, GL_FILL);
      glEnable(GL_DEPTH_TEST);
   }
   if (ProgramId) {
      glUseProgram(ProgramId);
   }

   //glm::mat4 Model = glm::transpose(glm::translate(glm::mat4(1.0), glm::vec3(sinf(triangleOffset), 0.0f, 0.0)));
   //glm::mat4 Model = glm::transpose(glm::translate(glm::mat4(1.0), glm::vec3(0.0f, sinf(triangleOffset), 0.0)));
   //glm::mat4 Model = glm::transpose(glm::translate(glm::mat4(1.0), glm::vec3(cosf(triangleOffset), sinf(triangleOffset), 0.0)));
   //glm::mat4 Model = glm::transpose(glm::rotate(glm::mat4(1.0), glm::radians(triangleOffset*180), glm::vec3(0, 0, 1)));
   //glm::mat4 Model = glm::translate(glm::mat4(1.0), glm::vec3(0.f, 0.0f, 10.f));
   //Model = glm::scale(Model, glm::vec3(10.f, 10.f, 10.f));
   //Model = glm::rotate(Model, glm::radians(triangleOffset * 180), glm::vec3(0, 1, 0));

   glm::mat4 Model = glm::mat4(1.0);
   glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(Model));
   //glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(pCamera->GetViewMatrix()));
   //glUniformMatrix4fv(ProjLocation, 1, GL_FALSE, glm::value_ptr(pCamera->GetProjectionMatrix()));

   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, VboId);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

   glEnableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

   //glDrawArrays(GL_TRIANGLES, 0, 3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IboId);
   glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

   glDisableVertexAttribArray(0);
}

void RenderFunction(Camera* pCamera)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

   RenderScene(pCamera);
}

void Cleanup()
{
	DestroyShaders();
	DestroyVBO();
}
int main(int argc, char* argv[])
{
   WGLRenderTarget* pRenderTarget = new WGLRenderTarget(WINDOW_WIDTH, WINDOW_HEIGHT);
   pRenderTarget->Initialize("Lab4");

   Initialize();

   Camera *pCamera = new Camera(45.f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 500.f,
                                glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

   glm::mat4 projection = pCamera->GetProjectionMatrix();

   double deltaTime = 0.f, lastFrame = 0.f;

   while (!pRenderTarget->ShouldClose()) {
      // input
      // -----
      pCamera->KeyControl(pRenderTarget->Keys(), deltaTime);
      pCamera->MouseControl(pRenderTarget->GetXChange(), pRenderTarget->GetYChange());

      // per-frame time logic
      // --------------------
      double currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      // render
      // ------
      RenderFunction(pCamera);

      glfwSwapBuffers(pRenderTarget->GetMainWindow());
      glfwPollEvents();
   }

   DestroyShaders();
}

