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
namespace SDLInputDeviceType {
	enum InputDeviceType_ID {
		IDT_DEVICE,
		IDT_PAD
	};
}

namespace SDLInputDevice {
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
	const static SDLInputDeviceType::InputDeviceType_ID DeviceType[] = {		
		SDLInputDeviceType::IDT_DEVICE,
		SDLInputDeviceType::IDT_PAD,
		SDLInputDeviceType::IDT_PAD,
		SDLInputDeviceType::IDT_PAD,
		SDLInputDeviceType::IDT_PAD
	};
}

namespace SDLInputDeviceSensor {
	enum InputDeviceSensor_ID {
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

namespace SDLInputPadSensorID
{
	enum {
		STICK_LEFT,
		STICK_RIGHT
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

#endif

class SdlInputManager
{
public:
	SdlInputManager();
	~SdlInputManager();

	std::vector<SDL_GameController*> controllers;

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
		SDLInputDevice::InputDevice_ID p_id
	);
	void initDevice(
		SDLInputDevice::InputDevice_ID p_id
	);

	/** Post-process a thumbstick, returning a normalized vec2f.
	 */
	vec2f postprocessThumbstick(
		SDL_GameController* p_controller,
		SDL_CONTROLLER_AXIS p_axisX,
		SDL_CONTROLLER_AXIS p_axisY,
		const int p_deadzone
	);

	static const int LEFT_THUMB_DEADZONE = 7849;
	static const int RIGHT_THUMB_DEADZONE = 8689;
};

#endif