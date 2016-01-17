// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFOURIER_H
#define MOAIFOURIER_H

#include <kissfft/kiss_fft.h>
#include <kissfft/tools/kiss_fftr.h>

#include <moai-core/MOAILua.h>
#include <moai-core/MOAILuaState-impl.h>
#include <moai-core/MOAILuaClass-impl.h>

//================================================================//
// MOAIFourier
//================================================================//
// TODO: doxygen
class MOAIFourier :
	public virtual MOAILuaObject {
private:

	static const u32 SAMPLE_RATE = 44100;

	size_t			mSize;
	bool			mInverse;
	
	kiss_fft_cfg	mKissFFT;
	kiss_fftr_cfg	mKissFFTR;
	
	u32				mOutputType;
	size_t			mOutputOctaves;
	size_t			mOutputBands;
	u32				mSampleRate;
	
	//----------------------------------------------------------------//
	static int		_getFastSize			( lua_State* L );
	static int		_init					( lua_State* L );
	static int		_setOutputType			( lua_State* L );
	static int		_transform				( lua_State* L );

	//----------------------------------------------------------------//
	void			Affirm					( u32 fft );
	void			ReadSample				( ZLStream& inStream, u32 inStreamType, bool complexIn, float& real, float& imag );
	void			WriteAverage			( float* amplitudes, ZLStream& outStream, u32 outStreamType );
	void			WriteOctaves			( float* amplitudes, ZLStream& outStream, u32 outStreamType );

public:
	
	enum {
		OUTPUT_COMPLEX,
		OUTPUT_REAL,
		OUTPUT_IMAGINARY,
		OUTPUT_AMPLITUDE,
		OUTPUT_AVERAGE,
		OUTPUT_OCTAVES,
	};
	
	DECL_LUA_FACTORY ( MOAIFourier )

	//----------------------------------------------------------------//
	size_t			CalculateOctaveBands	( float minBandWidth, size_t bandsPerOctave );
	void			Clear					();
	float			GetBandWidth			();
	size_t			GetBandForFrequency		( float frequency );
	size_t			GetFastSize				( size_t size );
	float			GetFrequencyForBand		( size_t band );
	void			Init					( size_t size, bool inverse );
					MOAIFourier				();
					~MOAIFourier			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetOutputType			( u32 outputType, u32 sampleRate, size_t bands, float minOctaveBandWidth );
	void			Transform				( ZLStream& inStream, u32 inStreamType, bool complexIn, ZLStream& outStream, u32 outStreamType, u32 stride, u32 average );
};

#endif
