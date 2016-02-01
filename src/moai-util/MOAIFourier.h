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
	
	u32				mSampleRate;
	
	u32				mOutputType;
	size_t			mOutputBands;
	size_t			mOutputOctaves;
	size_t			mBandsPerOctave;
	
	u32				mWindowFunction;
	float			mWindowAlpha;
	
	//----------------------------------------------------------------//
	static int		_countBands						( lua_State* L );
	static int		_countOctaves					( lua_State* L );
	static int		_getCenterFrequencyForBand		( lua_State* L );
	static int		_getCenterFrequencyForOctave	( lua_State* L );
	static int		_getFastSize					( lua_State* L );
	static int		_getFrequencyForIndex			( lua_State* L );
	static int		_getIndexForFrequency			( lua_State* L );
	static int		_getWidth						( lua_State* L );
	static int		_getWidthOfBand					( lua_State* L );
	static int		_getWidthOfOctave				( lua_State* L );
	static int		_init							( lua_State* L );
	static int		_setOutputType					( lua_State* L );
	static int		_setWindowFunction				( lua_State* L );
	static int		_transform						( lua_State* L );
	static int		_window							( lua_State* L );

	//----------------------------------------------------------------//
	void			Affirm							( u32 fft );
	void			ReadSample						( ZLStream& inStream, u32 inStreamType, bool complexIn, float& real, float& imag );
	static float	Window							( u32 func, size_t index, size_t length, float a = 0.0f );
	void			WriteAverage					( float* amplitudes, ZLStream& outStream, u32 outStreamType );
	void			WriteOctaves					( float* amplitudes, ZLStream& outStream, u32 outStreamType );

public:
	
	enum {
		OUTPUT_COMPLEX,
		OUTPUT_REAL,
		OUTPUT_IMAGINARY,
		OUTPUT_AMPLITUDE,
		OUTPUT_AVERAGE,
		OUTPUT_OCTAVES,
	};
	
	enum {
		BARTLETT,
		BARTLETT_HANN,
		BLACKMAN,
		COSINE,
		GAUSS,
		HAMMING,
		HANN,
		LANCZOS,
		RECTANGULAR,
		WELCH,
	};
	
	DECL_LUA_FACTORY ( MOAIFourier )

	//----------------------------------------------------------------//
	void			Clear								();
	size_t			CountBands							();
	size_t			CountOctaves						();
	float			GetCenterFrequencyForBand			( size_t band );
	float			GetCenterFrequencyForOctave			( size_t octave );
	static float	GetDefaultWindowAlpha				( u32 func );
	size_t			GetFastSize							( size_t size );
	float			GetFrequencyForIndex				( size_t index );
	size_t			GetIndexForFrequency				( float frequency );
	void			GetOctaveFrequencyBounds			( size_t octave, float& lowerBound, float& upperBound );
	float			GetWidth							();
	float			GetWidthOfBand						( size_t band );
	float			GetWidthOfOctave					( size_t octave );
	void			Init								( size_t size, bool inverse );
					MOAIFourier							();
					~MOAIFourier						();
	void			RegisterLuaClass					( MOAILuaState& state );
	void			RegisterLuaFuncs					( MOAILuaState& state );
	void			SetOutputType						( u32 outputType, u32 sampleRate, size_t bands, float minOctaveBandWidth );
	void			SetWindowFunction					( u32 func, float a );
	void			Transform							( ZLStream& inStream, u32 inStreamType, bool complexIn, ZLStream& outStream, u32 outStreamType, u32 stride, u32 average );
};

#endif
