#include "stdafx.h"
#include <uslscore/uslscore.h>
#include <moaicore-config.h>
#include "SledgeInputHandler.h"


//================================================================//
// lua
//================================================================//

/**
 * Sets gamepad deadzones.
 *
 * @author	Jetha Chan
 * @date	14/09/2013
 *
 * @param [in,out]	L	If non-null, the lua_State* to process.
 *
 * @return	.
 */
int SledgeInputWrapper::_setDeadzones( lua_State* L ) {
	MOAI_LUA_SETUP ( SledgeInputWrapper, "UNNNN" )

	float left = state.GetValue < float >( 2, 0.0f );
	float right = state.GetValue < float >( 3, 0.0f );
	float triggers = state.GetValue < float >( 4, 0.0f );
	float joystick = state.GetValue < float >( 5, 0.0f );

	printf(
		"Setting new deadzones...\t%0.2f\t%0.2f\t%0.2f\t%0.2f\n",
		left,
		right,
		triggers,
		joystick
	);

	if(_manager != NULL)
		_manager->setDeadzones(
			left,
			right,
			triggers,
			joystick
		);

	return 0;
}

/**
 *	Get the local name for the specified SDL scancode (numeric).
 *
 *	@param	scancode	SDL keyboard scancode.
 *	@return	name		Local name.
 */
int SledgeInputWrapper::_getNameForKeybScancode( lua_State* L )
{
	MOAI_LUA_SETUP ( SledgeInputWrapper, "UN" );

	// get the scancode
	SDL_Scancode sc = (SDL_Scancode)(state.GetValue<int>(2, 0));

	// get the name
	const char* name = SDL_GetScancodeName(sc);

	// ...okay, let's see how this works.
	lua_pushstring(L, name);

	// tell lua that we actually returned a thing.
	return 1;
}

//================================================================//
// SledgeInputHandler
//================================================================//

//----------------------------------------------------------------//
SledgeInputWrapper::SledgeInputWrapper () {
	
	// register all classes SledgeInputHandler derives from
	// we need this for custom RTTI implementation
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		
		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( SledgeInputHandlerBase )
	RTTI_END
}

//----------------------------------------------------------------//
SledgeInputWrapper::~SledgeInputWrapper () {
}

//----------------------------------------------------------------//
void SledgeInputWrapper::RegisterLuaClass ( MOAILuaState& state ) {

	// call any initializers for base classes here:
	// SledgeInputHandlerBase::RegisterLuaClass ( state );

	// also register constants:
	// state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );

	// here are the class methods:
	luaL_Reg regTable [] = {
		{ "setDeadzones", _setDeadzones }, 
		{ "getNameForKeybScancode", _getNameForKeybScancode },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
	printf ( "SledgeInputHandler::RegisterLuaClass\n" );
}

//----------------------------------------------------------------//
void SledgeInputWrapper::RegisterLuaFuncs ( MOAILuaState& state ) {
	// here are the instance methods:
	luaL_Reg regTable [] = {
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

SledgeInputManager* SledgeInputWrapper::_manager = NULL;

//----------------------------------------------------------------//
void SledgeInputWrapper::SetManager( SledgeInputManager* p_manager )
{
	SledgeInputWrapper::_manager = p_manager;
}

void SledgeInputWrapper::AKUInit()
{
	// Set input configuration name.
	AKUSetInputConfigurationName(SLEDGE_NAMESPACE::INPUTCONFIGNAME);
	
	AKUReserveInputDevices(SLEDGE_NAMESPACE::ID_TOTAL);

	// Do device (keyboard+mouse) init.
	SledgeDevice dvc;
	dvc.device_id = SLEDGE_NAMESPACE::ID_DEVICE;
	dvc.name = "KBAM";
}


void SledgeInputWrapper::DoAKUInit_Device( SledgeDevice* p_sledgedevice )
{
	SLEDGE_NAMESPACE::InputDevice_ID p_id = p_sledgedevice->device_id;

	AKUSetInputDevice(
		p_id,
		SLEDGE_NAMESPACE::InputDevice::DeviceName[p_id]
	);

	// tell MOAI what this device is named
	AKUSetInputDeviceExtendedName(
		p_id,
		p_sledgedevice->name
		);

	// reserve keyboard and mouse sensors
	AKUReserveInputDeviceSensors(
		p_id,
		SLEDGE_NAMESPACE::IDS_TOTAL
		);

	// keyboard
	AKUSetInputDeviceKeyboard(
		p_id,
		SLEDGE_NAMESPACE::IDS_KEYBOARD,
		SLEDGE_NAMESPACE::DeviceSensor::SensorName[SLEDGE_NAMESPACE::IDS_KEYBOARD]
	);

	// mouse
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

