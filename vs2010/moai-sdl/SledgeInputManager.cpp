#include "SledgeInputManager.h"

float SledgeInputManager::deadzone_thumbLeft = 0.0f;
float SledgeInputManager::deadzone_thumbRight = 0.0f;
float SledgeInputManager::deadzone_trigger = 0.0f;
float SledgeInputManager::deadzone_joystick = 0.0f;

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

	// Tell AKU about the mouse and keyboard.
	SledgeDevice dvc;
	dvc.device_id = SledgeInputDevice::ID_DEVICE;
	dvc.name = "Keyboard";
	doAKUDeviceInit(SledgeInputDeviceType::IDT_DEVICE, &dvc);

	// Disable controller events; we'll manually poll, thanks.
	SDL_GameControllerEventState(SDL_IGNORE);
	SDL_JoystickEventState(SDL_IGNORE);

	// Set the default thumbstick and trigger deadzones.
	setDeadzones
		((float)LEFT_THUMB_DEADZONE, (float)RIGHT_THUMB_DEADZONE, (float)TRIGGER_THRESHOLD, (float)JOYSTICK_DEADZONE);

	// Iterate over connected "joysticks".
	num_devices_connected = SDL_NumJoysticks();
	int gc_i = 0;
	int jy_i = 0;
	for (int i = 0; i < num_devices_connected; ++i)
	{

		if (SDL_IsGameController(i))
		{
			// GAME CONTROLLER
			SledgeController pad;
			pad.controller = SDL_GameControllerOpen(i);
			for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; ++j)
			{
				pad.buttons.push_back(false);
			}
			char* controllerName =
				const_cast<char*>(SDL_GameControllerNameForIndex(i));
			if (!controllerName) controllerName = "Unknown controller";
			pad.name = controllerName;
			pad.index = i;
			pad.index_controller = gc_i;

			pad.device_id = (SledgeInputDevice::InputDevice_ID)(gc_i+1);

			m_controllers.push_back(pad);

			doAKUDeviceInit(
				SledgeInputDeviceType::IDT_PAD,
				&pad
			);

			//printf("[%d][CONTROLLER][%d]: %s\n", i, pad.device_id, pad.name);

			++gc_i;
		} else
		{
			// REGULAR JOYSTICK
			SledgeJoystick stick;
			stick.joystick = SDL_JoystickOpen(i);

			// set name and idxs
			char* joystickName = 
				const_cast<char*>(SDL_JoystickName(stick.joystick));
			if (!joystickName) joystickName = "Unknown joystick";
			stick.name = joystickName;
			stick.index = i;
			stick.index_joystick = jy_i;

			stick.device_id = (SledgeInputDevice::InputDevice_ID)(jy_i+5);


			doAKUDeviceInit(
				SledgeInputDeviceType::IDT_JOY,
				&stick
				);
			m_joysticks.push_back(stick);

			//printf("[%d][JOYSTICK]: %s (buttons: %d)\n", i, stick.name, stick.buttons.size());

			++jy_i;
		}
	}
	printf("joystick count: %d\n", jy_i);
	printf("controllers count: %d\n", gc_i);
}

void SledgeInputManager::doAKUDeviceInit(
	SledgeInputDeviceType::InputDeviceType_ID p_typeid,
	void* p_sledgedevice
)
{
	// Infer from p_typeid what we're dealing with.
	switch (p_typeid)
	{
	case SledgeInputDeviceType::IDT_DEVICE:
		//printf("doAKUDeviceInit(IDT_DEVICE)\n");
		initDevice((SledgeDevice*)p_sledgedevice);
		break;

	case SledgeInputDeviceType::IDT_PAD:
		//printf("doAKUDeviceInit(IDT_PAD)\n");
		initPad((SledgeController*)p_sledgedevice);
		break;

	case SledgeInputDeviceType::IDT_JOY:
		//printf("doAKUDeviceInit(IDT_JOY)\n");
		initJoy((SledgeJoystick*)p_sledgedevice);
		break;

	}
}

