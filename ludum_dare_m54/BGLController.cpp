
#include "BGLController.h"

//std::vector<BGLControllerState> BGLController::gameControllers;
std::unordered_map<int, BGLControllerState> BGLController::gameControllers;

void BGLController::AddController(int32_t id)
{

	BGLControllerState controller = {};

	if(SDL_IsGameController(id))
	{
		controller.controller = SDL_GameControllerOpen(id);
		if(controller.controller)
		{
			SDL_Joystick *joy = SDL_GameControllerGetJoystick(controller.controller);
			controller.joystickInstance = SDL_JoystickInstanceID(joy);
			
			gameControllers[controller.id];
		}
	}
}


void BGLController::RemoveController(int32_t id)
{

}


void BGLController::UpdateControllerButton(SDL_ControllerButtonEvent event)
{
	// NOTE(brett): What happens if this event has an incorrect 'which'??
	// perhaps I should check and make sure it exists.
	BGLControllerState *controller = &gameControllers[event.which];
	uint8_t state = (event.state == SDL_PRESSED) ? 1 : 0;

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
			controller->inputsStates.buttons[event.button] = state;
			break;
		}
	}

}


void BGLController::UpdateControllerAxis(SDL_ControllerAxisEvent event)
{
	BGLControllerState *controller = &gameControllers[event.which];
	
	float normalizedValue = event.value / CONTROLLER_AXIS_MAX;

	switch(event.axis)
	{
		case SDL_CONTROLLER_AXIS_INVALID:
		{
			std::cout << "Controller axis is invalid?" << std::endl;
			break;
		}
		default:
		{
			controller->inputsStates.axis[event.axis] = normalizedValue;
		}
	}

}

void BGLController::ResetControllerStates()
{
	// TODO(brett): get all the controllers and clear the pressed states
	//BGLControllerState controller = gameControllers[event.which];
}

BGLControllerState BGLController::GetControllerState()
{
	// TODO(brett): need to make a way to get multiple controllers... because
	// that makes this game multiplayer instantly

	return (*(gameControllers.begin())).second;
}
