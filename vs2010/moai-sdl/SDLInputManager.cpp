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

	

}

void SdlInputManager::doAKUDeviceInit(
	SDLInputDevice::InputDevice_ID p_id,
	SDLInputDeviceType::InputDeviceType_ID p_type
)
{
	AKUSetInputDevice(
		p_id,
		SDLInputDevice::Names[p_id]
		);

	switch (p_type)
	{
	case SDLInputDeviceType::IDT_DEVICE:
		AKUSetInputDeviceKeyboard(
			p_id,
			SDLInputDeviceSensor::KEYBOARD,
			SDLInputDeviceSensor::Names[SDLInputDeviceSensor::KEYBOARD]
			);
		AKUSetInputDevicePointer(
			p_id,
			SDLInputDeviceSensor::POINTER,
			SDLInputDeviceSensor::Names[SDLInputDeviceSensor::POINTER]
			);
		AKUSetInputDeviceButton(
			p_id,
			SDLInputDeviceSensor::MOUSE_LEFT,
			SDLInputDeviceSensor::Names[SDLInputDeviceSensor::MOUSE_LEFT]
			);
		AKUSetInputDeviceButton(
			p_id,
			SDLInputDeviceSensor::MOUSE_MIDDLE,
			SDLInputDeviceSensor::Names[SDLInputDeviceSensor::MOUSE_MIDDLE]
			);
		AKUSetInputDeviceButton(
			p_id,
			SDLInputDeviceSensor::MOUSE_RIGHT,
			SDLInputDeviceSensor::Names[SDLInputDeviceSensor::MOUSE_RIGHT]
			);
		break;
	case SDLInputDeviceType::IDT_PAD:
		break;
	default:
		printf("Unhandled device type.");
		break;
	}
}