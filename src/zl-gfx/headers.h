// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_GFX_H
#define ZL_GFX_H

#include <zl-gfx/pch.h>

// hardware PVR support is based on device
#ifdef MOAI_OS_IPHONE
	#define ZGL_DEVCAPS_PVR_TEXTURE 1
#else
	#define ZGL_DEVCAPS_PVR_TEXTURE 0
#endif

//================================================================//
// gfx
//================================================================//

enum {

	ZGL_BLEND_FACTOR_DST_ALPHA,
	ZGL_BLEND_FACTOR_DST_COLOR,
	ZGL_BLEND_FACTOR_ONE,
	ZGL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
	ZGL_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
	ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	ZGL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
	ZGL_BLEND_FACTOR_SRC_ALPHA,
	ZGL_BLEND_FACTOR_SRC_ALPHA_SATURATE,
	ZGL_BLEND_FACTOR_SRC_COLOR,
	ZGL_BLEND_FACTOR_ZERO,

	ZGL_BLEND_MODE_ADD,
	ZGL_BLEND_MODE_MAX,
	ZGL_BLEND_MODE_MIN,
	ZGL_BLEND_MODE_REVERSE_SUBTRACT,
	ZGL_BLEND_MODE_SUBTRACT,

	ZGL_BUFFER_TARGET_ARRAY,
	ZGL_BUFFER_TARGET_ELEMENT_ARRAY,

	ZGL_BUFFER_USAGE_DYNAMIC_COPY,
	ZGL_BUFFER_USAGE_DYNAMIC_DRAW,
	ZGL_BUFFER_USAGE_DYNAMIC_READ,
	ZGL_BUFFER_USAGE_STATIC_COPY,
	ZGL_BUFFER_USAGE_STATIC_DRAW,
	ZGL_BUFFER_USAGE_STATIC_READ,
	ZGL_BUFFER_USAGE_STREAM_COPY,
	ZGL_BUFFER_USAGE_STREAM_DRAW,
	ZGL_BUFFER_USAGE_STREAM_READ,

	ZGL_COMPOSE_MODULATE,

	ZGL_CULL_ALL,
	ZGL_CULL_BACK,
	ZGL_CULL_FRONT,

	ZGL_DEPTH_ALWAYS,
	ZGL_DEPTH_EQUAL,
	ZGL_DEPTH_LESS,
	ZGL_DEPTH_LEQUAL,
	ZGL_DEPTH_GEQUAL,
	ZGL_DEPTH_GREATER,
	ZGL_DEPTH_NEVER,
	ZGL_DEPTH_NOTEQUAL,

	ZGL_ERROR_INVALID_ENUM,
	ZGL_ERROR_INVALID_OPERATION,
	ZGL_ERROR_INVALID_VALUE,
	ZGL_ERROR_NONE,
	ZGL_ERROR_OUT_OF_MEMORY,
	ZGL_ERROR_STACK_OVERFLOW,
	ZGL_ERROR_STACK_UNDERFLOW,

	ZGL_ERROR_UNKNOWN,

	ZGL_FRAMEBUFFER_ATTACHMENT_COLOR,
	ZGL_FRAMEBUFFER_ATTACHMENT_DEPTH,
	ZGL_FRAMEBUFFER_ATTACHMENT_STENCIL,

	ZGL_FRAMEBUFFER_STATUS_COMPLETE,

	ZGL_FRAMEBUFFER_TARGET_DRAW,
	ZGL_FRAMEBUFFER_TARGET_READ,
	ZGL_FRAMEBUFFER_TARGET_DRAW_READ,

	ZGL_MATRIX_COLOR,
	ZGL_MATRIX_MODELVIEW,
	ZGL_MATRIX_PROJECTION,
	ZGL_MATRIX_TEXTURE,

	ZGL_PIPELINE_BLEND,
	ZGL_PIPELINE_COLOR_ARRAY,
	ZGL_PIPELINE_CULL,
	ZGL_PIPELINE_DEPTH,
	ZGL_PIPELINE_NORMAL_ARRAY,
	ZGL_PIPELINE_SCISSOR,
	ZGL_PIPELINE_TEXTURE_2D,
	ZGL_PIPELINE_TEXTURE_COORD_ARRAY,
	ZGL_PIPELINE_VERTEX_ARRAY,

