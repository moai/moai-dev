// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIINPUTDEVICE_H
#define MOAIINPUTDEVICE_H

#include <moaicore/MOAILua.h>

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
	bool		mIsActive;

	USLeanArray < MOAISensor* > mSensors;

	//----------------------------------------------------------------//
	MOAISensor*		GetSensor			( u8 sensorID );

public:

	friend class MOAIInputMgr;

	DECL_LUA_FACTORY ( MOAIInputDevice )

	GET_SET ( STLString, Name, mName );
	GET_SET ( bool, Active, mIsActive );

	//----------------------------------------------------------------//
	void			HandleEvent			( u8 sensorID, USStream& eventStream );
					MOAIInputDevice		();
					~MOAIInputDevice	();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			ReserveSensors		( u8 total );
	void			Reset				();
	void			SetSensor			( u8 sensorID, cc8* name, u32 type );
};

#endif
