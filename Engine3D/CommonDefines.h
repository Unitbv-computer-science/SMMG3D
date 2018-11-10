#pragma once

#define ZERO_MEM(a)			memset(a, 0, sizeof(a))
#define SIZEOF_ARRAY(a)		(sizeof(a) / sizeof(a[0]))
#define SAFE_FREE(p)		if (p) {delete p; p = nullptr;}
#define SAFE_FREE_ARRAY(p)	if (p) {delete[] p; p = nullptr;}

#define SET_BIT(item, bit) (item) |= (1 << (bit))
#define CLEAR_BIT(item, bit) (item) &= ~(1 << (bit))
#define IS_BIT_SET(item, bit) (((item) & (1 << (bit))) != 0)

#ifdef _DEBUG
#define CheckOpenGLError() CheckWGLError(__FILE__, __LINE__)
#define SilentCheckOpenGLError() CheckWGLError(__FILE__, __LINE__, false)
#else
#define CheckOpenGLError()
#define SilentCheckOpenGLError()
#endif

// Check for OpenGL Errors
// Returns 1 if an OpenGL error occurred, 0 otherwise.
int CheckWGLError(const char *file, int line, bool log = true);