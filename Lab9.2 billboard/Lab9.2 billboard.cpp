// Lab9.2 billboard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h> 
#include <vector>

#include <GL/glew.h>

#define GLM_FORCE_CTOR_INIT 
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

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
   const float zFAR = 100.f;
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

   const glm::vec3 GetPosition() const
   {
      return position;
   }
   const glm::vec3 GetRight() const
   {
      return right;
   }
   const glm::vec3 GetUp() const
   {
      return up;
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

class Shader
{
public:
   // constructor generates the shader on the fly
   // ------------------------------------------------------------------------
   Shader(const char* vertexPath, const char* fragmentPath)
   {
      Init(vertexPath, fragmentPath);
   }

   ~Shader()
   {
      glDeleteProgram(ID);
   }

   // activate the shader
   // ------------------------------------------------------------------------
   void Use() const
   {
      glUseProgram(ID);
   }

   unsigned int GetID() const { return ID; }

   // MVP
   unsigned int loc_model_matrix;
   unsigned int loc_view_matrix;
   unsigned int loc_projection_matrix;

   // utility uniform functions
   void SetInt(const std::string &name, int value) const
   {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
   }
   void SetFloat(const std::string &name, const float &value) const
   {
      glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
   }
   void SetVec2(const std::string &name, float x, float y) const
   {
      glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
   }
   void SetVec3(const std::string &name, const glm::vec3 &value) const
   {
      glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
   }
   void SetVec3(const std::string &name, float x, float y, float z) const
   {
      glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
   }
   void SetMat4(const std::string &name, const glm::mat4 &mat) const
   {
      glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
   }

private:
   void Init(const char* vertexPath, const char* fragmentPath)
   {
      // 1. retrieve the vertex/fragment source code from filePath
      std::string vertexCode;
      std::string fragmentCode;
      std::ifstream vShaderFile;
      std::ifstream fShaderFile;
      // ensure ifstream objects can throw exceptions:
      vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      try {
         // open files
         vShaderFile.open(vertexPath);
         fShaderFile.open(fragmentPath);
         std::stringstream vShaderStream, fShaderStream;
         // read file's buffer contents into streams
         vShaderStream << vShaderFile.rdbuf();
         fShaderStream << fShaderFile.rdbuf();
         // close file handlers
         vShaderFile.close();
         fShaderFile.close();
         // convert stream into string
         vertexCode = vShaderStream.str();
         fragmentCode = fShaderStream.str();
      }
      catch (std::ifstream::failure e) {
         std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
      }
      const char* vShaderCode = vertexCode.c_str();
      const char * fShaderCode = fragmentCode.c_str();

      // 2. compile shaders
      unsigned int vertex, fragment;
      // vertex shader
      vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vShaderCode, NULL);
      glCompileShader(vertex);
      CheckCompileErrors(vertex, "VERTEX");
      // fragment Shader
      fragment = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment, 1, &fShaderCode, NULL);
      glCompileShader(fragment);
      CheckCompileErrors(fragment, "FRAGMENT");
      // shader Program
      ID = glCreateProgram();
      glAttachShader(ID, vertex);
      glAttachShader(ID, fragment);
      glLinkProgram(ID);
      CheckCompileErrors(ID, "PROGRAM");

      // 3. delete the shaders as they're linked into our program now and no longer necessery
      glDeleteShader(vertex);
      glDeleteShader(fragment);
   }

   // utility function for checking shader compilation/linking errors.
   // ------------------------------------------------------------------------
   void CheckCompileErrors(unsigned int shader, std::string type)
   {
      GLint success;
      GLchar infoLog[1024];
      if (type != "PROGRAM") {
         glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
         if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
         }
      }
      else {
         glGetProgramiv(shader, GL_LINK_STATUS, &success);
         if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
         }
      }
   }
private:
   unsigned int ID;
};


Camera *pCamera = nullptr;

