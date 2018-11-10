#pragma once

class Camera
{
public:
   Camera(const float FoVy, const float width, const float height, const float zNear, const float zFar,
          const glm::vec3 &position, const glm::vec3 &worldUp);
   ~Camera();

   void Set(const float FoVy, const float width, const float height, const float zNear, const float zFar,
            const glm::vec3 &position, const glm::vec3 &worldUp);

   void Reshape(int windowWidth, int windowHeight);

   const glm::mat4 GetViewMatrix() const;
   const glm::mat4 GetProjectionMatrix() const;

   void KeyControl(bool* keys, double deltaTime);
   void MouseControl(float xChange, float yChange);

   //void MouseCallBack(int Button, int State, int xPos, int yPos);
   void Log() const;
private:
   void Movefront(float deltaTime);
   void MoveBackward(float deltaTime);
   void MoveRight(float deltaTime);
   void MoveLeft(float deltaTime);


   void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
   void UpdateCameraVectors();

protected:
   const float cameraSpeedFactor = 0.05f;
   const float mouseSensitivity = 0.05f;

   // Perspective properties
   float zNear;
   float zFar;
   float FoVy;
   float width;
   float height;
   bool isPerspective;

   glm::vec3 position;
   glm::vec3 forward;
   glm::vec3 right;
   glm::vec3 up;
   glm::vec3 worldUp;

   // Euler Angles
   float yaw;
   float pitch;
};

