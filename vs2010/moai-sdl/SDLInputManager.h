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
	const static char* Names[] = {
		"device",
		"pad0",
		"pad1",
		"pad2",
		"pad3"
	};
}

namespace SDLInputDeviceSensor {
	enum InputDeviceSensor_ID {
		KEYBOARD,
		POINTER,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
		ID_TOTAL
	};
	const static char* Names[] = {
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

class SdlInputManager
{
public:
	SdlInputManager();
	~SdlInputManager();

	void doAKUInit();
protected:
private:
	void doAKUDeviceInit(
		SDLInputDevice::InputDevice_ID p_id,
		SDLInputDeviceType::InputDeviceType_ID p_type
	);
};

#endif