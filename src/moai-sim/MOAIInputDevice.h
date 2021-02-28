// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIINPUTDEVICE_H
#define MOAIINPUTDEVICE_H

class MOAISensor;

//================================================================//
// MOAIInputDevice
//================================================================//
/**	@lua	MOAIInputDevice
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
	MOAISensor*		GetSensor				( ZLIndex sensorID );
	void			SetSensor				( ZLIndex sensorID, cc8* name, MOAISensor* sensor );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	friend class MOAIInputContext;
	friend class MOAIInputMgr;

	DECL_LUA_FACTORY ( MOAIInputDevice )

	GET_SET ( STLString, Name, mName );
	GET_SET ( bool, Active, mIsActive );

	//----------------------------------------------------------------//
	void			ClearSensorState		();
					MOAIInputDevice			( ZLContext& context );
					~MOAIInputDevice		();
	void			ReserveSensors			( ZLSize total );
	void			ResetSensorState		();
	void			SetHardwareInfo			( cc8* hardwareInfo );
};

#endif
