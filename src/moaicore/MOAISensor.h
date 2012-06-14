// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISENSOR_H
#define MOAISENSOR_H

//================================================================//
// MOAISensor
//================================================================//
/**	@name	MOAISensor
	@text	Base class for sensors.
*/
class MOAISensor :
	public MOAILuaObject {
protected:

	u32	mType;
	STLString mName;

public:

	friend class MOAIInputMgr;
	friend class MOAIInputDevice;

	enum {
		BUTTON,
		COMPASS,
		JOYSTICK,
		KEYBOARD,
		LEVEL,
		LOCATION,
		POINTER,
		TOUCH,
		WHEEL,
	};

	//----------------------------------------------------------------//
	virtual void	HandleEvent			( USStream& eventStream ) = 0;
					MOAISensor			();
					~MOAISensor			();
	void			RegisterLuaClass	( MOAILuaState& state );
	virtual void	Reset				() {}
};

#endif
