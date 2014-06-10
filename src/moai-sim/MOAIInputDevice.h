// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIINPUTDEVICE_H
#define MOAIINPUTDEVICE_H

class MOAISensor;

//================================================================//
// MOAIInputDevice
//================================================================//
/**	@name	MOAIInputDevice
	@text	Manager class for input bindings. Has no public methods.
*/
class MOAIInputDevice :
	public virtual MOAILuaObject {
private:

	STLString	mName;
	STLString	mHardwareInfo;
	bool		mIsActive;

	ZLLeanArray < MOAISensor* > mSensors;

	//----------------------------------------------------------------//
	static int		_getHardwareInfo		( lua_State* L );

	//----------------------------------------------------------------//
	MOAISensor*		GetSensor				( u8 sensorID );

public:

	friend class MOAIInputContext;
	friend class MOAIInputMgr;

	DECL_LUA_FACTORY ( MOAIInputDevice )

	GET_SET ( STLString, Name, mName );
	GET_SET ( bool, Active, mIsActive );

	//----------------------------------------------------------------//
					MOAIInputDevice			();
					~MOAIInputDevice		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			ReserveSensors			( u8 total );
	void			ResetSensors			();
	void			SetHardwareInfo			( cc8* hardwareInfo );
	void			SetSensor				( u8 sensorID, cc8* name, u32 type );
};

#endif
