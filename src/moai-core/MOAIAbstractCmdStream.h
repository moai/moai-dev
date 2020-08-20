// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTCMDSTREAM_H
#define	MOAIABSTRACTCMDSTREAM_H

#include <moai-core/MOAICmdMedium.h>
#include <moai-core/MOAILua.h>

class MOAIAbstractCmdHandler;
class MOAIAbstractCmdStream;

//================================================================//
// MOAIAbstractCmdStream
//================================================================//
class MOAIAbstractCmdStream :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	friend class MOAIAbstractCmdMedium;

	ZLStrongPtr < MOAICmdMedium >					mMedium;

	//----------------------------------------------------------------//
	static int			_getInterface				( lua_State* L );

	//----------------------------------------------------------------//
	MOAICmdMedium&		AffirmMedium				();
	void				RetainObject				( MOAILuaObject* object );
	void				SubmitCommand				( MOAIAbstractCmdHandler& handler, u32 cmd, const void* param, ZLSize paramSize );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual void		MOAIAbstractCmdStream_RetainObject			( MOAILuaObject* object ) = 0;
	virtual void		MOAIAbstractCmdStream_SubmitCommand			( MOAIAbstractCmdHandler& handler, u32 cmd, const void* param, ZLSize paramSize ) = 0;

public:

	//----------------------------------------------------------------//
						MOAIAbstractCmdStream		();
						~MOAIAbstractCmdStream		();
	void				SetMedium					( MOAICmdMedium* medium );
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	MOAIConcreteCmdMedium < API_TYPE >& _ ( MOAIAbstractCmdHandler& handler ) {

		return this->AffirmMedium ()._ < API_TYPE >( handler );
	}
};

#endif
