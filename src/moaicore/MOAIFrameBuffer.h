// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFER_H
#define	MOAIFRAMEBUFFER_H

#include <moaicore/MOAIGfxResource.h>

//================================================================//
// MOAIFrameBuffer
//================================================================//
class MOAIFrameBuffer :
	public MOAIGfxResource {
private:
	
	u32					mWidth;
	u32					mHeight;
	
	GLuint				mGLFrameBufferID;
	
	GLuint				mGLColorBufferID;
	GLuint				mGLDepthBufferID;
	GLuint				mGLStencilBufferID;
	
	GLenum				mColorFormat;
	GLenum				mDepthFormat;
	GLenum				mStencilFormat;
	
	//----------------------------------------------------------------//
	static int		_init					( lua_State* L );
	
	//----------------------------------------------------------------//
	virtual bool		IsRenewable				();
	virtual void		OnBind					();
	virtual void		OnClear					();
	virtual void		OnLoad					();
	virtual void		OnRenew					();
	virtual void		OnUnload				();

public:
	
	friend class MOAITexture;
	
	//----------------------------------------------------------------//
	void				Init					( u32 width, u32 height, GLenum colorFormat, GLenum depthFormat, GLenum stencilFormat );
	bool				IsValid					();
						MOAIFrameBuffer			();
						~MOAIFrameBuffer		();
};

#endif
