// Lab7 - Iluminare Phong.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h> 

#include <GL/glew.h>

#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <glfw3.h>

#include "FileUtils.h"
#include "Shader.h"
#include "Mesh.h"
#include "TerrainGrid.h"
#include "GeometryGenerator.h"
#include "CommonDefines.h"

#include <iostream>

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "Engine3D.lib")

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

enum ECameraMovementType
{
	UNKNOWN,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
private:
	// Default camera values
	const float zNEAR = 0.1f;
	const float zFAR = 500.f;
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float FOV = 45.0f;
	glm::vec3 startPosition;

public:
	Camera(const int width, const int height, const glm::vec3 &position)
	{
		startPosition = position;
		Set(width, height, position);
	}

	void Set(const int width, const int height, const glm::vec3 &position)
	{
		this->isPerspective = true;
		this->yaw = YAW;
		this->pitch = PITCH;

		this->FoVy = FOV;
		this->width = width;
		this->height = height;
		this->zNear = zNEAR;
		this->zFar = zFAR;

		this->worldUp = glm::vec3(0, 1, 0);
		this->position = position;

		lastX = width / 2.0f;
		lastY = height / 2.0f;
		bFirstMouseMove = true;

		UpdateCameraVectors();
	}

	void Reset(const int width, const int height)
	{
		Set(width, height, startPosition);
	}

	void Reshape(int windowWidth, int windowHeight)
	{
		width = windowWidth;
		height = windowHeight;

		// define the viewport transformation
		glViewport(0, 0, windowWidth, windowHeight);
	}

	const glm::mat4 GetViewMatrix() const
	{
		// Returns the View Matrix
		return glm::lookAt(position, position + forward, up);
	}

	const glm::mat4 GetProjectionMatrix() const
	{
		glm::mat4 Proj = glm::mat4(1);
		if (isPerspective) {
			float aspectRatio = ((float)(width)) / height;
			Proj = glm::perspective(glm::radians(FoVy), aspectRatio, zNear, zFar);
		}
		else {
			float scaleFactor = 2000.f;
			Proj = glm::ortho<float>(
				-width / scaleFactor, width / scaleFactor,
				-height / scaleFactor, height / scaleFactor, -zFar, zFar);
		}
		return Proj;
	}

   glm::vec3 GetPosition() const {
      return position;
   }

	void ProcessKeyboard(ECameraMovementType direction, float deltaTime)
	{
		float velocity = (float)(cameraSpeedFactor * deltaTime);
		switch (direction) {
		case ECameraMovementType::FORWARD:
			position += forward * velocity;
			break;
		case ECameraMovementType::BACKWARD:
			position -= forward * velocity;
			break;
		case ECameraMovementType::LEFT:
			position -= right * velocity;
			break;
		case ECameraMovementType::RIGHT:
			position += right * velocity;
			break;
		case ECameraMovementType::UP:
			position += up * velocity;
			break;
		case ECameraMovementType::DOWN:
			position -= up * velocity;
			break;
		}
	}

	void MouseControl(float xPos, float yPos)
	{
		if (bFirstMouseMove) {
			lastX = xPos;
			lastY = yPos;
			bFirstMouseMove = false;
		}

		float xChange = xPos - lastX;
		float yChange = lastY - yPos;
		lastX = xPos;
		lastY = yPos;

		if (fabs(xChange) <= 1e-6 && fabs(yChange) <= 1e-6) {
			return;
		}
		xChange *= mouseSensitivity;
		yChange *= mouseSensitivity;

		ProcessMouseMovement(xChange, yChange);
	}

