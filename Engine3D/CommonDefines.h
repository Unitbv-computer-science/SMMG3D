#pragma once

#define ZERO_MEM(a)			memset(a, 0, sizeof(a))
#define SIZEOF_ARRAY(a)		(sizeof(a) / sizeof(a[0]))
#define SAFE_FREE(p)		if (p) {delete p; p = nullptr;}
#define SAFE_FREE_ARRAY(p)	if (p) {delete[] p; p = nullptr;}

#define SET_BIT(item, bit) (item) |= (1 << (bit))
#define CLEAR_BIT(item, bit) (item) &= ~(1 << (bit))
#define IS_BIT_SET(item, bit) (((item) & (1 << (bit))) != 0)

#define LENGTH(p)\
  (sqrtf(LENGTH_SQR(p)))

#define LENGTH_SQR(p)\
  (SQR((p)[0]) + SQR((p)[1]) + SQR((p)[2]))

#define DISTANCE_XYZ(x, y ,z, q)\
  (sqrtf(DISTANCE_SQR_XYZ(p, q)))

#define DISTANCE_SQR_XYZ(x, y, z, q)\
  (SQR(x-(q)[0]) + SQR(y-(q)[1]) + SQR(z-(q)[2]))

#define DOT_PRODUCT(p, q)\
  ((p)[0]*(q)[0] + (p)[1]*(q)[1] + (p)[2]*(q)[2])

#define DOT_PRODUCT_XYZ(x, y, z, q)\
  ((x)*(q)[0] + (y)*(q)[1] + (z)*(q)[2])

#define NORMAL(v, p0, p1, p2)\
  (void)(\
    (v)[0] = ((p1)[1] - (p0)[1]) * ((p2)[2] - (p0)[2]) -\
             ((p1)[2] - (p0)[2]) * ((p2)[1] - (p0)[1]),\
    (v)[1] = ((p1)[2] - (p0)[2]) * ((p2)[0] - (p0)[0]) -\
             ((p1)[0] - (p0)[0]) * ((p2)[2] - (p0)[2]),\
    (v)[2] = ((p1)[0] - (p0)[0]) * ((p2)[1] - (p0)[1]) -\
             ((p1)[1] - (p0)[1]) * ((p2)[0] - (p0)[0])\
  )

#define SCALE(x, y, scale) \
  ((void)((x)[0] = ((y)[0]*scale), (x)[1] = ((y)[1]*scale), (x)[2] = ((y)[2]*scale)))

#define NORMALIZE(x) SCALE(x, x, 1.f/LENGTH(x))


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