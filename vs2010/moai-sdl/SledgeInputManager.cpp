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

	// Tell AKU about four gamepads.
	doAKUPadInit(MAX_GAMECONTROLLERS);
	doAKUJoyInit(MAX_JOYSTICKS);

	// Iterate over connected "joysticks".
	num_devices_connected = SDL_NumJoysticks();

	_numjoysticks_lasttick = SDL_NumJoysticks();

	int gc_i = 0;
	int jy_i = 0;
	for (int i = 0; i < num_devices_connected; ++i)
	{

		if (SDL_IsGameController(i))
		{
			// GAME CONTROLLER
			connectController2(i, gc_i);

			++gc_i;
		} else
		{
			// REGULAR JOYSTICK
			/*
			SledgeJoystick stick;
			stick.joystick = SDL_JoystickOpen(i);
			stick.connected = SDL_JoystickGetAttached(stick.joystick) == SDL_TRUE;

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
			*/
			connectJoystick(i, jy_i);
			
			++jy_i;
		}
	}
	printf("\njoystick count: %d (m_joysticks length: %d)\n", jy_i, m_joysticks.size());
	printf("controllers count: %d (m_controllers length: %d)\n", gc_i, m_controllers.size());
}


bool SledgeInputManager::connectController2(int idx_device, int idx_gamepad)
{
	SledgeController pad;

	bool bUseExistingBinding = false;

	printf("Attempting to connect controller %d at index %d\n", idx_gamepad, idx_device);

	// iterate through m_controllers and m_joysticks looking for an existing 
	// binding for this idx
	printf("\tm_controllers status:");
	for (
		std::list<SledgeController>::iterator it = m_controllers.begin();
		it != m_controllers.end();
		++it
		)
	{
		SledgeController* thisController = (SledgeController*)&*it;

		printf(" %d", thisController->index);

		if(thisController->index == idx_device)
		{
			printf("<-[found!]");
			bUseExistingBinding = true;

			// we found an existing controller binding for this idx.
			// maybe we can reuse it?
			if(SDL_GameControllerGetAttached(thisController->controller) != 1)
			{
				// reconnect it
				printf("\n\t\treconnecting [%d]\n", idx_device);
				//SDL_GameControllerClose(thisController->controller);
				thisController->controller = SDL_GameControllerOpen(idx_device);
				printf("\t\tSDL_GameControllerOpen()\n");
			}
			printf("\t\SDL_GameControllerNameForIndex()\n");
			thisController->name = const_cast<char*>(SDL_GameControllerNameForIndex(idx_device));
			printf("\t\AKUSetInputDeviceActive()\n");
			AKUSetInputDeviceActive(
				thisController->device_id,
				true
			);

			printf(
				"\t\t connecting controller %d with pad%d\n",
				thisController->index,
				thisController->index_controller
				);


			break;
		}
	}
	printf("\n");
	for (
		std::list<SledgeJoystick>::iterator it = m_joysticks.begin();
		it != m_joysticks.end();
		)
	{
		SledgeJoystick* thisJoystick = (SledgeJoystick*)&it;

		if(thisJoystick->index == idx_device)
		{
			// we found an existing joystick binding for this idx.
			// let's destroy it.
			SDL_JoystickClose(thisJoystick->joystick);
			it = m_joysticks.erase(it);
			break;			
		} else {
			++it;
		}
	}

	if(!bUseExistingBinding)
	{
		printf("\t...use new binding\n");

		pad.controller = SDL_GameControllerOpen(idx_device);
		pad.connected = SDL_GameControllerGetAttached(pad.controller) == 1;

		//printf("\t\t connected?");
		if(pad.connected)
		{
			//printf("YES\n");
		} else {
			//printf("NO\n");
		}

		for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; ++j)
		{
			pad.buttons.push_back(false);
		}
		char* controllerName =
			const_cast<char*>(SDL_GameControllerNameForIndex(idx_device));
		if (!controllerName) controllerName = "Unknown controller";
		pad.name = controllerName;
		pad.index = idx_device;
		pad.index_controller = idx_gamepad;//m_controllers.size();

		pad.device_id = (SledgeInputDevice::InputDevice_ID)((int)SledgeInputDevice::ID_PAD_0 + pad.index_controller);

		printf(
			"\t\t connecting controller %d with pad%d\n",
			pad.index,
			pad.index_controller
			);

		AKUSetInputDeviceExtendedName(
			pad.device_id,
			pad.name
			);
		AKUSetInputDeviceActive(
			pad.device_id,
			true
			);

		m_controllers.push_back(pad);
	} else {
		printf("\t...use existing binding\n");
	}

	return true;
}

