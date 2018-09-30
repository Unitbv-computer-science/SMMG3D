// Hello GLFW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glew.h>
#include <glfw3.h>

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "opengl32.lib")

int main()
{
	// Initializare GLFW
	if (!glfwInit()) {
		glfwTerminate();
		return -1;
	}

	const GLint WINDOWS_WIDTH = 600, WINDOWS_HEIGHT = 400; // dimensiunile ferestrei
														   // Crearea fereastrei
	GLFWwindow* pMainWindow = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "Hello GLFW", NULL, NULL);
	if (!pMainWindow) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pMainWindow);// Setarea contextului ferestrei noastre ca fiind contextul curent

	glewExperimental = GL_TRUE;			// permite extensii moderne

	glfwSetInputMode(pMainWindow, GLFW_STICKY_KEYS, GL_TRUE); // Capturarea evenimentelor de la tastatura

	glClearColor(1.0, 0.0, 0.0, 1.0); // seteaza culoarea rosie pentru background

									  // Continua pana fereastra este inchisa sau tasta Esc este apasata 
	do {
		glClear(GL_COLOR_BUFFER_BIT); // Curata bufer-ul de culoare

		glfwSwapBuffers(pMainWindow); // Swapul buferelor
		glfwPollEvents(); // Procesarea evenimentelor de la utilizator

	} while (glfwGetKey(pMainWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(pMainWindow) == 0);

	glfwDestroyWindow(pMainWindow);  // Distrugem fereastra
	glfwTerminate();                 // Eliberam resursele

	return 0;
}

