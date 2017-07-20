// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/headers.h>

SUPPRESS_EMPTY_FILE_WARNING

#if MOAI_GFX_OPENGL

#include <algorithm>
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

	#define glGenVertexArrays		glGenVertexArraysAPPLE
	#define glBindVertexArray		glBindVertexArrayAPPLE
	#define glDeleteVertexArrays	glDeleteVertexArraysAPPLE

#endif

#ifdef MOAI_OS_IPHONE

	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>

	#define GL_WRITE_ONLY			0x88B9
	#define GL_RGBA8				GL_RGBA8_OES

	// TODO: should not have to do this
	// this is to suppress a false positive error in Xcode
	extern GLvoid*		glMapBufferOES				( GLenum target, GLenum access );
	extern GLboolean	glUnmapBufferOES			( GLenum target );

	#define glMapBuffer				glMapBufferOES
	#define glUnmapBuffer			glUnmapBufferOES

	// TODO: should not have to do this
	// this is to suppress a false positive error in Xcode
	extern GLvoid		glBindVertexArrayOES		( GLuint array );
	extern GLvoid		glGenVertexArraysOES		( GLsizei n, GLuint *arrays );
	extern GLvoid		glDeleteVertexArraysOES		( GLsizei n, const GLuint *arrays );

	#define glGenVertexArrays		glGenVertexArraysOES
	#define glBindVertexArray		glBindVertexArrayOES
	#define glDeleteVertexArrays	glDeleteVertexArraysOES

#endif

#ifdef MOAI_OS_ANDROID

	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	//#include <EGL/egl.h>

	#define GL_WRITE_ONLY			0x88B9

	typedef void*		( GL_APIENTRYP PFNGLMAPBUFFEROESPROC )			( GLenum target, GLenum access );
	typedef GLboolean	( GL_APIENTRYP PFNGLUNMAPBUFFEROESPROC )		( GLenum target );
	typedef void		( GL_APIENTRYP PFNGLGETBINDVERTEXARRAYPROC )	( GLuint array );
	typedef void		( GL_APIENTRYP PFNGLGETGENVERTEXARRAYSPROC )	( GLsizei n, GLuint* arrays );
	typedef void		( GL_APIENTRYP PFNGLDELETEVERTEXARRAYSPROC )	( GLsizei n, const GLuint* arrays );

	//static PFNGLMAPBUFFEROESPROC		glMapBuffer				= ( PFNGLMAPBUFFEROESPROC )eglGetProcAddress ( "glMapBufferOES" );
	//static PFNGLUNMAPBUFFEROESPROC		glUnmapBuffer			= ( PFNGLUNMAPBUFFEROESPROC )eglGetProcAddress ( "glUnmapBufferOES" );
	//static PFNGLGETGENVERTEXARRAYSPROC	glGenVertexArrays		= ( PFNGLGETGENVERTEXARRAYSPROC )eglGetProcAddress ( "glGenVertexArraysOES" );
	//static PFNGLGETBINDVERTEXARRAYPROC	glBindVertexArray		= ( PFNGLGETBINDVERTEXARRAYPROC )eglGetProcAddress ( "glBindVertexArrayOES" );
	//static PFNGLDELETEVERTEXARRAYSPROC	glDeleteVertexArrays	= ( PFNGLDELETEVERTEXARRAYSPROC )eglGetProcAddress ( "glDeleteVertexArraysOES" );

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

static u32	sMaxTextureUnits			= 0;
static u32	sMaxTextureSize				= 0;
static u32	sOperationDepth				= 0; // this is just the counter for tracking begin/end calls

#define ASSERT_OPERATION_DEPTH() ( assert ( sOperationDepth > 0 )) // Attempt to call zgl graphics method outside of operation.

//================================================================//
// enums
//================================================================//

//----------------------------------------------------------------//
u32 zglMapFromGLEnum( u32 glEnum ) {

	switch ( glEnum ) {
		case GL_DST_ALPHA:					return ZGL_BLEND_FACTOR_DST_ALPHA;
		case GL_DST_COLOR:					return ZGL_BLEND_FACTOR_DST_COLOR;
		case GL_ONE:						return ZGL_BLEND_FACTOR_ONE;
		case GL_ONE_MINUS_DST_ALPHA:		return ZGL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case GL_ONE_MINUS_DST_COLOR:		return ZGL_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		case GL_ONE_MINUS_SRC_ALPHA:		return ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case GL_ONE_MINUS_SRC_COLOR:		return ZGL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		case GL_SRC_ALPHA:					return ZGL_BLEND_FACTOR_SRC_ALPHA;
		case GL_SRC_ALPHA_SATURATE:			return ZGL_BLEND_FACTOR_SRC_ALPHA_SATURATE;
		case GL_SRC_COLOR:					return ZGL_BLEND_FACTOR_SRC_COLOR;
		case GL_ZERO:						return ZGL_BLEND_FACTOR_ZERO;
	};
	assert ( false );
	return 0;
}

