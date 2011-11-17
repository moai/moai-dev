// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIEASEDRIVER_H
#define MOAIEASEDRIVER_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIWeakPtr.h>

class MOAINode;

//================================================================//
// MOAIEaseDriverLink
//================================================================//
class MOAIEaseDriverLink {
public:
	
	MOAIWeakPtr < MOAINode >	mSource;
	u32							mSourceAttrID;
	
	MOAIWeakPtr < MOAINode >	mDest;
	u32							mDestAttrID;
	
	float						mV0;
	float						mV1;
	u32							mMode;
	float						mValue;
};

//================================================================//
// MOAIEaseDriver
//================================================================//
/**	@name MOAIEaseDriver
	@text Action that applies simple ease curves to node attributes.
*/
class MOAIEaseDriver :
	public MOAIAction {
private:

	USLeanArray < MOAIEaseDriverLink > mLinks;
	
	float	mTime;
	float	mLength;

	//----------------------------------------------------------------//
	static int		_reserveLinks		( lua_State* L );
	static int		_setLength			( lua_State* L );
	static int		_setLink			( lua_State* L );

	//----------------------------------------------------------------//
	void			Apply				( float step );

public:

	DECL_LUA_FACTORY ( MOAIEaseDriver )

	SET ( float, Length, mLength )

	//----------------------------------------------------------------//
	bool			IsDone				();
					MOAIEaseDriver		();
					~MOAIEaseDriver		();
	void			OnUpdate			( float step );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			ReserveLinks		( u32 total );
	void			SetLink				( u32 idx, MOAINode* dest, u32 destAttrID, float v1, u32 mode );
	void			SetLink				( u32 idx, MOAINode* dest, u32 destAttrID, MOAINode* source, u32 sourceAttrID, u32 mode );
};

#endif
