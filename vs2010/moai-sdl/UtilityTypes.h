#ifndef JETHA_UTILITYTYPES
#define JETHA_UTILITYTYPES

struct vec2u
{
	unsigned int x;
	unsigned int y;
};

struct vec2f
{
	float x;
	float y;
};
#endif

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

namespace SledgePadSensorAxes
{
	enum {
		PS_STICK_LEFT,
		PS_STICK_RIGHT,
		PS_TRIGGERS,
		PS_BUTTONS,
		PS_TOTAL
	};
	const static char* SensorName[] = {
		"stickLeft",
		"stickRight",
		"triggers",
		"buttons"
	};
}

namespace SledgePadSensorButtons
{
	enum {
		PSB_BUTTON_A,
		PSB_BUTTON_B,
		PSB_BUTTON_X,
		PSB_BUTTON_Y,
		PSB_BUTTON_START,
		PSB_BUTTON_BACK,

		PSB_BUTTON_LEFTSTICK,
		PSB_BUTTON_RIGHTSTICK,

		PSB_BUTTON_LEFTSHOULDER,
		PSB_BUTTON_RIGHTSHOULDER,

		PSB_BUTTON_DPAD_UP,
		PSB_BUTTON_DPAD_DOWN,
		PSB_BUTTON_DPAD_LEFT,
		PSB_BUTTON_DPAD_RIGHT
	};
	const static char* ButtonName[] = {
		"btnA",
		"btnB",
		"btnX",
		"btnY",
		"start",
		"back",
		"stickLeft",
		"stickRight",
		"shoulderLeft",
		"shoulderRight",
		"dpadUp",
		"dpadDown",
		"dpadLeft",
		"dpadRight"
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