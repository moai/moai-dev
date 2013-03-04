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

	doAKUDeviceInit(SledgeInputDevice::ID_DEVICE, "Keyboard");

	// disable controller events; we'll manually poll, thanks
	// ...or not? jesus.
	SDL_GameControllerEventState(SDL_IGNORE);
	SDL_JoystickEventState(SDL_IGNORE);

	setDeadzones
		((float)LEFT_THUMB_DEADZONE, (float)RIGHT_THUMB_DEADZONE, (float)TRIGGER_THRESHOLD);

	num_joysticks_total = SDL_NumJoysticks();
	num_controllers_total = 0;
	int controllerIdx = 1;
	int gc_i = 0;
	int jy_i = 0;
	for (int i = 0; i < num_joysticks_total; ++i)
	{
		printf("\t[controller %d]\n", i+1);
		char* controllerName =
			const_cast<char*>(SDL_GameControllerNameForIndex(i));

		if (SDL_IsGameController(i))
		{
			num_controllers_total++;
			printf("\t\tGame controller?\tYES\n");
			SDL_GameController* gc = SDL_GameControllerOpen(i);
			controllers.push_back(gc);
			NormalizedController nc;
			
			for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; ++j)
			{
				ButtonState_Old.state[j] = false;
			}
			controllers_normalized.push_back(nc);
			if (!controllerName)
			{
				controllerName = "Unknown controller";
			}
			doAKUDeviceInit((SledgeInputDevice::InputDevice_ID)(gc_i+1), controllerName);

			++gc_i;
		} else {
			printf("\t\tGame controller?\tNO\n");
			SDL_Joystick* joy = SDL_JoystickOpen(i);
			joysticks.push_back(joy);
			if (!controllerName)
			{
				controllerName = "Unknown joystick";
			}
			doAKUDeviceInit((SledgeInputDevice::InputDevice_ID)(jy_i+5), controllerName);

			++jy_i;
		}

		printf(
			"\t\tname: '%s'\n",
			controllerName
			);
	}

	num_joysticks_total -= num_controllers_total;

	printf("joystick count: %d\n", num_joysticks_total);
	printf("controllers count: %d\n", num_controllers_total);
}

void SledgeInputManager::doAKUDeviceInit(
	SledgeInputDevice::InputDevice_ID p_id,
	char* p_devicename
)
{
	AKUSetInputDevice(
		p_id,
		SledgeInputDevice::DeviceName[p_id]
	);
	AKUSetInputDeviceExtendedName(
		p_id,
		p_devicename
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

	case SledgeInputDeviceType::IDT_JOY:
		initJoy(p_id);
		break;

	default:
		printf("Unhandled device type.");
		break;
	}
}

