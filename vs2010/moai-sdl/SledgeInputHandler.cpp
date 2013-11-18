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
int SledgeInputHandler::_setDeadzones( lua_State* L ) {
	MOAI_LUA_SETUP ( SledgeInputHandler, "UNNNN" )

	printf ( "SledgeInputHandler setDeadzones!\n" );
	float left = state.GetValue < float >( 2, 0.0f );
	float right = state.GetValue < float >( 3, 0.0f );
	float triggers = state.GetValue < float >( 4, 0.0f );
	float joystick = state.GetValue < float >( 5, 0.0f );

	printf("%0.2f\t%0.2f\t%0.2f\t%0.2f\t\n", left, right, triggers, joystick);

	if(_manager != NULL)
		_manager->setDeadzones(
			left,
			right,
			triggers,
			joystick
		);

	return 0;
}


//================================================================//
// SledgeInputHandler
//================================================================//

//----------------------------------------------------------------//
SledgeInputHandler::SledgeInputHandler () {
	
	// register all classes SledgeInputHandler derives from
	// we need this for custom RTTI implementation
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		
		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( SledgeInputHandlerBase )
	RTTI_END
}

//----------------------------------------------------------------//
SledgeInputHandler::~SledgeInputHandler () {
}

//----------------------------------------------------------------//
void SledgeInputHandler::RegisterLuaClass ( MOAILuaState& state ) {

	// call any initializers for base classes here:
	// SledgeInputHandlerBase::RegisterLuaClass ( state );

	// also register constants:
	// state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );

	// here are the class methods:
	luaL_Reg regTable [] = {
		{ "setDeadzones", _setDeadzones }, 
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
	printf ( "SledgeInputHandler::RegisterLuaClass\n" );
}

//----------------------------------------------------------------//
void SledgeInputHandler::RegisterLuaFuncs ( MOAILuaState& state ) {
	// here are the instance methods:
	luaL_Reg regTable [] = {
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

SledgeInputManager* SledgeInputHandler::_manager = NULL;

//----------------------------------------------------------------//
void SledgeInputHandler::SetManager( SledgeInputManager* p_manager )
{
	SledgeInputHandler::_manager = p_manager;
}

void SledgeInputHandler::AKUInit()
{
	// Set input configuration name.
	AKUSetInputConfigurationName(SLEDGE_NAMESPACE::INPUTCONFIGNAME);
	AKUReserveInputDevices(SLEDGE_NAMESPACE::InputDevice_ID::ID_TOTAL);

	// Do device (keyboard+mouse) init.
	SledgeDevice dvc;
	dvc.device_id = SLEDGE_NAMESPACE::InputDevice_ID::ID_DEVICE;
	dvc.name = "KBAM";
}


void SledgeInputHandler::DoAKUInit_Device( SledgeDevice* p_sledgedevice )
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

