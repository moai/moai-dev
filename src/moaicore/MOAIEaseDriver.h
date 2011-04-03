// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIEASEDRIVER_H
#define MOAIEASEDRIVER_H

#include <moaicore/MOAIAction.h>

class MOAINode;

//================================================================//
// MOAIEaseDriverLink
//================================================================//
class MOAIEaseDriverLink {
public:
	USRef < MOAINode >	mTarget;
	u32					mAttrID;
	float				mValue;
	u32					mMode;
};

//================================================================//
// MOAIEaseDriver
//================================================================//
/**	@brief	Action that applies simple ease curves to node attributes.
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
	bool			IsBusy				();
					MOAIEaseDriver		();
					~MOAIEaseDriver		();
	void			OnUpdate			( float step );
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	void			ReserveLinks		( u32 total );
	void			SetLink				( u32 idx, MOAINode* target, u32 attrID, float force, u32 mode );
	void			SetLink				( u32 mode );
	STLString		ToString			();
};

#endif
