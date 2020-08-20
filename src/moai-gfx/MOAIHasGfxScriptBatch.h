// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXSCRIPTBATCH_H
#define	MOAIHASGFXSCRIPTBATCH_H

#include <moai-gfx/MOAIHasGfxScript.h>

//================================================================//
// MOAIHasGfxScriptBatch
//================================================================//
// TODO: doxygen
class MOAIHasGfxScriptBatch :
	public virtual MOAILuaObject {
private:

	ZLSize											mIndexBatchSize;
	ZLLeanArray < ZLStrongPtr < MOAIGfxScript > >	mGfxScripts;

	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	//----------------------------------------------------------------//
	static int				_getGfxScript					( lua_State* L );
	static int				_getIndexBatchSize				( lua_State* L );
	static int				_gfx							( lua_State* L );
	static int				_gfxScript						( lua_State* L );
	static int				_reserveGfxScripts				( lua_State* L );
	static int				_setGfxScript					( lua_State* L );
	static int				_setIndexBatchSize				( lua_State* L );

	//----------------------------------------------------------------//
	void					_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );

	//----------------------------------------------------------------//
	inline ZLIndex WrapIndex ( ZLIndex index ) {
	
		ZLSize totalGfxScripts = this->mGfxScripts.Size ();
		return (( this->mIndexBatchSize > 0 ) ? ( index / this->mIndexBatchSize ) : index ) % totalGfxScripts;
	}

public:

	GET_SET ( ZLSize, IndexBatchSize, mIndexBatchSize )

	DECL_LUA_FACTORY ( MOAIHasGfxScriptBatch )
	
	//----------------------------------------------------------------//
	MOAIGfxScript&			AffirmGfxScript					( ZLIndex index = 0 );
	MOAIGfxScript*			GetGfxScript					( ZLIndex index = 0 );
							MOAIHasGfxScriptBatch			();
							~MOAIHasGfxScriptBatch			();
	void					ReserveGfxScripts				( ZLSize size );
	void					SetGfxScript					( ZLIndex index, MOAIGfxScript* gfxScript );
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	MOAIConcreteCmdMedium < API_TYPE >& _ ( MOAIAbstractCmdHandler& handler, ZLIndex index = 0 ) {
		return this->AffirmGfxScript ( index )._ < API_TYPE >( handler );
	}
};

#endif
