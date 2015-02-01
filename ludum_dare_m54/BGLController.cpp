
#include "BGLController.h"


BGLInputState BGLController::SystemInputs = {};
BGLButtonState *BGLController::pressedButtons[PRESSED_BUTTON_MAX];
uint32_t BGLController::pressedButtonSz = 0;

void BGLController::AddController(int32_t id)
{
	if(SystemInputs.controllerSz >= CONTROLLER_HOOKUP_MAX)
	{
		std::cout << "WARNING: Not tracking that " << CONTROLLER_HOOKUP_MAX + 1 << "th controller" << std::endl;
		return;
	}

	std::cout << "Attempint to add game controller with ID: " << id << std::endl;

	if(SDL_IsGameController(id))
	{
		SDL_GameController *newGameController = SDL_GameControllerOpen(id);
		if(newGameController)
		{
			SDL_Joystick *joy = SDL_GameControllerGetJoystick(newGameController);

			//NOTE(brett): Not sure we will need this as the controller seems to have it all
			uint32_t joysticInstanceID = SDL_JoystickInstanceID(joy);
			
			std::cout << "Controller ID matches count? " << (bool)(SystemInputs.controllerSz == id) << std::endl;
			
			// NOTE(brett): this needs to be here so we can clean up later... though right now we
			// are single threaded. I'm pretty sure it will clean up itself
			SystemInputs.controllers[SystemInputs.controllerSz].cid = id;
			SystemInputs.controllers[SystemInputs.controllerSz].sdlGameController = newGameController;

			SystemInputs.controllerSz += 1;

			std::cout << "Controller added: " << id << std::endl;
		}
	}
}


void BGLController::RemoveController(int32_t id)
{
	for(int i = 0; i < SystemInputs.controllerSz; i++)
	{
		SDL_GameControllerClose(SystemInputs.controllers[id].sdlGameController);
	}
}


void BGLController::UpdateControllerButton(SDL_ControllerButtonEvent event)
{
	// NOTE(brett): What happens if this event has an incorrect 'which'??
	// perhaps I should check and make sure it exists.

	// NOTE(brett): Controller Ids start at 0
	if(event.which >= SystemInputs.controllerSz)
	{
		std::cout << "ERROR: Fuck, the ids are NON sequencial which means I need a lookup table" << std::endl;
	}

	ControllerInputs *inputs = &(SystemInputs.controllers[event.which]);
	bool state = (event.state == SDL_PRESSED) ? 1 : 0;

	// NOTE(brett): I think I can probably do this with a cleverly constructed array
	switch(event.button)
	{
		case SDL_CONTROLLER_BUTTON_INVALID:
		{
			std::cout << "Button recvd was invalid." << std::endl;
			break;
		}
		default:
		{
			inputs->buttons[event.button].down = state;
			if(!state)
			{
				inputs->buttons[event.button].pressed = true;
				pressedButtons[pressedButtonSz] = &inputs->buttons[event.button];
			}

			break;
		}
	}
}

void BGLController::UpdateControllerAxis(SDL_ControllerAxisEvent event)
{
	ControllerInputs *inputs = &(SystemInputs.controllers[event.which]);
	
	float normalizedValue = event.value / CONTROLLER_AXIS_MAX;
	normalizedValue = (fabsf(normalizedValue) < 0.15f) ? 0.0f : normalizedValue;

	switch(event.axis)
	{
		case SDL_CONTROLLER_AXIS_INVALID:
		{
			std::cout << "Controller axis is invalid?" << std::endl;
			break;
		}
		default:
		{
			inputs->axis[event.axis] = normalizedValue;
		}
	}
}

void BGLController::UpdateKeyboardButton(SDL_KeyboardEvent event)
{
	KeyboardInputs *inputs = &SystemInputs.keyboard;

	int32_t offset = 0;
	bool state = (event.state == SDL_PRESSED);
	if(event.keysym.sym >= SDLK_CAPSLOCK)
	{
		offset += SDLK_CAPSLOCK;
	}

	inputs->keys[event.keysym.sym - offset].down = state;

	// Not pressed anymore
	if(!state)
	{
		inputs->keys[event.keysym.sym - offset].pressed = true;
		pressedButtons[pressedButtonSz] = &inputs->keys[event.keysym.sym-offset];
	}
}


BGLButtonState BGLController::GetKey(SDL_Keycode key)
{
	KeyboardInputs *inputs = &SystemInputs.keyboard;

	int32_t offset = 0;
	if(key >= SDLK_CAPSLOCK)
	{
		offset += SDLK_CAPSLOCK;
	}

	return(inputs->keys[key - offset]);
}


void BGLController::UpdateMouseButton(SDL_MouseButtonEvent event)
{

}

void BGLController::UpdateMouseMotion(SDL_MouseMotionEvent event)
{

}

void BGLController::FrameClean()
{
	for(int i = 0; i < pressedButtonSz; ++i)
	{
		pressedButtons[i]->pressed = false;
	}
}


BGLInputState BGLController::GetInputState()
{
	return SystemInputs;
}
