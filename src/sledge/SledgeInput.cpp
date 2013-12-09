#include "stdafx.h"
#include "SledgeInput.h"

float SledgeInput::deadzone_thumbLeft = 0.0f;
float SledgeInput::deadzone_thumbRight = 0.0f;
float SledgeInput::deadzone_trigger = 0.0f;
float SledgeInput::deadzone_joystick = 0.0f;


SledgeInput::SledgeInput()
{
	// register all classes SledgeInput derives from
	// we need this for custom RTTI implementation
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )

		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( SledgeInput )
	RTTI_END

}

SledgeInput::~SledgeInput()
{
}

void SledgeInput::RegisterLuaClass(MOAILuaState& state)
{
	luaL_reg regTable[] = {
		{ "getNameForKeybScancode", _getNameForKeybScancode },
		{ NULL, NULL}
	};
	luaL_register(state, 0, regTable);
}

void SledgeInput::RegisterLuaFuncs(MOAILuaState& state) {
	luaL_Reg regTable[] = {
		{ NULL, NULL}
	};
	luaL_register(state, 0, regTable);
}

/**
 *	Get the local name for the specified SDL scancode (numeric).
 *
 *	@param	scancode	SDL keyboard scancode.
 *	@return	name		Local name.
 */
int SledgeInput::_getNameForKeybScancode( lua_State* L )
{
	MOAI_LUA_SETUP ( SledgeInput, "UN" );

	// get the scancode
	SDL_Scancode sc = (SDL_Scancode)(state.GetValue<int>(2, 0));

	// get the name
	const char* name = SDL_GetScancodeName(sc);

	// ...okay, let's see how this works.
	lua_pushstring(L, name);

	// tell lua that we actually returned a thing.
	return 1;
}

/**
 *	Perform initial AKU initialization - set a name for the input configuration,
 *	and reserve the correct number of devices (four pads, four joysticks).
 */
void SledgeInput::AKUInit()
{
	// Set input configuration name.
	AKUSetInputConfigurationName(SLEDGE_NAMESPACE::INPUTCONFIGNAME);
	AKUReserveInputDevices(SLEDGE_NAMESPACE::InputDevice_ID::ID_TOTAL);

	// Do device (i.e. keyboard + mouse, in the case of a computer) init.
	SledgeDevice dvc;
	dvc.device_id = SLEDGE_NAMESPACE::InputDevice_ID::ID_DEVICE;
	dvc.name = "KBAM";
	initDevice(&dvc);

	// Disable controller events; we'll manually poll, thanks.
	SDL_GameControllerEventState(SDL_IGNORE);
	SDL_JoystickEventState(SDL_IGNORE);

	// Set the default thumbstick and trigger deadzones.
	setDeadzones(
		(f32)LEFT_THUMB_DEADZONE,
		(f32)RIGHT_THUMB_DEADZONE,
		(f32)TRIGGER_THRESHOLD,
		(f32)JOYSTICK_DEADZONE
	);

	// Tell AKU about four gamepads and four joysticks.
	doAKUPadInit(MAX_GAMECONTROLLERS);
	doAKUJoyInit(MAX_JOYSTICKS);

	// Actually "connect" the joysticks.
	num_devices_connected = SDL_NumJoysticks();
	num_joysticks_lasttick = SDL_NumJoysticks();
	u32 gc_i = 0;
	u32 jy_i = 0;
	for (u32 i = 0; i < num_devices_connected; ++i)
	{
		if(SDL_IsGameController(i))
		{
			// GAME CONTROLLER
			connectController(i, gc_i);
			++gc_i;
		} else {
			// REGULAR JOYSTICK
			connectJoystick(i, jy_i);
			++jy_i;
		}
	}
	printf("\njoystick count: %d (m_joysticks length: %d)\n", jy_i, m_joysticks.size());
	printf("controllers count: %d (m_controllers length: %d)\n", gc_i, m_controllers.size());
	
}


/**
 *	Given a SledgeDevice structure, tell AKU about it.
 */