//----------------------------------------------------------------//
GLenum _remapEnum ( u32 zglEnum ) {

	switch ( zglEnum ) {

		case ZGL_BLEND_FACTOR_DST_ALPHA:					return GL_DST_ALPHA;
		case ZGL_BLEND_FACTOR_DST_COLOR:					return GL_DST_COLOR;
		case ZGL_BLEND_FACTOR_ONE:							return GL_ONE;
		case ZGL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:			return GL_ONE_MINUS_DST_ALPHA;
		case ZGL_BLEND_FACTOR_ONE_MINUS_DST_COLOR:			return GL_ONE_MINUS_DST_COLOR;
		case ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:			return GL_ONE_MINUS_SRC_ALPHA;
		case ZGL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:			return GL_ONE_MINUS_SRC_COLOR;
		case ZGL_BLEND_FACTOR_SRC_ALPHA:					return GL_SRC_ALPHA;
		case ZGL_BLEND_FACTOR_SRC_ALPHA_SATURATE:			return GL_SRC_ALPHA_SATURATE;
		case ZGL_BLEND_FACTOR_SRC_COLOR:					return GL_SRC_COLOR;
		case ZGL_BLEND_FACTOR_ZERO:							return GL_ZERO;

		case ZGL_BLEND_MODE_ADD:							return GL_FUNC_ADD;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_BLEND_MODE_MAX:						return GL_MAX;
			case ZGL_BLEND_MODE_MIN:						return GL_MIN;
		#endif

		case ZGL_BLEND_MODE_REVERSE_SUBTRACT:				return GL_FUNC_REVERSE_SUBTRACT;
		case ZGL_BLEND_MODE_SUBTRACT:						return GL_FUNC_SUBTRACT;

		case ZGL_BUFFER_TARGET_ARRAY:						return GL_ARRAY_BUFFER;
		case ZGL_BUFFER_TARGET_ELEMENT_ARRAY:				return GL_ELEMENT_ARRAY_BUFFER;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_BUFFER_USAGE_DYNAMIC_COPY:				return GL_DYNAMIC_COPY;
		#endif

		case ZGL_BUFFER_USAGE_DYNAMIC_DRAW:					return GL_DYNAMIC_DRAW;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_BUFFER_USAGE_DYNAMIC_READ:				return GL_DYNAMIC_READ;
			case ZGL_BUFFER_USAGE_STATIC_COPY:				return GL_STATIC_COPY;
		#endif

		case ZGL_BUFFER_USAGE_STATIC_DRAW:					return GL_STATIC_DRAW;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_BUFFER_USAGE_STATIC_READ:				return GL_STATIC_READ;
			case ZGL_BUFFER_USAGE_STREAM_COPY:				return GL_STREAM_COPY;
		#endif

		case ZGL_BUFFER_USAGE_STREAM_DRAW:					return GL_STREAM_DRAW;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_BUFFER_USAGE_STREAM_READ:				return GL_STREAM_READ;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_BLACKBERRY )
			case ZGL_COMPOSE_MODULATE:						return GL_MODULATE;
		#endif

		case ZGL_CULL_ALL:									return GL_FRONT_AND_BACK;
		case ZGL_CULL_BACK:									return GL_BACK;
		case ZGL_CULL_FRONT:								return GL_FRONT;

		case ZGL_DEPTH_ALWAYS:								return GL_ALWAYS;
		case ZGL_DEPTH_EQUAL:								return GL_EQUAL;
		case ZGL_DEPTH_LESS:								return GL_LESS;
		case ZGL_DEPTH_LEQUAL:								return GL_LEQUAL;
		case ZGL_DEPTH_GEQUAL:								return GL_GEQUAL;
		case ZGL_DEPTH_GREATER:								return GL_GREATER;
		case ZGL_DEPTH_NEVER:								return GL_NEVER;
		case ZGL_DEPTH_NOTEQUAL:							return GL_NOTEQUAL;

		case ZGL_FRAMEBUFFER_ATTACHMENT_COLOR:				return GL_COLOR_ATTACHMENT0;
		case ZGL_FRAMEBUFFER_ATTACHMENT_DEPTH:				return GL_DEPTH_ATTACHMENT;
		case ZGL_FRAMEBUFFER_ATTACHMENT_STENCIL:			return GL_STENCIL_ATTACHMENT;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_FRAMEBUFFER_TARGET_DRAW:				return GL_DRAW_FRAMEBUFFER;
			case ZGL_FRAMEBUFFER_TARGET_READ:				return GL_READ_FRAMEBUFFER;
		#endif

		case ZGL_FRAMEBUFFER_TARGET_DRAW_READ:				return GL_FRAMEBUFFER;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_MATRIX_COLOR:							return GL_COLOR;
			case ZGL_MATRIX_MODELVIEW:						return GL_MODELVIEW;
			case ZGL_MATRIX_PROJECTION:						return GL_PROJECTION;
		#endif

		case ZGL_MATRIX_TEXTURE:							return GL_TEXTURE;

		case ZGL_PIPELINE_BLEND:							return GL_BLEND;

		#if !defined ( MOAI_OS_NACL )
		  case ZGL_PIPELINE_COLOR_ARRAY:					return GL_COLOR_ARRAY;
		#endif
		case ZGL_PIPELINE_CULL:								return GL_CULL_FACE;
		case ZGL_PIPELINE_DEPTH:							return GL_DEPTH_TEST;

		#if !defined ( MOAI_OS_NACL )
			case ZGL_PIPELINE_NORMAL_ARRAY:					return GL_NORMAL_ARRAY;
		#endif

		case ZGL_PIPELINE_SCISSOR:							return GL_SCISSOR_TEST;
		case ZGL_PIPELINE_TEXTURE_2D:						return GL_TEXTURE_2D;

		#if !defined ( MOAI_OS_NACL )
			case ZGL_PIPELINE_TEXTURE_COORD_ARRAY:			return GL_TEXTURE_COORD_ARRAY;
			case ZGL_PIPELINE_VERTEX_ARRAY:					return GL_VERTEX_ARRAY;
		#endif

		case ZGL_PIXEL_FORMAT_ALPHA:						return GL_ALPHA;
		case ZGL_PIXEL_FORMAT_LUMINANCE:					return GL_LUMINANCE;
		case ZGL_PIXEL_FORMAT_LUMINANCE_ALPHA:				return GL_LUMINANCE_ALPHA;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_FORMAT_RED:						return GL_RED;
			case ZGL_PIXEL_FORMAT_RG:						return GL_RG;
		#endif

		case ZGL_PIXEL_FORMAT_RGB:							return GL_RGB;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_FORMAT_RGB4:						return GL_RGB4;
		#endif

		case ZGL_PIXEL_FORMAT_RGB5_A1:						return GL_RGB5_A1;

		#if defined ( MOAI_OS_ANDROID ) || defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_FORMAT_RGB565:					return GL_RGB565;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_FORMAT_RGB8:						return GL_RGB8;
			case ZGL_PIXEL_FORMAT_BGR:						return GL_BGR;
		#endif

		case ZGL_PIXEL_FORMAT_RGBA:							return GL_RGBA;
		case ZGL_PIXEL_FORMAT_RGBA4:						return GL_RGBA4;

		#if !defined ( MOAI_OS_ANDROID )
			case ZGL_PIXEL_FORMAT_RGBA8:					return GL_RGBA8;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_FORMAT_BGRA:						return GL_BGRA;
		#endif

		case ZGL_PIXEL_TYPE_BYTE:							return GL_BYTE;

		#ifdef MOAI_OS_IPHONE
			case ZGL_PIXEL_TYPE_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:	return GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
			case ZGL_PIXEL_TYPE_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:	return GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
			case ZGL_PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:	return GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
			case ZGL_PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:	return GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		#endif

		case ZGL_PIXEL_TYPE_FLOAT:							return GL_FLOAT;
		case ZGL_PIXEL_TYPE_INT:							return GL_INT;
		case ZGL_PIXEL_TYPE_SHORT:							return GL_SHORT;
		case ZGL_PIXEL_TYPE_UNSIGNED_BYTE:					return GL_UNSIGNED_BYTE;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_TYPE_UNSIGNED_BYTE_2_3_3_REV:	return GL_UNSIGNED_BYTE_2_3_3_REV;
			case ZGL_PIXEL_TYPE_UNSIGNED_BYTE_3_3_2:		return GL_UNSIGNED_BYTE_3_3_2;
		#endif

		case ZGL_PIXEL_TYPE_UNSIGNED_INT:					return GL_UNSIGNED_INT;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_TYPE_UNSIGNED_INT_8_8_8_8:			return GL_UNSIGNED_INT_8_8_8_8;
			case ZGL_PIXEL_TYPE_UNSIGNED_INT_8_8_8_8_REV:		return GL_UNSIGNED_INT_8_8_8_8_REV;
			case ZGL_PIXEL_TYPE_UNSIGNED_INT_2_10_10_10_REV:	return GL_UNSIGNED_INT_2_10_10_10_REV;
			case ZGL_PIXEL_TYPE_UNSIGNED_INT_10_10_10_2:		return GL_UNSIGNED_INT_10_10_10_2;
		#endif

		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT:					return GL_UNSIGNED_SHORT;
		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_6_5:			return GL_UNSIGNED_SHORT_5_6_5;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_6_5_REV:	return GL_UNSIGNED_SHORT_5_6_5_REV;
		#endif

		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4:			return GL_UNSIGNED_SHORT_4_4_4_4;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4_REV:		return GL_UNSIGNED_SHORT_4_4_4_4_REV;
			case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_1_5_5_5_REV:		return GL_UNSIGNED_SHORT_1_5_5_5_REV;
		#endif

		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1:		return GL_UNSIGNED_SHORT_5_5_5_1;

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

		#if !defined ( MOAI_OS_OSX ) && !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_LINUX ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_SHADER_TYPE_TESS_CONTROL:			return GL_TESS_CONTROL_SHADER;
			case ZGL_SHADER_TYPE_TESS_EVALUATION:		return GL_TESS_EVALUATION_SHADER;
		#endif

		case ZGL_SHADER_TYPE_FRAGMENT:					return GL_FRAGMENT_SHADER;

		#if !defined ( MOAI_OS_OSX ) && !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_SHADER_TYPE_GEOMETRY:				return GL_GEOMETRY_SHADER;
		#endif

		case ZGL_SHADER_TYPE_VERTEX:					return GL_VERTEX_SHADER;

		case ZGL_STRING_VENDOR:							return GL_VENDOR;
		case ZGL_STRING_VERSION:						return GL_VERSION;
		case ZGL_STRING_RENDERER:						return GL_RENDERER;
		case ZGL_STRING_SHADING_LANGUAGE_VERSION:		return GL_SHADING_LANGUAGE_VERSION;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_TEXTURE_BASE_LEVEL:				return GL_TEXTURE_BASE_LEVEL;
			case ZGL_TEXTURE_COMPARE_FUNC:				return GL_TEXTURE_COMPARE_FUNC;
			case ZGL_TEXTURE_COMPARE_MODE:				return GL_TEXTURE_COMPARE_MODE;
			//case ZGL_TEXTURE_DEPTH_STENCIL_MODE:		return GL_DEPTH_STENCIL_TEXTURE_MODE;
			case ZGL_TEXTURE_ENV_MODE:					return GL_TEXTURE_ENV_MODE;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_TEXTURE_LOD_BIAS:					return GL_TEXTURE_LOD_BIAS;
		#endif

  		case ZGL_TEXTURE_MAG_FILTER:					return GL_TEXTURE_MAG_FILTER;
		case ZGL_TEXTURE_MIN_FILTER:					return GL_TEXTURE_MIN_FILTER;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_TEXTURE_MAX_LEVEL:					return GL_TEXTURE_MAX_LEVEL;
			case ZGL_TEXTURE_MAX_LOD:					return GL_TEXTURE_MAX_LOD;
			case ZGL_TEXTURE_MIN_LOD:					return GL_TEXTURE_MIN_LOD;
		#endif

		#if !defined ( MOAI_OS_OSX ) && !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_LINUX ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_TEXTURE_SWIZZLE_A:					return GL_TEXTURE_SWIZZLE_A;
			case ZGL_TEXTURE_SWIZZLE_B:					return GL_TEXTURE_SWIZZLE_B;
			case ZGL_TEXTURE_SWIZZLE_G:					return GL_TEXTURE_SWIZZLE_G;
			case ZGL_TEXTURE_SWIZZLE_R:					return GL_TEXTURE_SWIZZLE_R;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_TEXTURE_WRAP_R:					return GL_TEXTURE_WRAP_R;
		#endif

		case ZGL_TEXTURE_WRAP_S:						return GL_TEXTURE_WRAP_S;
		case ZGL_TEXTURE_WRAP_T:						return GL_TEXTURE_WRAP_T;

		case ZGL_TYPE_BYTE:								return GL_BYTE;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_TYPE_DOUBLE:						return GL_DOUBLE;
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
void zglBegin () {

	sOperationDepth++;
}

//----------------------------------------------------------------//
void zglEnd () {

	ASSERT_OPERATION_DEPTH ();
	sOperationDepth--;
}

//----------------------------------------------------------------//
void zglFinalize () {
}

//----------------------------------------------------------------//
void zglInitialize () {

	ASSERT_OPERATION_DEPTH ();

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
	std::transform ( version.begin (), version.end(), version.begin(), ::tolower );
	
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

//================================================================//
// gfx
//================================================================//

//----------------------------------------------------------------//
void zglActiveTexture ( u32 textureUnit ) {

	ASSERT_OPERATION_DEPTH ();
	glActiveTexture ( GL_TEXTURE0 + textureUnit );
}

//----------------------------------------------------------------//
void zglBlendFunc ( u32 sourceFactor, u32 destFactor ) {

	ASSERT_OPERATION_DEPTH ();
	glBlendFunc ( _remapEnum ( sourceFactor ), _remapEnum ( destFactor ));
}

//----------------------------------------------------------------//
void zglBlendMode ( u32 mode ) {

	ASSERT_OPERATION_DEPTH ();
	glBlendEquation ( _remapEnum ( mode ));
}

//----------------------------------------------------------------//
void zglClear ( u32 mask ) {

	ASSERT_OPERATION_DEPTH ();

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

	ASSERT_OPERATION_DEPTH ();
	glClearColor ( r, g, b, a );
}

//----------------------------------------------------------------//
void zglColor ( float r, float g, float b, float a ) {

	ASSERT_OPERATION_DEPTH ();

	#if !MOAI_OS_NACL
		glColor4f ( r, g, b, a );
	#endif
}

//----------------------------------------------------------------//
void zglCullFace ( u32 mode ) {

	ASSERT_OPERATION_DEPTH ();
	glCullFace ( _remapEnum ( mode ));
}

//----------------------------------------------------------------//
void zglDeleteBuffer ( u32 buffer ) {

	ASSERT_OPERATION_DEPTH ();
	if ( buffer ) {
		glDeleteBuffers ( 1, &buffer );
	}
}

//----------------------------------------------------------------//
void zglDeleteFramebuffer ( u32 buffer ) {

	ASSERT_OPERATION_DEPTH ();
	if ( buffer ) {
		glDeleteFramebuffers ( 1, &buffer );
	}
}

//----------------------------------------------------------------//
void zglDeleteProgram ( u32 program ) {

	ASSERT_OPERATION_DEPTH ();
	if ( program ) {
		glDeleteProgram ( program );
	}
}

//----------------------------------------------------------------//
void zglDeleteRenderbuffer ( u32 buffer ) {

	ASSERT_OPERATION_DEPTH ();
	if ( buffer ) {
		glDeleteRenderbuffers ( 1, &buffer );
	}
}

//----------------------------------------------------------------//
void zglDeleteShader ( u32 shader ) {

	ASSERT_OPERATION_DEPTH ();
	if ( shader ) {
		glDeleteShader ( shader );
	}
}

//----------------------------------------------------------------//
void zglDeleteTexture ( u32 texture ) {

	ASSERT_OPERATION_DEPTH ();
	if ( texture ) {
		glDeleteTextures ( 1, &texture );
	}
}

//----------------------------------------------------------------//
void zglDepthFunc ( u32 depthFunc ) {

	ASSERT_OPERATION_DEPTH ();
	glDepthFunc ( _remapEnum ( depthFunc ));
}

//----------------------------------------------------------------//
void zglDepthMask ( bool flag ) {

	ASSERT_OPERATION_DEPTH ();
	glDepthMask ( flag ? GL_TRUE : GL_FALSE );
}

//----------------------------------------------------------------//
void zglDisable ( u32 cap ) {

	ASSERT_OPERATION_DEPTH ();
	glDisable ( _remapEnum ( cap ));
}

//----------------------------------------------------------------//
void zglDrawArrays ( u32 primType, u32 first, u32 count ) {

	ASSERT_OPERATION_DEPTH ();
	glDrawArrays ( _remapEnum ( primType ), ( GLint )first, ( GLsizei )count );
}

//----------------------------------------------------------------//
void zglDrawElements ( u32 primType, u32 count, u32 indexType, const void* indices ) {

	ASSERT_OPERATION_DEPTH ();
	glDrawElements ( _remapEnum ( primType ), ( GLsizei )count, _remapEnum ( indexType ), ( const GLvoid* )indices );
}

//----------------------------------------------------------------//
void zglEnable ( u32 cap ) {

	ASSERT_OPERATION_DEPTH ();
	glEnable ( _remapEnum ( cap ));
}

//----------------------------------------------------------------//
void zglFlush () {

	ASSERT_OPERATION_DEPTH ();
	glFlush ();
}

//----------------------------------------------------------------//
u32 zglGetCap ( u32 cap ) {

	ASSERT_OPERATION_DEPTH ();

	switch ( cap ) {
		case ZGL_CAPS_MAX_TEXTURE_SIZE:
			return sMaxTextureSize;
		case ZGL_CAPS_MAX_TEXTURE_UNITS:
			return sMaxTextureUnits;
	}
	return 0;
}

//----------------------------------------------------------------//
u32 zglGetError () {

	ASSERT_OPERATION_DEPTH ();

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
cc8* zglGetErrorString ( u32 error ) {

	ASSERT_OPERATION_DEPTH ();

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

	ASSERT_OPERATION_DEPTH ();
	return ( cc8* )glGetString ( _remapEnum ( stringID ));
}

//----------------------------------------------------------------//
void zglLineWidth ( float width ) {

	ASSERT_OPERATION_DEPTH ();
	glLineWidth (( GLfloat )width );
}

//----------------------------------------------------------------//
void zglReadPixels ( s32 x, s32 y, u32 width, u32 height, void* data ) {

	ASSERT_OPERATION_DEPTH ();
	glReadPixels (( GLint )x, ( GLint )y, ( GLsizei )width, ( GLsizei )height, GL_RGBA, GL_UNSIGNED_BYTE, ( GLvoid* )data );
}

//----------------------------------------------------------------//
void zglScissor ( s32 x, s32 y, u32 w, u32 h ) {

	ASSERT_OPERATION_DEPTH ();
	glScissor (( GLint )x, ( GLint )y, ( GLsizei )w, ( GLsizei )h );
}

//----------------------------------------------------------------//
void zglViewport ( s32 x, s32 y, u32 w, u32 h ) {

	ASSERT_OPERATION_DEPTH ();
	glViewport (( GLint )x, ( GLint )y, ( GLsizei )w, ( GLsizei )h );
}

//================================================================//
// vertex format
//================================================================//

//----------------------------------------------------------------//
void zglColorPointer ( u32 size, u32 type, u32 stride, const void* pointer ) {


	ASSERT_OPERATION_DEPTH ();

	#if !MOAI_OS_NACL
		glColorPointer (( GLint )size, _remapEnum ( type ), ( GLsizei )stride, ( const GLvoid* )pointer );
	#endif
}

//----------------------------------------------------------------//
void zglDisableClientState ( u32 cap ) {


	ASSERT_OPERATION_DEPTH ();

	#if !MOAI_OS_NACL
		glDisableClientState ( _remapEnum ( cap ));
	#endif
}

//----------------------------------------------------------------//
void zglDisableVertexAttribArray ( u32 index ) {

	ASSERT_OPERATION_DEPTH ();
	glDisableVertexAttribArray (( GLuint )index );
}

//----------------------------------------------------------------//
void zglNormalPointer ( u32 type, u32 stride, const void* pointer ) {

	ASSERT_OPERATION_DEPTH ();

	#if !MOAI_OS_NACL
		glNormalPointer ( _remapEnum ( type ), ( GLsizei )stride, ( const GLvoid* )pointer );
	#endif
}

//----------------------------------------------------------------//
void zglTexCoordPointer ( u32 size, u32 type, u32 stride, const void* pointer ) {


	ASSERT_OPERATION_DEPTH ();

	#if !MOAI_OS_NACL
		glTexCoordPointer (( GLint )size, _remapEnum ( type ), ( GLsizei )stride, ( const GLvoid* )pointer );
	#endif
}

//----------------------------------------------------------------//
void zglVertexAttribPointer ( u32 index, u32 size, u32 type, bool normalized, u32 stride, const void* pointer ) {

	ASSERT_OPERATION_DEPTH ();

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

	ASSERT_OPERATION_DEPTH ();

	#if !MOAI_OS_NACL
		glVertexPointer (( GLint )size, _remapEnum ( type ), ( GLsizei )stride, ( const GLvoid* )pointer );
	#endif
}

//----------------------------------------------------------------//
void zglEnableClientState ( u32 cap ) {

	ASSERT_OPERATION_DEPTH ();

	#if !MOAI_OS_NACL
		glEnableClientState ( _remapEnum ( cap ));
	#endif
}

//----------------------------------------------------------------//
void zglEnableVertexAttribArray ( u32 index ) {

	ASSERT_OPERATION_DEPTH ();
	glEnableVertexAttribArray (( GLuint )index );
}

//================================================================//
// shaders
//================================================================//

//----------------------------------------------------------------//
void zglAttachShader ( u32 program, u32 shader ) {

	ASSERT_OPERATION_DEPTH ();
	glAttachShader (( GLuint )program, ( GLuint )shader );
}

//----------------------------------------------------------------//
void zglBindAttribLocation ( u32 program, u32 index, cc8* name ) {

	ASSERT_OPERATION_DEPTH ();
	glBindAttribLocation (( GLuint )program, ( GLuint )index, ( const GLchar* )name );
}

//----------------------------------------------------------------//
void zglCompileShader ( u32 shader ) {

	ASSERT_OPERATION_DEPTH ();
	glCompileShader ( shader );
}

//----------------------------------------------------------------//
u32 zglCreateProgram () {

	ASSERT_OPERATION_DEPTH ();
	return ( u32 )glCreateProgram ();
}

//----------------------------------------------------------------//
u32 zglCreateShader ( u32 shaderType ) {

	ASSERT_OPERATION_DEPTH ();
	return ( u32 )glCreateShader ( _remapEnum ( shaderType ));
}

//----------------------------------------------------------------//
void zglGetProgramInfoLog ( u32 program, u32 maxLength, u32* length, char* log ) {

	ASSERT_OPERATION_DEPTH ();
	glGetProgramInfoLog (( GLuint )program, ( GLsizei )maxLength, ( GLsizei* )length, ( GLchar* )log );
}

//----------------------------------------------------------------//
void zglGetProgramiv ( u32 program, u32 name, s32* params ) {

	ASSERT_OPERATION_DEPTH ();
	glGetProgramiv (( GLuint )program, _remapEnum ( name ), ( GLint* )params );
}

//----------------------------------------------------------------//
void zglGetShaderInfoLog ( u32 shader, u32 maxLength, u32* length, char* log ) {

	ASSERT_OPERATION_DEPTH ();
	glGetShaderInfoLog (( GLuint )shader, ( GLsizei )maxLength, ( GLsizei* )length, ( GLchar* )log );
}

//----------------------------------------------------------------//
void zglGetShaderiv ( u32 shader, u32 name, s32* params ) {

	ASSERT_OPERATION_DEPTH ();
	glGetShaderiv (( GLuint )shader, _remapEnum ( name ), params );
}

//----------------------------------------------------------------//
u32 zglGetUniformLocation ( u32 program, cc8* name ) {

	ASSERT_OPERATION_DEPTH ();
	return ( u32 )glGetUniformLocation (( GLuint )program, ( const GLchar* )name );
}

//----------------------------------------------------------------//
void zglLinkProgram ( u32 program ) {

	ASSERT_OPERATION_DEPTH ();
	glLinkProgram (( GLuint )program );
}

//----------------------------------------------------------------//
void zglShaderSource ( u32 shader, u32 count, const char** string, const s32* length ) {

	ASSERT_OPERATION_DEPTH ();
	glShaderSource (( GLuint )shader, ( GLsizei )count, ( const GLchar** )string, ( const GLint* )length );
}

//----------------------------------------------------------------//
void zglValidateProgram ( u32 program ) {

	ASSERT_OPERATION_DEPTH ();
	glValidateProgram (( GLuint )program );
}

//----------------------------------------------------------------//
void zglUniform1f ( u32 location, float v0 ) {

	ASSERT_OPERATION_DEPTH ();
	glUniform1f (( GLint )location, ( GLfloat )v0 );
}

//----------------------------------------------------------------//
void zglUniform1i ( u32 location, s32 v0 ) {

	ASSERT_OPERATION_DEPTH ();
	glUniform1i (( GLint )location, ( GLint )v0 );
}

//----------------------------------------------------------------//
void zglUniform4fv ( u32 location, u32 count, const float* value ) {

	ASSERT_OPERATION_DEPTH ();
	glUniform4fv (( GLint )location, ( GLsizei )count, ( const GLfloat* )value );
}

//----------------------------------------------------------------//
void zglUniformMatrix3fv ( u32 location, u32 count, bool transpose, const float* value ) {

	ASSERT_OPERATION_DEPTH ();
	glUniformMatrix3fv (( GLint )location, ( GLsizei )count, transpose ? GL_TRUE : GL_FALSE, ( const GLfloat* )value );
}

//----------------------------------------------------------------//
void zglUniformMatrix4fv ( u32 location, u32 count, bool transpose, const float* value ) {

	ASSERT_OPERATION_DEPTH ();
	glUniformMatrix4fv (( GLint )location, ( GLsizei )count, transpose ? GL_TRUE : GL_FALSE, ( const GLfloat* )value );
}

//----------------------------------------------------------------//
void zglUseProgram ( u32 program ) {

	ASSERT_OPERATION_DEPTH ();
	glUseProgram (( GLuint )program );
}

//================================================================//
// texture
//================================================================//

//----------------------------------------------------------------//
void zglBindTexture ( u32 texID ) {

	ASSERT_OPERATION_DEPTH ();
	glBindTexture ( GL_TEXTURE_2D, ( GLuint )texID );
}

//----------------------------------------------------------------//
void zglCompressedTexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 imageSize, const void* data ) {

	ASSERT_OPERATION_DEPTH ();

	glCompressedTexImage2D (
		GL_TEXTURE_2D,
		( GLint )level,
		( GLint )_remapEnum ( internalFormat ),
		( GLsizei )width,
		( GLsizei )height,
		0,
		( GLsizei )imageSize,
		( const GLvoid* )data
	);
}

//----------------------------------------------------------------//
u32 zglCreateTexture () {

	ASSERT_OPERATION_DEPTH ();

	u32 textureID;
	glGenTextures ( 1, ( GLuint* )&textureID );
	return textureID;
}

//----------------------------------------------------------------//
void zglTexEnvi ( u32 pname, s32 param ) {

	ASSERT_OPERATION_DEPTH ();

	#if !MOAI_OS_NACL
		glTexEnvi ( GL_TEXTURE_ENV, _remapEnum ( pname ), ( GLint )param );
	#endif
}

//----------------------------------------------------------------//
void zglTexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, const void* data ) {

	ASSERT_OPERATION_DEPTH ();

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

	ASSERT_OPERATION_DEPTH ();
	glTexParameteri ( GL_TEXTURE_2D, _remapEnum ( pname ), ( GLint )_remapEnum ( param ));
}

//----------------------------------------------------------------//
void zglTexSubImage2D ( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, const void* data ) {

	ASSERT_OPERATION_DEPTH ();

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

	ASSERT_OPERATION_DEPTH ();
	glBindFramebuffer ( _remapEnum ( target ), frameBuffer );
}

//----------------------------------------------------------------//
void zglBindRenderbuffer ( u32 renderbuffer ) {

	ASSERT_OPERATION_DEPTH ();
	glBindRenderbuffer ( GL_RENDERBUFFER, renderbuffer );
}

//----------------------------------------------------------------//
u32 zglCheckFramebufferStatus ( u32 target ) {

	ASSERT_OPERATION_DEPTH ();

	GLenum status = glCheckFramebufferStatus ( _remapEnum ( target ));
	return status == GL_FRAMEBUFFER_COMPLETE ? ZGL_FRAMEBUFFER_STATUS_COMPLETE : 0;
}

//----------------------------------------------------------------//
u32 zglCreateFramebuffer () {

	ASSERT_OPERATION_DEPTH ();

	u32 bufferID;
	glGenFramebuffers ( 1, &bufferID );
	return bufferID;
}

//----------------------------------------------------------------//
u32 zglCreateRenderbuffer () {

	ASSERT_OPERATION_DEPTH ();

	u32 bufferID;
	glGenRenderbuffers ( 1, &bufferID );
	return bufferID;
}

//----------------------------------------------------------------//
void zglFramebufferRenderbuffer ( u32 target, u32 attachment, u32 renderbuffer ) {

	ASSERT_OPERATION_DEPTH ();
	glFramebufferRenderbuffer ( _remapEnum ( target ), _remapEnum ( attachment ), GL_RENDERBUFFER, ( GLuint )renderbuffer );
}

//----------------------------------------------------------------//
void zglFramebufferTexture2D ( u32 target, u32 attachment, u32 texture, s32 level ) {

	ASSERT_OPERATION_DEPTH ();
	glFramebufferTexture2D ( _remapEnum ( target ), _remapEnum ( attachment ), GL_TEXTURE_2D, ( GLuint )texture, ( GLint )level );
}

//----------------------------------------------------------------//
u32 zglGetCurrentFramebuffer () {

	ASSERT_OPERATION_DEPTH ();

	int buffer;
	glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &buffer );
	return ( u32 )buffer;
}

