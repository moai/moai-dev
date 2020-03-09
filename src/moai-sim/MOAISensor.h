// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	u32			mType;
	STLString	mName;
	double		mTimestamp;

	SET ( u32, Type, mType );

	//----------------------------------------------------------------//
	static int		_getTimestamp		( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass	( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs	( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	friend class MOAIInputContext;
	friend class MOAIInputDevice;
	friend class MOAIInputMgr;

	//----------------------------------------------------------------//
	virtual void	ClearState			() {}
	virtual void	ParseEvent			( ZLStream& eventStream ) = 0;
					MOAISensor			();
					~MOAISensor			();
	virtual void	ResetState			() {}
};

#endif
