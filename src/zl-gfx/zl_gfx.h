// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_GFX_H
#define ZL_GFX_H

#include <zl-gfx/pch.h>

//================================================================//
// gfx
//================================================================//

//----------------------------------------------------------------//
//extern void		zglBegin				();
//extern void		zglEnd					();
//extern void		zglFinalize				();
//extern void		zglInitialize			();

//----------------------------------------------------------------//
//extern void		zglActiveTexture		( u32 textureUnit );
//extern void		zglBlendFunc			( u32 sourceFactor, u32 destFactor );
//extern void		zglBlendMode			( u32 mode );
//extern void		zglClear				( u32 mask );
//extern void		zglClearColor			( float r, float g, float b, float a );
//extern void		zglColor				( float r, float g, float b, float a );
//extern void		zglCullFace				( u32 mode );
//extern void		zglDeleteBuffer			( u32 buffer );
//extern void		zglDeleteFramebuffer	( u32 buffer );
//extern void		zglDeleteProgram		( u32 program );
//extern void		zglDeleteRenderbuffer	( u32 buffer );
//extern void		zglDeleteShader			( u32 shader );
//extern void		zglDeleteTexture		( u32 texture );
//extern void		zglDepthFunc			( u32 depthFunc );
//extern void		zglDepthMask			( bool flag );
//extern void		zglDisable				( u32 cap );
//extern void		zglDrawArrays			( u32 primType, u32 first, u32 count );
//extern void		zglDrawElements			( u32 primType, u32 count, u32 indexType, const void* indices );
//extern void		zglEnable				( u32 cap );
//extern void		zglFlush				();
//extern u32		zglGetCap				( u32 cap );
//extern u32		zglGetError				();
//extern cc8*		zglGetErrorString		( u32 error );
//extern cc8*		zglGetString			( u32 stringID );
//extern void		zglLineWidth			( float width );
//extern void		zglReadPixels			( s32 x, s32 y, u32 width, u32 height, void* data );
//extern void		zglScissor				( s32 x, s32 y, u32 w, u32 h );
//extern void		zglViewport				( s32 x, s32 y, u32 w, u32 h );

//----------------------------------------------------------------//
//extern void		zglDisableClientState			( u32 cap );
//extern void		zglDisableVertexAttribArray		( u32 index );
//extern void		zglVertexAttribPointer			( u32 index, u32 size, u32 type, bool normalized, u32 stride, const void* pointer );
//extern void		zglEnableClientState			( u32 cap );
//extern void		zglEnableVertexAttribArray		( u32 index );

//----------------------------------------------------------------//
//extern void		zglAttachShader					( u32 program, u32 shader );
//extern void		zglBindAttribLocation			( u32 program, u32 index, cc8* name );
//extern void		zglCompileShader				( u32 shader );
//extern u32		zglCreateProgram				();
//extern u32		zglCreateShader					( u32 shaderType );
//extern void		zglGetProgramInfoLog			( u32 program, u32 maxLength, u32* length, char* log );
//extern void		zglGetProgramiv					( u32 program, u32 name, s32* params );
//extern void		zglGetShaderInfoLog				( u32 shader, u32 maxLength, u32* length, char* log );
//extern void		zglGetShaderiv					( u32 shader, u32 name, s32* params );
//extern u32		zglGetUniformLocation			( u32 program, cc8* name );
//extern void		zglLinkProgram					( u32 program );
//extern void		zglShaderSource					( u32 shader, u32 count, const char** string, const s32* length );
//extern void		zglValidateProgram				( u32 program );
//extern void		zglUniform1f					( u32 location, float v0 );
//extern void		zglUniform1i					( u32 location, s32 v0 );
//extern void		zglUniform4fv					( u32 location, u32 count, const float* value );
//extern void		zglUniformMatrix3fv				( u32 location, u32 count, bool transpose, const float* value );
//extern void		zglUniformMatrix4fv				( u32 location, u32 count, bool transpose, const float* value );
//extern void		zglUseProgram					( u32 program );

//----------------------------------------------------------------//
//extern void		zglBindTexture					( u32 texID );
//extern void		zglCompressedTexImage2D			( u32 level, u32 internalFormat, u32 width, u32 height, u32 imageSize, const void* data );
//extern u32		zglCreateTexture				();
//extern void		zglTexEnvi						( u32 pname, s32 param );
//extern void		zglTexImage2D					( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, const void* data );
//extern void		zglTexParameteri				( u32 pname, s32 param );
//extern void		zglTexSubImage2D				( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, const void* data );

//----------------------------------------------------------------//
//extern void		zglBindFramebuffer				( u32 target, u32 frameBuffer );
//extern void		zglBindRenderbuffer				( u32 renderbuffer );
//extern u32		zglCheckFramebufferStatus		( u32 target );
//extern u32		zglCreateFramebuffer			();
//extern u32		zglCreateRenderbuffer			();
//extern void		zglFramebufferRenderbuffer		( u32 target, u32 attachment, u32 renderbuffer );
//extern void		zglFramebufferTexture2D			( u32 target, u32 attachment, u32 texture, s32 level );
//extern u32		zglGetCurrentFramebuffer		();
//extern void		zglRenderbufferStorage			( u32 internalFormat, u32 width, u32 height );

//----------------------------------------------------------------//
//extern void		zglBindBuffer					( u32 target, u32 buffer );
//extern void		zglBufferData					( u32 target, u32 size, const void* data, u32 usage );
//extern void		zglBufferSubData				( u32 target, u32 offset, u32 size, const void* data );
//extern u32		zglCreateBuffer					();
//extern void*		zglMapBuffer					( u32 target );
//extern void		zglUnmapBuffer					( u32 target );

//----------------------------------------------------------------//
//extern void		zglBindVertexArray				( u32 vertexArrayID );
//extern u32		zglCreateVertexArray			();
//extern void		zglDeleteVertexArray			( u32 vertexArrayID );

#endif
