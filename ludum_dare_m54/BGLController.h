#pragma once

#include "language.h"

#include <SDL.h>
#include <unordered_map>

#include <cmath>

#define CONTROLLER_AXIS_MAX 32767.0f
#define CONTROLLER_HOOKUP_MAX 16
#define PRESSED_BUTTON_MAX SDL_CONTROLLER_BUTTON_MAX + sizeof(KeyboardInputs)/sizeof(BGLButtonState)

struct BGLButtonState
{
	bool down;
	bool pressed;
};

typedef struct
{

	// the controller id from sdl
	int32_t cid;
	SDL_GameController *sdlGameController;

	struct
	{
		union
		{
			struct
			{
				BGLButtonState A;
				BGLButtonState B;
				BGLButtonState X;
				BGLButtonState Y;
				BGLButtonState BACK;
				BGLButtonState GUIDE;
				BGLButtonState START;
				BGLButtonState LSTICK;
				BGLButtonState RSTICK;
				BGLButtonState LSHOULDER;
				BGLButtonState RSHOULDER;
				BGLButtonState UP;
				BGLButtonState DOWN;
				BGLButtonState LEFT;
				BGLButtonState RIGHT;
			};
			BGLButtonState buttons[SDL_CONTROLLER_BUTTON_MAX];	
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
	};
} ControllerInputs;


typedef struct
{
	// as per the SDL2 docs the sdl_scancodes are from 0-127 (128)
	// then 1073741881 to 1073742106 (225 => 226); (128 + 226 => 354)
	BGLButtonState keys[354];

} KeyboardInputs;


typedef struct
{


} MouseInputs;

/**
 This is the aggregate object for a bglcontroller which is everytype of input and their states
 mouse/keyboard/controller
 */
typedef struct
{
	uint8_t controllerSz;
	ControllerInputs controllers[CONTROLLER_HOOKUP_MAX];
	KeyboardInputs keyboard;
	MouseInputs mouse;
} BGLInputState;


/**
	This is the controller for managing the system inputs. The platform will use this to 
	update the saved controller state
*/

class BGLController
{
public:
	static void AddController(int32_t id);
	static void RemoveController(int32_t id);

	static void UpdateControllerButton(SDL_ControllerButtonEvent event);
	static void UpdateControllerAxis(SDL_ControllerAxisEvent event);

	static void UpdateKeyboardButton(SDL_KeyboardEvent event);
	static BGLButtonState GetKey(SDL_Keycode key);
	static void UpdateMouseButton(SDL_MouseButtonEvent event);
	static void UpdateMouseMotion(SDL_MouseMotionEvent event);


	static void FrameClean();

	// Returns a copy of the current system input
	static BGLInputState GetInputState();

private:
	static BGLInputState SystemInputs;
	static BGLButtonState *pressedButtons[PRESSED_BUTTON_MAX];
	static uint32_t pressedButtonSz;

};