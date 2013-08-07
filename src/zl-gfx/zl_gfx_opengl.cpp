// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/headers.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_GFX_OPENGL

#include <string>
using namespace std;

#ifdef MOAI_OS_WINDOWS
	#define GLEW_STATIC
	#include <gl/glew.h>
#endif

#ifdef MOAI_OS_OSX
	//#include <GL/glew.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glext.h>
#endif

#ifdef MOAI_OS_IPHONE
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>
	#import <OpenGLES/ES2/gl.h>
	#import <OpenGLES/ES2/glext.h>
	
	// TODO: replace this w/ runtime ogl extension checks
	#define MOAI_TEST_PVR
	
	#define GL_RGBA8 GL_RGBA8_OES
	
#endif

#ifdef MOAI_OS_ANDROID
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif

#ifdef MOAI_OS_LINUX
	#ifndef MOAI_OS_NACL
		#ifndef ANDROID
			#include <GL/glew.h>
		#endif
	#endif
#endif

#ifdef MOAI_OS_NACL
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>

	#define GL_RGBA8 GL_RGBA8_OES
#endif

#ifdef MOAI_OS_HTML
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#define MOAI_OS_NACL 1
	#define GL_RGBA8 GL_RGBA8_OES
#endif



#ifdef MOAI_OS_BLACKBERRY
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	
	#define GL_RGBA8 GL_RGBA8_OES
#endif


#define REMAP_EXTENSION_PTR(target, ext) target = target ? target : ext;

//================================================================//
// globals
//================================================================//

static bool	sIsFramebufferSupported		= false;
static bool	sIsOpenGLES					= false;
static bool	sIsProgrammable				= false;
static u32	sMaxTextureUnits			= 0;
static u32	sMaxTextureSize				= 0;

//================================================================//
// enums
//================================================================//