	ZGL_PIXEL_FORMAT_ALPHA,
	ZGL_PIXEL_FORMAT_LUMINANCE,
	ZGL_PIXEL_FORMAT_LUMINANCE_ALPHA,
	ZGL_PIXEL_FORMAT_RED,
	ZGL_PIXEL_FORMAT_RG,
	ZGL_PIXEL_FORMAT_RGB,
	ZGL_PIXEL_FORMAT_RGB4,
	ZGL_PIXEL_FORMAT_RGB5_A1,
	ZGL_PIXEL_FORMAT_RGB565,
	ZGL_PIXEL_FORMAT_RGB8,
	ZGL_PIXEL_FORMAT_BGR,
	ZGL_PIXEL_FORMAT_RGBA,
	ZGL_PIXEL_FORMAT_RGBA4,
	ZGL_PIXEL_FORMAT_RGBA8,
	ZGL_PIXEL_FORMAT_BGRA,

	ZGL_PIXEL_FORMAT_DEPTH_COMPONENT16,
	ZGL_PIXEL_FORMAT_STENCIL_INDEX8,

	ZGL_PIXEL_TYPE_BYTE,
	ZGL_PIXEL_TYPE_COMPRESSED_RGB_PVRTC_2BPPV1_IMG,
	ZGL_PIXEL_TYPE_COMPRESSED_RGB_PVRTC_4BPPV1_IMG,
	ZGL_PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG,
	ZGL_PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG,
	ZGL_PIXEL_TYPE_FLOAT,
	ZGL_PIXEL_TYPE_INT,
	ZGL_PIXEL_TYPE_SHORT,
	ZGL_PIXEL_TYPE_UNSIGNED_BYTE,
	ZGL_PIXEL_TYPE_UNSIGNED_BYTE_2_3_3_REV,
	ZGL_PIXEL_TYPE_UNSIGNED_BYTE_3_3_2,
	ZGL_PIXEL_TYPE_UNSIGNED_INT,
	ZGL_PIXEL_TYPE_UNSIGNED_INT_8_8_8_8,
	ZGL_PIXEL_TYPE_UNSIGNED_INT_8_8_8_8_REV,
	ZGL_PIXEL_TYPE_UNSIGNED_INT_2_10_10_10_REV,
	ZGL_PIXEL_TYPE_UNSIGNED_INT_10_10_10_2,
	ZGL_PIXEL_TYPE_UNSIGNED_SHORT,
	ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_6_5,
	ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_6_5_REV,
	ZGL_PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4,
	ZGL_PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4_REV,
	ZGL_PIXEL_TYPE_UNSIGNED_SHORT_1_5_5_5_REV,
	ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1,

	ZGL_PRIM_LINE_LOOP,
	ZGL_PRIM_LINE_STRIP,
	ZGL_PRIM_LINES,
	ZGL_PRIM_POINTS,
	ZGL_PRIM_TRIANGLE_FAN,
	ZGL_PRIM_TRIANGLE_STRIP,
	ZGL_PRIM_TRIANGLES,

	ZGL_PROGRAM_INFO_ACTIVE_ATTRIBUTES,
	ZGL_PROGRAM_INFO_ACTIVE_ATTRIBUTE_MAX_LENGTH,
	ZGL_PROGRAM_INFO_ACTIVE_UNIFORMS,
	ZGL_PROGRAM_INFO_ACTIVE_UNIFORM_MAX_LENGTH,
	ZGL_PROGRAM_INFO_ATTACHED_SHADERS,
	ZGL_PROGRAM_INFO_DELETE_STATUS,
	ZGL_PROGRAM_INFO_LOG_LENGTH,
	ZGL_PROGRAM_INFO_LINK_STATUS,
	ZGL_PROGRAM_INFO_VALIDATE_STATUS,

	ZGL_SAMPLE_LINEAR,
	ZGL_SAMPLE_LINEAR_MIPMAP_LINEAR,
	ZGL_SAMPLE_LINEAR_MIPMAP_NEAREST,
	ZGL_SAMPLE_NEAREST,
	ZGL_SAMPLE_NEAREST_MIPMAP_LINEAR,
	ZGL_SAMPLE_NEAREST_MIPMAP_NEAREST,

	ZGL_SHADER_INFO_COMPILE_STATUS,
	ZGL_SHADER_INFO_DELETE_STATUS,
	ZGL_SHADER_INFO_LOG_LENGTH,
	ZGL_SHADER_INFO_SOURCE_LENGTH,
	ZGL_SHADER_INFO_TYPE,

