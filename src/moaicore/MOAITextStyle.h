// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTSTYLE_H
#define	MOAITEXTSTYLE_H

#include <moaicore/MOAILua.h>

class MOAIFont;

//================================================================//
// MOAITextStyleState
//================================================================//
class MOAITextStyleState {
protected:

	friend class MOAITextBox;
	friend class MOAITextDesigner;
	friend class MOAITextStyler;

	MOAIFont*	mFont;
	float		mSize;
	u32			mColor;

public:
	
	//----------------------------------------------------------------//
	bool		IsMatch					( const MOAITextStyleState& compare ) const;
};

//================================================================//
// MOAITextStyle
//================================================================//
class MOAITextStyle :
	public MOAILuaObject,
	public MOAITextStyleState {
private:

	//----------------------------------------------------------------//
	static int		_setColor				( lua_State* L );
	static int		_setFont				( lua_State* L );
	static int		_setSize				( lua_State* L );
	
public:

	friend class MOAITextDesigner;

	DECL_LUA_FACTORY ( MOAITextStyle )
	
	GET ( MOAIFont*, Font, mFont );
	GET_SET ( float, Size, mSize );
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