//----------------------------------------------------------------//
//----------------------------------------------------------------//
GLenum _remapEnum ( u32 zglEnum ) {
	
	switch ( zglEnum ) {
		  
	  case ZGL_BLEND_FACTOR_DST_ALPHA:				return GL_DST_ALPHA;
		case ZGL_BLEND_FACTOR_DST_COLOR:				return GL_DST_COLOR;
		case ZGL_BLEND_FACTOR_ONE:						return GL_ONE;
		case ZGL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:		return GL_ONE_MINUS_DST_ALPHA;
		case ZGL_BLEND_FACTOR_ONE_MINUS_DST_COLOR:		return GL_ONE_MINUS_DST_COLOR;
		case ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:		return GL_ONE_MINUS_SRC_ALPHA;
		case ZGL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:		return GL_ONE_MINUS_SRC_COLOR;
		case ZGL_BLEND_FACTOR_SRC_ALPHA:				return GL_SRC_ALPHA;
		case ZGL_BLEND_FACTOR_SRC_ALPHA_SATURATE:		return GL_SRC_ALPHA_SATURATE;
		case ZGL_BLEND_FACTOR_SRC_COLOR:				return GL_SRC_COLOR;
		case ZGL_BLEND_FACTOR_ZERO:						return GL_ZERO;

		case ZGL_BLEND_MODE_ADD:						return GL_FUNC_ADD;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
  		case ZGL_BLEND_MODE_MAX:						return GL_MAX;
  		case ZGL_BLEND_MODE_MIN:						return GL_MIN;
		#endif

		case ZGL_BLEND_MODE_REVERSE_SUBTRACT:			return GL_FUNC_REVERSE_SUBTRACT;
		case ZGL_BLEND_MODE_SUBTRACT:					return GL_FUNC_SUBTRACT;

		case ZGL_BUFFER_TARGET_ELEMENT_ARRAY:			return GL_ELEMENT_ARRAY_BUFFER;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
		case ZGL_BUFFER_USAGE_DYNAMIC_COPY:				return GL_DYNAMIC_COPY;
    #endif 
    
		case ZGL_BUFFER_USAGE_DYNAMIC_DRAW:				return GL_DYNAMIC_DRAW;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
  		case ZGL_BUFFER_USAGE_DYNAMIC_READ:				return GL_DYNAMIC_READ;
  		case ZGL_BUFFER_USAGE_STATIC_COPY:				return GL_STATIC_COPY;
    #endif
    
		case ZGL_BUFFER_USAGE_STATIC_DRAW:				return GL_STATIC_DRAW;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
  		case ZGL_BUFFER_USAGE_STATIC_READ:				return GL_STATIC_READ;
  		case ZGL_BUFFER_USAGE_STREAM_COPY:				return GL_STREAM_COPY;
		#endif
		
		case ZGL_BUFFER_USAGE_STREAM_DRAW:				return GL_STREAM_DRAW;
		
		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
		  case ZGL_BUFFER_USAGE_STREAM_READ:				return GL_STREAM_READ;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_BLACKBERRY )
		  case ZGL_COMPOSE_MODULATE:						return GL_MODULATE;
    #endif
    
		case ZGL_CULL_ALL:								return GL_FRONT_AND_BACK;
		case ZGL_CULL_BACK:								return GL_BACK;
		case ZGL_CULL_FRONT:							return GL_FRONT;

		case ZGL_DEPTH_ALWAYS:							return GL_ALWAYS;
		case ZGL_DEPTH_EQUAL:							return GL_EQUAL;
		case ZGL_DEPTH_LESS:							return GL_LESS;
		case ZGL_DEPTH_LEQUAL:							return GL_LEQUAL;
		case ZGL_DEPTH_GEQUAL:							return GL_GEQUAL;
		case ZGL_DEPTH_GREATER:							return GL_GREATER;
		case ZGL_DEPTH_NEVER:							return GL_NEVER;
		case ZGL_DEPTH_NOTEQUAL:						return GL_NOTEQUAL;

		case ZGL_FRAMEBUFFER_ATTACHMENT_COLOR:			return GL_COLOR_ATTACHMENT0;
		case ZGL_FRAMEBUFFER_ATTACHMENT_DEPTH:			return GL_DEPTH_ATTACHMENT;
		case ZGL_FRAMEBUFFER_ATTACHMENT_STENCIL:		return GL_STENCIL_ATTACHMENT;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
  		case ZGL_FRAMEBUFFER_TARGET_DRAW:				return GL_DRAW_FRAMEBUFFER;
  		case ZGL_FRAMEBUFFER_TARGET_READ:				return GL_READ_FRAMEBUFFER;
		#endif

		case ZGL_FRAMEBUFFER_TARGET_DRAW_READ:			return GL_FRAMEBUFFER;

    #if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
  		case ZGL_MATRIX_COLOR:							return GL_COLOR;
  		case ZGL_MATRIX_MODELVIEW:						return GL_MODELVIEW;
  		case ZGL_MATRIX_PROJECTION:						return GL_PROJECTION;
		#endif
		
		case ZGL_MATRIX_TEXTURE:						return GL_TEXTURE;

		case ZGL_PIPELINE_BLEND:						return GL_BLEND;
		
		#if !defined ( MOAI_OS_NACL )
		  case ZGL_PIPELINE_COLOR_ARRAY:					return GL_COLOR_ARRAY;
		#endif
		case ZGL_PIPELINE_CULL:							return GL_CULL_FACE;
		case ZGL_PIPELINE_DEPTH:						return GL_DEPTH_TEST;

		#if !defined ( MOAI_OS_NACL )
		  case ZGL_PIPELINE_NORMAL_ARRAY:					return GL_NORMAL_ARRAY;
    #endif
    
		case ZGL_PIPELINE_SCISSOR:						return GL_SCISSOR_TEST;
		case ZGL_PIPELINE_TEXTURE_2D:					return GL_TEXTURE_2D;

		#if !defined ( MOAI_OS_NACL )
  		case ZGL_PIPELINE_TEXTURE_COORD_ARRAY:			return GL_TEXTURE_COORD_ARRAY;
  		case ZGL_PIPELINE_VERTEX_ARRAY:					return GL_VERTEX_ARRAY;
    #endif

		case ZGL_PIXEL_FORMAT_ALPHA:						return GL_ALPHA;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
		  case ZGL_PIXEL_FORMAT_RED:							return GL_RED;
		  case ZGL_PIXEL_FORMAT_RG:							return GL_RG;
		#endif
		
		case ZGL_PIXEL_FORMAT_RGB:							return GL_RGB;
		
		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
		  case ZGL_PIXEL_FORMAT_RGB4:							return GL_RGB4;
		#endif
		
		case ZGL_PIXEL_FORMAT_RGB5_A1:						return GL_RGB5_A1;

		#if defined ( MOAI_OS_ANDROID )
			case ZGL_PIXEL_FORMAT_RGB565:							return GL_RGB565;
		#endif
		
		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
		  case ZGL_PIXEL_FORMAT_RGB8:							return GL_RGB8;
		  case ZGL_PIXEL_FORMAT_BGR:							return GL_BGR;
		#endif
		
		case ZGL_PIXEL_FORMAT_RGBA:							return GL_RGBA;
		case ZGL_PIXEL_FORMAT_RGBA4:						return GL_RGBA4;

		#if !defined ( MOAI_OS_ANDROID )
			case ZGL_PIXEL_FORMAT_RGBA8:						return GL_RGBA8;
		#endif
			
		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_ANDROID )
		  case ZGL_PIXEL_FORMAT_BGRA:							return GL_BGRA;
    #endif
    
		case ZGL_PIXEL_TYPE_BYTE:							return GL_BYTE;
		case ZGL_PIXEL_TYPE_FLOAT:							return GL_FLOAT;
		case ZGL_PIXEL_TYPE_INT:							return GL_INT;
		case ZGL_PIXEL_TYPE_SHORT:							return GL_SHORT;
		case ZGL_PIXEL_TYPE_UNSIGNED_BYTE:					return GL_UNSIGNED_BYTE;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
		  case ZGL_PIXEL_TYPE_UNSIGNED_BYTE_2_3_3_REV:		return GL_UNSIGNED_BYTE_2_3_3_REV;
		  case ZGL_PIXEL_TYPE_UNSIGNED_BYTE_3_3_2:			return GL_UNSIGNED_BYTE_3_3_2;
    #endif

		case ZGL_PIXEL_TYPE_UNSIGNED_INT:					return GL_UNSIGNED_INT;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
		  case ZGL_PIXEL_TYPE_UNSIGNED_INT_8_8_8_8:			return GL_UNSIGNED_INT_8_8_8_8;
  		case ZGL_PIXEL_TYPE_UNSIGNED_INT_8_8_8_8_REV:		return GL_UNSIGNED_INT_8_8_8_8_REV;
  		case ZGL_PIXEL_TYPE_UNSIGNED_INT_2_10_10_10_REV:	return GL_UNSIGNED_INT_2_10_10_10_REV;
  		case ZGL_PIXEL_TYPE_UNSIGNED_INT_10_10_10_2:		return GL_UNSIGNED_INT_10_10_10_2;
    #endif
    
		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT:					return GL_UNSIGNED_SHORT;
		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_6_5:			return GL_UNSIGNED_SHORT_5_6_5;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
		  case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_6_5_REV:		return GL_UNSIGNED_SHORT_5_6_5_REV;
    #endif

		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4:			return GL_UNSIGNED_SHORT_4_4_4_4;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
  		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4_REV:		return GL_UNSIGNED_SHORT_4_4_4_4_REV;
  		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_1_5_5_5_REV:		return GL_UNSIGNED_SHORT_1_5_5_5_REV;
    #endif

		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1:			return GL_UNSIGNED_SHORT_5_5_5_1;

		case ZGL_PRIM_LINE_LOOP:						return GL_LINE_LOOP;
		case ZGL_PRIM_LINE_STRIP:						return GL_LINE_STRIP;
		case ZGL_PRIM_LINES:							return GL_LINES;
		case ZGL_PRIM_POINTS:							return GL_POINTS;
		case ZGL_PRIM_TRIANGLE_FAN:						return GL_TRIANGLE_FAN;	
		case ZGL_PRIM_TRIANGLE_STRIP:					return GL_TRIANGLE_STRIP;
		case ZGL_PRIM_TRIANGLES:						return GL_TRIANGLES;

		case ZGL_PROGRAM_INFO_ACTIVE_ATTRIBUTES:			return GL_ACTIVE_ATTRIBUTES;
		case ZGL_PROGRAM_INFO_ACTIVE_ATTRIBUTE_MAX_LENGTH:	return GL_ACTIVE_ATTRIBUTE_MAX_LENGTH;
		case ZGL_PROGRAM_INFO_ACTIVE_UNIFORMS:				return GL_ACTIVE_UNIFORMS;
		case ZGL_PROGRAM_INFO_ACTIVE_UNIFORM_MAX_LENGTH:	return GL_ACTIVE_UNIFORM_MAX_LENGTH;
		case ZGL_PROGRAM_INFO_ATTACHED_SHADERS:				return GL_ATTACHED_SHADERS;
		case ZGL_PROGRAM_INFO_DELETE_STATUS:				return GL_DELETE_STATUS;
		case ZGL_PROGRAM_INFO_LOG_LENGTH:					return GL_INFO_LOG_LENGTH;
		case ZGL_PROGRAM_INFO_LINK_STATUS:					return GL_LINK_STATUS;
		case ZGL_PROGRAM_INFO_VALIDATE_STATUS:				return GL_VALIDATE_STATUS;

		case ZGL_SAMPLE_LINEAR:							return GL_LINEAR;
		case ZGL_SAMPLE_LINEAR_MIPMAP_LINEAR:			return GL_LINEAR_MIPMAP_LINEAR;
		case ZGL_SAMPLE_LINEAR_MIPMAP_NEAREST:			return GL_LINEAR_MIPMAP_NEAREST;
		case ZGL_SAMPLE_NEAREST:						return GL_NEAREST;
		case ZGL_SAMPLE_NEAREST_MIPMAP_LINEAR:			return GL_NEAREST_MIPMAP_LINEAR;
		case ZGL_SAMPLE_NEAREST_MIPMAP_NEAREST:			return GL_NEAREST_MIPMAP_NEAREST;

		case ZGL_SHADER_INFO_COMPILE_STATUS:			return GL_COMPILE_STATUS;
		case ZGL_SHADER_INFO_DELETE_STATUS:				return GL_DELETE_STATUS;
		case ZGL_SHADER_INFO_LOG_LENGTH:				return GL_INFO_LOG_LENGTH;
		case ZGL_SHADER_INFO_SOURCE_LENGTH:				return GL_SHADER_SOURCE_LENGTH;
		case ZGL_SHADER_INFO_TYPE:						return GL_SHADER_TYPE;

		#if !defined ( MOAI_OS_OSX ) && !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_LINUX ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
  		case ZGL_SHADER_TYPE_TESS_CONTROL:				return GL_TESS_CONTROL_SHADER;
  		case ZGL_SHADER_TYPE_TESS_EVALUATION:			return GL_TESS_EVALUATION_SHADER;
    #endif
    
		case ZGL_SHADER_TYPE_FRAGMENT:					return GL_FRAGMENT_SHADER;

		#if !defined ( MOAI_OS_OSX ) && !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
  		case ZGL_SHADER_TYPE_GEOMETRY:					return GL_GEOMETRY_SHADER;
    #endif

		case ZGL_SHADER_TYPE_VERTEX:					return GL_VERTEX_SHADER;

		case ZGL_STRING_VENDOR:							return GL_VENDOR;
		case ZGL_STRING_VERSION:						return GL_VERSION;
		case ZGL_STRING_RENDERER:						return GL_RENDERER;
		case ZGL_STRING_SHADING_LANGUAGE_VERSION:		return GL_SHADING_LANGUAGE_VERSION;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
  		case ZGL_TEXTURE_BASE_LEVEL:					return GL_TEXTURE_BASE_LEVEL;
  		case ZGL_TEXTURE_COMPARE_FUNC:					return GL_TEXTURE_COMPARE_FUNC;
  		case ZGL_TEXTURE_COMPARE_MODE:					return GL_TEXTURE_COMPARE_MODE;
  		//case ZGL_TEXTURE_DEPTH_STENCIL_MODE:			return GL_DEPTH_STENCIL_TEXTURE_MODE;
  		case ZGL_TEXTURE_ENV_MODE:						return GL_TEXTURE_ENV_MODE;
    #endif
    
		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
  		case ZGL_TEXTURE_LOD_BIAS:						return GL_TEXTURE_LOD_BIAS;
    #endif

  		case ZGL_TEXTURE_MAG_FILTER:					return GL_TEXTURE_MAG_FILTER;
		case ZGL_TEXTURE_MIN_FILTER:					return GL_TEXTURE_MIN_FILTER;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
  		case ZGL_TEXTURE_MAX_LEVEL:						return GL_TEXTURE_MAX_LEVEL;
  		case ZGL_TEXTURE_MAX_LOD:						return GL_TEXTURE_MAX_LOD;
  		case ZGL_TEXTURE_MIN_LOD:						return GL_TEXTURE_MIN_LOD;
		#endif
		
		#if !defined ( MOAI_OS_OSX ) && !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_LINUX ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
  		case ZGL_TEXTURE_SWIZZLE_A:						return GL_TEXTURE_SWIZZLE_A;
  		case ZGL_TEXTURE_SWIZZLE_B:						return GL_TEXTURE_SWIZZLE_B;
  		case ZGL_TEXTURE_SWIZZLE_G:						return GL_TEXTURE_SWIZZLE_G;
  		case ZGL_TEXTURE_SWIZZLE_R:						return GL_TEXTURE_SWIZZLE_R;
		#endif
		
		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
		  case ZGL_TEXTURE_WRAP_R:						return GL_TEXTURE_WRAP_R;
		#endif
		
		case ZGL_TEXTURE_WRAP_S:						return GL_TEXTURE_WRAP_S;
		case ZGL_TEXTURE_WRAP_T:						return GL_TEXTURE_WRAP_T;

		case ZGL_TYPE_BYTE:								return GL_BYTE;
		
		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID )
		  case ZGL_TYPE_DOUBLE:							return GL_DOUBLE;
    #endif
    
		case ZGL_TYPE_FLOAT:							return GL_FLOAT;
		case ZGL_TYPE_INT:								return GL_INT;
		case ZGL_TYPE_SHORT:							return GL_SHORT;
		case ZGL_TYPE_UNSIGNED_BYTE:					return GL_UNSIGNED_BYTE;
		case ZGL_TYPE_UNSIGNED_INT:						return GL_UNSIGNED_INT;
		case ZGL_TYPE_UNSIGNED_SHORT:					return GL_UNSIGNED_SHORT;

		case ZGL_WRAP_MODE_CLAMP:						return GL_CLAMP_TO_EDGE;
		case ZGL_WRAP_MODE_REPEAT:						return GL_REPEAT;
	}
  	
	assert ( false );
	return 0;
}