	ZGL_SHADER_TYPE_TESS_CONTROL,
	ZGL_SHADER_TYPE_TESS_EVALUATION,
	ZGL_SHADER_TYPE_FRAGMENT,
	ZGL_SHADER_TYPE_GEOMETRY,
	ZGL_SHADER_TYPE_VERTEX,

	ZGL_STRING_VENDOR,
	ZGL_STRING_VERSION,
	ZGL_STRING_RENDERER,
	ZGL_STRING_SHADING_LANGUAGE_VERSION,

	ZGL_TEXTURE_BASE_LEVEL,
	ZGL_TEXTURE_COMPARE_FUNC,
	ZGL_TEXTURE_COMPARE_MODE,
	//ZGL_TEXTURE_DEPTH_STENCIL_MODE,
	ZGL_TEXTURE_ENV_MODE,
	ZGL_TEXTURE_LOD_BIAS,
	ZGL_TEXTURE_MAG_FILTER,
	ZGL_TEXTURE_MIN_FILTER,
	ZGL_TEXTURE_MAX_LEVEL,
	ZGL_TEXTURE_MAX_LOD,
	ZGL_TEXTURE_MIN_LOD,
	ZGL_TEXTURE_SWIZZLE_A,
	ZGL_TEXTURE_SWIZZLE_B,
	ZGL_TEXTURE_SWIZZLE_G,
	ZGL_TEXTURE_SWIZZLE_R,
	ZGL_TEXTURE_WRAP_R,
	ZGL_TEXTURE_WRAP_S,
	ZGL_TEXTURE_WRAP_T,

	ZGL_TYPE_BYTE,
	ZGL_TYPE_DOUBLE,
	ZGL_TYPE_FLOAT,
	ZGL_TYPE_INT,
	ZGL_TYPE_SHORT,
	ZGL_TYPE_UNSIGNED_BYTE,
	ZGL_TYPE_UNSIGNED_INT,
	ZGL_TYPE_UNSIGNED_SHORT,

	ZGL_WRAP_MODE_CLAMP,
	ZGL_WRAP_MODE_REPEAT,
};

enum {
	ZGL_CAPS_IS_FRAMEBUFFER_SUPPORTED,
	ZGL_CAPS_IS_PROGRAMMABLE,
	ZGL_CAPS_MAX_TEXTURE_SIZE,
	ZGL_CAPS_MAX_TEXTURE_UNITS,
};

enum {
	ZGL_CLEAR_COLOR_BUFFER_BIT		= 0x01,
	ZGL_CLEAR_DEPTH_BUFFER_BIT		= 0x02,
	ZGL_CLEAR_STENCIL_BUFFER_BIT	= 0x04,
};

#define ZGL_INVALID_UNIFORM_ADDR (( u32 )-1 )

//----------------------------------------------------------------//
extern u32 zglMapFromGLEnum( u32 glEnum ); // don't do this; this should be local to the OpenGL implementation

extern void		zglBegin				();
extern void		zglEnd					();
extern void		zglFinalize				();
extern void		zglInitialize			();

//----------------------------------------------------------------//
extern void		zglActiveTexture		( u32 textureUnit );
extern void		zglBlendFunc			( u32 sourceFactor, u32 destFactor );
extern void		zglBlendMode			( u32 mode );
extern void		zglClear				( u32 mask );
extern void		zglClearColor			( float r, float g, float b, float a );
extern void		zglColor				( float r, float g, float b, float a );
extern void		zglCullFace				( u32 mode );
extern void		zglDeleteBuffer			( u32 buffer );
extern void		zglDeleteFramebuffer	( u32 buffer );
extern void		zglDeleteProgram		( u32 program );
extern void		zglDeleteRenderbuffer	( u32 buffer );
extern void		zglDeleteShader			( u32 shader );
extern void		zglDeleteTexture		( u32 texture );
extern void		zglDepthFunc			( u32 depthFunc );
extern void		zglDepthMask			( bool flag );
extern void		zglDisable				( u32 cap );
extern void		zglDrawArrays			( u32 primType, u32 first, u32 count );
extern void		zglDrawElements			( u32 primType, u32 count, u32 indexType, const void* indices );
extern void		zglEnable				( u32 cap );
extern void		zglFlush				();
extern u32		zglGetCap				( u32 cap );
extern u32		zglGetError				();
extern cc8*		zglGetErrorString		( u32 error );
extern cc8*		zglGetString			( u32 stringID );
extern void		zglLineWidth			( float width );
extern void		zglLoadIdentity			();
extern void		zglLoadMatrix			( const float* matrix );
extern void		zglMatrixMode			( u32 mode );
extern void		zglMultMatrix			( const float* matrix );
extern void		zglPointSize			( float size );
extern void		zglReadPixels			( s32 x, s32 y, u32 width, u32 height, void* data );
extern void		zglScissor				( s32 x, s32 y, u32 w, u32 h );
extern void		zglViewport				( s32 x, s32 y, u32 w, u32 h );

