#pragma once

#include "language.h"

#include <SDL.h>
#include <unordered_map>

#define CONTROLLER_AXIS_MAX 32767.0f

typedef struct
{
	union
	{
		struct
		{
			uint8_t A;
			uint8_t B;
			uint8_t X;
			uint8_t Y;
			uint8_t BACK;
			uint8_t GUIDE;
			uint8_t START;
			uint8_t LSTICK;
			uint8_t RSTICK;
			uint8_t LSHOULDER;
			uint8_t RSHOULDER;
			uint8_t UP;
			uint8_t DOWN;
			uint8_t LEFT;
			uint8_t RIGHT;
		};
		uint8_t buttons[SDL_CONTROLLER_BUTTON_MAX];	
	};
	
	union
	{
		struct
		{
			float LX;
			float LY;
			float RX;
			float RY;
			float LTRIG;
			float RTRIG;
		};
		float axis[SDL_CONTROLLER_AXIS_MAX];	
	};

} ControllerInputs;

typedef struct
{
	// the id that sdl gave us to reference the controller
	int32_t id;

	// the sdl controller instance
	SDL_GameController *controller;

	// the joystick instance that sdl gave us
	int32_t joystickInstance;

	ControllerInputs inputsStates;
	//ControllerInputs pressed;

} BGLControllerState;

class BGLController
{
public:
	static void AddController(int32_t id);
	static void RemoveController(int32_t id);
	static void UpdateControllerButton(SDL_ControllerButtonEvent event);
	static void UpdateControllerAxis(SDL_ControllerAxisEvent event);
	static void ResetControllerStates();
	static BGLControllerState GetControllerState();

public:
	static std::unordered_map<int, BGLControllerState> gameControllers;
	//static std::vector<BGLControllerState> gameControllers;

};