//================================================================//
// setup
//================================================================//

//----------------------------------------------------------------//
void zglFinalize () {
}

//----------------------------------------------------------------//
void zglInitialize () {

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

	string version = zglGetString ( ZGL_STRING_VERSION );
	for ( u32 i = 0; version [ i ]; i++ ) {
		version [ i ] = ( char )tolower ( version [ i ]);
	}
	
	string gles = "opengl es";
	
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
	
	#ifdef __FLASH__
		isOpenGLES = true;
		sIsProgrammable = false;
		sIsFramebufferSupported = false;
	#else
		sIsProgrammable = ( majorVersion >= 2 );
		sIsFramebufferSupported = true;
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
				// looks like frame buffer isn't supported
				sIsFramebufferSupported = false;
			}
		}

	#endif
	
	int maxTextureUnits = 0;
	
	if ( majorVersion == 1 ) {
		#if USE_OPENGLES1
			glGetIntegerv ( GL_MAX_TEXTURE_UNITS, &maxTextureUnits );
		#endif
	}
	else {
		glGetIntegerv ( GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits );
	}

	sMaxTextureUnits = ( u32 )maxTextureUnits;
	
	int maxTextureSize;
	glGetIntegerv ( GL_MAX_TEXTURE_SIZE, &maxTextureSize );
	sMaxTextureSize = ( u32 )maxTextureSize;
}

