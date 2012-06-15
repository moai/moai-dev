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

	MOAIFont*	mFont;
	float		mSize;
	float		mScale;
	u32			mColor;

public:

	//----------------------------------------------------------------//
				MOAITextStyleState		();
				~MOAITextStyleState		();
	bool		NeedsLayout				( const MOAITextStyleState& compare ) const;

};

//================================================================//
// MOAITextStyle
//================================================================//
/**	@name	MOAITextStyle
	@text	Represents a style that may be applied to a text box or a
			secion of text in a text box using a style escape.
*/
class MOAITextStyle :
	public MOAINode,
	public MOAITextStyleState {
private:

	friend class MOAITextBox;
	friend class MOAITextDesigner;
	friend class MOAITextStyler;

	//----------------------------------------------------------------//
	static int		_getColor				( lua_State* L );
	static int		_getFont				( lua_State* L );
	static int		_getScale				( lua_State* L );
	static int		_getSize				( lua_State* L );
	static int		_setColor				( lua_State* L );
	static int		_setFont				( lua_State* L );
	static int		_setScale				( lua_State* L );
	static int		_setSize				( lua_State* L );
	
public:

	DECL_LUA_FACTORY ( MOAITextStyle )
	
	GET ( MOAIFont*, Font, mFont );
	GET ( float, Size, mSize );
	GET_SET ( u32, Color, mColor );
	
	//----------------------------------------------------------------//
	void			AffirmGlyph				( u32 c );
	void			Init					( MOAITextStyle& style );
					MOAITextStyle			();
					~MOAITextStyle			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			SetFont					( MOAIFont* font );
	void			SetSize					( float size );
};

#endif
