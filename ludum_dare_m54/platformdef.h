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