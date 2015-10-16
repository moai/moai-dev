// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISENSOR_H
#define MOAISENSOR_H

class MOAIInputMgr;

//================================================================//
// MOAISensor
//================================================================//
/**	@lua	MOAISensor
	@text	Base class for sensors.
*/
class MOAISensor :
	public MOAILuaObject {
protected:

	u32			mType;
	STLString	mName;
	double		mTimestamp;

	//----------------------------------------------------------------//
	static int	_getTimestamp			( lua_State* L );

	SET ( u32, Type, mType );

public:

	friend class MOAIInputContext;
	friend class MOAIInputDevice;
	friend class MOAIInputMgr;

	//----------------------------------------------------------------//
	virtual void	ClearState			() {}
	virtual void	ParseEvent			( ZLStream& eventStream ) = 0;
					MOAISensor			();
					~MOAISensor			();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	virtual void	ResetState			() {}
};

#endif
