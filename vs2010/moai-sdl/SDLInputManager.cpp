#include "SDLInputManager.h"

SdlInputManager::SdlInputManager()
{

}

SdlInputManager::~SdlInputManager()
{

}

void SdlInputManager::doAKUInit()
{
	AKUSetInputConfigurationName("AKUSDL2");
	AKUReserveInputDevices(SDLInputDevice::ID_TOTAL);

	doAKUDeviceInit(SDLInputDevice::ID_DEVICE);

}

void SdlInputManager::doAKUDeviceInit(
	SDLInputDevice::InputDevice_ID p_id	
)
{
	AKUSetInputDevice(
		p_id,
		SDLInputDevice::DeviceName[p_id]
		);

	SDLInputDeviceType::InputDeviceType_ID
		devicetype = SDLInputDevice::DeviceType[p_id];

	switch (devicetype)
	{
	case SDLInputDeviceType::IDT_DEVICE:
		AKUReserveInputDeviceSensors(
			p_id,
			SDLInputDeviceSensor::ID_TOTAL
			);
		AKUSetInputDeviceKeyboard(
			p_id,
			SDLInputDeviceSensor::KEYBOARD,
			SDLInputDeviceSensor::SensorName[SDLInputDeviceSensor::KEYBOARD]
			);
		AKUSetInputDevicePointer(
			p_id,
			SDLInputDeviceSensor::POINTER,
			SDLInputDeviceSensor::SensorName[SDLInputDeviceSensor::POINTER]
			);
		AKUSetInputDeviceButton(
			p_id,
			SDLInputDeviceSensor::MOUSE_LEFT,
			SDLInputDeviceSensor::SensorName[SDLInputDeviceSensor::MOUSE_LEFT]
			);
		AKUSetInputDeviceButton(
			p_id,
			SDLInputDeviceSensor::MOUSE_MIDDLE,
			SDLInputDeviceSensor::SensorName[SDLInputDeviceSensor::MOUSE_MIDDLE]
			);
		AKUSetInputDeviceButton(
			p_id,
			SDLInputDeviceSensor::MOUSE_RIGHT,
			SDLInputDeviceSensor::SensorName[SDLInputDeviceSensor::MOUSE_RIGHT]
			);
		break;

	// @todo	actually implement this
	case SDLInputDeviceType::IDT_PAD:
		break;

	default:
		printf("Unhandled device type.");
		break;
	}
}