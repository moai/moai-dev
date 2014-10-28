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
	public virtual MOAIStream {
protected:
	
	MOAILuaSharedPtr < MOAIStream > mStream;
	ZLStreamAdapter* mAdapter;
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_openBase64Reader		( lua_State* L );
	static int		_openBase64Writer		( lua_State* L );
	static int		_openDeflateReader		( lua_State* L );
	static int		_openDeflateWriter		( lua_State* L );
	static int		_openHex				( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIStreamAdapter )

	//----------------------------------------------------------------//
	void			Close					();
					MOAIStreamAdapter		();
					~MOAIStreamAdapter		();
	bool			Open					( MOAIStream* stream, ZLStreamAdapter* reader );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );

	//----------------------------------------------------------------//
	template < typename TYPE >
	int Open ( MOAILuaState& state, int idx ) {
	
		this->Close ();
		
		MOAIStream* stream = state.GetLuaObject < MOAIStream >( idx, true );
		if ( !stream ) return 0;
		
		ZLStreamAdapter* adapter = new TYPE ();
		
		bool result = this->Open ( stream, adapter );
		
		state.Push ( result );
		return 1;
	}
};

#endif