//----------------------------------------------------------------//
void zglRenderbufferStorage ( u32 internalFormat, u32 width, u32 height ) {

	ASSERT_OPERATION_DEPTH ();
	glRenderbufferStorage ( GL_RENDERBUFFER, _remapEnum ( internalFormat ), ( GLsizei )width, ( GLsizei )height );
}

//================================================================//
// buffer
//================================================================//

//----------------------------------------------------------------//
void zglBindBuffer ( u32 target, u32 buffer ) {

	ASSERT_OPERATION_DEPTH ();
	glBindBuffer ( _remapEnum ( target ), buffer );
}

//----------------------------------------------------------------//
void zglBufferData ( u32 target, u32 size, const void* data, u32 usage ) {

	ASSERT_OPERATION_DEPTH ();
	glBufferData ( _remapEnum ( target ), ( GLsizeiptr )size, ( const GLvoid* )data, _remapEnum ( usage ));
}

//----------------------------------------------------------------//
void zglBufferSubData ( u32 target, u32 offset, u32 size, const void* data ) {

	ASSERT_OPERATION_DEPTH ();
	glBufferSubData ( _remapEnum ( target ), ( GLintptr )offset, ( GLsizeiptr )size, ( const GLvoid* )data );
}

//----------------------------------------------------------------//
u32 zglCreateBuffer () {

	ASSERT_OPERATION_DEPTH ();

	u32 bufferID;
	glGenBuffers ( 1, &bufferID );
	return bufferID;
}

