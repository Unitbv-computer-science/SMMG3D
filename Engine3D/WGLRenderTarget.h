#pragma once

#include <string>
#include <list>

#include <GLM.hpp>

//forward declarations
class InputController;
struct GLFWwindow;

struct WindowProperties
{
   std::string title;
   glm::ivec2 size;
   glm::ivec2 cursorPos;
};

class WGLRenderTarget
{
   friend class WGLRenderEngine; // Allow to call private methods like UpdateObservers
public:
   WGLRenderTarget(int width = 800, int height = 600);
   ~WGLRenderTarget();

   void Set(int width, int height);

   int Initialize(const char* windowTitle);

   int GetBufferWidth() const  { return bufferWidth;  }
   int GetBufferHeight() const { return bufferHeight; };

   GLFWwindow* GetMainWindow() { return mainWindow; }

   bool ShouldClose() const;
   void Close();

   void ShowCursor();
   void HideCursor();
   void DisableCursor();

   // Get Input State
   bool KeyHold(int keyCode) const;
   bool MouseHold(int button) const;
   int GetSpecialKeyState() const;

   bool* Keys() { return keys; }

   double GetXChange();
   double GetYChange();

   void SubscribeToEvents(InputController * pIC);
   void UnsubscribeFromEvents(InputController * pIC);

private:
   // Update event listeners (key press / mouse move / window events)
   void UpdateObservers(double deltaTime);

   void SetCallbacks();

   static void keysCallback(GLFWwindow* window, int Key, int code, int action, int mode);
   static void mouseCallback(GLFWwindow* window, double xPos, double yPos);

   static void OnKeyCallback(GLFWwindow *window, int key, int scanCode, int action, int mods);
   static void OnCursorMove(GLFWwindow *window, double posX, double posY);
   static void OnMouseClick(GLFWwindow *window, int button, int action, int mods);
   static void OnMouseScroll(GLFWwindow * window, double offsetX, double offsetY);
   static void OnClose(GLFWwindow * window);
   static void OnResize(GLFWwindow *window, int width, int height);
   static void OnError(int error, const char * description);

private:
   GLFWwindow* mainWindow;

   WindowProperties windowProperties;
   int bufferWidth, bufferHeight;
   bool keys[1024];

   bool firstMouse = true;
   double lastX = 0;
   double lastY = 0;
   double xChange = 0;
   double yChange = 0;

   // Window state and events
   bool hiddenPointer;
   bool resizeEvent;

   // Mouse button callback
   int mouseButtonCallback;			// Bit field for button callback
   int mouseButtonAction;				// Bit field for button state
   int mouseButtonStates;				// Bit field for mouse button state

                                    // Mouse move event
   bool mouseMoveEvent;
   int mouseDeltaX;
   int mouseDeltaY;

   // Mouse scroll event
   bool scrollEvent;
   int mouseScrollDeltaX;
   int mouseScrollDeltaY;

   // States for keyboard buttons - PRESSED(true) / RELEASED(false)
   int registeredKeyEvents;
   int keyEvents[128];
   bool keyStates[384];

   // Platform specific key codes - PRESSED(true) / RELEASED(false)
   bool keyScanCode[512];

   // Special keys (ALT, CTRL, SHIFT, CAPS LOOK, OS KEY) active alongside with normal key or mouse input
   int keyMods;

   // Input Observers
   std::list<InputController*> observers;
};

