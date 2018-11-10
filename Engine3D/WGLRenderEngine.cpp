#include "stdafx.h"
#include "WGLRenderEngine.h"

#include "WGLRenderTarget.h"
#include "Camera.h"
#include "CameraInput.h"
#include "SceneManager.h"
#include "Shader.h"
#include "Mesh.h"

WGLRenderEngine::WGLRenderEngine(const char* windowTitle, int width, int height)
{
   Initialize(windowTitle, width, height);
}


WGLRenderEngine::~WGLRenderEngine()
{
   Terminate();
}

bool WGLRenderEngine::Initialize(const char* windowTitle, int width, int height)
{
   spWGLRenderTarget.reset(new WGLRenderTarget(width, height));
   if (!spWGLRenderTarget->Initialize(windowTitle)) {
      return false;
   }

   // Create scene manager for meshes and shaders
   spSceneManager.reset(new SceneManager(width, height));

   // Register camera input to get notifications
   spWGLRenderTarget->SubscribeToEvents(spSceneManager->GetCameraInput());

   return true;
}

void WGLRenderEngine::Terminate()
{
   glfwTerminate();
}

void WGLRenderEngine::Run()
{
   if (!spWGLRenderTarget.get())
      return;

   while (!spWGLRenderTarget->ShouldClose()) {
      LoopUpdate();
   }
}

void WGLRenderEngine::Exit()
{
   spWGLRenderTarget->Close();
}

void WGLRenderEngine::LoopUpdate()
{
   // Polls and buffers the events
   glfwPollEvents();

   // Computes frame deltaTime in seconds
   ComputeFrameTime();

   // Calls the methods of the instance of InputController in the following order
   // OnWindowResize, OnMouseMove, OnMouseBtnPress, OnMouseBtnRelease, OnMouseScroll, OnKeyPress, OnMouseScroll, OnInputUpdate
   // OnInputUpdate will be called each frame, the other functions are called only if an event is registered
   spWGLRenderTarget->UpdateObservers(deltaTime);

   // Frame processing
   RenderFrame();

   // Swap front and back buffers - image will be displayed to the screen
   glfwSwapBuffers(spWGLRenderTarget->GetMainWindow());
   CheckOpenGLError();
}

void WGLRenderEngine::RenderFrame()
{
   FrameStart();
   Update(static_cast<float>(deltaTime));
   FrameEnd();
}


void WGLRenderEngine::ComputeFrameTime()
{
   double currentTime = glfwGetTime();
   deltaTime = currentTime - elapsedTime;
   elapsedTime = currentTime;
}