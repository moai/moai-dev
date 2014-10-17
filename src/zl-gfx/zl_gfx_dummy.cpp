// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/headers.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_GFX_DUMMY

//================================================================//
// enums
//================================================================//

//----------------------------------------------------------------//
u32 zglMapFromGLEnum( u32 glEnum ) {
	UNUSED ( glEnum );
	return 0;
}

//================================================================//
// setup
//================================================================//

//----------------------------------------------------------------//
void zglBegin () {
}

//----------------------------------------------------------------//
void zglEnd () {
}

//----------------------------------------------------------------//
void zglFinalize () {
}

//----------------------------------------------------------------//
void zglInitialize () {
}

//================================================================//
// gfx
//================================================================//

//----------------------------------------------------------------//
void zglActiveTexture ( u32 textureUnit ) {
	UNUSED ( textureUnit );
}

//----------------------------------------------------------------//
void zglBlendFunc ( u32 sourceFactor, u32 destFactor ) {
	UNUSED ( sourceFactor );
	UNUSED ( destFactor );
}

//----------------------------------------------------------------//
void zglBlendMode ( u32 mode ) {
	UNUSED ( mode );
}

//----------------------------------------------------------------//
void zglClear ( u32 mask ) {
	UNUSED ( mask );
}

//----------------------------------------------------------------//
void zglClearColor ( float r, float g, float b, float a ) {
	UNUSED ( r );
	UNUSED ( g );
	UNUSED ( b );
	UNUSED ( a );
}

//----------------------------------------------------------------//
void zglColor ( float r, float g, float b, float a ) {
	UNUSED ( r );
	UNUSED ( g );
	UNUSED ( b );
	UNUSED ( a );
}

//----------------------------------------------------------------//
void zglCullFace ( u32 mode ) {
	UNUSED ( mode );
}

//----------------------------------------------------------------//
void zglDeleteBuffer ( u32 buffer ) {
	UNUSED ( buffer );
}

//----------------------------------------------------------------//
void zglDeleteFramebuffer ( u32 buffer ) {
	UNUSED ( buffer );
}

//----------------------------------------------------------------//
void zglDeleteProgram ( u32 program ) {
	UNUSED ( program );
}

//----------------------------------------------------------------//
void zglDeleteRenderbuffer ( u32 buffer ) {
	UNUSED ( buffer );
}

//----------------------------------------------------------------//
void zglDeleteShader ( u32 shader ) {
	UNUSED ( shader );
}

//----------------------------------------------------------------//
void zglDeleteTexture ( u32 texture ) {
	UNUSED ( texture );
}

//----------------------------------------------------------------//
void zglDepthFunc ( u32 depthFunc ) {
	UNUSED ( depthFunc );
}

//----------------------------------------------------------------//
void zglDepthMask ( bool flag ) {
	UNUSED ( flag );
}

//----------------------------------------------------------------//
void zglDisable ( u32 cap ) {
	UNUSED ( cap );
}

//----------------------------------------------------------------//
void zglDrawArrays ( u32 primType, u32 first, u32 count ) {
	UNUSED ( primType );
	UNUSED ( first );
	UNUSED ( count );
}

//----------------------------------------------------------------//
void zglDrawElements ( u32 primType, u32 count, u32 indexType, const void* indices ) {
	UNUSED ( primType );
	UNUSED ( count );
	UNUSED ( indexType );
	UNUSED ( indices );
}

//----------------------------------------------------------------//
void zglEnable ( u32 cap ) {
	UNUSED ( cap );
}

//----------------------------------------------------------------//
void zglFlush () {
}

//----------------------------------------------------------------//
u32 zglGetCap ( u32 cap ) {
	return 0;
}

//----------------------------------------------------------------//
u32 zglGetError () {
	return 0;
}

//----------------------------------------------------------------//
cc8* zglGetErrorString ( u32 error ) {
	UNUSED ( error );
	return "";
}

//----------------------------------------------------------------//
void zglGetIntegerv ( u32 name, s32* params ) {
	UNUSED ( name );
	UNUSED ( params );
}

//----------------------------------------------------------------//
extern cc8* zglGetString ( u32 stringID ) {
	UNUSED ( stringID );
	return "";
}

//----------------------------------------------------------------//
void zglLineWidth ( float width ) {
	UNUSED ( width );
}

//----------------------------------------------------------------//
void zglLoadIdentity () {
}

//----------------------------------------------------------------//
void zglLoadMatrix ( const float* matrix ) {
	UNUSED ( matrix );
}

//----------------------------------------------------------------//
void zglMatrixMode ( u32 mode ) {
	UNUSED ( mode );
}

//----------------------------------------------------------------//
void zglMultMatrix ( const float* matrix ) {
	UNUSED ( matrix );
}

//----------------------------------------------------------------//
void zglPointSize ( float size ) {
	UNUSED ( size );
}

//----------------------------------------------------------------//
void zglReadPixels ( s32 x, s32 y, u32 width, u32 height, void* data ) {
	UNUSED ( x );
	UNUSED ( y );
	UNUSED ( width );
	UNUSED ( height );
	UNUSED ( data );
}

