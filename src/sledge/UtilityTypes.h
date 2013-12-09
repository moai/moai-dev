#include "stdafx.h"

#ifndef SDLINPUTTYPES
#define SDLINPUTTYPES

namespace SLEDGE_NAMESPACE {
	namespace InputDevice {
		const static char* DeviceName[] = {
			"device",
			"pad0",
			"pad1",
			"pad2",
			"pad3",
			"joy0",
			"joy1",
			"joy2",
			"joy3"
		};
		const static InputDeviceType_ID DeviceType[] = {		
			IDT_DEVICE,
			IDT_PAD,
			IDT_PAD,
			IDT_PAD,
			IDT_PAD,
			IDT_JOY,
			IDT_JOY,
			IDT_JOY,
			IDT_JOY
		};
	}
	namespace DeviceSensor {
		const static char* SensorName[] = {
			"keyboard",
			"pointer",
			"mouseLeft",
			"mouseMiddle",
			"mouseRight"
		};
	}
	namespace PadSensorAxes
	{
		const static char* SensorName[] = {
			"stickLeft",
			"stickRight",
			"triggers",
			"buttons"
		};
	}
	namespace JoySensor
	{
		const static char* SensorName[] = {
			"stick",
			"buttons"
		};
	}
	namespace PadSensor
	{
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
}



#endif