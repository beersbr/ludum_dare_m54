
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
			
			//gameControllers.push_back(controller);
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
			//std::cout << "SDL BUTTON: " << (int)(char)event.button << std::endl;
			controller->inputsStates.buttons[event.button] = state;
			break;
		}

		//case SDL_CONTROLLER_BUTTON_A:
		//{	
		//	controller->inputsStates.A = state;

		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_B:
		//{	
		//	controller->inputsStates.B = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_X:
		//{	
		//	controller.inputsStates.X = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_Y:
		//{	
		//	controller.inputsStates.Y = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_BACK:
		//{	
		//	controller.inputsStates.BACK = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_GUIDE:
		//{	
		//	controller.inputsStates.GUIDE = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_START:
		//{	
		//	controller.inputsStates.START = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_LEFTSTICK:
		//{	
		//	controller.inputsStates.LSTICK = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
		//{	
		//	controller.inputsStates.RSTICK = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
		//{	
		//	controller.inputsStates.LSHOULDER = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		//{	
		//	controller.inputsStates.RSHOULDER = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_DPAD_UP:
		//{	
		//	controller.inputsStates.UP = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		//{	
		//	controller.inputsStates.DOWN = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		//{	
		//	controller.inputsStates.LEFT = state;
		//	break;
		//}
		//case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
		//{
		//	controller.inputsStates.RIGHT = state;
		//	break;
		//}
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

		//case SDL_CONTROLLER_AXIS_LEFTX:
		//{
		//	controller.inputsStates.LX = normalizedValue;
		//	break;
		//}
		//case SDL_CONTROLLER_AXIS_LEFTY:
		//{
		//	controller.inputsStates.LY = normalizedValue;
		//	break;
		//}
		//case SDL_CONTROLLER_AXIS_RIGHTX:
		//{
		//	controller.inputsStates.RX = normalizedValue;
		//	break;
		//}
		//case SDL_CONTROLLER_AXIS_RIGHTY:
		//{
		//	controller.inputsStates.RY = normalizedValue;
		//	break;
		//}
		//case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
		//{
		//	controller.inputsStates.LTRIG = normalizedValue;
		//	break;
		//}
		//case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
		//{
		//	controller.inputsStates.RTRIG = normalizedValue;
		//	break;
		//}
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
