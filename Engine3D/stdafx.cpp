// stdafx.cpp : source file that includes just the standard includes
// Engine3D.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "Engine3D.lib")
#pragma comment (lib, "glfw3dll.lib")

static std::string GLerrorDescription[] = {
   "GL_INVALID_ENUM",						// 0x0500
   "GL_INVALID_VALUE",						// 0x0501
   "GL_INVALID_OPERATION",					// 0x0502
   "GL_STACK_OVERFLOW",					// 0x0503
   "GL_STACK_UNDERFLOW",					// 0x0504
   "GL_OUT_OF_MEMORY",						// 0x0505
   "GL_INVALID_FRAMEBUFFER_OPERATION" 		// 0x0506
};

void PrintGLErrorDescription(unsigned int glErr)
{
   std::cout << "\n[OpenGL Error]\n";
   std::cout << "\t[" << glErr << "] : " << GLerrorDescription[glErr - GL_INVALID_ENUM].c_str() << "\n";
}

int CheckWGLError(const char * file, int line, bool log)
{
#ifdef _DEBUG
   GLenum glErr = glGetError();
   if (glErr != GL_NO_ERROR && log) {
      PrintGLErrorDescription(glErr);
      std::cout << "\t[File] : " << file << "\t[Line] : " << line << std::endl;
      return glErr;
   }
#endif
   return 0;
}