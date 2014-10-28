// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDATABUFFER_H
#define MOAIDATABUFFER_H

#include <moai-util/MOAIDataBuffer.h>
#include <moai-util/MOAIMutex.h>

class MOAIDataIOTask;

//================================================================//
// MOAIDataBuffer
//================================================================//
/**	@lua	MOAIDataBuffer
	@text	Buffer for loading and holding data. Data operations may be
			performed without additional penalty of marshalling buffers
			between Lua and C.
*/
class MOAIDataBuffer :
	public virtual MOAILuaObject {
private:
	
	MOAIMutex			mMutex;
	ZLLeanArray < u8 >	mBytes;
	
	//----------------------------------------------------------------//
	static int		_base64Decode		( lua_State* L );
	static int		_base64Encode		( lua_State* L );
	static int		_clear				( lua_State* L );
	static int		_deflate			( lua_State* L );
	static int		_getSize			( lua_State* L );
	static int		_getString			( lua_State* L );
	static int		_hexDecode			( lua_State* L );
	static int		_hexEncode			( lua_State* L );
	static int		_inflate			( lua_State* L );
	static int		_load				( lua_State* L );
	static int		_loadAsync			( lua_State* L );
	static int		_save				( lua_State* L );
	static int		_saveAsync			( lua_State* L );
	static int		_setString			( lua_State* L );
	static int		_toCppHeader		( lua_State* L );

	//----------------------------------------------------------------//
	bool			Decode				( ZLStreamAdapter& reader );
	bool			Encode				( ZLStreamAdapter& writer );
	static bool		IsZipFilename		( cc8* filename );

public:
	
	DECL_LUA_FACTORY ( MOAIDataBuffer )

	enum {
		NO_INFLATE,
		FORCE_INFLATE,
		INFLATE_ON_EXT,
	};

	//----------------------------------------------------------------//
	bool			Base64Decode			();
	bool			Base64Encode			();
	void			Clear					();
	bool			Deflate					( int level = ZLDeflateWriter::DEFAULT_LEVEL, int windowBits = ZLDeflateWriter::DEFAULT_WBITS );
	void*			GetBuffer				(); // NOTE: unsafe; bypasses the mutex
	bool			HexDecode				();
	bool			HexEncode				();
	bool			Inflate					( int windowBits = ZLDeflateWriter::DEFAULT_WBITS );
	bool			Load					( cc8* filename );
	void			Load					( void* bytes, size_t size );
	void			Load					( MOAILuaState& state, int idx );
	void			Lock					( void** bytes, size_t* size );
					MOAIDataBuffer			();
					~MOAIDataBuffer			();
	void			PushString				( MOAILuaState& state );
	size_t			Read					( void* buffer, size_t size );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	size_t			Size					();
	bool			Save					( cc8* filename );
	void			Unlock					();
};

#endif