void SledgeInputManager::initJoy(
	SledgeJoystick* p_sledgejoystick
)
{
	int num_buttons = SDL_JoystickNumButtons(p_sledgejoystick->joystick);
	int num_sticks = SDL_JoystickNumAxes(p_sledgejoystick->joystick) / 2;

	char joy_name[64];
	sprintf(joy_name, "joy%d", p_sledgejoystick->index_joystick);

	//printf("initJoy(%d, %s)", p_sledgejoystick->device_id, joy_name);

	// tell AKU about the device
	AKUSetInputDevice(
		p_sledgejoystick->device_id,
		joy_name
	);
	AKUSetInputDeviceExtendedName(
		p_sledgejoystick->device_id,
		p_sledgejoystick->name
	);

	// reserve sensors - # of sticks, +1 for the buttons
	AKUReserveInputDeviceSensors(
		p_sledgejoystick->device_id,
		num_sticks + 1
	);

	// sticks
	p_sledgejoystick->sticks.clear();
	char stick_name[64];
	for (int i = 0; i < num_sticks; ++i)
	{
		sprintf(stick_name, "stick%d", i);
		vec2f thisStick;
		p_sledgejoystick->sticks.push_back(thisStick);
		AKUSetInputDeviceJoystick(
			p_sledgejoystick->device_id,
			i,
			stick_name
		);
	}

	// buttons
	p_sledgejoystick->buttons.clear();
	for (int i = 0; i < num_buttons; ++i)
	{
		p_sledgejoystick->buttons.push_back(false);
	}
	//printf("number of buttons: %d\tlength of buttons vector: %d\n", num_buttons, p_sledgejoystick->buttons.size());

	AKUSetInputDeviceKeyboard(
		p_sledgejoystick->device_id,
		num_sticks, // not a typo!
		"buttons"
		);

	//printf("joysticks: %d buttons: %d\n", p_sledgejoystick->sticks.size(), p_sledgejoystick->buttons.size());
}


void SledgeInputManager::initPad(
	SledgeController* p_sledgecontroller
)
{
	char pad_name[64];
	sprintf(pad_name, "pad%d", p_sledgecontroller->index_controller);

	// tell AKU about the device
	AKUSetInputDevice(
		p_sledgecontroller->device_id,
		pad_name
	);
	AKUSetInputDeviceExtendedName(
		p_sledgecontroller->device_id,
		p_sledgecontroller->name
	);

	AKUReserveInputDeviceSensors(
		p_sledgecontroller->device_id,
		SledgePadSensorAxes::PS_TOTAL
		);
	AKUSetInputDeviceJoystick(
		p_sledgecontroller->device_id,
		SledgePadSensorAxes::PS_STICK_LEFT,
		SledgePadSensorAxes::SensorName[SledgePadSensorAxes::PS_STICK_LEFT]
	);
	AKUSetInputDeviceJoystick(
		p_sledgecontroller->device_id,
		SledgePadSensorAxes::PS_STICK_RIGHT,
		SledgePadSensorAxes::SensorName[SledgePadSensorAxes::PS_STICK_RIGHT]
	);
	AKUSetInputDeviceJoystick(
		p_sledgecontroller->device_id,
		SledgePadSensorAxes::PS_TRIGGERS,
		SledgePadSensorAxes::SensorName[SledgePadSensorAxes::PS_TRIGGERS]
	);
	AKUSetInputDeviceKeyboard(
		p_sledgecontroller->device_id,
		SledgePadSensorAxes::PS_BUTTONS, 
		SledgePadSensorAxes::SensorName[SledgePadSensorAxes::PS_BUTTONS]
	);
}


void SledgeInputManager::initDevice(
	SledgeDevice* p_sledgedevice
)
{
	SledgeInputDevice::InputDevice_ID p_id = p_sledgedevice->device_id;

	AKUSetInputDevice(
		p_id,
		SledgeInputDevice::DeviceName[p_id]
		);
	AKUSetInputDeviceExtendedName(
		p_id,
		p_sledgedevice->name
		);

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
	if(m_controllers.size() != 0)
	{		
		for (
			int i = 0;
			i < m_controllers.size();
			++i
		)
		{
			SledgeController* thisController = &m_controllers[i];

			// I love how this function returns an integer instead of an SDL_bool.
			bool bIsAttached = SDL_GameControllerGetAttached(thisController->controller) == 1;
			if(bIsAttached)
			{
				//printf("buttons: %d\n", thisController->buttons.size());
				updateController(thisController);
			} else {

				// This controller is disconnected.

				// @todo notify Lua
				printf("[Controller][%d] disconnected\n", i);

			}
			++i;
		}
	}
	SDL_JoystickEventState(SDL_QUERY);
	SDL_JoystickUpdate();
	if(m_joysticks.size() != 0)
	{
		//printf("m_joysticks: %d\n", m_joysticks.size());
		for (
			int i = 0;
			i < m_joysticks.size();
			++i
			)
		{
			SledgeJoystick* thisJoystick = &m_joysticks[i];

			bool bIsAttached = SDL_JoystickGetAttached(thisJoystick->joystick) == SDL_TRUE;

			if(bIsAttached)
			{
				//printf("buttons: %d\tsticks: %d\n", thisJoystick->buttons.size(), thisJoystick->sticks.size());
				updateJoystick(thisJoystick);
			} else {

				// This joystick is disconnected.

				// @todo notify Lua
				printf("[Joystick][%d] disconnected\n", i);

			}
		}
	}
}