void SledgeInput::initDevice(
	SledgeDevice* p_sledgedevice
	)
{
	SLEDGE_NAMESPACE::InputDevice_ID p_id = p_sledgedevice->device_id;

	AKUSetInputDevice(
		p_id,
		SLEDGE_NAMESPACE::InputDevice::DeviceName[p_id]
	);
	AKUSetInputDeviceExtendedName(
		p_id,
		p_sledgedevice->name
		);

	AKUReserveInputDeviceSensors(
		p_id,
		SLEDGE_NAMESPACE::IDS_TOTAL
		);
	AKUSetInputDeviceKeyboard(
		p_id,
		SLEDGE_NAMESPACE::IDS_KEYBOARD,
		SLEDGE_NAMESPACE::DeviceSensor::SensorName[SLEDGE_NAMESPACE::IDS_KEYBOARD]
	);
	AKUSetInputDevicePointer(
		p_id,
		SLEDGE_NAMESPACE::IDS_POINTER,
		SLEDGE_NAMESPACE::DeviceSensor::SensorName[SLEDGE_NAMESPACE::IDS_POINTER]
	);
	AKUSetInputDeviceButton(
		p_id,
		SLEDGE_NAMESPACE::IDS_MOUSE_LEFT,
		SLEDGE_NAMESPACE::DeviceSensor::SensorName[SLEDGE_NAMESPACE::IDS_MOUSE_LEFT]
	);
	AKUSetInputDeviceButton(
		p_id,
		SLEDGE_NAMESPACE::IDS_MOUSE_MIDDLE,
		SLEDGE_NAMESPACE::DeviceSensor::SensorName[SLEDGE_NAMESPACE::IDS_MOUSE_MIDDLE]
	);
	AKUSetInputDeviceButton(
		p_id,
		SLEDGE_NAMESPACE::IDS_MOUSE_RIGHT,
		SLEDGE_NAMESPACE::DeviceSensor::SensorName[SLEDGE_NAMESPACE::IDS_MOUSE_RIGHT]
	);
}

/**
 *	Given a number of gamepads, tell AKU about them.
 */
void SledgeInput::doAKUPadInit(int p_padcount)
{
	char pad_name[64];
	for (int i = 0; i < p_padcount; ++i)
	{
		sprintf(pad_name, "pad%d", i);

		int deviceid = (int)SLEDGE_NAMESPACE::ID_PAD_0 + i;


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
			SLEDGE_NAMESPACE::PS_TOTAL
			);
		AKUSetInputDeviceJoystick(
			deviceid,
			SLEDGE_NAMESPACE::PS_STICK_LEFT,
			SLEDGE_NAMESPACE::PadSensorAxes::SensorName[SLEDGE_NAMESPACE::PS_STICK_LEFT]
		);
		AKUSetInputDeviceJoystick(
			deviceid,
			SLEDGE_NAMESPACE::PS_STICK_RIGHT,
			SLEDGE_NAMESPACE::PadSensorAxes::SensorName[SLEDGE_NAMESPACE::PS_STICK_RIGHT]
		);
		AKUSetInputDeviceJoystick(
			deviceid,
			SLEDGE_NAMESPACE::PS_TRIGGERS,
			SLEDGE_NAMESPACE::PadSensorAxes::SensorName[SLEDGE_NAMESPACE::PS_TRIGGERS]
		);
		AKUSetInputDeviceKeyboard(
			deviceid,
			SLEDGE_NAMESPACE::PS_BUTTONS, 
			SLEDGE_NAMESPACE::PadSensorAxes::SensorName[SLEDGE_NAMESPACE::PS_BUTTONS]
		);

	}
}

/**
 *	Given a number of joysticks, tell AKU about them.
 */
void SledgeInput::doAKUJoyInit(int p_joycount)
{
	char joy_name[64];
	for (int i = 0; i < p_joycount; ++i)
	{
		sprintf(joy_name, "joy%d", i);

		int deviceid = (int)SLEDGE_NAMESPACE::ID_JOY_0 + i;

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

void SledgeInput::setDeadzones
	(float p_thumbLeft, float p_thumbRight, float p_trigger, float p_joystick)
{
	SledgeInput::deadzone_thumbLeft = p_thumbLeft;
	SledgeInput::deadzone_thumbRight = p_thumbRight;
	SledgeInput::deadzone_trigger = p_trigger;
	SledgeInput::deadzone_joystick = p_joystick;
}

bool SledgeInput::connectController(int idx_device, int idx_gamepad)
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

		pad.device_id = (SLEDGE_NAMESPACE::InputDevice_ID)((int)SLEDGE_NAMESPACE::ID_PAD_0 + pad.index_controller);

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

bool SledgeInput::connectJoystick(int idx_device, int idx_joystick)
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
	stick.device_id = (SLEDGE_NAMESPACE::InputDevice_ID)(SLEDGE_NAMESPACE::ID_JOY_0 + idx_joystick);

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
		vec2<f32> thisStick;
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