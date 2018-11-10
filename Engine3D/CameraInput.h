#pragma once

#include "InputController.h"

class Camera;

class CameraInput : public InputController
{
public:
   CameraInput(Camera *pCamera);

   void OnInputUpdate(float deltaTime, int mods) override;
   void OnKeyPress(int key, int mods) override;
   void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;

private:
   const Camera *pCamera;
};