//================================================================//
// gfx
//================================================================//

//----------------------------------------------------------------//
void zglActiveTexture ( u32 textureUnit ) {
	glActiveTexture ( GL_TEXTURE0 + textureUnit );
}

//----------------------------------------------------------------//
void zglBlendFunc ( u32 sourceFactor, u32 destFactor ) {
	glBlendFunc ( _remapEnum ( sourceFactor ), _remapEnum ( destFactor ));
}

//----------------------------------------------------------------//
void zglBlendMode ( u32 mode ) {
	glBlendEquation ( _remapEnum ( mode ));
}

//----------------------------------------------------------------//
void zglClear ( u32 mask ) {

	GLbitfield glMask = 0;

	if ( mask & ZGL_CLEAR_COLOR_BUFFER_BIT ) {
		glMask |= GL_COLOR_BUFFER_BIT;
	}
	
	if ( mask & ZGL_CLEAR_DEPTH_BUFFER_BIT ) {
		glMask |= GL_DEPTH_BUFFER_BIT;
	}
	
	if ( mask & ZGL_CLEAR_STENCIL_BUFFER_BIT ) {
		glMask |= GL_STENCIL_BUFFER_BIT;
	}

	glClear ( glMask );
}

//----------------------------------------------------------------//
void zglClearColor ( float r, float g, float b, float a ) {
	glClearColor ( r, g, b, a );
}

