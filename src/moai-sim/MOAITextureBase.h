// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTUREBASE_H
#define	MOAITEXTUREBASE_H

#include <moai-sim/MOAIGfxResource.h>

//================================================================//
// MOAITextureBase
//================================================================//
/**	@lua	MOAITextureBase
	@text	Base class for texture resources.

	@const	GL_LINEAR
	@const	GL_LINEAR_MIPMAP_LINEAR
	@const	GL_LINEAR_MIPMAP_NEAREST
	@const	GL_NEAREST
	@const	GL_NEAREST_MIPMAP_LINEAR
	@const	GL_NEAREST_MIPMAP_NEAREST
*/
class MOAITextureBase :
	public virtual MOAILuaObject,
	public virtual ZLTexture {
protected:

	friend class MOAIGfxMgr;
	friend class ZLGfxStateGPUCache;
	friend class MOAIImageFormat;

	//----------------------------------------------------------------//
	static int			_getSize				( lua_State* L );
	static int			_release				( lua_State* L );
	static int			_setDebugName			( lua_State* L );
	static int			_setFilter				( lua_State* L );
	static int			_setWrap				( lua_State* L );
	
	//----------------------------------------------------------------//
//	virtual ZLTexture&		MOAITextureBase_AsZLTexture		() = 0;
	
public:

	//----------------------------------------------------------------//
						MOAITextureBase				();
						~MOAITextureBase			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
//	ZLTexture&			AsZLTexture					();
};

#endif