//----------------------------------------------------------------//
void zglScissor ( s32 x, s32 y, u32 w, u32 h ) {
	UNUSED ( x );
	UNUSED ( y );
	UNUSED ( w );
	UNUSED ( h );
}

//----------------------------------------------------------------//
void zglViewport ( s32 x, s32 y, u32 w, u32 h ) {
	UNUSED ( x );
	UNUSED ( y );
	UNUSED ( w );
	UNUSED ( h );
}

//================================================================//
// vertex format
//================================================================//

//----------------------------------------------------------------//
void zglColorPointer ( u32 size, u32 type, u32 stride, const void* pointer ) {
	UNUSED ( size );
	UNUSED ( type );
	UNUSED ( stride );
	UNUSED ( pointer );
}

//----------------------------------------------------------------//
void zglDisableClientState ( u32 cap ) {
	UNUSED ( cap );
}

//----------------------------------------------------------------//
void zglDisableVertexAttribArray ( u32 index ) {
	UNUSED ( index );
}

//----------------------------------------------------------------//
void zglNormalPointer ( u32 type, u32 stride, const void* pointer ) {
	UNUSED ( type );
	UNUSED ( stride );
	UNUSED ( pointer );
}

//----------------------------------------------------------------//
void zglTexCoordPointer ( u32 size, u32 type, u32 stride, const void* pointer ) {
	UNUSED ( size );
	UNUSED ( type );
	UNUSED ( stride );
	UNUSED ( pointer );
}

//----------------------------------------------------------------//
void zglVertexAttribPointer ( u32 index, u32 size, u32 type, bool normalized, u32 stride, const void* pointer ) {
	UNUSED ( index );
	UNUSED ( size );
	UNUSED ( type );
	UNUSED ( normalized );
	UNUSED ( stride );
	UNUSED ( pointer );
}

//----------------------------------------------------------------//
void zglVertexPointer ( u32 size, u32 type, u32 stride, const void* pointer ) {
	UNUSED ( size );
	UNUSED ( type );
	UNUSED ( stride );
	UNUSED ( pointer );
}

//----------------------------------------------------------------//
void zglEnableClientState ( u32 cap ) {
	UNUSED ( cap );
}

//----------------------------------------------------------------//
void zglEnableVertexAttribArray ( u32 index ) {
	UNUSED ( index );
}

//================================================================//
// shaders
//================================================================//

//----------------------------------------------------------------//
void zglAttachShader ( u32 program, u32 shader ) {
	UNUSED ( program );
	UNUSED ( shader );
}

//----------------------------------------------------------------//
void zglBindAttribLocation ( u32 program, u32 index, cc8* name ) {
	UNUSED ( program );
	UNUSED ( index );
	UNUSED ( name );
}

//----------------------------------------------------------------//
void zglCompileShader ( u32 shader ) {
	UNUSED ( shader );
}

//----------------------------------------------------------------//
u32 zglCreateProgram () {
	return 0;
}

//----------------------------------------------------------------//
u32 zglCreateShader ( u32 shaderType ) {
	UNUSED ( shaderType );
	return 0;
}

//----------------------------------------------------------------//
void zglGetProgramInfoLog ( u32 program, u32 maxLength, u32* length, char* log ) {
	UNUSED ( program );
	UNUSED ( maxLength );
	UNUSED ( length );
	UNUSED ( log );
}

//----------------------------------------------------------------//
void zglGetProgramiv ( u32 program, u32 name, s32* params ) {
	UNUSED ( program );
	UNUSED ( name );
	UNUSED ( params );
}

//----------------------------------------------------------------//
void zglGetShaderInfoLog ( u32 shader, u32 maxLength, u32* length, char* log ) {
	UNUSED ( shader );
	UNUSED ( maxLength );
	UNUSED ( length );
	UNUSED ( log );
}

//----------------------------------------------------------------//
void zglGetShaderiv ( u32 shader, u32 name, s32* params ) {
	UNUSED ( shader );
	UNUSED ( name );
	UNUSED ( params );
}

//----------------------------------------------------------------//
u32 zglGetUniformLocation ( u32 program, cc8* name ) {
	UNUSED ( program );
	UNUSED ( name );
	return 0;
}

//----------------------------------------------------------------//
void zglLinkProgram ( u32 program ) {
	UNUSED ( program );
}

//----------------------------------------------------------------//
void zglShaderSource ( u32 shader, u32 count, const char** string, const s32* length ) {
	UNUSED ( shader );
	UNUSED ( count );
	UNUSED ( string );
	UNUSED ( length );
}

//----------------------------------------------------------------//
void zglValidateProgram ( u32 program ) {
	UNUSED ( program );
}

//----------------------------------------------------------------//
void zglUniform1f ( u32 location, float v0 ) {
	UNUSED ( location );
	UNUSED ( v0 );
}

//----------------------------------------------------------------//
void zglUniform1i ( u32 location, s32 v0 ) {
	UNUSED ( location );
	UNUSED ( v0 );
}