//----------------------------------------------------------------//
void* zglMapBuffer ( u32 target ) {

	ASSERT_OPERATION_DEPTH ();
	#if defined( MOAI_OS_ANDROID ) || defined( MOAI_OS_HTML )
		return 0;
	#else
		return glMapBuffer ( _remapEnum ( target ), GL_WRITE_ONLY );
	#endif
}

//----------------------------------------------------------------//
void zglUnmapBuffer ( u32 target ) {

	ASSERT_OPERATION_DEPTH ();
	#if defined( MOAI_OS_ANDROID ) || defined( MOAI_OS_HTML )
		return;
	#else
		glUnmapBuffer ( _remapEnum ( target ));
	#endif
}

//================================================================//
// vertex array
//================================================================//

//----------------------------------------------------------------//
void zglBindVertexArray ( u32 vertexArrayID ) {

	ASSERT_OPERATION_DEPTH ();

	#if defined( MOAI_OS_ANDROID ) || defined( MOAI_OS_HTML )
		return;
	#else
		glBindVertexArray ( vertexArrayID );
	#endif
}

//----------------------------------------------------------------//
u32 zglCreateVertexArray () {

	ASSERT_OPERATION_DEPTH ();

	#if defined( MOAI_OS_ANDROID ) || defined( MOAI_OS_HTML )
		return 0;
	#else
		u32 vertexArrayID;
		glGenVertexArrays ( 1, &vertexArrayID );
		return vertexArrayID;
	#endif
}

//----------------------------------------------------------------//
void zglDeleteVertexArray ( u32 vertexArrayID ) {

	ASSERT_OPERATION_DEPTH ();

	#if defined( MOAI_OS_ANDROID ) || defined( MOAI_OS_HTML )
		return;
	#else
		glDeleteVertexArrays ( 1, &vertexArrayID );
	#endif
}

#endif