//----------------------------------------------------------------//
void zglColor ( float r, float g, float b, float a ) {
  #if !MOAI_OS_NACL
	  glColor4f ( r, g, b, a );
	#endif
}

//----------------------------------------------------------------//
void zglCullFace ( u32 mode ) {
	glCullFace ( _remapEnum ( mode ));
}

//----------------------------------------------------------------//
void zglDeleteBuffer ( u32 buffer ) {
	glDeleteBuffers ( 1, &buffer );
}

//----------------------------------------------------------------//
void zglDeleteFramebuffer ( u32 buffer ) {
	glDeleteFramebuffers ( 1, &buffer );
}

//----------------------------------------------------------------//
void zglDeleteProgram ( u32 program ) {
	glDeleteProgram ( program );
}

//----------------------------------------------------------------//
void zglDeleteRenderbuffer ( u32 buffer ) {
	glDeleteRenderbuffers ( 1, &buffer );
}

//----------------------------------------------------------------//
void zglDeleteShader ( u32 shader ) {
	glDeleteShader ( shader );
}

//----------------------------------------------------------------//
void zglDeleteTexture ( u32 texture ) {
	glDeleteTextures ( 1, &texture );
}

//----------------------------------------------------------------//
void zglDepthFunc ( u32 depthFunc ) {
	glDepthFunc ( _remapEnum ( depthFunc ));
}

//----------------------------------------------------------------//
void zglDepthMask ( bool flag ) {
	glDepthMask ( flag ? GL_TRUE : GL_FALSE );
}

//----------------------------------------------------------------//
void zglDisable ( u32 cap ) {
	glDisable ( _remapEnum ( cap ));
}

//----------------------------------------------------------------//
void zglDrawArrays ( u32 primType, u32 first, u32 count ) {
	glDrawArrays ( _remapEnum ( primType ), ( GLint )first, ( GLsizei )count );
}

//----------------------------------------------------------------//
void zglDrawElements ( u32 primType, u32 count, u32 indexType, const void* indices ) {
	glDrawElements ( _remapEnum ( primType ), ( GLsizei )count, _remapEnum ( indexType ), ( const GLvoid* )indices );
}

