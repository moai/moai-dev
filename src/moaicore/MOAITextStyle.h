// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTSTYLE_H
#define	MOAITEXTSTYLE_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITextStyler.h>
#include <moaicore/MOAITextStyleMap.h>
#include <moaicore/MOAITransform.h>

class MOAIAnimCurve;
class MOAIFont;

//================================================================//
// MOAITextStyle
//================================================================//
class MOAITextStyle :
	public MOAILuaObject {
private:

	MOAILuaSharedPtr < MOAIFont > mFont;
	
	float	mPoints;
	u32		mColor;

	//----------------------------------------------------------------//
	static int		_setFont				( lua_State* L );
	
public:

	DECL_LUA_FACTORY ( MOAITextStyle )
	
	GET ( MOAIFont*, Font, mFont );
	GET_SET ( float, Points, mPoints );
	GET_SET ( u32, Color, mColor );
	
	//----------------------------------------------------------------//
					MOAITextStyle			();
					~MOAITextStyle			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			SetFont					( MOAIFont* font );
};

#endif
