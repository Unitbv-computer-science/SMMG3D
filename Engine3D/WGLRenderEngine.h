#pragma once

#include <memory>

class WGLRenderTarget;
class SceneManager;

class WGLRenderEngine
{
public:
   WGLRenderEngine(const char* windowTitle, int width = 800, int height = 600);
   ~WGLRenderEngine();

   virtual void Run() final;
   virtual void Exit() final;

   virtual void Init() {};

   SceneManager& GetSceneManager() { return *spSceneManager.get(); }

protected:
   virtual void FrameStart() {};
   virtual void Update(float deltaTime) {};
   virtual void FrameEnd() {};

protected:
   std::unique_ptr<WGLRenderTarget> spWGLRenderTarget;
   std::unique_ptr<SceneManager> spSceneManager;

private:
   bool Initialize(const char* windowTitle, int width, int height);
   void Terminate();

   void LoopUpdate();
   void RenderFrame();
   void ComputeFrameTime();

private:
   // Frame Time
   double elapsedTime;
   double deltaTime;
};

