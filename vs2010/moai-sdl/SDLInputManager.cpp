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
	// ...or not? jesus.
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
			SDL_GameController* gc = SDL_GameControllerOpen(i);
			controllers.push_back(gc);
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

void SdlInputManager::doOnTick()
{
	SDL_Event event;
	int _count = 0;
	SDL_GameControllerEventState(SDL_QUERY);
	
	vec2f thumbstickLeft = postprocessThumbstick(
		controllers[0],
		SDL_CONTROLLER_AXIS_LEFTX,
		SDL_CONTROLLER_AXIS_LEFTY,
		LEFT_THUMB_DEADZONE
	);
	vec2f thumbstickRight = postprocessThumbstick(
		controllers[0],
		SDL_CONTROLLER_AXIS_RIGHTX,
		SDL_CONTROLLER_AXIS_RIGHTY,
		RIGHT_THUMB_DEADZONE
	);

	printf("left: [%0.2f\t%0.2f] right: [%0.2f\t%0.2f]\n",
		thumbstickLeft.x,
		thumbstickLeft.y,
		thumbstickRight.x,
		thumbstickRight.y
	);
	
	/*
	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_CONTROLLERAXISMOTION:
			printf("axis motion\n");
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			printf("button down!\n");
			break;
		case SDL_CONTROLLERBUTTONUP:
			printf("button up!\n");
			break;
		default:
			break;
		}			
		_count++;
		}*/

	//printf("%d pending events\n", _count);
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

void SdlInputManager::inputNotify_onPadAxisMove(SDL_ControllerAxisEvent* p_event)
{
	//printf("SDL_ControllerAxisEvent [controller: %d]\n", p_event->which);
	printf(
		"\t%s: %d\n",
		JethaSDLControllerAxis::AxisName[p_event->axis],
		p_event->value
	);
}

vec2f SdlInputManager::postprocessThumbstick(
	SDL_GameController* p_controller,
	SDL_CONTROLLER_AXIS p_axisX,
	SDL_CONTROLLER_AXIS p_axisY,
	const int p_deadzone
)
{
	float X_raw =
		(float)SDL_GameControllerGetAxis(p_controller, p_axisX);
	float Y_raw =
		(float)SDL_GameControllerGetAxis(p_controller, p_axisY);
	float Deadzone = (float)p_deadzone;

	vec2f result;

	// use raw values to calculate normalized direction vector
	float Magnitude_raw =
		sqrt(X_raw * X_raw + Y_raw * Y_raw);
	float X_dir = X_raw / Magnitude_raw;
	float Y_dir = Y_raw / Magnitude_raw;
	
	// enforce deadzone to get normalized magnitude
	float Magnitude_norm = 0.0f;
	if(Magnitude_raw > Deadzone)
	{
		if(Magnitude_raw > 32767.0f) Magnitude_raw = 32767.0f;
		Magnitude_raw -= Deadzone;
		Magnitude_norm = Magnitude_raw / (32767.0f - Deadzone);
	} else {
		Magnitude_raw = 0.0f;
		Magnitude_norm = 0.0f;
	}

	// multiply normalized magnitude by direction vector to get 
	// final result
	result.x = Magnitude_norm * X_dir;
	result.y = Magnitude_norm * Y_dir;

	return result;
}