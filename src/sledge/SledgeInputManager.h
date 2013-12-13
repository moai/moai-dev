#include "stdafx.h"
#ifndef SDLINPUTMANAGER
#define SDLINPUTMANAGER

#include "UtilityTypes.h"


//================================================================//
// SledgeInputManager
//================================================================//
/**	@name	SledgeInputManager
	@text	HID manager class; handles gamepads, joysticks, keyboard and mouse
			input. Wrapped for Lua by SledgeInputWrapper.
*/
class SledgeInputManager
{
private:
	enum { BS_PING, BS_PONG };
	int pingpongSide;
	pingpongState buttonStates[4];

	pingpongState_keyb pp_keybState;

	float deadzone_thumbLeft;
	float deadzone_thumbRight;
	float deadzone_trigger;
	float deadzone_joystick;

	bool bHideCursorWhenInsideWindow;

	static const int LEFT_THUMB_DEADZONE = 7849;
	static const int RIGHT_THUMB_DEADZONE = 8689;
	static const int JOYSTICK_DEADZONE = 8269;
	static const int TRIGGER_THRESHOLD = 3855;
	static const int AXIS_MAX = 32767;

	static const int MAX_GAMECONTROLLERS = 4;
	static const int MAX_JOYSTICKS = 4;
	static const int MAX_JOYSTICK_STICKS = 2;

	//----------------------------------------------------------------//
	void 					doAKUDeviceInit			( SLEDGE_NAMESPACE::InputDeviceType_ID p_typeid, void* p_sledgedevice );
	void					doAKUPadInit			( int p_padcount );
	void					doAKUJoyInit			( int p_joycount );


	void 					initDevice				( SledgeDevice* p_sledgedevice );
	void 					initPad					( SledgeController* p_sledgecontroller );
	void 					initJoy					( SledgeJoystick* p_sledgejoystick );
	  
	vec2<f32>					postprocessStick		( float p_x, float p_y, float p_deadzone );
	vec2<f32>					postprocessThumbstick	( SDL_GameController* p_controller, SDL_GameControllerAxis p_axisX, SDL_GameControllerAxis p_axisY, const int p_deadzone );
	float					postprocessTrigger		( SDL_GameController* p_controller, SDL_GameControllerAxis p_axisT, const int p_deadzone );

	void					updateController		( SledgeController* p_sledgecontroller );
	void					updateJoystick			( SledgeJoystick* p_sledgejoystick );
	
	bool					connectController		( int idx_device, int idx_gamepad );
	bool					connectJoystick			( int idx_device, int idx_joystick );

protected: 
	int _numjoysticks_lasttick;
	int num_controllers_total;
	int num_controllers_connected;

	int num_devices_connected;

public:
	std::vector<SDL_GameController*> controllers;
	std::vector<NormalizedController> controllers_normalized;
	std::vector<SDL_Joystick*> joysticks;
	std::vector<NormalizedJoystick> joysticks_normalized;

	std::list<SledgeController> m_controllers;
	std::list<SledgeJoystick> m_joysticks;

	//----------------------------------------------------------------//
			SledgeInputManager			();
			~SledgeInputManager			();
	void	blankKeybFlags				();
	void	doAKUInit					();	
	void	doOnTick					();
	void	inputNotify_onKeyDown		(SDL_KeyboardEvent* p_event);
	void	inputNotify_onMouseMove		(SDL_MouseMotionEvent* p_event);
	void	inputNotify_onMouseButton	(SDL_MouseButtonEvent* p_event);
	void	setDeadzones				(float p_thumbLeft, float p_thumbRight, float p_trigger, float p_joystick);

	void	hideCursorInsideWindow		(bool bHide);
};

#endif