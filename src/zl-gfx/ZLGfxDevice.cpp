// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLGfx-gles.h>
#include <zl-gfx/ZLGfxDevice.h>
#include <zl-gfx/ZLGfxEnum.h>

//================================================================//
// globals
//================================================================//

static u32	sMaxTextureUnits			= 0;
static u32	sMaxTextureSize				= 0;
static u32	sOperationDepth				= 0; // this is just the counter for tracking begin/end calls

//================================================================//
// ZLGfxDevice
//================================================================//

//----------------------------------------------------------------//
void ZLGfxDevice::Begin () {

	//zglBegin ();
}

//----------------------------------------------------------------//
void ZLGfxDevice::End () {

	//zglEnd ();
}

//----------------------------------------------------------------//
void ZLGfxDevice::Finalize () {

	//zglFinalize ();
}

//----------------------------------------------------------------//
u32 ZLGfxDevice::GetCap ( u32 cap ) {
	
	switch ( cap ) {
		case ZGL_CAPS_MAX_TEXTURE_SIZE:
			return sMaxTextureSize;
		case ZGL_CAPS_MAX_TEXTURE_UNITS:
			return sMaxTextureUnits;
	}
	return 0;
}

//----------------------------------------------------------------//
u32 ZLGfxDevice::GetError () {

	GLenum error = glGetError ();

	switch ( error ) {
		case GL_NO_ERROR:			return ZGL_ERROR_NONE;
		case GL_INVALID_ENUM:		return ZGL_ERROR_INVALID_ENUM;
		case GL_INVALID_OPERATION:	return ZGL_ERROR_INVALID_OPERATION;
		case GL_INVALID_VALUE:		return ZGL_ERROR_INVALID_VALUE;
		case GL_OUT_OF_MEMORY:		return ZGL_ERROR_OUT_OF_MEMORY;
	}
	return ZGL_ERROR_UNKNOWN;
}

//----------------------------------------------------------------//
cc8* ZLGfxDevice::GetErrorString ( u32 error ) {

	switch ( error ) {
		case ZGL_ERROR_NONE:				return "ZGL_ERROR_NONE";
		case ZGL_ERROR_INVALID_ENUM:		return "ZGL_ERROR_INVALID_ENUM";
		case ZGL_ERROR_INVALID_OPERATION:	return "ZGL_ERROR_INVALID_OPERATION";
		case ZGL_ERROR_INVALID_VALUE:		return "ZGL_ERROR_INVALID_VALUE";
		case ZGL_ERROR_OUT_OF_MEMORY:		return "ZGL_ERROR_OUT_OF_MEMORY";
		case ZGL_ERROR_STACK_OVERFLOW:		return "ZGL_ERROR_STACK_OVERFLOW";
		case ZGL_ERROR_STACK_UNDERFLOW:		return "ZGL_ERROR_STACK_UNDERFLOW";
	}
	return "";
}

//----------------------------------------------------------------//
cc8* ZLGfxDevice::GetString ( u32 stringID ) {

	return ( cc8* )glGetString ( ZLGfxEnum::MapZLToNative ( stringID ));
}

//----------------------------------------------------------------//
void ZLGfxDevice::Initialize () {

	u32 majorVersion = 0;
	u32 minorVersion = 0;

	bool isOpenGLES = false;

	#ifdef __GLEW_H__
		static bool initGlew = true;
		if ( initGlew ) {
			glewInit ();
			initGlew = false;
		}
	#endif

	STLString version = ZLGfxDevice::GetString ( ZGL_STRING_VERSION );
	std::transform ( version.begin (), version.end(), version.begin(), ::tolower );
	
	STLString gles = "opengl es";

	if ( version.find ( gles ) != version.npos ) {
		isOpenGLES = true;
		version = version.substr ( gles.length ());

		size_t space = version.find ( ' ' );
		if ( space != version.npos ) {
			version = version.substr ( space + 1 );
		}
	}
	else {
		isOpenGLES = false;
	}

	version = version.substr ( 0, 3 );

	majorVersion = version.at ( 0 ) - '0';
	minorVersion = version.at ( 2 ) - '0';

	#ifdef EMSCRIPTEN 
		isOpenGLES = true;
	#endif
	
	#if defined ( __GLEW_H__ )

		// if framebuffer object is not in code, check to see if it's available as
		// an extension and remap to core function pointers if so
		if (( isOpenGLES == false ) && ( majorVersion < 3 )) {

			if ( glewIsSupported ( "GL_EXT_framebuffer_object" )) {

				REMAP_EXTENSION_PTR ( glBindFramebuffer,						glBindFramebufferEXT )
				REMAP_EXTENSION_PTR ( glCheckFramebufferStatus,					glCheckFramebufferStatusEXT )
				REMAP_EXTENSION_PTR ( glDeleteFramebuffers,						glDeleteFramebuffersEXT )
				REMAP_EXTENSION_PTR ( glDeleteRenderbuffers,					glDeleteRenderbuffersEXT )
				REMAP_EXTENSION_PTR ( glFramebufferRenderbuffer,				glFramebufferRenderbufferEXT )
				REMAP_EXTENSION_PTR ( glFramebufferTexture1D,					glFramebufferTexture1DEXT )
				REMAP_EXTENSION_PTR ( glFramebufferTexture2D,					glFramebufferTexture2DEXT )
				REMAP_EXTENSION_PTR ( glFramebufferTexture3D,					glFramebufferTexture3DEXT )
				REMAP_EXTENSION_PTR ( glGenFramebuffers,						glGenFramebuffersEXT )
				REMAP_EXTENSION_PTR ( glGenRenderbuffers,						glGenRenderbuffersEXT )
				REMAP_EXTENSION_PTR ( glGenerateMipmap,							glGenerateMipmapEXT )
				REMAP_EXTENSION_PTR ( glGetFramebufferAttachmentParameteriv,	glGetFramebufferAttachmentParameterivEXT )
				REMAP_EXTENSION_PTR ( glGetRenderbufferParameteriv,				glGetRenderbufferParameterivEXT )
				REMAP_EXTENSION_PTR ( glIsFramebuffer,							glIsFramebufferEXT )
				REMAP_EXTENSION_PTR ( glIsRenderbuffer,							glIsRenderbufferEXT )
				REMAP_EXTENSION_PTR ( glRenderbufferStorage,					glRenderbufferStorageEXT )
			}
			else {
				assert ( false ); // needs framebuffer
			}
		}

	#endif

	int maxTextureUnits = 0;

	if ( majorVersion == 1 ) {
		assert ( false ); // OpenGL ES1 no longer supported
	}
	else {
		glGetIntegerv ( GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits );
	}

	sMaxTextureUnits = ( u32 )maxTextureUnits;

	int maxTextureSize;
	glGetIntegerv ( GL_MAX_TEXTURE_SIZE, &maxTextureSize );
	sMaxTextureSize = ( u32 )maxTextureSize;
}
