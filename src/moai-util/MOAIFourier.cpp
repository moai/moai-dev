// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIFourier.h>
#include <moai-util/MOAIStream.h>

enum {
	KISS_FFT,
	KISS_FFTR,
	KISS_FFTRI,
};

enum {
	STREAM_CCI,		// 000
	STREAM_CCF,		// 001
	STREAM_CRI,		// 010
	STREAM_CRF,		// 011
	STREAM_RCI,		// 100
	STREAM_RCF,		// 101
	STREAM_RRI,		// 110
	STREAM_RRF,		// 111
};

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
	MOAI_LUA_SETUP ( MOAIFourier, "UUNBUNB" )
	
	MOAIStream* inStream		= state.GetLuaObject < MOAIStream >( 2, true );
	u32 inStreamType			= state.GetValue < u32 >( 3, ZLSample::SAMPLE_FLOAT );
	bool complexIn				= state.GetValue < bool >( 4, false );
	MOAIStream* outStream		= state.GetLuaObject < MOAIStream >( 5, true );
	u32 outStreamType			= state.GetValue < u32 >( 6, ZLSample::SAMPLE_FLOAT );
	bool complexOut				= state.GetValue < bool >( 7, true );
    u32 stride                  = state.GetValue < u32 >( 8, 1 );
    u32 average                 = state.GetValue < u32 >( 9, 1 );
	
	if ( inStream && outStream ) {
		self->Transform ( *inStream, inStreamType, complexIn, *outStream, outStreamType, complexOut, stride, average );
	}
	return 0;
}

//================================================================//
// MOAIFourier
//================================================================//