void Cleanup()
{
   delete pCamera;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// timing
double deltaTime = 0.0f;	// time between current frame and last frame
double lastFrame = 0.0f;

unsigned int CreateTexture(const std::string& strTexturePath);

int main(int argc, char** argv)
{
   std::string strFullExeFileName = argv[0];
   std::string strExePath;
   const size_t last_slash_idx = strFullExeFileName.rfind('\\');
   if (std::string::npos != last_slash_idx) {
      strExePath = strFullExeFileName.substr(0, last_slash_idx);
   }


   // glfw: initialize and configure
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   // glfw window creation
   GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab 7", NULL, NULL);
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

   glEnable(GL_DEPTH_TEST);

   float cubeVertices[] = {
      // positions          // texture Coords
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
   };
   float planeVertices[] = {
      // positions          // texture Coords 
      5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
      -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
      -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

      5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
      -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
      5.0f, -0.5f, -5.0f,  2.0f, 2.0f
   };
   float transparentVertices[] = {
      // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
      0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
      0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
      1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

      0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
      1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
      1.0f,  0.5f,  0.0f,  1.0f,  0.0f
   };
   // cube VAO
   unsigned int cubeVAO, cubeVBO;
   glGenVertexArrays(1, &cubeVAO);
   glGenBuffers(1, &cubeVBO);
   glBindVertexArray(cubeVAO);
   glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   // plane VAO
   unsigned int planeVAO, planeVBO;
   glGenVertexArrays(1, &planeVAO);
   glGenBuffers(1, &planeVBO);
   glBindVertexArray(planeVAO);
   glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   // transparent VAO
   unsigned int transparentVAO, transparentVBO;
   glGenVertexArrays(1, &transparentVAO);
   glGenBuffers(1, &transparentVBO);
   glBindVertexArray(transparentVAO);
   glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   glBindVertexArray(0);

   // The VBO containing the 4 vertices of the particles.
   const GLfloat billboardVertices[] = {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      -0.5f,  0.5f, 0.0f,
      0.5f,  0.5f, 0.0f,
   };
   GLuint billboardVAO;
   glGenVertexArrays(1, &billboardVAO);
   glBindVertexArray(billboardVAO);

   GLuint billboardVBO;
   glGenBuffers(1, &billboardVBO);
   glBindBuffer(GL_ARRAY_BUFFER, billboardVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(billboardVertices), billboardVertices, GL_DYNAMIC_DRAW);

   unsigned int cubeTexture = CreateTexture(strExePath + "\\Stones.jpg");
   unsigned int floorTexture = CreateTexture(strExePath + "\\ColoredFloor.png");
   unsigned int transparentTexture = CreateTexture(strExePath + "\\grass2.png");

   std::vector<glm::vec3> vegetation
   {
      glm::vec3(-1.5f, 0.0f, -0.48f),
      glm::vec3(1.5f, 0.0f, 0.51f),
      glm::vec3(0.0f, 0.0f, 0.7f),
      glm::vec3(-0.3f, 0.0f, -2.3f),
      glm::vec3(0.5f, 0.0f, -0.6f)
   };

   // Create camera
   pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0, 0.0, 3.0));

   Shader shaderFloor("Floor.vs", "Floor.fs");
   Shader shaderBillboard("Billboard.vs", "Billboard.fs");

   // render loop
   while (!glfwWindowShouldClose(window)) {
      // per-frame time logic
      double currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      // input
      processInput(window);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glm::mat4 projection = pCamera->GetProjectionMatrix();
      glm::mat4 view = pCamera->GetViewMatrix();

      // We will need the camera's position in order to sort the particles
      // w.r.t the camera's distance.
      // There should be a getCameraPosition() function in common/controls.cpp, 
      // but this works too.
      glm::vec3 cameraPosition = pCamera->GetPosition();

      glm::mat4 viewProjection = projection * view;

      //glEnable(GL_BLEND);
      //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      //shaderBillboard.Use();

      //// Bind our texture in Texture Unit 0
      //glActiveTexture(GL_TEXTURE0);
      //glBindTexture(GL_TEXTURE_2D, transparentTexture);
      //// Set our "billboardTextureSampler" sampler to use Texture Unit 0
      //shaderBillboard.SetInt("billboardTextureSampler", 0);

      //shaderBillboard.SetVec3("CameraRightWorldspace", pCamera->GetRight());
      //shaderBillboard.SetVec3("CameraUpWorldspace", pCamera->GetUp());
      //shaderBillboard.SetVec3("BillboardPos", 0.0f, 0.5f, 0.0f);
      //shaderBillboard.SetVec2("BillboardSize", 1.0f, 0.125f);
      //shaderFloor.SetMat4("VP", viewProjection);

      //// 1rst attribute buffer : vertices
      //glBindVertexArray(billboardVAO);
      //glEnableVertexAttribArray(0);
      //glBindBuffer(GL_ARRAY_BUFFER, billboardVBO);
      //glVertexAttribPointer(
      //   0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
      //   3,                  // size
      //   GL_FLOAT,           // type
      //   GL_FALSE,           // normalized?
      //   0,                  // stride
      //   (void*)0            // array buffer offset
      //);

      //// Draw the billboard
      //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      glDisable(GL_BLEND);
      shaderFloor.Use();
      shaderFloor.SetInt("texture1", 0);

      shaderFloor.SetMat4("projection", projection);
      shaderFloor.SetMat4("view", view);

      glBindVertexArray(cubeVAO);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, cubeTexture);

      glm::mat4 model;
      model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
      shaderFloor.SetMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4();
      model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
      shaderFloor.SetMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // floor
      shaderFloor.Use();
      glBindVertexArray(planeVAO);
      //shaderFloor.SetInt("texture1", 1);
      glBindTexture(GL_TEXTURE_2D, floorTexture);
      shaderFloor.SetMat4("model", glm::mat4());
      glDrawArrays(GL_TRIANGLES, 0, 6);
      
      // vegetation
      glBindVertexArray(transparentVAO);
      glBindTexture(GL_TEXTURE_2D, transparentTexture);
      for (GLuint i = 0; i < vegetation.size(); i++)
      {
         model = glm::mat4();
         model = glm::translate(model, vegetation[i]);
         shaderFloor.SetMat4("model", model);
         shaderFloor.SetInt("texture1", 0);
         glDrawArrays(GL_TRIANGLES, 0, 6);
      }

      // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   Cleanup();

   glDeleteVertexArrays(1, &cubeVAO);
   glDeleteVertexArrays(1, &planeVAO);
   glDeleteBuffers(1, &cubeVBO);
   glDeleteBuffers(1, &planeVBO);

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

unsigned int CreateTexture(const std::string& strTexturePath)
{
   unsigned int textureId = -1;

   // load image, create texture and generate mipmaps
   int width, height, nrChannels;
   //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
   unsigned char *data = stbi_load(strTexturePath.c_str(), &width, &height, &nrChannels, 0);
   if (data) {
      GLenum format;
      if (nrChannels == 1)
         format = GL_RED;
      else if (nrChannels == 3)
         format = GL_RGB;
      else if (nrChannels == 4)
         format = GL_RGBA;

      glGenTextures(1, &textureId);
      glBindTexture(GL_TEXTURE_2D, textureId);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      // set the texture wrapping parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
      // set texture filtering parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   }
   else {
      std::cout << "Failed to load texture: " << strTexturePath << std::endl;
   }
   stbi_image_free(data);

   return textureId;
}

