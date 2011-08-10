// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFER_H
#define	MOAIFRAMEBUFFER_H

//================================================================//
// MOAIFrameBuffer
//================================================================//
class MOAIFrameBuffer {
private:
	
	bool				mError;
	
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
	bool			Bind					();
	void			Clear					();

public:
	
	friend class MOAITexture;
	
	//----------------------------------------------------------------//
	void			Init					( u32 width, u32 height, GLenum colorFormat, GLenum depthFormat, GLenum stencilFormat );
	bool			IsOK					();
					MOAIFrameBuffer			();
					~MOAIFrameBuffer		();
	STLString		ToString				();
};

#endif
