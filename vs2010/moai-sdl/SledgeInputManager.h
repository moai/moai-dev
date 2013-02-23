#ifndef SDLINPUTMANAGER
#define SDLINPUTMANAGER

#include <string>
#include <cstdio>
#include <vector>

#include <SDL.h>
#define SDL_main main

//#include <moaicore/MOAILuaObject.h>


#ifndef HAS_AKU
#define HAS_AKU
#include <aku/AKU.h>
#endif

#include "UtilityTypes.h"





class SledgeInputManager
{
public:
	SledgeInputManager();
	~SledgeInputManager();

	std::vector<SDL_GameController*> controllers;
	std::vector<NormalizedController> controllers_normalized;

	/** Perform all AKU-related input initialization.
	 */
	void doAKUInit();
	
	void doOnTick();

	void inputNotify_onMouseMove(SDL_MouseMotionEvent* p_event);
	void inputNotify_onMouseButton(SDL_MouseButtonEvent* p_event);

	void inputNotify_onPadAxisMove(SDL_ControllerAxisEvent* p_event);
	void setDeadzones(float p_thumbLeft, float p_thumbRight, float p_trigger);
protected:
	int num_joysticks;
private:
	/** Initialize a given device, whether a device (mouse+keyboard) or a pad.
	 */
	void doAKUDeviceInit(
		SledgeInputDevice::InputDevice_ID p_id
	);
	void initDevice(
		SledgeInputDevice::InputDevice_ID p_id
	);
	void initPad(
		SledgeInputDevice::InputDevice_ID p_id
	);

	/** Post-process a thumbstick, returning a normalized vec2f.
	 */
	vec2f postprocessThumbstick(
		SDL_GameController* p_controller,
		SDL_CONTROLLER_AXIS p_axisX,
		SDL_CONTROLLER_AXIS p_axisY,
		const int p_deadzone
	);

	/** Post-process a trigger, returning a normalized float.
	 */
	float postprocessTrigger(
		SDL_GameController* p_controller,
		SDL_CONTROLLER_AXIS p_axisT,
		const int p_deadzone
	);

	NormalizedController postprocessController(
		SDL_GameController* p_controller
	);

	void updateAKU_Controller(
		SledgeInputDevice::InputDevice_ID p_deviceid,
		NormalizedController* p_nc
	);


	static float deadzone_thumbLeft;
	static float deadzone_thumbRight;
	static float deadzone_trigger;

	static const int LEFT_THUMB_DEADZONE = 7849;
	static const int RIGHT_THUMB_DEADZONE = 8689;
	static const int TRIGGER_THRESHOLD = 3855;

	static const int AXIS_MAX = 32767;

	//static const int AKU_SCALING_FACTOR = 10000;
};

#endif