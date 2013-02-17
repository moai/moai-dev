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

	// disable controller events; we'll manually poll, thanks
	SDL_GameControllerEventState(SDL_IGNORE);


	num_joysticks = SDL_NumJoysticks();
	printf("joystick count: %d\n", num_joysticks);
	int controllerIdx = 1;
	for (int i = 0; i < num_joysticks; ++i)
	{
		printf("\t[controller %d]\n", i+1);
		if (SDL_IsGameController(i))
		{
			printf("\t\tGame controller?\tYES\n");
			SDL_GameControllerOpen(i);
			doAKUDeviceInit((SDLInputDevice::InputDevice_ID)(i+1));
		} else {
			printf("\t\tGame controller?\tNO\n");
		}

		char* controllerName =
			const_cast<char*>(SDL_GameControllerNameForIndex(i));
		

		if (controllerName) {
			printf(
				"\t\tname: '%s'\n",
				controllerName
			);
		} else {
			printf("\t\tname: NONE\n");
		}
		
	}
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
		initDevice(p_id);
		break;

	// @todo	actually implement this
	case SDLInputDeviceType::IDT_PAD:
		break;

	default:
		printf("Unhandled device type.");
		break;
	}
}

void SdlInputManager::initDevice(SDLInputDevice::InputDevice_ID p_id)
{
	AKUReserveInputDeviceSensors(
		p_id,
		SDLInputDeviceSensor::IDS_TOTAL
	);
	AKUSetInputDeviceKeyboard(
		p_id,
		SDLInputDeviceSensor::IDS_KEYBOARD,
		SDLInputDeviceSensor::SensorName[SDLInputDeviceSensor::IDS_KEYBOARD]
	);
	AKUSetInputDevicePointer(
		p_id,
		SDLInputDeviceSensor::IDS_POINTER,
		SDLInputDeviceSensor::SensorName[SDLInputDeviceSensor::IDS_POINTER]
	);
	AKUSetInputDeviceButton(
		p_id,
		SDLInputDeviceSensor::IDS_MOUSE_LEFT,
		SDLInputDeviceSensor::SensorName[SDLInputDeviceSensor::IDS_MOUSE_LEFT]
	);
	AKUSetInputDeviceButton(
		p_id,
		SDLInputDeviceSensor::IDS_MOUSE_MIDDLE,
		SDLInputDeviceSensor::SensorName[SDLInputDeviceSensor::IDS_MOUSE_MIDDLE]
	);
	AKUSetInputDeviceButton(
		p_id,
		SDLInputDeviceSensor::IDS_MOUSE_RIGHT,
		SDLInputDeviceSensor::SensorName[SDLInputDeviceSensor::IDS_MOUSE_RIGHT]
	);
}

void SdlInputManager::inputNotify_onMouseMove(SDL_MouseMotionEvent* p_event)
{
	AKUEnqueuePointerEvent (
		SDLInputDevice::ID_DEVICE,
		SDLInputDeviceSensor::IDS_POINTER,
		p_event->x,
		p_event->y
	);
}
void SdlInputManager::inputNotify_onMouseButton(SDL_MouseButtonEvent* p_event)
{
	// @ todo	Add keyboard modifier support.
	switch (p_event->button)		
	{
	case SDL_BUTTON_LEFT:
		AKUEnqueueButtonEvent(
			SDLInputDevice::ID_DEVICE,
			SDLInputDeviceSensor::IDS_MOUSE_LEFT,
			(p_event->state == SDL_PRESSED)
			);
		break;
	case SDL_BUTTON_MIDDLE:
		AKUEnqueueButtonEvent(
			SDLInputDevice::ID_DEVICE,
			SDLInputDeviceSensor::IDS_MOUSE_MIDDLE,
			(p_event->state == SDL_PRESSED)
			);
		break;
	case SDL_BUTTON_RIGHT:
		AKUEnqueueButtonEvent(
			SDLInputDevice::ID_DEVICE,
			SDLInputDeviceSensor::IDS_MOUSE_RIGHT,
			(p_event->state == SDL_PRESSED)
			);
		break;
	}
}