void SledgeInputManager::setDeadzones
	(float p_thumbLeft, float p_thumbRight, float p_trigger, float p_joystick)
{
	SledgeInputManager::deadzone_thumbLeft = p_thumbLeft;
	SledgeInputManager::deadzone_thumbRight = p_thumbRight;
	SledgeInputManager::deadzone_trigger = p_trigger;
	SledgeInputManager::deadzone_joystick = p_joystick;
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

void SledgeInputManager::updateController( SledgeController* p_sledgecontroller )
{
	// sticks
	p_sledgecontroller->stick_left = postprocessThumbstick(
		p_sledgecontroller->controller,
		SDL_CONTROLLER_AXIS_LEFTX,
		SDL_CONTROLLER_AXIS_LEFTY,
		deadzone_thumbLeft
		);
	p_sledgecontroller->stick_right = postprocessThumbstick(
		p_sledgecontroller->controller,
		SDL_CONTROLLER_AXIS_RIGHTX,
		SDL_CONTROLLER_AXIS_RIGHTY,
		deadzone_thumbRight
		);
	p_sledgecontroller->triggers.x = postprocessTrigger(
		p_sledgecontroller->controller,
		SDL_CONTROLLER_AXIS_TRIGGERLEFT,
		deadzone_trigger
		);
	p_sledgecontroller->triggers.y = postprocessTrigger(
		p_sledgecontroller->controller,
		SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
		deadzone_trigger
		);
	AKUEnqueueJoystickEvent (
		p_sledgecontroller->device_id,
		SledgePadSensorAxes::PS_STICK_LEFT,
		p_sledgecontroller->stick_left.x,
		p_sledgecontroller->stick_left.y
		);
	AKUEnqueueJoystickEvent (
		p_sledgecontroller->device_id,
		SledgePadSensorAxes::PS_STICK_RIGHT,
		p_sledgecontroller->stick_right.x,
		p_sledgecontroller->stick_right.y
		);
	AKUEnqueueJoystickEvent (
		p_sledgecontroller->device_id,
		SledgePadSensorAxes::PS_TRIGGERS,
		p_sledgecontroller->triggers.x,
		p_sledgecontroller->triggers.y
		);

	// buttons
	for(int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
	{
		bool bDownThisFrame = SDL_GameControllerGetButton(p_sledgecontroller->controller, (SDL_CONTROLLER_BUTTON)i) == 1;
		bool bDown =
			bDownThisFrame == p_sledgecontroller->buttons[i] ||
			(bDownThisFrame == true && p_sledgecontroller->buttons[i] == false);


		if(p_sledgecontroller->buttons[i] == true || bDownThisFrame == true)
		{
			AKUEnqueueKeyboardEvent(
				p_sledgecontroller->device_id,
				SledgePadSensorAxes::PS_BUTTONS,
				i,
				bDown
				);
		}
		p_sledgecontroller->buttons[i] = bDownThisFrame;
	}
}

void SledgeInputManager::updateJoystick( SledgeJoystick* p_sledgejoystick )
{
	// sticks
	for (int i = 0; i < p_sledgejoystick->sticks.size(); ++i)
	{
		float X_raw = (float)SDL_JoystickGetAxis(p_sledgejoystick->joystick, 0 + i * 2);
		float Y_raw = (float)SDL_JoystickGetAxis(p_sledgejoystick->joystick, 1 + i * 2);
		p_sledgejoystick->sticks[i] = postprocessStick(X_raw, Y_raw, deadzone_joystick);

		AKUEnqueueJoystickEvent (
			p_sledgejoystick->device_id,
			i,
			p_sledgejoystick->sticks[i].x,
			p_sledgejoystick->sticks[i].y
			);
	}

	// buttons
	int num_buttons = p_sledgejoystick->buttons.size();//SDL_JoystickNumButtons(p_sledgejoystick->joystick);
	//printf("num_buttons: %d \n", num_buttons);
	for (int i = 0; i < p_sledgejoystick->buttons.size(); ++i)
	{
		bool bDownThisFrame = SDL_JoystickGetButton(p_sledgejoystick->joystick, i) == 1;
		bool bDown = 
			bDownThisFrame == p_sledgejoystick->buttons[i] ||
			(bDownThisFrame == true && p_sledgejoystick->buttons[i] == false);

		if(bDownThisFrame == true || p_sledgejoystick->buttons[i] == true)
		{
			AKUEnqueueKeyboardEvent(
				p_sledgejoystick->device_id,
				p_sledgejoystick->sticks.size(),
				i,
				bDown
				);
		}
		p_sledgejoystick->buttons[i] = bDownThisFrame;
	}
}
 
 