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





#ifndef SDLINPUTTYPES
#define SDLINPUTTYPES
namespace SledgeInputDeviceType {
	enum InputDeviceType_ID {
		IDT_DEVICE,
		IDT_PAD
	};
}

namespace SledgeInputDevice {
	enum InputDevice_ID {
		ID_DEVICE,
		ID_PAD_0,
		ID_PAD_1,
		ID_PAD_2,
		ID_PAD_3,
		ID_TOTAL
	};
	const static char* DeviceName[] = {
		"device",
		"pad0",
		"pad1",
		"pad2",
		"pad3"
	};
	const static SledgeInputDeviceType::InputDeviceType_ID DeviceType[] = {		
		SledgeInputDeviceType::IDT_DEVICE,
		SledgeInputDeviceType::IDT_PAD,
		SledgeInputDeviceType::IDT_PAD,
		SledgeInputDeviceType::IDT_PAD,
		SledgeInputDeviceType::IDT_PAD
	};
}

namespace SledgeDeviceSensor {
	enum DeviceSensor_ID {
		IDS_KEYBOARD,
		IDS_POINTER,
		IDS_MOUSE_LEFT,
		IDS_MOUSE_MIDDLE,
		IDS_MOUSE_RIGHT,
		IDS_TOTAL
	};
	const static char* SensorName[] = {
		"keyboard",
		"pointer",
		"mouseLeft",
		"mouseMiddle",
		"mouseRight"
	};
}

namespace SledgePadSensor
{
	enum {
		PS_STICK_LEFT,
		PS_STICK_RIGHT,
		PS_TRIGGERS,
		PS_TOTAL
	};
	const static char* SensorName[] = {
		"stickLeft",
		"stickRight",
		"triggers"
	};
}

namespace JethaSDLControllerAxis
{
	const static char* AxisName[] = {
		"Left X",
		"Left Y",
		"Right X",
		"Right Y",
		"Left Trigger",
		"Right Trigger"
	};
}

struct NormalizedController
{
	vec2f stick_left;
	vec2f stick_right;
	vec2f triggers;
};

#endif

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

	static const int LEFT_THUMB_DEADZONE = 7849;
	static const int RIGHT_THUMB_DEADZONE = 8689;
	static const int TRIGGER_THRESHOLD = 3855;

	static const int AXIS_MAX = 32767;

	static const int AKU_SCALING_FACTOR = 10000;
};

#endif