//----------------------------------------------------------------//
void zglUniform4fv ( u32 location, u32 count, const float* value ) {
	UNUSED ( location );
	UNUSED ( count );
	UNUSED ( value );
}

//----------------------------------------------------------------//
void zglUniformMatrix3fv ( u32 location, u32 count, bool transpose, const float* value ) {
	UNUSED ( location );
	UNUSED ( count );
	UNUSED ( transpose );
	UNUSED ( value );
}

//----------------------------------------------------------------//
void zglUniformMatrix4fv ( u32 location, u32 count, bool transpose, const float* value ) {
	UNUSED ( location );
	UNUSED ( count );
	UNUSED ( transpose );
	UNUSED ( value );
}

//----------------------------------------------------------------//
void zglUseProgram ( u32 program ) {
	UNUSED ( program );
}

//================================================================//
// texture
//================================================================//

//----------------------------------------------------------------//
void zglBindTexture ( u32 texID ) {
	UNUSED ( texID );
}

//----------------------------------------------------------------//
void zglCompressedTexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 imageSize, const void* data ) {

	UNUSED ( level );
	UNUSED ( internalFormat );
	UNUSED ( width );
	UNUSED ( height );
	UNUSED ( imageSize );
	UNUSED ( data );
}

//----------------------------------------------------------------//
u32 zglCreateTexture () {
	return 0;
}

//----------------------------------------------------------------//
void zglTexEnvi ( u32 pname, s32 param ) {
	UNUSED ( pname );
	UNUSED ( param );
}

//----------------------------------------------------------------//
void zglTexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, const void* data ) {
	UNUSED ( level );
	UNUSED ( internalFormat );
	UNUSED ( width );
	UNUSED ( height );
	UNUSED ( format );
	UNUSED ( type );
	UNUSED ( data );
}

//----------------------------------------------------------------//
void zglTexParameteri ( u32 pname, s32 param ) {
	UNUSED ( pname );
	UNUSED ( param );
}

//----------------------------------------------------------------//
void zglTexSubImage2D ( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, const void* data ) {
	UNUSED ( level );
	UNUSED ( xOffset );
	UNUSED ( yOffset );
	UNUSED ( width );
	UNUSED ( height );
	UNUSED ( format );
	UNUSED ( type );
	UNUSED ( data );
}

//================================================================//
// framebuffer
//================================================================//

//----------------------------------------------------------------//
void zglBindFramebuffer ( u32 target, u32 frameBuffer ) {
	UNUSED ( target );
	UNUSED ( frameBuffer );
}

//----------------------------------------------------------------//
void zglBindRenderbuffer ( u32 renderbuffer ) {
	UNUSED ( renderbuffer );
}

//----------------------------------------------------------------//
u32 zglCheckFramebufferStatus ( u32 target ) {
	UNUSED ( target );
	return 0;
}

//----------------------------------------------------------------//
u32 zglCreateFramebuffer () {
	return 0;
}

//----------------------------------------------------------------//
u32 zglCreateRenderbuffer () {
	return 0;
}

//----------------------------------------------------------------//
void zglFramebufferRenderbuffer ( u32 target, u32 attachment, u32 renderbuffer ) {
	UNUSED ( target );
	UNUSED ( attachment );
	UNUSED ( renderbuffer );
}

//----------------------------------------------------------------//
void zglFramebufferTexture2D ( u32 target, u32 attachment, u32 texture, s32 level ) {
	UNUSED ( target );
	UNUSED ( attachment );
	UNUSED ( texture );
	UNUSED ( level );
}

//----------------------------------------------------------------//
u32 zglGetCurrentFramebuffer () {

	return 0;
}

//----------------------------------------------------------------//
void zglRenderbufferStorage ( u32 internalFormat, u32 width, u32 height ) {
	UNUSED ( internalFormat );
	UNUSED ( width );
	UNUSED ( height );
}

//================================================================//
// buffer
//================================================================//

//----------------------------------------------------------------//
void zglBindBuffer ( u32 target, u32 buffer ) {
	UNUSED ( target );
	UNUSED ( buffer );
}

//----------------------------------------------------------------//
void zglBufferData ( u32 target, u32 size, const void* data, u32 usage ) {
	UNUSED ( target );
	UNUSED ( size );
	UNUSED ( data );
	UNUSED ( usage );
}

//----------------------------------------------------------------//
u32 zglCreateBuffer () {
	return 0;
}

//----------------------------------------------------------------//
void* zglMapBuffer ( u32 target ) {
	UNUSED ( target );
}

//----------------------------------------------------------------//
void zglUnmapBuffer ( u32 target ) {
	UNUSED ( target );
}

//================================================================//
// vertex array
//================================================================//

//----------------------------------------------------------------//
void zglBindVertexArray ( u32 vertexArrayID ) {
	UNUSED ( vertexArrayID );
}

//----------------------------------------------------------------//
u32 zglCreateVertexArray () {
}

//----------------------------------------------------------------//
void zglDeleteVertexArray ( u32 vertexArrayID ) {
	UNUSED ( vertexArrayID );
}

#endif
