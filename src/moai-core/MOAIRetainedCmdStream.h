// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRETAINEDCMDSTREAM_H
#define	MOAIRETAINEDCMDSTREAM_H

#include <moai-core/MOAIAbstractCmdStream.h>
#include <moai-core/MOAILua.h>

class MOAIAbstractCmdHandler;
class MOAIAbstractCmdStream;

//================================================================//
// MOAIAbstractCmdStreamFilter
//================================================================//
// TODO: doxygen
class MOAIAbstractCmdStreamFilter :
	public virtual ZLRefCountedObject {
protected:

	//----------------------------------------------------------------//
	virtual bool		MOAIAbstractCmdStreamFilter_FilterCommand			( u32 cmd, const void* param, ZLSize paramSize ) = 0;

public:

	//----------------------------------------------------------------//
	bool				FilterCommand				( u32 cmd, const void* param, ZLSize paramSize );
};

//================================================================//
// MOAIRetainedCmdStream
//================================================================//
class MOAIRetainedCmdStream :
	public virtual MOAIAbstractCmdStreamWithMedium {
protected:

	ZLMemStream						mCommandStream;
	ZLLeanArray < u8 >				mBytecode;
	STLSet < MOAILuaObject* >		mRetainedObjects;

	//----------------------------------------------------------------//
	static int			_affirmBytecode							( lua_State* L );
	static int			_executeBytecode						( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAIAbstractCmdStream_RetainObject		( MOAILuaObject* object );
	void				MOAIAbstractCmdStream_SubmitCommand		( MOAIAbstractCmdHandler& handler, u32 cmd, const void* param, ZLSize paramSize );

public:

	DECL_LUA_FACTORY ( MOAIRetainedCmdStream )

	//----------------------------------------------------------------//
	void				AffirmBytecode							();
	void 				ExecuteBytecode							( MOAIAbstractCmdStreamFilter* filter = NULL );
	bool				HasContent								();
						MOAIRetainedCmdStream					( ZLContext& context );
						~MOAIRetainedCmdStream					();
	void				Reset									();
};

#endif
