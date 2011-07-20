// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONT_H
#define	MOAIFONT_H

class MOAIDataBuffer;
class MOAIImage;
class MOAITexture;

//================================================================//
// MOAIFont
//================================================================//
/**	@name	MOAIFont
	@text	Font class.
*/
class MOAIFont :
	public virtual USLuaObject,
	public USFont {
private:

	USRef < MOAITexture >	mTexture;
	USRef < MOAIImage >		mImage;

	//----------------------------------------------------------------//
	static int	_getImage			( lua_State* L );
	static int	_getScale			( lua_State* L );
	static int	_getTexture			( lua_State* L );
	static int	_load				( lua_State* L );
	static int	_loadFromTTF		( lua_State* L );
	static int	_setImage			( lua_State* L );
	static int	_setTexture			( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIFont )
	
	//----------------------------------------------------------------//
	USFont*			Bind				();
					MOAIFont			();
					~MOAIFont			();
	void			LoadFont			( MOAIDataBuffer& fontImageData, cc8* charCodes );
	void			LoadFont			( cc8* fontImageFileName, cc8* charCodes );
	void			LoadFontFromTTF		( cc8* filename, cc8* charCodes, float points, u32 dpi );
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	void			Render				();
	void			SerializeIn			( USLuaState& state, USLuaSerializer& serializer );
	void			SerializeOut		( USLuaState& state, USLuaSerializer& serializer );
	void			SetImage			( MOAIImage* image );
	void			SetTexture			( MOAITexture* texture );
	STLString		ToString			();
};

#endif
