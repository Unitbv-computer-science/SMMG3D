#include "stdafx.h"
#include "WGLRenderTarget.h"

void WGLRenderTarget::OnKeyCallback(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
   WGLRenderTarget* renderTarget = static_cast<WGLRenderTarget*>(glfwGetWindowUserPointer(window));
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      renderTarget->Close();
   }

   renderTarget->keyMods = mods;
   if (renderTarget->keyStates[key] == (action ? true : false))
      return;
   renderTarget->keyStates[key] = action ? true : false;
   renderTarget->keyEvents[renderTarget->registeredKeyEvents] = key;
   renderTarget->registeredKeyEvents++;
}

void WGLRenderTarget::OnCursorMove(GLFWwindow *window, double posX, double posY)
{
   WGLRenderTarget* renderTarget = static_cast<WGLRenderTarget*>(glfwGetWindowUserPointer(window));
   // Save information for processing later on the Update thread
   if (renderTarget->mouseMoveEvent) {
      renderTarget->mouseDeltaX += (int)(posX - renderTarget->windowProperties.cursorPos.x);
      renderTarget->mouseDeltaY += (int)(posY - renderTarget->windowProperties.cursorPos.y);
   } else {
      renderTarget->mouseMoveEvent = true;
      renderTarget->mouseDeltaX = (int)(posX - renderTarget->windowProperties.cursorPos.x);
      renderTarget->mouseDeltaY = (int)(posY - renderTarget->windowProperties.cursorPos.y);
   }
   renderTarget->windowProperties.cursorPos = glm::ivec2(posX, posY);
}

void WGLRenderTarget::OnMouseClick(GLFWwindow *window, int button, int action, int mods)
{
   WGLRenderTarget* renderTarget = static_cast<WGLRenderTarget*>(glfwGetWindowUserPointer(window));
   // Only button events and mods are kept
   // Mouse position is the current frame position
   renderTarget->keyMods = mods;
   SET_BIT(renderTarget->mouseButtonAction, button);
   action ? SET_BIT(renderTarget->mouseButtonStates, button) : CLEAR_BIT(renderTarget->mouseButtonStates, button);

}

void WGLRenderTarget::OnMouseScroll(GLFWwindow * window, double offsetX, double offsetY)
{
   WGLRenderTarget* renderTarget = static_cast<WGLRenderTarget*>(glfwGetWindowUserPointer(window));
   renderTarget->mouseScrollDeltaX = (int)offsetX;
   renderTarget->mouseScrollDeltaY = (int)offsetY;
}

void WGLRenderTarget::OnClose(GLFWwindow * window)
{
   WGLRenderTarget* renderTarget = static_cast<WGLRenderTarget*>(glfwGetWindowUserPointer(window));
   renderTarget->Close();
}

void WGLRenderTarget::OnResize(GLFWwindow *window, int width, int height)
{
   WGLRenderTarget* renderTarget = static_cast<WGLRenderTarget*>(glfwGetWindowUserPointer(window));
   renderTarget->Set(width, height);
}

void WGLRenderTarget::OnError(int error, const char * description)
{
   std::cout << "[GLFW ERROR]\t" << error << "\t" << description << std::endl;
}