#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
#define MAX_PADS 1
//#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;
struct _SDL_GameController;
struct _SDL_Haptic;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum GamePadInput
{
	A = 0,
	B,
	X,
	Y,
	L1,
	R1,
	L3,
	R3,
	UP_PAD,
	DOWN_PAD,
	LEFT_PAD,
	RIGHT_PAD,
	START,
	BACK,
	GUIDE,
};

struct GamePad
{
	//Input data
	//bool start, back, guide;
	//bool x, y, a, b, l1, r1, l3, r3;
	//bool up, down, left, right;
	float l2, r2;
	float l_x, l_y, r_x, r_y, l_dz, r_dz;

	//Controller data
	bool enabled;
	int index;
	_SDL_GameController* controller = nullptr;
	_SDL_Haptic* haptic = nullptr;

	//Rumble controller
	int rumble_countdown;
	float rumble_strength;
};

class Input : public Module
{

public:

	Input();

	virtual ~Input();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	void UpdateKeyboardInput();

	void UpdateMouseInput();

	void UpdateGamepadsInput();

	bool CleanUp();

	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetControl(int id) const
	{
		return gamePad[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	void ControllerPressedStateLogic(int id);

	void ControllerReleasedStateLogic(int id);

	bool GetWindowEvent(EventWindow ev);

	void GetMousePosition(int &x, int &y);

	void GetMouseMotion(int& x, int& y);

	void HandleDeviceConnection(int index);

	void HandleDeviceRemoval(int index);

	bool ShakeController(int id, int duration, float strength = 0.5f);

	const char* GetControllerName(int id) const;

private:

	bool windowEvents[WE_COUNT];

	KeyState* keyboard = nullptr;
	
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	GamePad pads[MAX_PADS];

	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;
public:
	KeyState* gamePad = nullptr;
};

#endif // __INPUT_H__