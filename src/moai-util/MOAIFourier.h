// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFOURIER_H
#define MOAIFOURIER_H

#include <kissfft/kiss_fft.h>

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

	size_t			mSize;
	kiss_fft_cfg	mKissFFT;
	
	//----------------------------------------------------------------//
	static int		_getFastSize		( lua_State* L );
	static int		_init				( lua_State* L );
	static int		_transform			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIFourier )

	//----------------------------------------------------------------//
	void			Clear				();
	void			Init				( size_t size, bool inverse );
					MOAIFourier			();
					~MOAIFourier		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			Transform			( ZLStream& inStream, ZLStream& outStream, bool complexIn, bool complexOut );
};

#endif
