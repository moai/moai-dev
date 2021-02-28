// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTSTYLE_H
#define	MOAITEXTSTYLE_H

class MOAIFont;
class MOAIShader;

//================================================================//
// MOAITextStyleState
//================================================================//
class MOAITextStyleState {
protected:

	friend class MOAITextLabel;
	friend class MOAITextLayoutEngine;
	friend class MOAITextLayout;
	friend class MOAITextStyle;
	friend class MOAITextStyleParser;
	friend class MOAITextStyleCache;
	friend class MOAITextStyleMap;

	ZLStrongPtr < MOAIShader > mShader;
	
	MOAIFont*		mFont;
	float			mSize;
	ZLVec2D			mScale;
	u32				mColor;
	ZLRect			mPadding;

public:

	GET ( MOAIFont*, Font, mFont );
	GET_SET ( float, Size, mSize );
	GET_SET ( u32, Color, mColor );

	//----------------------------------------------------------------//
	void				AffirmGlyph				( u32 c );
	virtual void		Init					( MOAITextStyleState& style );
						MOAITextStyleState		();
	virtual				~MOAITextStyleState		();
	bool				NeedsLayout				( const MOAITextStyleState& compare ) const;
	virtual void		SetFont					( MOAIFont* font );
	virtual void		SetShader				( MOAIShader* shader );
};

//================================================================//
// MOAITextStyle
//================================================================//
/**	@lua	MOAITextStyle
	@text	Represents a style that may be applied to a text box or a
			section of text in a text box using a style escape.
*/
class MOAITextStyle :
	public virtual MOAINode,
	public virtual MOAITextStyleState {
private:

	friend class MOAITextLabel;
	friend class MOAITextLayoutEngine;
	friend class MOAITextLayout;
	friend class MOAITextStyleParser;
	friend class MOAITextStyleCache;
	friend class MOAITextStyleMap;

	//----------------------------------------------------------------//
	static int		_getColor				( lua_State* L );
	static int		_getFont				( lua_State* L );
	static int		_getScale				( lua_State* L );
	static int		_getSize				( lua_State* L );
	static int		_setColor				( lua_State* L );
	static int		_setFont				( lua_State* L );
	static int		_setShader				( lua_State* L );
	static int		_setPadding				( lua_State* L );
	static int		_setScale				( lua_State* L );
	static int		_setSize				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	
public:

	DECL_LUA_FACTORY ( MOAITextStyle )
	
	//----------------------------------------------------------------//
	void			Init					( MOAITextStyleState& style );
					MOAITextStyle			( ZLContext& context );
					~MOAITextStyle			();
	void			SetFont					( MOAIFont* font );
	void			SetShader				( MOAIShader* shader );
};

#endif
