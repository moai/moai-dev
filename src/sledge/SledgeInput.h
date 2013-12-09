#include "stdafx.h"

#ifndef SLEDGEINPUT_H
#define SLEDGEINPUT_H


#include <moaicore/MOAIGlobals.h>
#include <moaicore/MOAIRtti.h>
#include <moaicore/MOAILua.h>

// need a way to map "virtual keys" to actual... keys.


//============================================================================//
// SledgeInput
//============================================================================//
class SledgeInput :
	public MOAIGlobalClass <SledgeInput, MOAILuaObject> {
private:
	static float deadzone_thumbLeft;
	static float deadzone_thumbRight;
	static float deadzone_trigger;
	static float deadzone_joystick;
protected: 
	int num_joysticks_lasttick;
	int num_controllers_total;
	int num_controllers_connected;
	int num_devices_connected;
public:
	DECL_LUA_SINGLETON ( SledgeInput )

	static const int LEFT_THUMB_DEADZONE = 7849;
	static const int RIGHT_THUMB_DEADZONE = 8689;
	static const int JOYSTICK_DEADZONE = 8269;
	static const int TRIGGER_THRESHOLD = 3855;
	static const int AXIS_MAX = 32767;

	static const int MAX_GAMECONTROLLERS = 4;
	static const int MAX_JOYSTICKS = 4;
	static const int MAX_JOYSTICK_STICKS = 2;

	std::vector<SDL_GameController*> controllers;
	std::vector<NormalizedController> controllers_normalized;
	std::vector<SDL_Joystick*> joysticks;
	std::vector<NormalizedJoystick> joysticks_normalized;

	std::list<SledgeController> m_controllers;
	std::list<SledgeJoystick> m_joysticks;
			
					SledgeInput				();
					~SledgeInput			();
	//----------------------------------------------------------------//
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			AKUInit					();

	//----------------------------------------------------------------//
	void			initDevice				( SledgeDevice* p_sledgedevice );
	void			doAKUPadInit			( int p_padcount );
	void			doAKUJoyInit			( int p_joycount );
	void			setDeadzones			( float p_thumbLeft, float p_thumbRight, float p_trigger, float p_joystick );

	bool			connectController		( int idx_device, int idx_gamepad );
	bool			connectJoystick			( int idx_device, int idx_joystick );

	//----------------------------------------------------------------//
	static int		_getNameForKeybScancode	( lua_State* L );
};


#endif