//----------------------------------------------------------------//
extern void		zglColorPointer					( u32 size, u32 type, u32 stride, const void* pointer );
extern void		zglDisableClientState			( u32 cap );
extern void		zglDisableVertexAttribArray		( u32 index );
extern void		zglNormalPointer				( u32 type, u32 stride, const void* pointer );
extern void		zglTexCoordPointer				( u32 size, u32 type, u32 stride, const void* pointer );
extern void		zglVertexAttribPointer			( u32 index, u32 size, u32 type, bool normalized, u32 stride, const void* pointer );
extern void		zglVertexPointer				( u32 size, u32 type, u32 stride, const void* pointer );
extern void		zglEnableClientState			( u32 cap );
extern void		zglEnableVertexAttribArray		( u32 index );

//----------------------------------------------------------------//
extern void		zglAttachShader					( u32 program, u32 shader );
extern void		zglBindAttribLocation			( u32 program, u32 index, cc8* name );
extern void		zglCompileShader				( u32 shader );
extern u32		zglCreateProgram				();
extern u32		zglCreateShader					( u32 shaderType );
extern void		zglGetProgramInfoLog			( u32 program, u32 maxLength, u32* length, char* log );
extern void		zglGetProgramiv					( u32 program, u32 name, s32* params );
extern void		zglGetShaderInfoLog				( u32 shader, u32 maxLength, u32* length, char* log );
extern void		zglGetShaderiv					( u32 shader, u32 name, s32* params );
extern u32		zglGetUniformLocation			( u32 program, cc8* name );
extern void		zglLinkProgram					( u32 program );
extern void		zglShaderSource					( u32 shader, u32 count, const char** string, const s32* length );
extern void		zglValidateProgram				( u32 program );
extern void		zglUniform1f					( u32 location, float v0 );
extern void		zglUniform1i					( u32 location, s32 v0 );
extern void		zglUniform4fv					( u32 location, u32 count, const float* value );
extern void		zglUniformMatrix3fv				( u32 location, u32 count, bool transpose, const float* value );
extern void		zglUniformMatrix4fv				( u32 location, u32 count, bool transpose, const float* value );
extern void		zglUseProgram					( u32 program );

//----------------------------------------------------------------//
extern void		zglBindTexture					( u32 texID );
extern void		zglCompressedTexImage2D			( u32 level, u32 internalFormat, u32 width, u32 height, u32 imageSize, const void* data );
extern u32		zglCreateTexture				();
extern void		zglTexEnvi						( u32 pname, s32 param );
extern void		zglTexImage2D					( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, const void* data );
extern void		zglTexParameteri				( u32 pname, s32 param );
extern void		zglTexSubImage2D				( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, const void* data );

//----------------------------------------------------------------//
extern void		zglBindFramebuffer				( u32 target, u32 frameBuffer );
extern void		zglBindRenderbuffer				( u32 renderbuffer );
extern u32		zglCheckFramebufferStatus		( u32 target );
extern u32		zglCreateFramebuffer			();
extern u32		zglCreateRenderbuffer			();
extern void		zglFramebufferRenderbuffer		( u32 target, u32 attachment, u32 renderbuffer );
extern void		zglFramebufferTexture2D			( u32 target, u32 attachment, u32 texture, s32 level );
extern u32		zglGetCurrentFramebuffer		();
extern void		zglRenderbufferStorage			( u32 internalFormat, u32 width, u32 height );

//----------------------------------------------------------------//
extern void		zglBindBuffer					( u32 target, u32 buffer );
extern void		zglBufferData					( u32 target, u32 size, const void* data, u32 usage );
extern u32		zglCreateBuffer					();
extern void*	zglMapBuffer					( u32 target );
extern void		zglUnmapBuffer					( u32 target );

//----------------------------------------------------------------//
extern void		zglBindVertexArray				( u32 vertexArrayID );
extern u32		zglCreateVertexArray			();
extern void		zglDeleteVertexArray			( u32 vertexArrayID );

#endif
