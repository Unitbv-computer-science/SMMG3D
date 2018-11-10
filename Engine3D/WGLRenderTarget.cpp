#include "stdafx.h"

#include "WGLRenderTarget.h"
#include "InputController.h"

WGLRenderTarget::WGLRenderTarget(int width, int height)
{
   firstMouse = true;

   memset(keys, 0, sizeof(keys));
   Set(width, height);
}


WGLRenderTarget::~WGLRenderTarget()
{
   glfwDestroyWindow(mainWindow);
   glfwTerminate();
}

void WGLRenderTarget::Set(int width, int height)
{
   // make sure the viewport matches the new window dimensions; note that width and 
   // height will be significantly larger than specified on retina displays.
   glViewport(0, 0, width, height);

   windowProperties.size.x = width;
   windowProperties.size.y = height;

   glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

   this->lastX = (int)(width / 2.0f);
   this->lastY = (int)(height / 2.0f);
}

int WGLRenderTarget::Initialize(const char* windowTitle)
{
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   windowProperties.title = windowTitle;
   mainWindow = glfwCreateWindow(windowProperties.size.x, windowProperties.size.y, windowTitle, NULL, NULL);
   if (mainWindow == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(mainWindow);
   glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

   glewExperimental = GL_TRUE;

   glewInit();

   // tell GLFW to capture our mouse
   //glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   
   // Capturarea evenimentelor de la tastatura
   glfwSetInputMode(mainWindow, GLFW_STICKY_KEYS, GL_TRUE); 

   glfwSetWindowUserPointer(mainWindow, this);

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // culoarea de fond a ecranului

   // Activam face cooling-ul
   glEnable(GL_CULL_FACE);

   // Orientarea poligoanelor este directa
   glFrontFace(GL_CCW);

   // Sunt eliminate poligoanele orientate invers
   glCullFace(GL_BACK);

   //glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_LIGHTING);

   SetCallbacks();

   return 1;
}

bool WGLRenderTarget::ShouldClose() const
{
   return glfwWindowShouldClose(mainWindow);
}


void WGLRenderTarget::Close()
{
   glfwSetWindowShouldClose(mainWindow, GL_TRUE);
}

void WGLRenderTarget::ShowCursor()
{
   hiddenPointer = false;
   glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void WGLRenderTarget::HideCursor()
{
   hiddenPointer = true;
   glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void WGLRenderTarget::DisableCursor()
{
   hiddenPointer = true;
   glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool WGLRenderTarget::KeyHold(int keyCode) const
{
   return keyStates[keyCode];
}

bool WGLRenderTarget::MouseHold(int button) const
{
   return IS_BIT_SET(mouseButtonStates, button);
}

int WGLRenderTarget::GetSpecialKeyState() const
{
   return keyMods;
}

double WGLRenderTarget::GetXChange()
{
   double theXChange = xChange;
   xChange = 0;
   return theXChange;
}

double WGLRenderTarget::GetYChange()
{
   double theYChange = yChange;
   yChange = 0;
   return theYChange;
}

void WGLRenderTarget::SubscribeToEvents(InputController * pIC)
{
   observers.push_back(pIC);
   pIC->Subscribe(this);
}

void WGLRenderTarget::UnsubscribeFromEvents(InputController * pIC)
{
   observers.remove(pIC);
   pIC->UnSubscribe(this);
}

void WGLRenderTarget::SetCallbacks()
{
   glfwSetKeyCallback(mainWindow, keysCallback);
   glfwSetCursorPosCallback(mainWindow, mouseCallback);

   glfwSetWindowCloseCallback(mainWindow, WGLRenderTarget::OnClose);
   glfwSetWindowSizeCallback(mainWindow, WGLRenderTarget::OnResize);
   glfwSetKeyCallback(mainWindow, WGLRenderTarget::OnKeyCallback);
   glfwSetMouseButtonCallback(mainWindow, WGLRenderTarget::OnMouseClick);
   glfwSetCursorPosCallback(mainWindow, WGLRenderTarget::OnCursorMove);
   glfwSetScrollCallback(mainWindow, WGLRenderTarget::OnMouseScroll);
}

// Update event listeners (key press / mouse move / window events)
void WGLRenderTarget::UpdateObservers(double deltaTime)
{
   // Signal window resize
   if (resizeEvent) {
      resizeEvent = false;
      for (auto obs : observers) {
         obs->OnWindowResize(windowProperties.size.x, windowProperties.size.y);
      }
   }

   // Signal mouse move event
   if (mouseMoveEvent) {
      mouseMoveEvent = false;
      for (auto obs : observers) {
         obs->OnMouseMove(windowProperties.cursorPos.x, windowProperties.cursorPos.y, mouseDeltaX, mouseDeltaY);
      }
   }

   // Signal mouse button press event
   auto pressEvent = mouseButtonAction & mouseButtonStates;
   if (pressEvent) {
      if (IS_BIT_SET(pressEvent, GLFW_MOUSE_BUTTON_RIGHT)) {
         DisableCursor();
      }
      auto pressEvent = mouseButtonAction & mouseButtonStates;
      for (auto obs : observers) {
         obs->OnMouseBtnPress(windowProperties.cursorPos.x, windowProperties.cursorPos.y, pressEvent, keyMods);
      }
   }

   // Signal mouse button release event
   auto releaseEvent = mouseButtonAction & (~mouseButtonStates);
   if (releaseEvent) {
      if (IS_BIT_SET(releaseEvent, GLFW_MOUSE_BUTTON_RIGHT)) {
         ShowCursor();
      }

      for (auto obs : observers) {
         obs->OnMouseBtnRelease(windowProperties.cursorPos.x, windowProperties.cursorPos.y, releaseEvent, keyMods);
      }
   }

   // Signal mouse scroll event
   if (scrollEvent) {
      scrollEvent = false;
      for (auto obs : observers) {
         obs->OnMouseScroll(windowProperties.cursorPos.x, windowProperties.cursorPos.y, mouseScrollDeltaX, mouseScrollDeltaY);
      }
   }

   // Signal key events
   if (registeredKeyEvents) {
      for (int i = 0; i < registeredKeyEvents; i++) {
         for (auto obs : observers)
            keyStates[keyEvents[i]] ? obs->OnKeyPress(keyEvents[i], keyMods) : obs->OnKeyRelease(keyEvents[i], keyMods);
      }
      registeredKeyEvents = 0;
   }

   if (MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
      // Continuous events
      for (auto obs : observers) {

         obs->OnInputUpdate(static_cast<float>(deltaTime), keyMods);
      }
   }

   mouseButtonAction = 0;
}

void WGLRenderTarget::keysCallback(GLFWwindow* window, int Key, int code, int action, int mode)
{
   WGLRenderTarget* renderTarget = static_cast<WGLRenderTarget*>(glfwGetWindowUserPointer(window));

   if (Key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      renderTarget->Close();
   }

   if (Key >= 0 && Key <= 1024) {
      if (action == GLFW_PRESS) {
         renderTarget->keys[Key] = true;
      }
      if (action == GLFW_RELEASE) {
         renderTarget->keys[Key] = false;
      }
   }
}

void WGLRenderTarget::mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
   WGLRenderTarget* renderTarget = static_cast<WGLRenderTarget*>(glfwGetWindowUserPointer(window));

   if (renderTarget->firstMouse) {
      renderTarget->lastX = xPos;
      renderTarget->lastY = yPos;
      renderTarget->firstMouse = false;
   }

   renderTarget->xChange = xPos - renderTarget->lastX;
   renderTarget->yChange = renderTarget->lastY - yPos;
   renderTarget->lastX = xPos;
   renderTarget->lastY = yPos;
}