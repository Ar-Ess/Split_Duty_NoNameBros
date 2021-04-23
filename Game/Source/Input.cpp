#include "App.h"
#include "Input.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#define MAX_KEYS 300
#define MAX_GAMEPADS 2

Input::Input() : Module()
{
	name.Create("input");

	keyboard = new KeyState[MAX_KEYS];
	gamePad = new KeyState[MAX_GAMEPADS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);

	memset(&pads[0], 0, sizeof(GamePad) * MAX_PADS);
}

Input::~Input()
{
	delete[] keyboard;
}

bool Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Input::Start()
{
	SDL_StopTextInput();
	return true;
}

bool Input::PreUpdate()
{
	static SDL_Event event;

	UpdateKeyboardInput();

	UpdateMouseInput();

	UpdateGamepadsInput();

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case(SDL_CONTROLLERDEVICEADDED):
				HandleDeviceConnection(event.cdevice.which);
			break;

			case(SDL_CONTROLLERDEVICEREMOVED):
				HandleDeviceRemoval(event.cdevice.which);

			case SDL_MOUSEBUTTONDOWN:
				mouseButtons[event.button.button - 1] = KEY_DOWN;
				//LOG("Mouse button %d down", event.button.button-1);
			break;

			case SDL_MOUSEBUTTONUP:
				mouseButtons[event.button.button - 1] = KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
			break;

			case SDL_MOUSEMOTION:
				int scale = app->win->GetScale();
				mouseMotionX = event.motion.xrel / scale;
				mouseMotionY = event.motion.yrel / scale;
				mouseX = event.motion.x / scale;
				mouseY = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;
		}
	}

	return true;
}

void Input::UpdateKeyboardInput()
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1) // BUTTON PRESSED
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else // BUTTON RELEASED
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}
}

void Input::UpdateMouseInput()
{
	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouseButtons[i] == KEY_DOWN)
			mouseButtons[i] = KEY_REPEAT;

		if (mouseButtons[i] == KEY_UP)
			mouseButtons[i] = KEY_IDLE;
	}
}

void Input::UpdateGamepadsInput()
{
	// Iterate through all active gamepads and update all input data
	for (int i = 0; i < MAX_PADS; ++i)
	{
		GamePad& pad = pads[i];

		if (pad.enabled == true)
		{
			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_A)) ControllerPressedStateLogic(0);
			else ControllerReleasedStateLogic(0);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_B)) ControllerPressedStateLogic(1);
			else ControllerReleasedStateLogic(1);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_X)) ControllerPressedStateLogic(2);
			else ControllerReleasedStateLogic(2);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_Y)) ControllerPressedStateLogic(3);
			else ControllerReleasedStateLogic(3);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) ControllerPressedStateLogic(4);
			else ControllerReleasedStateLogic(4);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) ControllerPressedStateLogic(5);
			else ControllerReleasedStateLogic(5);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_LEFTSTICK)) ControllerPressedStateLogic(6);
			else ControllerReleasedStateLogic(6);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK)) ControllerPressedStateLogic(7);
			else ControllerReleasedStateLogic(7);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_DPAD_UP)) ControllerPressedStateLogic(8);
			else ControllerReleasedStateLogic(8);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) ControllerPressedStateLogic(9);
			else ControllerReleasedStateLogic(9);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) ControllerPressedStateLogic(10);
			else ControllerReleasedStateLogic(10);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) ControllerPressedStateLogic(11);
			else ControllerReleasedStateLogic(11);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_START)) ControllerPressedStateLogic(12);
			else ControllerReleasedStateLogic(12);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_GUIDE)) ControllerPressedStateLogic(13);
			else ControllerReleasedStateLogic(13);

			if (SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_BACK)) ControllerPressedStateLogic(14);
			else ControllerReleasedStateLogic(14);

			pad.l2 = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT)) / 32767.0f;
			pad.r2 = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) / 32767.0f;

			pad.l_x = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_LEFTX)) / 32767.0f;
			pad.l_y = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_LEFTY)) / 32767.0f;
			pad.r_x = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_RIGHTX)) / 32767.0f;
			pad.r_y = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_RIGHTY)) / 32767.0f;

			// Apply deadzone. All values below the deadzone will be discarded
			pad.l_x = (fabsf(pad.l_x) > pad.l_dz) ? pad.l_x : 0.0f;
			pad.l_y = (fabsf(pad.l_y) > pad.l_dz) ? pad.l_y : 0.0f;
			pad.r_x = (fabsf(pad.r_x) > pad.r_dz) ? pad.r_x : 0.0f;
			pad.r_y = (fabsf(pad.r_y) > pad.r_dz) ? pad.r_y : 0.0f;

			if (pad.rumble_countdown > 0)
				pad.rumble_countdown--;
		}
	}
}

