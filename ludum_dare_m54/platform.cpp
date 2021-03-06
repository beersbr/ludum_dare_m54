
#include <chrono>
#include <GL/glew.h>
#include "platformdef.h"
#include "BGL.h"
#include "BGLController.h"
#include "BGLAudio.h"
#include "Game.h"

bglinternal void
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


	std::cout << "VSYNC: " << platformWindow->vsync << std::endl;
	SDL_GL_SetSwapInterval(platformWindow->vsync);
}

int main(int argc, char *argv[])
{
	std::cout << "starting main" << std::endl;

	if(SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO) != 0)
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
	PlatformCreateWindow(&Window, "Mini LD 54", 1200, 800, true); 

	if(!Window.isValid)
	{
		std::cout << "Could not inialize PlatformWindow. Stopping." << std::endl;
		exit(1);
	}

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH);

	// Set screen clear color to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);


	if(!BGLAudio::Initialize())
	{
		std::cout << "Could not initialize SDL2 AUDIO via BGLAudio. Stopping." << std::endl;
		exit(1);
	}
	BGLAudio::SetVolume(MIX_MAX_VOLUME/3);

	InitializeFrame(0.0f, (float)Window.width, (float)Window.height, 0.0f);

    std::vector<std::pair<std::string, std::string>> resFileList;
    std::vector<std::pair<std::string, std::string>> textureFileList;

    resFileList.push_back(std::pair<std::string, std::string>("PlayerSheet", "./json/PlayerSheet.json"));
    resFileList.push_back(std::pair<std::string, std::string>("map01", "./json/maps/map01.json"));
    resFileList.push_back(std::pair<std::string, std::string>("monsterOne", "./json/enemies/monsterOne.json"));
    resFileList.push_back(std::pair<std::string, std::string>("tileOne", "./json/tiles/tile01.json"));
    textureFileList.push_back(std::pair<std::string, std::string>("masterSprite", "./images/ship_spritesheet.png"));


	Game* curGame = new Game();

    //Game* curGame = new Game(resFileList, textureFileList);
    //curGame->startGame("map01");


	uint64_t LastTick = SDL_GetTicks();
	uint64_t CurrentTick = SDL_GetTicks();
	float GameUpdateHz = 1000/60.0f;
	float CycleTime = 0.0f;

	int32_t frames = 0;
	int32_t secondSum = 0;
	int32_t offsetTime = 0;

	SDL_Event event;

	bool Running = true;
	while(Running)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_WINDOWEVENT:
				{
					switch(event.window.event)
					{
						case SDL_WINDOWEVENT_FOCUS_GAINED:
						{
							
						}
						case SDL_WINDOWEVENT_FOCUS_LOST:
						{
							
						}
						case SDL_WINDOWEVENT_MOVED:
						{
							//TODO(brett): this should only be done if vsync is active
							CurrentTick = SDL_GetTicks();
						}
						case SDL_WINDOWEVENT_ENTER:
						{
							
						}
						case SDL_WINDOWEVENT_LEAVE:
						{
							
						}
					}
				}

				case SDL_KEYDOWN:
				{
					BGLController::UpdateKeyboardButton(event.key);
					break;
				}
				case SDL_KEYUP:
				{
					BGLController::UpdateKeyboardButton(event.key);
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

		LastTick = CurrentTick;
		CurrentTick = SDL_GetTicks();

		// NOTE(brett): ElapsedTick is in ms
		uint64_t ElapsedTick = CurrentTick - LastTick;
		CycleTime += ElapsedTick;

		// NOTE(brett): This doesnt need to happen if we are covering the entire screen something

		float dt = ElapsedTick/1000.0f;

		// Update and Render
         curGame->update(frames, dt);

		BGLController::FrameClean();

		// NOTE(brett): This is blocking with vsync on.
		SDL_GL_SwapWindow(Window.sdlWindow);

		frames += 1;
		secondSum += ElapsedTick;
		if(secondSum >= 1000)
		{
			//std::cout << "FPS: " << frames/(float)secondSum * 1000 << std::endl;
			//std::cout << "AVG t/frmame: " << secondSum/1000.0f / frames << std::endl;

			secondSum -= 1000;
			frames = 0;
		}

		if(!Running)
			curGame->cleanup();

	}

	BGLAudio::Cleanup();

	return 0;

}