void SledgeInputManager::initJoy(
	SledgeInputDevice::InputDevice_ID p_id
	)
{
	int derived_device_id = (int)(p_id - SledgeInputDevice::ID_JOY_0);
	int num_axes = SDL_JoystickNumAxes(joysticks[derived_device_id]);
	int num_sticks = num_axes / 2;
	char stick_name[64];
	AKUReserveInputDeviceSensors(
		p_id,
		num_sticks + 1
		);
	NormalizedJoystick nj;
	nj.sticks.clear();
	nj.buttons.clear();
	for (int i = 0; i < num_sticks; ++i)
	{
		sprintf(stick_name, "stick%d", i);
		AKUSetInputDeviceJoystick(
			p_id,
			i,
			stick_name
		);
		vec2f thisStick;
		nj.sticks.push_back(thisStick);
	}

	int num_buttons = SDL_JoystickNumButtons(joysticks[derived_device_id]);
	for (int i = 0; i < num_buttons; ++i)		
	{
		bool btn = false;
		nj.buttons.push_back(btn);
	}
	AKUSetInputDeviceKeyboard(
		p_id,
		num_sticks, 
		"buttons"
	);

	joysticks_normalized.push_back(nj);
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
	AKUSetInputDeviceKeyboard(
		p_id,
		SledgePadSensorAxes::PS_BUTTONS, 
		SledgePadSensorAxes::SensorName[SledgePadSensorAxes::PS_BUTTONS]
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
	if(controllers_normalized.size() != 0)
	{
		for (
			int i = 0;
			i < controllers_normalized.size();
			++i
		)
		{
			// I love how this function returns an integer instead of an SDL_bool.
			bool bIsAttached = SDL_GameControllerGetAttached(controllers[i]) == 1;

			if(bIsAttached)
			{
				controllers_normalized[i] = postprocessController(controllers[i]);
				pollPadButtons(controllers[i], (SledgeInputDevice::InputDevice_ID)((int)SledgeInputDevice::ID_PAD_0+i));
				updateAKU_Controller(
					(SledgeInputDevice::InputDevice_ID)((int)SledgeInputDevice::ID_PAD_0+i),
					&controllers_normalized[i]
				);
			} else {

				// This controller is disconnected.
				
				// @todo notify Lua

			}
		}
	}

	SDL_JoystickEventState(SDL_QUERY);
	SDL_JoystickUpdate();
	if(joysticks_normalized.size() != 0)
	{
		for (
			int i = 0;
			i < joysticks_normalized.size();
			++i
		)
		{
			bool bIsAttached = SDL_JoystickGetAttached(joysticks[i]) == SDL_TRUE;

			if(bIsAttached)
			{
				int num_axes = SDL_JoystickNumAxes(joysticks[i]);
				//printf("[joystick %d][%d]", i, num_axes);
				postprocessJoystick(&joysticks_normalized[i], joysticks[i], RIGHT_THUMB_DEADZONE);
				// @todo	poll pad buttons
				for (int j = 0; j < joysticks_normalized[i].sticks.size(); ++j)
				{
					//printf("[stick %d]", j);
					updateAKU_Joystick(
						(SledgeInputDevice::InputDevice_ID)((int)SledgeInputDevice::ID_JOY_0+i),
						j,
						&joysticks_normalized[i].sticks[j]
					);
				}
				pollJoyButtons(joysticks[i], (SledgeInputDevice::InputDevice_ID)((int)SledgeInputDevice::ID_JOY_0+i));
				//printf("\n");
			} else {

				// This joystick is disconnected.

				// @todo notify Lua

			}
		}
	}
}

void SledgeInputManager::setDeadzones
	(float p_thumbLeft, float p_thumbRight, float p_trigger)
{
	SledgeInputManager::deadzone_thumbLeft = p_thumbLeft;
	SledgeInputManager::deadzone_thumbRight = p_thumbRight;
	SledgeInputManager::deadzone_trigger = p_trigger;
}

void SledgeInputManager::inputNotify_onKeyDown(SDL_KeyboardEvent* p_event)
{
	bool bIsDown = p_event->state == SDL_PRESSED;
	SDL_Scancode scancode = p_event->keysym.scancode;
	SDL_Scancode scancode2 = SDL_GetScancodeFromKey(p_event->keysym.sym);

	if(scancode != scancode2)
		scancode = scancode2;

	AKUEnqueueKeyboardEvent(
		SledgeInputDevice::ID_DEVICE,
		SledgeDeviceSensor::IDS_KEYBOARD,
		scancode2,
		bIsDown
	);
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
	 

	return postprocessStick(X_raw, Y_raw, Deadzone);
}

void SledgeInputManager::postprocessJoystick(NormalizedJoystick* p_nj, SDL_Joystick* p_stick, const int p_deadzone )
{
	float X_raw = (float)SDL_JoystickGetAxis(p_stick, 0);
	float Y_raw = (float)SDL_JoystickGetAxis(p_stick, 1);
	float X2_raw = (float)SDL_JoystickGetAxis(p_stick, 2);
	float Y2_raw = (float)SDL_JoystickGetAxis(p_stick, 3);
	float Deadzone = (float)p_deadzone;

	//printf("[%0.2f %0.2f][%0.2f %0.2f]\n", X_raw, Y_raw, X2_raw, Y2_raw);

	for (int i = 0; i < p_nj->sticks.size(); ++i)
	{
		float X_raw = (float)SDL_JoystickGetAxis(p_stick, 0 + i * 2);
		float Y_raw = (float)SDL_JoystickGetAxis(p_stick, 1 + i * 2);
		p_nj->sticks[i] = postprocessStick(X_raw, Y_raw, Deadzone);
	}

	//return _gen_postprocessStick(X_raw, Y_raw, Deadzone);
}

vec2f SledgeInputManager::postprocessStick( float p_x, float p_y, float p_deadzone )
{
	vec2f result;

	// use raw values to calculate normalized direction vector
	float Magnitude_raw =
		sqrt(p_x * p_x + p_y * p_y);
	float X_dir = p_x / Magnitude_raw;
	float Y_dir = p_y / Magnitude_raw;

	// enforce deadzone to get normalized magnitude
	float Magnitude_norm = 0.0f;
	if(Magnitude_raw > p_deadzone)
	{
		if(Magnitude_raw > 32767.0f) Magnitude_raw = 32767.0f;
		Magnitude_raw -= p_deadzone;
		Magnitude_norm = Magnitude_raw / (32767.0f - p_deadzone);
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

void SledgeInputManager::pollPadButtons(
	SDL_GameController* p_controller,
	SledgeInputDevice::InputDevice_ID p_deviceid
)
{
	buttonState newState;
	int deviceIdx = p_deviceid - 1;

	//printf("pollPadButtons(p_deviceid: %d)\n", p_deviceid);

	for(int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
	{
		bool bDownThisFrame = SDL_GameControllerGetButton(p_controller, (SDL_CONTROLLER_BUTTON)i) == 1;

		newState.state[i] = bDownThisFrame;
		bool bDown =
			newState.state[i] == ButtonState_Old.state[i] ||
			(newState.state[i] == true && ButtonState_Old.state[i] == false); 

		if(ButtonState_Old.state[i] == true || newState.state[i] == true)
		{
			AKUEnqueueKeyboardEvent(
				p_deviceid,
				SledgePadSensorAxes::PS_BUTTONS,
				i,
				bDown
			);
		}
	}
	//controllers_normalized[0].lastButtonState = newState;
	ButtonState_Old = newState;
	//controllers_normalized[0].lastButtonState = newState;
}

void SledgeInputManager::pollJoyButtons( SDL_Joystick* p_joystick, SledgeInputDevice::InputDevice_ID p_deviceid )
{
	int derived_norm_idx = (int)(p_deviceid - SledgeInputDevice::ID_JOY_0);
	int num_buttons = SDL_JoystickNumButtons(p_joystick);
	//bool* newState = new bool[num_buttons];

	for (int i = 0; i < num_buttons; ++i)
	{
		bool bDownThisFrame = SDL_JoystickGetButton(p_joystick, i) == 1;

		//newState[i] = bDownThisFrame;
		bool bDown = 
			bDownThisFrame == joysticks_normalized[derived_norm_idx].buttons[i] ||
			(bDownThisFrame == true && joysticks_normalized[derived_norm_idx].buttons[i] == false);

		if(bDownThisFrame == true || joysticks_normalized[derived_norm_idx].buttons[i] == true)
		{
			AKUEnqueueKeyboardEvent(
				p_deviceid,
				joysticks_normalized[derived_norm_idx].sticks.size(),
				i,
				bDown
			);
		}
		joysticks_normalized[derived_norm_idx].buttons[i] = bDownThisFrame;
	}
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

void SledgeInputManager::updateAKU_Joystick(SledgeInputDevice::InputDevice_ID p_deviceid, int p_stick_id, vec2f* p_stick)
{
	//printf("[%0.2f %0.2f]", p_stick->x, p_stick->y);
	AKUEnqueueJoystickEvent(
		p_deviceid,
		p_stick_id,
		p_stick->x,
		p_stick->y
	);
}