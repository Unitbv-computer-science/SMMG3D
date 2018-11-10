#pragma once

#include "WGLRenderTarget.h"
#include "WGLRenderEngine.h"
#include "SceneManager.h"
#include "CommonDefines.h"

class Lab5Engine : public WGLRenderEngine
{
public:
   Lab5Engine(const char* windowTitle, int width = 800, int height = 600) :
      WGLRenderEngine(windowTitle, width, height)
   {}

   virtual void Init();
   void Update(float deltaTimeSeconds);
};

