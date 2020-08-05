// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRETAINEDDSLSTREAM_H
#define	MOAIRETAINEDDSLSTREAM_H

#include <moai-core/MOAIAbstractDSLStream.h>

//================================================================//
// MOAIRetainedDSLStream
//================================================================//
// TODO: doxygen
class MOAIRetainedDSLStream :
	public virtual MOAIAbstractDSLStream {
protected:

	ZLMemStream						mCommandStream;
	ZLLeanArray < u8 >				mBytecode;
	STLSet < MOAILuaObject* >		mRetainedObjects;

	//----------------------------------------------------------------//
	static int			_affirmBytecode							( lua_State* L );
	static int			_executeBytecode						( lua_State* L );

	//----------------------------------------------------------------//
	void 				ExecuteBytecode							();

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAIAbstractDSLHandler_RetainObject 	( MOAILuaObject* object );
	void				MOAIAbstractDSLHandler_SubmitCommand	( u32 cmd, const void* param, ZLSize paramSize );

public:

	DECL_LUA_FACTORY ( MOAIRetainedDSLStream )

	//----------------------------------------------------------------//
	void				AffirmBytecode							();
	bool				HasContent								();
						MOAIRetainedDSLStream					();
						~MOAIRetainedDSLStream					();
	void				Reset									();
};

#endif
