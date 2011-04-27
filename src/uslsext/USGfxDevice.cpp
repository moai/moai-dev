// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USGfxDevice.h>

//================================================================//
// USGfxDevice
//================================================================//

//----------------------------------------------------------------//
void USGfxDevice::ClearErrors () {

	while ( glGetError () != GL_NO_ERROR );
}

//----------------------------------------------------------------//
u32 USGfxDevice::CountErrors () {

	u32 count = 0;
	while ( glGetError () != GL_NO_ERROR ) count++;
	return count;
}

//----------------------------------------------------------------//
u32 USGfxDevice::GetHeight () {

	return this->mHeight;
}

//----------------------------------------------------------------//
USRect USGfxDevice::GetRect () {

	USRect rect;
	rect.mXMin = 0;
	rect.mYMin = 0;
	rect.mXMax = ( float )this->mWidth;
	rect.mYMax = ( float )this->mHeight;
	
	return rect;
}

//----------------------------------------------------------------//
u32 USGfxDevice::GetWidth () {

	return this->mWidth;
}

//----------------------------------------------------------------//
cc8* USGfxDevice::GetErrorString ( int error ) {

	switch ( error ) {
		case GL_INVALID_ENUM:		return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:		return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:	return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW:		return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW:	return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY:		return "GL_OUT_OF_MEMORY";
	}
	return "";
}

//----------------------------------------------------------------//
u32 USGfxDevice::PrintErrors () {

	u32 count = 0;
	for ( int error = glGetError (); error != GL_NO_ERROR; error = glGetError (), ++count ) {
		printf ( "OPENGL ERROR: %s\n", this->GetErrorString ( error ));
	}
	return count;
}

//----------------------------------------------------------------//
void USGfxDevice::SetSize ( u32 width, u32 height ) {

	this->mWidth = width;
	this->mHeight = height;
}

//----------------------------------------------------------------//
USGfxDevice::USGfxDevice () :
	mWidth ( 0 ),
	mHeight ( 0 ) {
}

//----------------------------------------------------------------//
USGfxDevice::~USGfxDevice () {
}