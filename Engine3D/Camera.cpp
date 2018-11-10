#include "stdafx.h"
#include <GL/glew.h>
#include <glfw3.h>

#include "Camera.h"



Camera::Camera(const float FoVy, const float width, const float height, const float zNear, const float zFar,
               const glm::vec3 &position, const glm::vec3 &worldUp)
{
   this->isPerspective = true;
   this->yaw = 90.f;
   this->pitch = 0.f;
   Set(FoVy, width, height, zNear, zFar, position, worldUp);
}

Camera::~Camera()
{}

void Camera::Set(const float FoVy, const float width, const float height, const float zNear, const float zFar,
                 const glm::vec3 &position, const glm::vec3 &worldUp)
{
   this->FoVy = FoVy;
   this->width = width;
   this->height = height;
   this->zNear = zNear;
   this->zFar = zFar;

   this->worldUp = worldUp;
   this->position = position;

   UpdateCameraVectors();
}

void Camera::Reshape(int windowWidth, int windowHeight)
{
   width = (float)windowWidth;
   height = (float)windowHeight;

   /* define the viewport transformation */
   glViewport(0, 0, windowWidth, windowHeight);
}

const glm::mat4 Camera::GetViewMatrix() const
{
   // Returns the View Matrix
   return glm::lookAt(position, position + forward, up);
}

const glm::mat4 Camera::GetProjectionMatrix() const
{
   glm::mat4 Proj = glm::mat4(1);
   if (isPerspective) {
      float aspectRatio = width / height;
      Proj = glm::perspective(glm::radians(FoVy), aspectRatio, zNear, zFar);
   } else {
      float scaleFactor = 2.f;
      Proj = glm::ortho<float>(
         -width / scaleFactor, width / scaleFactor,
         -height / scaleFactor, height / scaleFactor, -zFar, zFar);
   }
   return Proj;
}

void Camera::KeyControl(bool* keys, double deltaTime)
{
   float velocity = (float)(cameraSpeedFactor * deltaTime);
   if (keys[GLFW_KEY_W]) {
      position += forward * velocity;
   }
   if (keys[GLFW_KEY_S]) {
      position -= forward * velocity;
   }
   if (keys[GLFW_KEY_A]) {
      position -= right * velocity;
   }
   if (keys[GLFW_KEY_D]) {
      position += right * velocity;
   }
}

void Camera::MouseControl(float xChange, float yChange)
{
   if (fabs(xChange) <= 1e-6 && fabs(yChange) <= 1e-6) {
      return;
   }
   xChange *= mouseSensitivity;
   yChange *= mouseSensitivity;

   ProcessMouseMovement(xChange, yChange);
}

void Camera::Log() const
{
   std::cout.precision(2);
   std::cout << "Camera =>" << std::endl;
   std::cout << "Rotation: " << yaw << pitch << std::endl;
   std::cout << "Position: " << position.x << position.y << position.z << std::endl;
   std::cout << "Forward : " << -forward.x << -forward.y << -forward.z << std::endl;
   std::cout << "Up      : " << up.x << up.y << up.z << std::endl;
   std::cout << "--------------------------------------------------" << std::endl;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
   yaw += xOffset;
   pitch += yOffset;

   std::cout << "yaw = " << yaw << std::endl;
   std::cout << "pitch = " << pitch << std::endl;

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

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::UpdateCameraVectors()
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