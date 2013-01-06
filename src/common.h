
#ifndef COMMON_H
#define COMMON_H


#ifdef _WIN32

////////////// Allow code to compile cleanly at warning level 4 ///////////////


/* nonstandard extension 'single line comment' was used */
#pragma warning(disable:4001)

// unreferenced formal parameter
#pragma warning(disable:4100)

// Note: Creating precompiled header 
#pragma warning(disable:4699)

// function not inlined
#pragma warning(disable:4710)

// unreferenced inline function has been removed
#pragma warning(disable:4514)

// assignment operator could not be generated
#pragma warning(disable:4512)

// cast truncates constant value
#pragma warning(disable:4310)

// identifier was truncated to '255' characters in the debug information
#pragma warning(disable:4786)


#include <windows.h>
#endif //_WIN32

#include <GL/gl.h>
#include <GL/glu.h>
//#include <SDL.h>
#include <math.h>

#ifndef M_PI
#define M_PI            3.14159265358979323846 
#endif //M_PI

#ifndef M_PI2
#define M_PI2 6.283185307179586476925286766559
#endif //M_PI2

#define ALMOST_ZERO(x) ((x<0.0f) ? x > 1e-4f : x < 1e-4f)

extern const char* g_AppName;

#endif //COMMON_H

