#ifndef SDLINPUTMANAGER
#define SDLINPUTMANAGER

#include <string>
#include <cstdio>

#include <SDL.h>
#define SDL_main main


#ifndef HAS_AKU
#define HAS_AKU
#include <aku/AKU.h>
#endif

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
#endif

class SdlInputManager
{
public:
	SdlInputManager();
	~SdlInputManager();

	/** Perform all AKU-related input initialization.
	 */
	void doAKUInit();
	
	void inputNotify_onMouseMove(SDL_MouseMotionEvent* p_event);
	void inputNotify_onMouseButton(SDL_MouseButtonEvent* p_event);
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

};

#endif