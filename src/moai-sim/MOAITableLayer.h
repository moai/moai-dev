// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITABLELAYER_H
#define	MOAITABLELAYER_H

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAILayerBase.h>
#include <moai-sim/MOAIPartitionHolder.h>
#include <moai-sim/MOAIRenderPassBase.h>
#include <moai-sim/MOAIViewport.h>

class MOAICamera;

//================================================================//
// MOAITableLayer
//================================================================//
// TODO: doxygen
class MOAITableLayer :
	public virtual MOAILayerBase {
private:

	MOAILuaMemberRef	mRenderTable;

	//----------------------------------------------------------------//
	static int			_getRenderTable			( lua_State* L );
	static int			_setRenderTable			( lua_State* L );

	//----------------------------------------------------------------//
	void				MOAILayerBase_Draw		( int subPrimID );

public:
	
	DECL_LUA_FACTORY ( MOAITableLayer )
	
	//----------------------------------------------------------------//
						MOAITableLayer		();
						~MOAITableLayer		();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
