#include "stdafx.h"
#include "InputController.h"


InputController::InputController()
{
   isAttached = false;
}


InputController::~InputController()
{}

void InputController::Subscribe(WGLRenderTarget* pWGLRenderTarget)
{
   this->pWGLRenderTarget = pWGLRenderTarget;
}

void InputController::UnSubscribe(WGLRenderTarget* pWGLRenderTarget)
{
   this->pWGLRenderTarget = nullptr;
}