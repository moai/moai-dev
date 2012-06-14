// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFER_H
#define	MOAIFRAMEBUFFER_H

#include <moaicore/MOAITextureBase.h>

//================================================================//
// MOAIFrameBuffer
//================================================================//
/**	@name	MOAIFrameBuffer
	@text	This is an implementation of a frame buffer that may be
			attached to a MOAILayer for offscreen rendering. It is
			also a texture that may be bound and used like any other.
*/
class MOAIFrameBuffer :
	public MOAITextureBase {
private:
	
	GLbitfield			mClearFlags;
	u32					mClearColor;
	
	GLuint				mGLFrameBufferID;
	
	GLuint				mGLColorBufferID;
	GLuint				mGLDepthBufferID;
	GLuint				mGLStencilBufferID;
	
	GLenum				mColorFormat;
	GLenum				mDepthFormat;
	GLenum				mStencilFormat;

	//----------------------------------------------------------------//
	static int			_init					( lua_State* L );
	static int			_setClearColor			( lua_State* L );
	static int			_setClearDepth			( lua_State* L );
	
	//----------------------------------------------------------------//
	virtual bool		IsRenewable				();
	virtual void		OnCreate				();
	virtual void		OnDestroy				();
	virtual void		OnInvalidate			();
	virtual void		OnLoad					();

public:
	
	friend class MOAIGfxDevice;
	friend class MOAITextureBase;
	
	DECL_LUA_FACTORY ( MOAIFrameBuffer )
	
	//----------------------------------------------------------------//
	void				BindAsFrameBuffer		();
	void				Init					( u32 width, u32 height, GLenum colorFormat, GLenum depthFormat, GLenum stencilFormat );
	bool				IsValid					();
						MOAIFrameBuffer			();
						~MOAIFrameBuffer		();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