//----------------------------------------------------------------//
void zglEnable ( u32 cap ) {
	glEnable ( _remapEnum ( cap ));
}

//----------------------------------------------------------------//
void zglFlush () {
	glFlush ();
}

//----------------------------------------------------------------//
u32 zglGetCap ( u32 cap ) {

	switch ( cap ) {
		case ZGL_CAPS_IS_FRAMEBUFFER_SUPPORTED:
			return sIsFramebufferSupported ? 1 : 0;
		case ZGL_CAPS_IS_PROGRAMMABLE:
			return sIsProgrammable ? 1 : 0;
		case ZGL_CAPS_MAX_TEXTURE_SIZE:
			return sMaxTextureSize;
		case ZGL_CAPS_MAX_TEXTURE_UNITS:
			return sMaxTextureUnits;
	}
	return 0;
}

//----------------------------------------------------------------//
u32 zglGetError () {

	GLenum error = glGetError ();
	
	switch ( error ) {
	
		case GL_NO_ERROR:			return ZGL_ERROR_NONE;
	
		case GL_INVALID_ENUM:		return ZGL_ERROR_INVALID_ENUM;
		case GL_INVALID_OPERATION:	return ZGL_ERROR_INVALID_OPERATION;
		case GL_INVALID_VALUE:		return ZGL_ERROR_INVALID_VALUE;
		case GL_OUT_OF_MEMORY:		return ZGL_ERROR_OUT_OF_MEMORY;
		
		#if USE_OPENGLES1
			case GL_STACK_OVERFLOW:		return ZGL_ERROR_STACK_OVERFLOW;
			case GL_STACK_UNDERFLOW:	return ZGL_ERROR_STACK_UNDERFLOW;
		#endif					
	}
	
	return ZGL_ERROR_UNKNOWN;
}