//----------------------------------------------------------------//
void MOAIFourier::Affirm ( u32 fft ) {

	if ( fft == KISS_FFT ) {
		if ( !this->mKissFFT ) {
			this->Clear ();
			this->mKissFFT = kiss_fft_alloc (( int )this->mSize, this->mInverse ? 1 : 0, 0, 0 );
		}
	}
	else {
		if ( !this->mKissFFTR ) {
			this->Clear ();
			this->mKissFFTR = kiss_fftr_alloc (( int )this->mSize, this->mInverse ? 1 : 0, 0, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFourier::Clear () {

	if ( this->mKissFFT ) {
		free ( this->mKissFFT );
		this->mKissFFT = 0;
	}
	
	if ( this->mKissFFTR ) {
		free ( this->mKissFFTR );
		this->mKissFFTR = 0;
	}
}

//----------------------------------------------------------------//
void MOAIFourier::Init ( size_t size, bool inverse ) {

	this->Clear ();
	this->mSize		= size;
	this->mInverse	= inverse;
}

//----------------------------------------------------------------//
MOAIFourier::MOAIFourier () :
	mSize ( 0 ),
	mInverse ( false ),
	mKissFFT ( 0 ),
	mKissFFTR ( 0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIFourier::~MOAIFourier () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIFourier::ReadSample ( ZLStream& inStream, u32 inStreamType, bool complexIn, float& real, float& imag ) {

	real = 0.0f;
	imag = 0.0f;

	ZLSample::ReadSample ( inStream, inStreamType, &real, ZLSample::SAMPLE_FLOAT );
	if ( complexIn ) {
		ZLSample::ReadSample ( inStream, inStreamType, &imag, ZLSample::SAMPLE_FLOAT );
	}
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
void MOAIFourier::Transform ( ZLStream& inStream, u32 inStreamType, bool complexIn, ZLStream& outStream, u32 outStreamType, bool complexOut, u32 stride, u32 average ) {

	// the way kissft is written the cases are either C to C, R to C or C to R. There appears to be no R to R.

	assert ( sizeof ( kiss_fft_scalar ) == sizeof ( float ));

	u32 code = (( this->mInverse ? 0 : 1 ) + ( complexOut ? 0 : 2 ) + ( complexIn ? 0 : 4 ));

	size_t halfSize = this->mSize >> 1;

	u32 fft;
	bool fftComplexIn;
	bool fftComplexOut;
	
	bool halfInput = false;
	bool halfOutput = false;
	
	switch ( code ) {
	
		case STREAM_CCI:
		case STREAM_CCF:
		case STREAM_RCI:
		case STREAM_CRF:
			fft				= KISS_FFT;
			fftComplexIn	= true;
			fftComplexOut	= true;
			break;
			
		case STREAM_CRI:
		case STREAM_RRI:
			fft				= KISS_FFTRI;
			fftComplexIn	= true;
			fftComplexOut	= false;
			halfInput		= true;
			break;
			
		case STREAM_RCF:
		case STREAM_RRF:
			fft				= KISS_FFTR;
			fftComplexIn	= false;
			fftComplexOut	= true;
			halfOutput		= true;
			break;
	}
	
	this->Affirm ( fft );

	void* in	= alloca ( this->mSize * ( fftComplexIn ? sizeof ( kiss_fft_cpx ) : sizeof ( kiss_fft_scalar )));
	void* out	= alloca ( this->mSize * ( fftComplexOut ? sizeof ( kiss_fft_cpx ) : sizeof ( kiss_fft_scalar )));

	size_t sampleSize = ZLSample::GetSize ( inStreamType );
	
	stride *= complexIn ? sampleSize * 2 : sampleSize;

	for ( size_t i = 0; i < this->mSize; ++i ) {
		
		float realResult = 0.0f;
		float imagResult = 0.0f;
		
		size_t next = inStream.GetCursor () + stride;
		
		if ( average > 1 ) {
		
			float realAvg = 0.0f;
			float imagAvg = 0.0f;
			
			float scale = 1 / ( float )average;
			
			for ( u32 j = 0; j < average; ++j ) {
			
				float real;
				float imag;
				
				this->ReadSample ( inStream, inStreamType, complexIn, real, imag );
				
				real = 0;
				imag = 0;
				
				realAvg += real * scale;
				imagAvg += imag * scale;
			}
		
			realResult = realAvg;
			imagResult = imagAvg;
		}
		else {
		
			this->ReadSample ( inStream, inStreamType, complexIn, realResult, imagResult );
		}
		
		if ( fftComplexIn ) {
		
			kiss_fft_cpx* complexBuffer = ( kiss_fft_cpx* )in;
		
			complexBuffer [ i ].r = realResult;
			complexBuffer [ i ].i = imagResult;
		}
		else {
		
			(( kiss_fft_scalar* )in )[ i ] = realResult;
		}
		
		if ( inStream.GetCursor () != next ) {
			inStream.Seek ( next, SEEK_SET );
		}
		
		if ( halfInput && ( i > halfSize )) break;
	}
	
	switch ( fft ) {
	
		case KISS_FFT:
			kiss_fft ( this->mKissFFT, ( kiss_fft_cpx* )in, ( kiss_fft_cpx* )out );
			break;
			
		case KISS_FFTR:
			kiss_fftr ( this->mKissFFTR, ( kiss_fft_scalar* )in, ( kiss_fft_cpx* )out );
			break;
		
		case KISS_FFTRI:
			kiss_fftri ( this->mKissFFTR, ( kiss_fft_cpx* )in, ( kiss_fft_scalar* )out );
			break;
	}
	
	for ( size_t i = 0; i < this->mSize; ++i ) {
		
		float realResult = 0.0f;
		float imagResult = 0.0f;
		
		if ( fftComplexOut ) {
		
			size_t j = halfOutput ? (( i < halfSize ) ? i : halfSize - ( i % halfSize )) : i;
		
			kiss_fft_cpx* complexBuffer = ( kiss_fft_cpx* )out;
		
			realResult = complexBuffer [ j ].r;
			imagResult = complexBuffer [ j ].i;
		}
		else {
		
			realResult = (( kiss_fft_scalar* )out )[ i ];
		}
		
		ZLSample::WriteSample ( outStream, outStreamType, &realResult, ZLSample::SAMPLE_FLOAT );
		
		if ( complexOut ) {
			ZLSample::WriteSample ( outStream, outStreamType, &imagResult, ZLSample::SAMPLE_FLOAT );
		}
	}
}
