// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGE_H
#define	MOAIIMAGE_H

//================================================================//
// MOAIImage
//================================================================//
/**	@name	MOAIImage
	@text	Image/bitmap class.
	
	@flag	POW_TWO
	@flag	QUANTIZE
	@flag	TRUECOLOR
	@flag	PREMULTIPLY_ALPHA
	
	@flag	PIXEL_FMT_TRUECOLOR
	@flag	PIXEL_FMT_INDEX_4
	@flag	PIXEL_FMT_INDEX_8
	
	@flag	COLOR_FMT_A_8
	@flag	COLOR_FMT_RGB_888
	@flag	COLOR_FMT_RGB_565
	@flag	COLOR_FMT_RGBA_5551
	@flag	COLOR_FMT_RGBA_4444
	@flag	COLOR_FMT_RGBA_8888
*/
class MOAIImage :
	public virtual USLuaObject,
	public USImage {
private:

	//----------------------------------------------------------------//
	static int		_bleedRect			( lua_State* L );
	static int		_convertColors		( lua_State* L );
	static int		_copy				( lua_State* L );
	static int		_copyBits			( lua_State* L );
	static int		_getColor32			( lua_State* L );
	static int		_getFormat			( lua_State* L );
	static int		_getRGBA			( lua_State* L );
	static int		_getSize			( lua_State* L );
	static int		_init				( lua_State* L );
	static int		_load				( lua_State* L );
	static int		_padToPow2			( lua_State* L );
	static int		_resizeCanvas		( lua_State* L );
	static int		_setColor32			( lua_State* L );
	static int		_setRGBA			( lua_State* L );
	static int		_writePNG			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIImage )
	
	//----------------------------------------------------------------//
						MOAIImage				();
						~MOAIImage				();
	void				SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void				SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	void				RegisterLuaClass		( USLuaState& state );
	void				RegisterLuaFuncs		( USLuaState& state );
	STLString			ToString				();
};

#endif