//----------------------------------------------------------------//
cc8* zglGetErrorString ( u32 error ) {

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
extern cc8* zglGetString ( u32 stringID ) {
	return ( cc8* )glGetString ( _remapEnum ( stringID ));
}

//----------------------------------------------------------------//
void zglLineWidth ( float width ) {
	glLineWidth (( GLfloat )width );
}

//----------------------------------------------------------------//
void zglLoadIdentity () {
  #if !MOAI_OS_NACL
	  glLoadIdentity ();
	#endif
}

//----------------------------------------------------------------//
void zglLoadMatrix ( const float* matrix ) {
  #if !MOAI_OS_NACL
	  glLoadMatrixf ( matrix );
	#endif
}

//----------------------------------------------------------------//
void zglMatrixMode ( u32 mode ) {
  #if !MOAI_OS_NACL
	  glMatrixMode ( _remapEnum ( mode ));
	#endif
}

//----------------------------------------------------------------//
void zglMultMatrix ( const float* matrix ) {
  #if !MOAI_OS_NACL
	  glMultMatrixf ( matrix );
	#endif
}

//----------------------------------------------------------------//
void zglPointSize ( float size ) {
  #if !MOAI_OS_NACL
	  glPointSize (( GLfloat )size );
	#endif
}

//----------------------------------------------------------------//
void zglReadPixels ( s32 x, s32 y, u32 width, u32 height, void* data ) {
	glReadPixels (( GLint )x, ( GLint )y, ( GLsizei )width, ( GLsizei )height, GL_RGBA, GL_UNSIGNED_BYTE, ( GLvoid* )data );
}

//----------------------------------------------------------------//
void zglScissor ( s32 x, s32 y, u32 w, u32 h ) {
	glScissor (( GLint )x, ( GLint )y, ( GLsizei )w, ( GLsizei )h );
}

//----------------------------------------------------------------//
void zglViewport ( s32 x, s32 y, u32 w, u32 h ) {
	glViewport (( GLint )x, ( GLint )y, ( GLsizei )w, ( GLsizei )h );
}

//================================================================//
// vertex format
//================================================================//

//----------------------------------------------------------------//
void zglColorPointer ( u32 size, u32 type, u32 stride, const void* pointer ) {
  #if !MOAI_OS_NACL
	  glColorPointer (( GLint )size, _remapEnum ( type ), ( GLsizei )stride, ( const GLvoid* )pointer );
	#endif
}

//----------------------------------------------------------------//
void zglDisableClientState ( u32 cap ) {
  #if !MOAI_OS_NACL
	  glDisableClientState ( _remapEnum ( cap ));
	#endif
}

//----------------------------------------------------------------//
void zglDisableVertexAttribArray ( u32 index ) {
	glDisableVertexAttribArray (( GLuint )index );
}

//----------------------------------------------------------------//
void zglNormalPointer ( u32 type, u32 stride, const void* pointer ) {
  #if !MOAI_OS_NACL
	  glNormalPointer ( _remapEnum ( type ), ( GLsizei )stride, ( const GLvoid* )pointer );
	#endif
}

//----------------------------------------------------------------//
void zglTexCoordPointer ( u32 size, u32 type, u32 stride, const void* pointer ) {
  #if !MOAI_OS_NACL
	  glTexCoordPointer (( GLint )size, _remapEnum ( type ), ( GLsizei )stride, ( const GLvoid* )pointer );
	#endif
}

//----------------------------------------------------------------//
void zglVertexAttribPointer ( u32 index, u32 size, u32 type, bool normalized, u32 stride, const void* pointer ) {
	glVertexAttribPointer (
		( GLuint )index,
		( GLint )size,
		_remapEnum ( type ),
		normalized ? GL_TRUE : GL_FALSE,
		( GLsizei )stride,
		( const GLvoid* )pointer
	);
}

//----------------------------------------------------------------//
void zglVertexPointer ( u32 size, u32 type, u32 stride, const void* pointer ) {
  #if !MOAI_OS_NACL
	  glVertexPointer (( GLint )size, _remapEnum ( type ), ( GLsizei )stride, ( const GLvoid* )pointer );
	#endif
}

//----------------------------------------------------------------//
void zglEnableClientState ( u32 cap ) {
  #if !MOAI_OS_NACL
	  glEnableClientState ( _remapEnum ( cap ));
	#endif
}

//----------------------------------------------------------------//
void zglEnableVertexAttribArray ( u32 index ) {
	glEnableVertexAttribArray (( GLuint )index );
}

//================================================================//
// shaders
//================================================================//

//----------------------------------------------------------------//
void zglAttachShader ( u32 program, u32 shader ) {
	glAttachShader (( GLuint )program, ( GLuint )shader );
}

//----------------------------------------------------------------//
void zglBindAttribLocation ( u32 program, u32 index, cc8* name ) {
	glBindAttribLocation (( GLuint )program, ( GLuint )index, ( const GLchar* )name );
}

//----------------------------------------------------------------//
void zglCompileShader ( u32 shader ) {
	glCompileShader ( shader );
}

//----------------------------------------------------------------//
u32 zglCreateProgram () {
	return ( u32 )glCreateProgram ();
}

//----------------------------------------------------------------//
u32 zglCreateShader ( u32 shaderType ) {
	return ( u32 )glCreateShader ( _remapEnum ( shaderType ));
}

//----------------------------------------------------------------//
void zglGetProgramInfoLog ( u32 program, u32 maxLength, u32* length, char* log ) {
	glGetProgramInfoLog (( GLuint )program, ( GLsizei )maxLength, ( GLsizei* )length, ( GLchar* )log );
}

//----------------------------------------------------------------//
void zglGetProgramiv ( u32 program, u32 name, s32* params ) {
	glGetProgramiv (( GLuint )program, _remapEnum ( name ), ( GLint* )params );
}

//----------------------------------------------------------------//
void zglGetShaderInfoLog ( u32 shader, u32 maxLength, u32* length, char* log ) {
	glGetShaderInfoLog (( GLuint )shader, ( GLsizei )maxLength, ( GLsizei* )length, ( GLchar* )log );
}

//----------------------------------------------------------------//
void zglGetShaderiv ( u32 shader, u32 name, s32* params ) {
	glGetShaderiv (( GLuint )shader, _remapEnum ( name ), params );
}

//----------------------------------------------------------------//
u32 zglGetUniformLocation ( u32 program, cc8* name ) {
	return ( u32 )glGetUniformLocation (( GLuint )program, ( const GLchar* )name );
}

//----------------------------------------------------------------//
void zglLinkProgram ( u32 program ) {
	glLinkProgram (( GLuint )program );
}

//----------------------------------------------------------------//
void zglShaderSource ( u32 shader, u32 count, const char** string, const s32* length ) {
	glShaderSource (( GLuint )shader, ( GLsizei )count, ( const GLchar** )string, ( const GLint* )length );
}

//----------------------------------------------------------------//
void zglValidateProgram ( u32 program ) {
	glValidateProgram (( GLuint )program );
}

//----------------------------------------------------------------//
void zglUniform1f ( u32 location, float v0 ) {
	glUniform1f (( GLint )location, ( GLfloat )v0 );
}

//----------------------------------------------------------------//
void zglUniform1i ( u32 location, s32 v0 ) {
	glUniform1i (( GLint )location, ( GLint )v0 );
}

//----------------------------------------------------------------//
void zglUniform4fv ( u32 location, u32 count, const float* value ) {
	glUniform4fv (( GLint )location, ( GLsizei )count, ( const GLfloat* )value );
}

//----------------------------------------------------------------//
void zglUniformMatrix4fv ( u32 location, u32 count, bool transpose, const float* value ) {
	glUniformMatrix4fv (( GLint )location, ( GLsizei )count, transpose ? GL_TRUE : GL_FALSE, ( const GLfloat* )value );
}

//----------------------------------------------------------------//
void zglUseProgram ( u32 program ) {
	glUseProgram (( GLuint )program );
}

//================================================================//
// texture
//================================================================//

//----------------------------------------------------------------//
void zglBindTexture ( u32 texID ) {
	glBindTexture ( GL_TEXTURE_2D, ( GLuint )texID );
}

//----------------------------------------------------------------//
u32 zglCreateTexture () {

	u32 textureID;
	glGenTextures ( 1, ( GLuint* )&textureID );
	return textureID;
}

//----------------------------------------------------------------//
void zglTexEnvi ( u32 pname, s32 param ) {
  #if !MOAI_OS_NACL
	  glTexEnvi ( GL_TEXTURE_ENV, _remapEnum ( pname ), ( GLint )param );
	#endif
}

//----------------------------------------------------------------//
void zglTexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, const void* data ) {

	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

	glTexImage2D (
		GL_TEXTURE_2D,
		( GLint )level,
		( GLint )_remapEnum ( internalFormat ),
		( GLsizei )width,
		( GLsizei )height,
		0,
		_remapEnum ( format ),
		_remapEnum ( type ),
		( const GLvoid* )data
	);
}