	void ProcessMouseScroll(float yOffset)
	{
		if (FoVy >= 1.0f && FoVy <= 90.0f) {
			FoVy -= yOffset;
		}
		if (FoVy <= 1.0f)
			FoVy = 1.0f;
		if (FoVy >= 90.0f)
			FoVy = 90.0f;
	}

private:
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true)
	{
		yaw += xOffset;
		pitch += yOffset;

		//std::cout << "yaw = " << yaw << std::endl;
		//std::cout << "pitch = " << pitch << std::endl;

		// Avem grijã sã nu ne dãm peste cap
		if (constrainPitch) {
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		// Se modificã vectorii camerei pe baza unghiurilor Euler
		UpdateCameraVectors();
	}

	void UpdateCameraVectors()
	{
		// Calculate the new forward vector
		this->forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		this->forward.y = sin(glm::radians(pitch));
		this->forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		this->forward = glm::normalize(this->forward);
		// Also re-calculate the Right and Up vector
		right = glm::normalize(glm::cross(forward, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		up = glm::normalize(glm::cross(right, forward));
	}

protected:
	const float cameraSpeedFactor = 2.5f;
	const float mouseSensitivity = 0.1f;

	// Perspective properties
	float zNear;
	float zFar;
	float FoVy;
	int width;
	int height;
	bool isPerspective;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	// Euler Angles
	float yaw;
	float pitch;

	bool bFirstMouseMove = true;
	float lastX = 0.f, lastY = 0.f;
};

Camera *pCamera = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// timing
double deltaTime = 0.0f;	// time between current frame and last frame
double lastFrame = 0.0f;

int main(int argc, char** argv)
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab 5", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewInit();

   std::string strFullExeFileName = argv[0];
   std::string strExePath = FileUtils::getPath(strFullExeFileName);

   std::string strResourcesPath = "D:\\Projects\\SMMG3D\\Resources\\Shaders\\";

   Shader lightingShader(strResourcesPath.c_str(), "BasicLighting.VS.glsl", "BasicLighting.FS.glsl");
   Shader lampShader(strResourcesPath.c_str(), "Lamp.VS.glsl", "Lamp.FS.glsl");
   Shader terrainShader(strResourcesPath.c_str(), "TerrainLighting.VS.glsl", "TerrainLighting.FS.glsl");

   // indexurile cubului
   unsigned int Indices[] = {
      0,1,2,
      0,2,3,
      1,5,6,
      1,6,2,
      5,4,7,
      5,7,6,
      4,0,3,
      4,3,7,
      0,1,5,
      0,5,4,
      3,2,6,
      3,6,7
   };

   // varfurile cubului
   GLfloat Vertices[] = {
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      0.0f, 1.0f, 1.0f,
      0.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f
   };

   size_t nVertices = SIZEOF_ARRAY(Vertices);
   size_t nIndices = SIZEOF_ARRAY(Indices);

   //Mesh cube(nVertices/3, Vertices, nIndices, Indices);
   GeometryGenerator geometryGenerator;
   Mesh cube = geometryGenerator.CreateSphere(-1, 100, 100);

   TerrainGrid terrainGrid(1000, 2000, 100, 200);

   // lighting
   glm::vec3 lightPos(1.2f, 0.0f, 2.0f);

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // culoarea de fond a ecranului
   glEnable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_COLOR_MATERIAL);
   glDisable(GL_LIGHTING);

   glFrontFace(GL_CCW);
   glCullFace(GL_BACK);

   // Create camera
   pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0, 0.0, 3));

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      //RenderFunction();

      lightPos.x = 1.0 + 0.5*cos(glfwGetTime());
      lightPos.y = 1.0 + 0.5*sin(glfwGetTime());

      // be sure to activate shader when setting uniforms/drawing objects
      lightingShader.Use();
      lightingShader.SetVec3("objectColor", 0.5f, 1.0f, 0.31f);
      lightingShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
      lightingShader.SetVec3("lightPos", lightPos);
      lightingShader.SetVec3("viewPos", pCamera->GetPosition());

      // draw cube
      glm::mat4 projection = pCamera->GetProjectionMatrix();
      glm::mat4 view = pCamera->GetViewMatrix();
      lightingShader.SetMat4("projection", projection);
      lightingShader.SetMat4("view", view);

      // world transformation
      glm::mat4 model = glm::mat4(1.0);
      model = glm::scale(model, glm::vec3(1.7f));
      lightingShader.SetMat4("model", model);

      cube.Draw(lightingShader);

      terrainShader.Use();
      terrainShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
      terrainShader.SetVec3("lightPos", lightPos);
      terrainShader.SetVec3("viewPos", pCamera->GetPosition());
      terrainShader.SetMat4("projection", projection);
      terrainShader.SetMat4("view", view);
      terrainShader.SetMat4("model", glm::mat4(1.0));

      //terrainGrid.Draw(terrainShader);

      // also draw the lamp object
      lampShader.Use();
      lampShader.SetMat4("projection", projection);
      lampShader.SetMat4("view", view);

      model = glm::mat4(1.0);
      model = glm::translate(model, lightPos);
      model = glm::scale(model, glm::vec3(0.05f)); // a smaller cube
      lampShader.SetMat4("model", model);

      cube.Draw(lampShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(UP, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(DOWN, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->Reset(width, height);

	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	pCamera->Reshape(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	pCamera->MouseControl((float)xpos, (float)ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
	pCamera->ProcessMouseScroll((float)yOffset);
}