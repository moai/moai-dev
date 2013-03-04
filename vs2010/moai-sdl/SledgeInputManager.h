#ifndef SDLINPUTMANAGER
#define SDLINPUTMANAGER

#include <string>
#include <cstdio>
#include <vector>

#include <SDL.h>
#define SDL_main main

#ifndef HAS_AKU
#define HAS_AKU
#include <aku/AKU.h>
#endif

#include "UtilityTypes.h"


//================================================================//
// SledgeInputManager
//================================================================//
/**	@name	SledgeInputManager
	@text	Input manager.
*/
class SledgeInputManager
{
private:
	buttonState ButtonState_Old;
	static float deadzone_thumbLeft;
	static float deadzone_thumbRight;
	static float deadzone_trigger;
	static const int LEFT_THUMB_DEADZONE = 7849;
	static const int RIGHT_THUMB_DEADZONE = 8689;
	static const int TRIGGER_THRESHOLD = 3855;
	static const int AXIS_MAX = 32767;

	//----------------------------------------------------------------//
	void 					doAKUDeviceInit			( SledgeInputDevice::InputDevice_ID p_id, char* p_devicename );
	void 					initDevice				( SledgeInputDevice::InputDevice_ID p_id );
	void 					initPad					( SledgeInputDevice::InputDevice_ID p_id );
	void 					initJoy					( SledgeInputDevice::InputDevice_ID p_id );
	void 					pollPadButtons			( SDL_GameController* p_controller, SledgeInputDevice::InputDevice_ID p_deviceid );
	void					pollJoyButtons			( SDL_Joystick* p_pad, SledgeInputDevice::InputDevice_ID p_deviceid );
	NormalizedController	postprocessController	( SDL_GameController* p_controller );
	void					postprocessJoystick		( NormalizedJoystick* p_nj, SDL_Joystick* p_stick, const int p_deadzone );
	vec2f					postprocessStick		( float p_x, float p_y, float p_deadzone );
	vec2f					postprocessThumbstick	( SDL_GameController* p_controller, SDL_CONTROLLER_AXIS p_axisX, SDL_CONTROLLER_AXIS p_axisY, const int p_deadzone );
	float					postprocessTrigger		( SDL_GameController* p_controller, SDL_CONTROLLER_AXIS p_axisT, const int p_deadzone );
	void					updateAKU_Controller	( SledgeInputDevice::InputDevice_ID p_deviceid, NormalizedController* p_nc	);
	void					updateAKU_Joystick		( SledgeInputDevice::InputDevice_ID p_deviceid, int p_stick_id, vec2f* p_stick );

protected:
	int num_joysticks_total;
	int num_joysticks_connected;
	int num_controllers_total;
	int num_controllers_connected;

public:
	std::vector<SDL_GameController*> controllers;
	std::vector<NormalizedController> controllers_normalized;
	std::vector<SDL_Joystick*> joysticks;
	std::vector<NormalizedJoystick> joysticks_normalized;

	//----------------------------------------------------------------//
			SledgeInputManager			();
			~SledgeInputManager			();
	void	doAKUInit					();	
	void	doOnTick					();
	void	inputNotify_onKeyDown		(SDL_KeyboardEvent* p_event);
	void	inputNotify_onMouseMove		(SDL_MouseMotionEvent* p_event);
	void	inputNotify_onMouseButton	(SDL_MouseButtonEvent* p_event);
	void	inputNotify_onPadAxisMove	(SDL_ControllerAxisEvent* p_event);
	void	setDeadzones				(float p_thumbLeft, float p_thumbRight, float p_trigger);
};

#endif