bool SledgeInputManager::connectJoystick(int idx_device, int idx_joystick)
{
	SledgeJoystick stick;
	printf("-----------------\n");
	printf("Attempting to connect joystick %d at index %d\n", idx_joystick, idx_device);

	// unlike gamecontrollers, we can't assume that all joysticks are created
	// equal. instead of iterating through our list looking for something to
	// reuse, we're going to look for something to delete.
	printf("\tm_joysticks status (size %d):", m_joysticks.size());
	for (
		std::list<SledgeJoystick>::iterator it = m_joysticks.begin();
		it != m_joysticks.end();
		)
	{
		SledgeJoystick* thisJoystick = (SledgeJoystick*)&*it;
		printf(" %d", thisJoystick->index);

		if(thisJoystick->index == idx_device)
		{
			printf("<-[found!]");
			SDL_JoystickClose(thisJoystick->joystick);
			thisJoystick->joystick = NULL;
			it = m_joysticks.erase(it);
		} else {
			++it;
		}
	}

	// at this point we should be able to create a new m_joysticks entry
	stick.joystick = SDL_JoystickOpen(idx_device);
	stick.connected = SDL_JoystickGetAttached(stick.joystick) == SDL_TRUE;
	char* joystickName = 
		const_cast<char*>(SDL_JoystickName(stick.joystick));
	if (!joystickName) joystickName = "Unknown joystick";
	stick.name = joystickName;

	stick.index = idx_device;
	stick.index_joystick = idx_joystick;
	stick.device_id = (SledgeInputDevice::InputDevice_ID)(SledgeInputDevice::ID_JOY_0 + idx_joystick);

	AKUSetInputDeviceExtendedName(
		stick.device_id,
		stick.name
		);
	AKUSetInputDeviceActive(
		stick.device_id,
		true
		);

	// buttons and sticks
	stick.sticks.clear();
	for (int i = 0; i < MAX_JOYSTICK_STICKS; ++i)
	{
		vec2f thisStick;
		stick.sticks.push_back(thisStick);
	}
	int num_buttons = SDL_JoystickNumButtons(stick.joystick);
	stick.buttons.clear();
	for (int i = 0; i < num_buttons; ++i)
	{
		stick.buttons.push_back(false);
	}
	m_joysticks.push_back(stick);
	printf("\tm_joysticks status (size %d) \n", m_joysticks.size());
	printf("-----------------\n");

	return true;
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
		printf("doAKUDeviceInit(IDT_PAD)\n");
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



void SledgeInputManager::doAKUPadInit(int p_padcount)
{
	char pad_name[64];
	for (int i = 0; i < p_padcount; ++i)
	{
		sprintf(pad_name, "pad%d", i);

		int deviceid = (int)SledgeInputDevice::ID_PAD_0 + i;


		AKUSetInputDevice(
			deviceid,
			pad_name
			);

		// default to inactive
		AKUSetInputDeviceActive(
			deviceid,
			false
		);

		AKUReserveInputDeviceSensors(
			deviceid,
			SledgePadSensorAxes::PS_TOTAL
			);
		AKUSetInputDeviceJoystick(
			deviceid,
			SledgePadSensorAxes::PS_STICK_LEFT,
			SledgePadSensorAxes::SensorName[SledgePadSensorAxes::PS_STICK_LEFT]
		);
		AKUSetInputDeviceJoystick(
			deviceid,
			SledgePadSensorAxes::PS_STICK_RIGHT,
			SledgePadSensorAxes::SensorName[SledgePadSensorAxes::PS_STICK_RIGHT]
		);
		AKUSetInputDeviceJoystick(
			deviceid,
			SledgePadSensorAxes::PS_TRIGGERS,
			SledgePadSensorAxes::SensorName[SledgePadSensorAxes::PS_TRIGGERS]
		);
		AKUSetInputDeviceKeyboard(
			deviceid,
			SledgePadSensorAxes::PS_BUTTONS, 
			SledgePadSensorAxes::SensorName[SledgePadSensorAxes::PS_BUTTONS]
		);

	}
}

void SledgeInputManager::doAKUJoyInit(int p_joycount)
{
	char joy_name[64];
	for (int i = 0; i < p_joycount; ++i)
	{
		sprintf(joy_name, "joy%d", i);

		int deviceid = (int)SledgeInputDevice::ID_JOY_0 + i;

		AKUSetInputDevice(
			deviceid,
			joy_name
			);

		// reserve sensors - # of sticks, +1 for the buttons
		AKUReserveInputDeviceSensors(
			deviceid,
			MAX_JOYSTICK_STICKS + 1
			);
		char stick_name[64];
		for (int i = 0; i < MAX_JOYSTICK_STICKS; ++i)
		{
			sprintf(stick_name, "stick%d", i);
			AKUSetInputDeviceJoystick(
				deviceid,
				i,
				stick_name
				);
		}
		AKUSetInputDeviceKeyboard(
			deviceid,
			MAX_JOYSTICK_STICKS, // not a typo!
			"buttons"
			);
	}
}

void SledgeInputManager::initPad(
	SledgeController* p_sledgecontroller
)
{
	char pad_name[64];
	sprintf(pad_name, "pad%d", p_sledgecontroller->index_controller);

	printf("initPad()\t%s\t%s\n", pad_name, p_sledgecontroller->name);

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

	int _numjoysticks_thistick = SDL_NumJoysticks();

	//printf("# of joysticks connected: %d\n", _numjoysticks);

	if(_numjoysticks_lasttick != _numjoysticks_thistick)
	{
		// # of joysticks connected has changed!
		printf("# of joysticks connected: %d\n", _numjoysticks_thistick);

		// deactivate all controllers and sticks for the time being
		for (int i = 0; i < MAX_GAMECONTROLLERS; ++i)
		{
			AKUSetInputDeviceActive((int)(i+SledgeInputDevice::ID_PAD_0), false);
		}
		for (int i = 0; i < MAX_JOYSTICKS; ++i)
		{
			AKUSetInputDeviceActive((int)(i+SledgeInputDevice::ID_JOY_0), false);
		}
		for (
			std::list<SledgeController>::iterator it = m_controllers.begin();
			it != m_controllers.end();
			++it
		)
		{
			SledgeController* cont = (SledgeController*)&*it;
			SDL_GameControllerClose(cont->controller);
			cont->controller = NULL;
		}

		int gc_count = 0;
		for (int i = 0; i < _numjoysticks_thistick; ++i)
		{

			if (SDL_IsGameController(i))
			{
				printf("Controller joystick at index %d\n", i);
				connectController2(i, gc_count);
				gc_count++;
			} else {
				printf("Non-controller joystick at index %d\n", i);
				connectJoystick(i, i - gc_count);
			}
		}

		printf("m_controllers.size(): %d\n", m_controllers.size());
	}

	SDL_GameControllerEventState(SDL_QUERY);	
	if(m_controllers.size() != 0)
	{	
		int i = 0;
		for (
			std::list<SledgeController>::iterator it = m_controllers.begin();
			it != m_controllers.end();
			++it
			/**
			int i = 0;
			i < m_controllers.size();
			++i
			*/
		)
		{
			SledgeController* thisController = (SledgeController*)&*it;//m_controllers[i];

			// I love how this function returns an integer instead of an SDL_bool.
			bool bIsAttached = SDL_GameControllerGetAttached(thisController->controller) == 1;
			if(bIsAttached)
			{
				//printf("buttons: %d\n", thisController->buttons.size());
				//if(i == 1)
				updateController(thisController);
				/*
				printf(
					"[Controller %d][%s][%d] connected\n",
					thisController->index,
					thisController->name,
					i
				);
				*/
			} else {

				// This controller is disconnected.

				// @todo notify Lua
				/*
				printf(
					"[Controller %d][%s][%d] disconnected\n",
					thisController->index,
					thisController->name,
					i
				);
				*/
			}
			++i;
		}
	}
	SDL_JoystickEventState(SDL_QUERY);
	SDL_JoystickUpdate();
	//printf("m_joysticks size: %d\n", m_joysticks.size());
	if(m_joysticks.size() != 0)
	{
		//printf("m_joysticks: %d\n", m_joysticks.size());
		for (
			std::list<SledgeJoystick>::iterator it = m_joysticks.begin();
			it != m_joysticks.end();
			++it
			/**
			int i = 0;
			i < m_joysticks.size();
			++i
			*/
		)
		{
			SledgeJoystick* thisJoystick = (SledgeJoystick*)&*it;//m_joysticks[i];

			bool bIsAttached = SDL_JoystickGetAttached(thisJoystick->joystick) == SDL_TRUE;

			if(bIsAttached)
			{
				//printf("buttons: %d\tsticks: %d\n", thisJoystick->buttons.size(), thisJoystick->sticks.size());
				updateJoystick(thisJoystick);
			} else {

				// This joystick is disconnected.

				// @todo notify Lua
				//printf("[Joystick][%d] disconnected\n", i);

			}
		}
	}
	_numjoysticks_lasttick = _numjoysticks_thistick;

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

	//8printf("[device %d]\n", p_sledgecontroller->device_id);
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
	//printf("updateJoystick(%d)\n", p_sledgejoystick->index);

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
 
 