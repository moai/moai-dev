// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERTEXTURE_H
#define	MOAIFRAMEBUFFERTEXTURE_H

#include <moaicore/MOAIFrameBuffer.h>
#include <moaicore/MOAITextureBase.h>

//================================================================//
// MOAIFrameBufferTexture
//================================================================//
/**	@name	MOAIFrameBufferTexture
	@text	This is an implementation of a frame buffer that may be
			attached to a MOAILayer for offscreen rendering. It is
			also a texture that may be bound and used like any other.
*/
class MOAIFrameBufferTexture :
	public MOAIFrameBuffer,
	public MOAITextureBase {
private:
	
	GLuint				mGLColorBufferID;
	GLuint				mGLDepthBufferID;
	GLuint				mGLStencilBufferID;
	
	GLenum				mColorFormat;
	GLenum				mDepthFormat;
	GLenum				mStencilFormat;
	
	//----------------------------------------------------------------//
	static int			_init					( lua_State* L );
	
	//----------------------------------------------------------------//
	virtual bool		IsRenewable				();
	virtual void		OnCreate				();
	virtual void		OnDestroy				();
	virtual void		OnInvalidate			();
	virtual void		OnLoad					();

public:
	
	friend class MOAIGfxDevice;
	friend class MOAITextureBase;
	
	DECL_LUA_FACTORY ( MOAIFrameBufferTexture )
	
	//----------------------------------------------------------------//
	void				Init						( u32 width, u32 height, GLenum colorFormat, GLenum depthFormat, GLenum stencilFormat );
	bool				IsValid						();
						MOAIFrameBufferTexture		();
						~MOAIFrameBufferTexture		();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				Render						();
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
