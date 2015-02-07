#pragma once

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <GL\glew.h>

#include "language.h"

typedef struct
{
	SDL_Window *sdlWindow;
	SDL_GLContext glContext;
	int32_t width;
	int32_t height;
	bool vsync;
	bool isValid;
} PlatformWindow;


#define SharedLibrarySearchPathFunction(name) void name(char const *searchPath)
typedef SharedLibrarySearchPathFunction(SetSharedLibraryPathFn);


SharedLibrarySearchPathFunction(SetSharedLibraryPathStub)
{
	std::cout << "WARNING: Not setting any special shared library path" << std::endl;
}



#ifdef __WIN32__
#include <Windows.h>
SharedLibrarySearchPathFunction(WIN32_SetSharedLibraryPath)
{
	SetDllDirectoryA(searchPath);
}

global
SetSharedLibraryPathFn *SetSharedLibPath = &WIN32_SetSharedLibraryPath;
#endif