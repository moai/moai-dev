// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTUREBASEGL_H
#define	MOAITEXTUREBASEGL_H

#include <moai-gfx/MOAIGfxResourceGL.h>

//================================================================//
// MOAITextureBaseGL
//================================================================//
/**	@lua	MOAITextureBaseGL
	@text	Base class for texture resources.

	@const	GL_LINEAR
	@const	GL_LINEAR_MIPMAP_LINEAR
	@const	GL_LINEAR_MIPMAP_NEAREST
	@const	GL_NEAREST
	@const	GL_NEAREST_MIPMAP_LINEAR
	@const	GL_NEAREST_MIPMAP_NEAREST
*/
class MOAITextureBaseGL :
	public virtual MOAILuaObject,
	public virtual ZLTextureBaseGL {
protected:

	friend class MOAITextureFormat;

	//----------------------------------------------------------------//
	static int			_getSize				( lua_State* L );
	static int			_release				( lua_State* L );
	static int			_setDebugName			( lua_State* L );
	static int			_setFilter				( lua_State* L );
	static int			_setWrap				( lua_State* L );
	
public:

	//----------------------------------------------------------------//
						MOAITextureBaseGL			();
						~MOAITextureBaseGL			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