void Input::ControllerPressedStateLogic(int id)
{
	if (gamePad[id] == KEY_IDLE)
		gamePad[id] = KEY_DOWN;
	else
		gamePad[id] = KEY_REPEAT;
}

void Input::ControllerReleasedStateLogic(int id)
{
	if (gamePad[id] == KEY_REPEAT || gamePad[id] == KEY_DOWN)
		gamePad[id] = KEY_UP;
	else
		gamePad[id] = KEY_IDLE;
}

bool Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	for (uint i = 0; i < MAX_PADS; ++i)
	{
		if (pads[i].haptic != nullptr)
		{
			SDL_HapticStopAll(pads[i].haptic);
			SDL_HapticClose(pads[i].haptic);
		}
		if (pads[i].controller != nullptr) SDL_GameControllerClose(pads[i].controller);
	}

	SDL_QuitSubSystem(SDL_INIT_HAPTIC);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}

bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

void Input::GetMousePosition(int& x, int& y)
{
	x = mouseX;
	y = mouseY;
}

void Input::GetMouseMotion(int& x, int& y)
{
	x = mouseMotionX;
	y = mouseMotionY;
}

// CONTROLLER
void Input::HandleDeviceConnection(int index)
{
	if (SDL_IsGameController(index))
	{
		for (int i = 0; i < MAX_PADS; ++i)
		{
			GamePad& pad = pads[i];

			if (pad.enabled == false)
			{
				if (pad.controller = SDL_GameControllerOpen(index))
				{
					LOG("Found a gamepad with id %i named %s", i, SDL_GameControllerName(pad.controller));
					pad.enabled = true;
					pad.l_dz = pad.r_dz = 0.1f;
					pad.haptic = SDL_HapticOpen(index);
					if (pad.haptic != nullptr)
						LOG("... gamepad has force feedback capabilities");
					pad.index = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(pad.controller));
				}
			}
		}
	}
}

void Input::HandleDeviceRemoval(int index)
{
	// If an existing gamepad has the given index, deactivate all SDL device functionallity
	for (int i = 0; i < MAX_PADS; ++i)
	{
		GamePad& pad = pads[i];
		if (pad.enabled && pad.index == index)
		{
			SDL_HapticClose(pad.haptic);
			SDL_GameControllerClose(pad.controller);
			memset(&pad, 0, sizeof(GamePad));
		}
	}
}

bool Input::ShakeController(int id, int duration, float strength)
{
	bool ret = false;

	// Check if the given id is valid within the array bounds
	if (id < 0 || id >= MAX_PADS) return ret;

	// Check if the gamepad is active and allows rumble
	GamePad& pad = pads[id];
	if (!pad.enabled || pad.haptic == nullptr || SDL_HapticRumbleSupported(pad.haptic) != SDL_TRUE) return ret;

	// If the pad is already in rumble state and the new strength is below the current value, ignore this call
	if (duration < pad.rumble_countdown && strength < pad.rumble_strength)
		return ret;

	if (SDL_HapticRumbleInit(pad.haptic) == -1)
	{
		LOG("Cannot init rumble for controller number %d", id);
	}
	else
	{
		SDL_HapticRumbleStop(pad.haptic);
		SDL_HapticRumblePlay(pad.haptic, strength, duration / 60 * 1000); //Conversion from frames to ms at 60FPS

		pad.rumble_countdown = duration;
		pad.rumble_strength = strength;

		ret = true;
	}

	return ret;
}

const char* Input::GetControllerName(int id) const
{
	// Check if the given id has a valid controller
	if (id >= 0 && id < MAX_PADS && pads[id].enabled == true && pads[id].controller != nullptr)
		return SDL_GameControllerName(pads[id].controller);

	return "unplugged";
}