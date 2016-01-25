// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIFourier.h>
#include <moai-util/MOAIStream.h>

// http://madebyevan.com/dft/
// http://betterexplained.com/articles/an-interactive-guide-to-the-fourier-transform/
// http://toxicdump.org/stuff/FourierToy.swf
// http://stackoverflow.com/questions/4364823/how-do-i-obtain-the-frequencies-of-each-value-in-a-fft
// http://code.compartmental.net/2007/03/21/fft-averages/
// http://stackoverflow.com/questions/20408388/how-to-filter-fft-data-for-audio-visualisation

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
int MOAIFourier::_setOutputType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )
	
	u32 outputType					= state.GetValue < u32 >( 2, OUTPUT_COMPLEX );
	u32 sampleRate					= state.GetValue < u32 >( 3, SAMPLE_RATE );
	u32 bands						= state.GetValue < u32 >( 4, 1 ); // total bands for linear average OR bands per octave
	float minOctaveBandWidth		= state.GetValue < float >( 5, 8.0f );
	
	self->SetOutputType ( outputType, sampleRate, bands, minOctaveBandWidth );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_transform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "UUNBUN" )
	
	MOAIStream* inStream		= state.GetLuaObject < MOAIStream >( 2, true );
	u32 inStreamType			= state.GetValue < u32 >( 3, ZLSample::SAMPLE_FLOAT );
	bool complexIn				= state.GetValue < bool >( 4, false );
	MOAIStream* outStream		= state.GetLuaObject < MOAIStream >( 5, true );
	u32 outStreamType			= state.GetValue < u32 >( 6, ZLSample::SAMPLE_FLOAT );
    u32 stride                  = state.GetValue < u32 >( 7, 1 );
    u32 average                 = state.GetValue < u32 >( 8, 1 );
	
	if ( inStream && outStream ) {
		self->Transform ( *inStream, inStreamType, complexIn, *outStream, outStreamType, stride, average );
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
size_t MOAIFourier::CalculateOctaveBands ( float minBandWidth, size_t bandsPerOctave ) {

	float nyquist = ( float )this->mSampleRate / 2.0f;
	
	size_t octaves = 1;
	while (( nyquist /= 2.0f ) > minBandWidth ) {
      octaves++;
    }
    return octaves * bandsPerOctave;
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
float MOAIFourier::GetBandWidth () {

	return ( 2.0f / ( float )this->mSize ) * ( this->mSampleRate / 2.0f );
}

//----------------------------------------------------------------//
size_t MOAIFourier::GetBandForFrequency ( float frequency) {

	float hBandWidth = this->GetBandWidth () / 2.0f;

	if ( frequency < hBandWidth ) return 0;
	if ( frequency > (( float )( this->mSampleRate >> 1 ) - hBandWidth )) return this->mSize >> 1;
	
	return ( u32 )ZLFloat::Round (( float )this->mSize * ( frequency / ( float )this->mSampleRate ));
}

//----------------------------------------------------------------//
size_t MOAIFourier::GetFastSize ( size_t size ) {

	return ( size_t )kiss_fft_next_fast_size (( int )size );
}

//----------------------------------------------------------------//
float MOAIFourier::GetFrequencyForBand ( size_t band ) {

	return (( float )band * ( float )this->mSampleRate ) / ( float )this->mSize;
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
	mKissFFTR ( 0 ),
	mOutputType ( OUTPUT_COMPLEX ),
	mOutputOctaves ( 0 ),
	mOutputBands ( 0 ),
	mSampleRate ( SAMPLE_RATE ) {
	
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
	
	state.SetField ( -1, "OUTPUT_COMPLEX",		( u32 )OUTPUT_COMPLEX );
	state.SetField ( -1, "OUTPUT_REAL",			( u32 )OUTPUT_REAL );
	state.SetField ( -1, "OUTPUT_IMAGINARY",	( u32 )OUTPUT_IMAGINARY );
	state.SetField ( -1, "OUTPUT_AMPLITUDE",	( u32 )OUTPUT_AMPLITUDE );
	state.SetField ( -1, "OUTPUT_AVERAGE",		( u32 )OUTPUT_AVERAGE );
	state.SetField ( -1, "OUTPUT_OCTAVES",		( u32 )OUTPUT_OCTAVES );
}

//----------------------------------------------------------------//
void MOAIFourier::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getFastSize",				_getFastSize },
		{ "init",						_init },
		{ "setOutputType",				_setOutputType },
		{ "transform",					_transform },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFourier::SetOutputType ( u32 outputType, u32 sampleRate, size_t bands, float minOctaveBandWidth ) {

	this->mOutputType = outputType;
	this->mSampleRate = sampleRate;
	
	if ( outputType == OUTPUT_OCTAVES ) {
		
		float nyquist = ( float )this->mSampleRate / 2.0f;
	
		this->mOutputOctaves = 1;
		while (( nyquist /= 2.0f ) > minOctaveBandWidth ) {
		  this->mOutputOctaves++;
		}
		this->mOutputBands = bands > 0 ? bands : 1;
	}
	else {
		
		this->mOutputOctaves = 0;
		this->mOutputBands = bands;
	}
}

//----------------------------------------------------------------//
void MOAIFourier::Transform ( ZLStream& inStream, u32 inStreamType, bool complexIn, ZLStream& outStream, u32 outStreamType, u32 stride, u32 average ) {

	assert ( sizeof ( kiss_fft_scalar ) == sizeof ( float ));

	bool complexOut = (( this->mOutputType == OUTPUT_COMPLEX ) || ( this->mOutputType == OUTPUT_IMAGINARY ));

	size_t halfSize = this->mSize >> 1;

	// the way kissft is written the cases are either C to C, R to C or C to R. There appears to be no R to R.

	u32 code = (( this->mInverse ? 0 : 1 ) + ( complexOut ? 0 : 2 ) + ( complexIn ? 0 : 4 ));

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
	void* out	= alloca ( this->mSize * ( fftComplexOut ? sizeof ( kiss_fft_cpx ) : sizeof ( kiss_fft_scalar ))); // TODO: get smarter about this size

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
	
	float* amplitudes = 0;
	
	if (( this->mOutputType == OUTPUT_AMPLITUDE ) || ( this->mOutputType == OUTPUT_AMPLITUDE ) || ( this->mOutputType == OUTPUT_AMPLITUDE )) {
		amplitudes = ( float* )in; // let's re-use this buffer
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
		
		switch ( this->mOutputType ) {
			
			case OUTPUT_COMPLEX:
				ZLSample::WriteSample ( outStream, outStreamType, &realResult, ZLSample::SAMPLE_FLOAT );
				ZLSample::WriteSample ( outStream, outStreamType, &imagResult, ZLSample::SAMPLE_FLOAT );
				break;
			
			case OUTPUT_REAL:
				ZLSample::WriteSample ( outStream, outStreamType, &realResult, ZLSample::SAMPLE_FLOAT );
				break;
			
			case OUTPUT_IMAGINARY:
				ZLSample::WriteSample ( outStream, outStreamType, &imagResult, ZLSample::SAMPLE_FLOAT );
				break;
			
			case OUTPUT_AMPLITUDE:
			case OUTPUT_AVERAGE:
			case OUTPUT_OCTAVES: {
			
				float amplitude = complexOut ? sqrtf (( realResult * realResult ) + ( imagResult * imagResult )) : realResult;
			
				if ( this->mOutputType == OUTPUT_AMPLITUDE ) {
				
					ZLSample::WriteSample ( outStream, outStreamType, &amplitude, ZLSample::SAMPLE_FLOAT );
				}
				else {
					amplitudes = ( float* )in;
					amplitudes [ i ] = amplitude;
				}
				break;
			}
		}
	}
	
	switch ( this->mOutputType ) {
		
		case OUTPUT_COMPLEX:
		case OUTPUT_REAL:
		case OUTPUT_IMAGINARY:
		case OUTPUT_AMPLITUDE:
			return;
		
		case OUTPUT_AVERAGE:
			this->WriteAverage ( amplitudes, outStream, outStreamType );
			return;
		
		case OUTPUT_OCTAVES:
			this->WriteOctaves ( amplitudes, outStream, outStreamType );
			return;
	}
}

//----------------------------------------------------------------//
void MOAIFourier::WriteAverage ( float* amplitudes, ZLStream& outStream, u32 outStreamType ) {

	size_t chunkSize = this->mSize / this->mOutputBands;
	
	for ( size_t i = 0; i < this->mOutputBands; ++i ) {
	
		size_t chunk = i * chunkSize;
		
		float avg = 0.0f;
		float div = 1.0f / ( float )chunkSize;
		
		for ( size_t j = 0; j < chunkSize; ++j ) {
			avg += amplitudes [ chunk + j ] * div;
		}
		
		ZLSample::WriteSample ( outStream, outStreamType, &avg, ZLSample::SAMPLE_FLOAT );
	}
}

//----------------------------------------------------------------//
void MOAIFourier::WriteOctaves ( float* amplitudes, ZLStream& outStream, u32 outStreamType ) {

	size_t hSampleRate = this->mSampleRate >> 1;

	size_t totalBands = this->mOutputOctaves * this->mOutputBands;

	float powScalar = 1.0f / ( float )this->mOutputBands;

	for ( size_t i = 0; i < totalBands; i++ ) {
		
		float lowerBound	= i == 0 ? 0.0f : (( float )hSampleRate / powf ( 2.0f, ( totalBands - ( float )i )) * powScalar );
		float upperBound	= (( float )hSampleRate / powf ( 2.0f, (( totalBands - 1 ) - ( float )i )) * powScalar );
		
		size_t lowerBand = this->GetBandForFrequency ( lowerBound );
		size_t upperBand = this->GetBandForFrequency ( upperBound );
		
		float avg = 0.0f;
		float div = 1.0f / ( float )( upperBand - lowerBand + 1 );
		
		for ( size_t j = lowerBand; j <= upperBand; j++ ) {
			avg += amplitudes [ j ] * div;
		}
		
		ZLSample::WriteSample ( outStream, outStreamType, &avg, ZLSample::SAMPLE_FLOAT );
	}
}
