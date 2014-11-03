// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISTREAMADAPTER_H
#define MOAISTREAMADAPTER_H

#include <moai-util/MOAIStream.h>

//================================================================//
// MOAIStreamAdapter
//================================================================//
/**	@lua	MOAIStreamAdapter
	@text	MOAIStreamAdapter may be attached to another stream for the
			purpose of encoding/decoding and/or compressing/decompressing
			bytes read in that stream using a given algorithm (such as base64
			or 'deflate'). 
*/
class MOAIStreamAdapter :
	public ZLStreamProxy,
	public virtual MOAIStream {
protected:
	
	// to save ourselves the trouble of having to create a unique class for each supported adapter
	// type, we resort to some trickery. we subclass from ZLStreamProxy, which wraps a stream. the
	// stream we wrap is the instance of ZLStreamAdapter. it, in turn, adapts the stream held in the
	// MOAIStream the user gives us. So we have ZLStreamProxy => ZLStreamAdapter => MOAIStream.
	
	ZLStreamAdapter* mAdapter; // this is the adapter that does the work
	
	MOAILuaSharedPtr < MOAIStream > mAdaptedStream; // this is the inner stream
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_openBase64Reader		( lua_State* L );
	static int		_openBase64Writer		( lua_State* L );
	static int		_openDeflateReader		( lua_State* L );
	static int		_openDeflateWriter		( lua_State* L );
	static int		_openHex				( lua_State* L );

	//----------------------------------------------------------------//
	int				Open					( MOAILuaState& state, int idx, ZLStreamAdapter* adapter );

public:
	
	DECL_LUA_FACTORY ( MOAIStreamAdapter )

	//----------------------------------------------------------------//
	void			Clear					(); // closes *and* clears the adapter
	void			Close					(); // clears the stream and closes the adapter, but *doesn't* also clear the adapter
					MOAIStreamAdapter		();
					~MOAIStreamAdapter		();
	bool			Open					( ZLStreamAdapter* adapter, MOAIStream* stream );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
