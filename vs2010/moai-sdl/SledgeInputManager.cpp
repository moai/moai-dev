#include "SledgeInputManager.h"

float SledgeInputManager::deadzone_thumbLeft = 0.0f;
float SledgeInputManager::deadzone_thumbRight = 0.0f;
float SledgeInputManager::deadzone_trigger = 0.0f;

SledgeInputManager::SledgeInputManager()
{

}

SledgeInputManager::~SledgeInputManager()
{

}

void SledgeInputManager::doAKUInit()
{
	AKUSetInputConfigurationName("AKUSDL2");
	AKUReserveInputDevices(SledgeInputDevice::ID_TOTAL);

	doAKUDeviceInit(SledgeInputDevice::ID_DEVICE);

	// disable controller events; we'll manually poll, thanks
	// ...or not? jesus.
	SDL_GameControllerEventState(SDL_IGNORE);

	setDeadzones
		((float)LEFT_THUMB_DEADZONE, (float)RIGHT_THUMB_DEADZONE, (float)TRIGGER_THRESHOLD);

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
			NormalizedController nc;
			controllers_normalized.push_back(nc);
			doAKUDeviceInit((SledgeInputDevice::InputDevice_ID)(i+1));
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

void SledgeInputManager::doAKUDeviceInit(
	SledgeInputDevice::InputDevice_ID p_id	
)
{
	AKUSetInputDevice(
		p_id,
		SledgeInputDevice::DeviceName[p_id]
		);

	SledgeInputDeviceType::InputDeviceType_ID
		devicetype = SledgeInputDevice::DeviceType[p_id];

	switch (devicetype)
	{
	case SledgeInputDeviceType::IDT_DEVICE:
		initDevice(p_id);
		break;

	// @todo	actually implement this
	case SledgeInputDeviceType::IDT_PAD:
		initPad(p_id);
		break;

	default:
		printf("Unhandled device type.");
		break;
	}
}

void SledgeInputManager::initPad(
	SledgeInputDevice::InputDevice_ID p_id
)
{
	AKUReserveInputDeviceSensors(
		p_id,
		SledgePadSensorAxes::PS_TOTAL
		);
	AKUSetInputDeviceJoystick(
		p_id,
		SledgePadSensorAxes::PS_STICK_LEFT,
		SledgePadSensorAxes::SensorName[SledgePadSensorAxes::PS_STICK_LEFT]
	);
	AKUSetInputDeviceJoystick(
		p_id,
		SledgePadSensorAxes::PS_STICK_RIGHT,
		SledgePadSensorAxes::SensorName[SledgePadSensorAxes::PS_STICK_RIGHT]
	);
	AKUSetInputDeviceJoystick(
		p_id,
		SledgePadSensorAxes::PS_TRIGGERS,
		SledgePadSensorAxes::SensorName[SledgePadSensorAxes::PS_TRIGGERS]
	);	
}


void SledgeInputManager::initDevice(
	SledgeInputDevice::InputDevice_ID p_id
)
{
	AKUReserveInputDeviceSensors(
		p_id,
		SledgeDeviceSensor::IDS_TOTAL
	);
	AKUSetInputDeviceKeyboard(
		p_id,
		SledgeDeviceSensor::IDS_KEYBOARD,
		SledgeDeviceSensor::SensorName[SledgeDeviceSensor::IDS_KEYBOARD]
	);
	AKUSetInputDevicePointer(
		p_id,
		SledgeDeviceSensor::IDS_POINTER,
		SledgeDeviceSensor::SensorName[SledgeDeviceSensor::IDS_POINTER]
	);
	AKUSetInputDeviceButton(
		p_id,
		SledgeDeviceSensor::IDS_MOUSE_LEFT,
		SledgeDeviceSensor::SensorName[SledgeDeviceSensor::IDS_MOUSE_LEFT]
	);
	AKUSetInputDeviceButton(
		p_id,
		SledgeDeviceSensor::IDS_MOUSE_MIDDLE,
		SledgeDeviceSensor::SensorName[SledgeDeviceSensor::IDS_MOUSE_MIDDLE]
	);
	AKUSetInputDeviceButton(
		p_id,
		SledgeDeviceSensor::IDS_MOUSE_RIGHT,
		SledgeDeviceSensor::SensorName[SledgeDeviceSensor::IDS_MOUSE_RIGHT]
	);
}

void SledgeInputManager::doOnTick()
{
	SDL_Event event;
	int _count = 0;
	SDL_GameControllerEventState(SDL_QUERY);
	
	controllers_normalized[0] = postprocessController(controllers[0]);
	
	updateAKU_Controller(
		SledgeInputDevice::ID_PAD_0,
		&controllers_normalized[0]
	);
		/*
	printf("left: [%0.2f] right: [%0.2f]\n",
		controllers_normalized[0].triggers.x,
		controllers_normalized[0].triggers.y
	);
	*/
	/*
	printf("left: [%0.2f\t%0.2f] right: [%0.2f\t%0.2f]\n",
		controllers_normalized[0].stick_left.x,
		controllers_normalized[0].stick_left.y,
		controllers_normalized[0].stick_right.x,
		controllers_normalized[0].stick_right.y
	);*/
	
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

void SledgeInputManager::setDeadzones
	(float p_thumbLeft, float p_thumbRight, float p_trigger)
{
	SledgeInputManager::deadzone_thumbLeft = p_thumbLeft;
	SledgeInputManager::deadzone_thumbRight = p_thumbRight;
	SledgeInputManager::deadzone_trigger = p_trigger;
}

void SledgeInputManager::inputNotify_onMouseMove(SDL_MouseMotionEvent* p_event)
{
	AKUEnqueuePointerEvent (
		SledgeInputDevice::ID_DEVICE,
		SledgeDeviceSensor::IDS_POINTER,
		p_event->x,
		p_event->y
	);
}
void SledgeInputManager::inputNotify_onMouseButton(SDL_MouseButtonEvent* p_event)
{
	// @ todo	Add keyboard modifier support.
	switch (p_event->button)		
	{
	case SDL_BUTTON_LEFT:
		AKUEnqueueButtonEvent(
			SledgeInputDevice::ID_DEVICE,
			SledgeDeviceSensor::IDS_MOUSE_LEFT,
			(p_event->state == SDL_PRESSED)
			);
		break;
	case SDL_BUTTON_MIDDLE:
		AKUEnqueueButtonEvent(
			SledgeInputDevice::ID_DEVICE,
			SledgeDeviceSensor::IDS_MOUSE_MIDDLE,
			(p_event->state == SDL_PRESSED)
			);
		break;
	case SDL_BUTTON_RIGHT:
		AKUEnqueueButtonEvent(
			SledgeInputDevice::ID_DEVICE,
			SledgeDeviceSensor::IDS_MOUSE_RIGHT,
			(p_event->state == SDL_PRESSED)
			);
		break;
	}
}

void SledgeInputManager::inputNotify_onPadAxisMove(SDL_ControllerAxisEvent* p_event)
{
	//printf("SDL_ControllerAxisEvent [controller: %d]\n", p_event->which);
	printf(
		"\t%s: %d\n",
		JethaSDLControllerAxis::AxisName[p_event->axis],
		p_event->value
	);
}

vec2f SledgeInputManager::postprocessThumbstick(
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

float SledgeInputManager::postprocessTrigger(
	SDL_GameController* p_controller,
	SDL_CONTROLLER_AXIS p_axisT,
	const int p_deadzone
	)
{
	float result = 0.0f;

	float T_raw =
		(float)SDL_GameControllerGetAxis(p_controller, p_axisT);
	float T_norm = 0.0f;
	float Deadzone = (float)p_deadzone;

	if(T_raw > Deadzone)
	{
		//result = T_raw;

		// clip magnitude
		if(T_raw > (float)AXIS_MAX) T_raw = (float)AXIS_MAX;

		T_norm = (T_raw - Deadzone) / ((float)AXIS_MAX - Deadzone);
		//printf("trigger magnitude: %0.2f\n", T_raw);

		result = T_norm;
	}


	return result;
}


NormalizedController SledgeInputManager::postprocessController(
	SDL_GameController* p_controller
)
{
	NormalizedController result;

	result.stick_left = postprocessThumbstick(
		p_controller,
		SDL_CONTROLLER_AXIS_LEFTX,
		SDL_CONTROLLER_AXIS_LEFTY,
		LEFT_THUMB_DEADZONE
	);
	result.stick_right = postprocessThumbstick(
		p_controller,
		SDL_CONTROLLER_AXIS_RIGHTX,
		SDL_CONTROLLER_AXIS_RIGHTY,
		RIGHT_THUMB_DEADZONE
	);
	result.triggers.x = postprocessTrigger(
		p_controller,
		SDL_CONTROLLER_AXIS_TRIGGERLEFT,
		TRIGGER_THRESHOLD
	);
	result.triggers.y = postprocessTrigger(
		p_controller,
		SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
		TRIGGER_THRESHOLD
	);


	return result;
}

void SledgeInputManager::updateAKU_Controller(
	SledgeInputDevice::InputDevice_ID p_deviceid,
	NormalizedController* p_nc
)
{	
	//float scalingFactor = (float)AKU_SCALING_FACTOR;

	AKUEnqueueJoystickEvent (
		p_deviceid,
		SledgePadSensorAxes::PS_STICK_LEFT,
		p_nc->stick_left.x,
		p_nc->stick_left.y
		);
	AKUEnqueueJoystickEvent (
		p_deviceid,
		SledgePadSensorAxes::PS_STICK_RIGHT,
		p_nc->stick_right.x,
		p_nc->stick_right.y
		);
	AKUEnqueueJoystickEvent (
		p_deviceid,
		SledgePadSensorAxes::PS_TRIGGERS,
		p_nc->triggers.x,
		p_nc->triggers.y
		);
}