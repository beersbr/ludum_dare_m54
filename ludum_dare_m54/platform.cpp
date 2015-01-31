
#include <GL/glew.h>
#include "platformdef.h"
#include "BGL.h"
#include "BGLController.h"

internal void
PlatformCreateWindow(PlatformWindow *platformWindow, char *title, int32_t width, int32_t height, bool vsync = false)
{

	platformWindow->width = width;
	platformWindow->height = height;
	platformWindow->vsync = vsync;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// not sure if i need this because we are doing a 2d game. (maybe just not as big)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


	platformWindow->isValid = true;
	platformWindow->sdlWindow = SDL_CreateWindow(title,
												SDL_WINDOWPOS_UNDEFINED,
												SDL_WINDOWPOS_UNDEFINED,
												width,
												height,
												SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(!platformWindow->sdlWindow)
	{
		std::cout << "Could not create the SDL window" << std::endl;
		platformWindow->isValid = false;
		return;
	}


	platformWindow->glContext = SDL_GL_CreateContext(platformWindow->sdlWindow);

	if(!platformWindow->glContext)
	{
		std::cout << "Could not create the SDL window GL context" << std::endl;
		platformWindow->isValid = false;
		return;
	}

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cout << "Could not initialize GLEW for the opengl extensions. Stopping." << std::endl;
		exit(1);
	}


	SDL_GL_SetSwapInterval(platformWindow->vsync);
}



int main(int argc, char *argv[])
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Could not initialize SDL2. Stopping." << std::endl;
		exit(1);
	}

	
	if(IMG_Init(IMG_INIT_PNG) == 0)
	{
		std::cout << "SDL2 Image library failed to initialize. Stopping." << std::endl;
		exit(1);
	}

	PlatformWindow Window = {};
	PlatformCreateWindow(&Window, "mini ld 54", 1200, 800); 

	if(!Window.isValid)
	{
		std::cout << "Could not inialize PlatformWindow. Stopping." << std::endl;
		exit(1);
	}

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set screen clear color to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	InitializeFrame(0, Window.width, Window.height, 0);

	ShaderHandler::Load("sprite", "shaders/sprite.vertex", "shaders/sprite.fragment");

	TextureHandler::Load("diffuse", "./images/diffuse.png");
	TextureHandler::Load("normal", "./images/normal.png");

	Sprite TestSprite = Sprite::Create("diffuse", "normal", 100, 100, 0, 1, &(BGLRectMake(0, 0, 64, 64)));

	uint64_t LastTick = SDL_GetTicks();
	uint64_t CurrentTick = SDL_GetTicks();
	SDL_Event event;



	bool Running = true;
	while(Running)
	{
		LastTick = CurrentTick;
		CurrentTick = SDL_GetTicks();

		// NOTE(brett): ElapsedTick is in ms
		uint64_t ElapsedTick = CurrentTick - LastTick;

		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
				{
					break;
				}
				case SDL_KEYUP:
				{
					break;
				}
				case SDL_MOUSEMOTION:
				{
					break;
				}
				case SDL_CONTROLLERBUTTONDOWN:
				{
					BGLController::UpdateControllerButton(event.cbutton);
					break;
				}
				case SDL_CONTROLLERBUTTONUP:
				{
					BGLController::UpdateControllerButton(event.cbutton);
					break;
				}
				case SDL_CONTROLLERAXISMOTION:
				{
					BGLController::UpdateControllerAxis(event.caxis);
					break;
				}
				case SDL_CONTROLLERDEVICEADDED:
				{
					std::cout << "Controller added: " << event.cdevice.which << std::endl;
					BGLController::AddController(event.cdevice.which);
					break;
				}
				case SDL_CONTROLLERDEVICEREMOVED:
				{
					std::cout << "Controller removed" << std::endl;

					/*
						TODO(brett): this is for when I feel like doing cleanup
						SDL_GameController *pad = YOUR_FUNCTION_THAT_RETRIEVES_A_MAPPING( id );
						SDL_GameControllerClose( pad );
					*/

					break;
				}
				case SDL_QUIT:
				{
					Running = false;
					break;
				}
				default:
				{
					break;
				}
			}
		}

		// NOTE(brett): This doesnt need to happen if we are covering the entire screen something
		glClear(GL_COLOR_BUFFER_BIT);

		BGLControllerState Controller = BGLController::GetControllerState();

		if(Controller.inputsStates.A)
		{
			std::cout << SDL_GetTicks() << "  Pressing the A button..." << std::endl;
		}

		if(Controller.inputsStates.B)
		{
			//std::cout << "Pressing the B button..." << std::endl;
			std::cout << "Left Stick: " << Controller.inputsStates.LX  << ", " << Controller.inputsStates.LY << std::endl;
		}
		

		// Update and Render
		TestSprite.Render();

		SDL_GL_SwapWindow(Window.sdlWindow);


	}

	return 0;

}