#include "stdafx.h"
#include "CameraInput.h"
#include "Camera.h"
#include "WGLRenderTarget.h"

CameraInput::CameraInput(Camera *pCamera)
{
   this->pCamera = pCamera;
}

void CameraInput::OnInputUpdate(float deltaTime, int mods)
{
   if (pWGLRenderTarget == nullptr) {
      return;
   }

   if (!pWGLRenderTarget->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
      return;
   }

   if (pWGLRenderTarget->GetSpecialKeyState() & GLFW_MOD_SHIFT) {
      deltaTime *= 2;
   }

   //if (pWGLRenderTarget->KeyHold(GLFW_KEY_W))	pCamera->MoveForward(deltaTime);
   //if (pWGLRenderTarget->KeyHold(GLFW_KEY_S))	pCamera->MoveBackward(deltaTime);
   //if (pWGLRenderTarget->KeyHold(GLFW_KEY_A))	pCamera->MoveLeft(deltaTime);
   //if (pWGLRenderTarget->KeyHold(GLFW_KEY_D))	pCamera->MoveRight(deltaTime);
   //if (pWGLRenderTarget->KeyHold(GLFW_KEY_Q))	pCamera->MoveDown(deltaTime);
   //if (pWGLRenderTarget->KeyHold(GLFW_KEY_E))	pCamera->MoveUp(deltaTime);

   //if (pWGLRenderTarget->KeyHold(GLFW_KEY_KP_MULTIPLY))pCamera->UpdateSpeed();
   //if (pWGLRenderTarget->KeyHold(GLFW_KEY_KP_DIVIDE))	pCamera->UpdateSpeed(-0.2f);

   //if (pWGLRenderTarget->KeyHold(GLFW_KEY_KP_4)) pCamera->RotateOY(500 * deltaTime);
   //if (pWGLRenderTarget->KeyHold(GLFW_KEY_KP_6)) pCamera->RotateOY(-500 * deltaTime);
   //if (pWGLRenderTarget->KeyHold(GLFW_KEY_KP_8)) pCamera->RotateOX(700 * deltaTime);
   //if (pWGLRenderTarget->KeyHold(GLFW_KEY_KP_5)) pCamera->RotateOX(-700 * deltaTime);

   //pCamera->Update();
}

void CameraInput::OnKeyPress(int key, int mods)
{
   if (mods) return;
   if (key == GLFW_KEY_C) {
      pCamera->Log();
   }
}

void CameraInput::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

   if (pWGLRenderTarget->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
      //pCamera->RotateOY(-(float)deltaX);
      //pCamera->RotateOX(-(float)deltaY);
      //pCamera->Update();
   }
}
