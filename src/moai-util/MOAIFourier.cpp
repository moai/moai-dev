// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIFourier.h>
#include <moai-util/MOAIStream.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_getFastSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )
	
	int size = state.GetValue < int >( 2, 0 );
	
	state.Push ( kiss_fft_next_fast_size ( size ));
	
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )
	
	u32 size		= state.GetValue < u32 >( 2, 0 );
	bool inverse	= state.GetValue < bool >( 3, false );
	
	self->Init ( size, inverse );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_transform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "UUU" )
	
	MOAIStream* inStream		= state.GetLuaObject < MOAIStream >( 2, true );
	u32 inStreamType			= state.GetValue < u32 >( 3, ZLSample::SAMPLE_FLOAT );
	bool complexIn				= state.GetValue < bool >( 4, false );
	MOAIStream* outStream		= state.GetLuaObject < MOAIStream >( 5, true );
	u32 outStreamType			= state.GetValue < u32 >( 6, ZLSample::SAMPLE_FLOAT );
	bool complexOut				= state.GetValue < bool >( 7, true );
	
	if ( inStream && outStream ) {
		self->Transform ( *inStream, inStreamType, complexIn, *outStream, outStreamType, complexOut );
	}
	return 0;
}

//================================================================//
// MOAIFourier
//================================================================//

//----------------------------------------------------------------//
void MOAIFourier::Clear () {

	if ( this->mKissFFT ) {
		free ( this->mKissFFT );
	}
}

//----------------------------------------------------------------//
void MOAIFourier::Init ( size_t size, bool inverse ) {

	this->Clear ();
	this->mKissFFT = kiss_fft_alloc (( int )size, inverse ? 1 : 0, 0, 0 );
}

//----------------------------------------------------------------//
MOAIFourier::MOAIFourier () :
	mSize ( 0 ),
	mKissFFT ( 0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIFourier::~MOAIFourier () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIFourier::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "SAMPLE_S8",			( u32 )ZLSample::SAMPLE_S8 );
	state.SetField ( -1, "SAMPLE_U8",			( u32 )ZLSample::SAMPLE_U8 );
	state.SetField ( -1, "SAMPLE_S16",			( u32 )ZLSample::SAMPLE_S16 );
	state.SetField ( -1, "SAMPLE_U16",			( u32 )ZLSample::SAMPLE_U16 );
	state.SetField ( -1, "SAMPLE_S32",			( u32 )ZLSample::SAMPLE_S32 );
	state.SetField ( -1, "SAMPLE_U32",			( u32 )ZLSample::SAMPLE_U32 );
	state.SetField ( -1, "SAMPLE_FLOAT",		( u32 )ZLSample::SAMPLE_FLOAT );
}

//----------------------------------------------------------------//
void MOAIFourier::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getFastSize",		_getFastSize },
		{ "init",				_init },
		{ "transform",			_transform },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFourier::Transform ( ZLStream& inStream, u32 inStreamType, bool complexIn, ZLStream& outStream, u32 outStreamType, bool complexOut ) {

	ZLSample sample;

	kiss_fft_cpx* in = ( kiss_fft_cpx* )alloca ( this->mSize * sizeof ( kiss_fft_cpx ));
	kiss_fft_cpx* out = ( kiss_fft_cpx* )alloca ( this->mSize * sizeof ( kiss_fft_cpx ));

	for ( size_t i = 0; i < this->mSize; ++i ) {
	
		sample.ReadSample ( inStream, inStreamType, &in [ i ].r, ZLSample::SAMPLE_FLOAT );
	
		if ( complexIn ) {
			sample.ReadSample ( inStream, inStreamType, &in [ i ].i, ZLSample::SAMPLE_FLOAT );
		}
		else {
			in [ i ].i = 0.0f;
		}
	}

	kiss_fft ( this->mKissFFT, in, out );
	
	for ( size_t i = 0; i < this->mSize; ++i ) {
		
		sample.WriteSample ( outStream, outStreamType, &out [ i ].r, ZLSample::SAMPLE_FLOAT );
	
		if ( complexOut ) {
			sample.WriteSample ( outStream, outStreamType, &out [ i ].i, ZLSample::SAMPLE_FLOAT );
		}
	}
}