//----------------------------------------------------------------//
void zglTexParameteri ( u32 pname, s32 param ) {
	glTexParameteri ( GL_TEXTURE_2D, _remapEnum ( pname ), ( GLint )_remapEnum ( param ));
}

//----------------------------------------------------------------//
void zglTexSubImage2D ( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, const void* data ) {

	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

	glTexSubImage2D (
		GL_TEXTURE_2D,
		( GLint )level,
		( GLint )xOffset,
		( GLint )yOffset,
		( GLsizei )width,
		( GLsizei )height,  
		_remapEnum ( format ),
		_remapEnum ( type ),
		( const GLvoid* )data
	);
}

//================================================================//
// framebuffer
//================================================================//

//----------------------------------------------------------------//
void zglBindFramebuffer ( u32 target, u32 frameBuffer ) {
	glBindFramebuffer ( _remapEnum ( target ), frameBuffer );
}

//----------------------------------------------------------------//
void zglBindRenderbuffer ( u32 renderbuffer ) {
	glBindRenderbuffer ( GL_RENDERBUFFER, renderbuffer );
}

//----------------------------------------------------------------//
u32 zglCheckFramebufferStatus ( u32 target ) {

	GLenum status = glCheckFramebufferStatus ( _remapEnum ( target ));
	return status == GL_FRAMEBUFFER_COMPLETE ? ZGL_FRAMEBUFFER_STATUS_COMPLETE : 0;
}

//----------------------------------------------------------------//
u32 zglCreateFramebuffer () {

	u32 bufferID;
	glGenFramebuffers ( 1, &bufferID );
	return bufferID;
}

//----------------------------------------------------------------//
u32 zglCreateRenderbuffer () {

	u32 bufferID;
	glGenRenderbuffers ( 1, &bufferID );
	return bufferID;
}

//----------------------------------------------------------------//
void zglFramebufferRenderbuffer ( u32 target, u32 attachment, u32 renderbuffer ) {
	glFramebufferRenderbuffer ( _remapEnum ( target ), _remapEnum ( attachment ), GL_RENDERBUFFER, ( GLuint )renderbuffer );
}

//----------------------------------------------------------------//
void zglFramebufferTexture2D ( u32 target, u32 attachment, u32 texture, s32 level ) {
	glFramebufferTexture2D ( _remapEnum ( target ), _remapEnum ( attachment ), GL_TEXTURE_2D, ( GLuint )texture, ( GLint )level );
}

//----------------------------------------------------------------//
void zglRenderbufferStorage ( u32 internalFormat, u32 width, u32 height ) {
	glRenderbufferStorage ( GL_RENDERBUFFER, _remapEnum ( internalFormat ), ( GLsizei )width, ( GLsizei )height );
}

//================================================================//
// buffer
//================================================================//

//----------------------------------------------------------------//
void zglBindBuffer ( u32 target, u32 buffer ) {
	glBindBuffer ( _remapEnum ( target ), buffer );
}

//----------------------------------------------------------------//
void zglBufferData ( u32 target, u32 size, const void* data, u32 usage ) {
	glBufferData ( _remapEnum ( target ), ( GLsizeiptr )size, ( const GLvoid* )data, _remapEnum ( usage ));
}

//----------------------------------------------------------------//
u32 zglCreateBuffer () {

	u32 bufferID;
	glGenFramebuffers ( 1, &bufferID );
	return bufferID;
}

#endif
