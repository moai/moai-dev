// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERPASS_H
#define	MOAIRENDERPASS_H

#include <moai-sim/MOAIRenderPassBase.h>

class MOAIColor;
class MOAIFrameBuffer;
class MOAIPartition;

//================================================================//
// MOAIRenderPass
//================================================================//
class MOAIRenderPass :
	public virtual MOAIRenderPassBase {
private:

	MOAILuaMemberRef						mRenderTable;

	//----------------------------------------------------------------//
	static int		_getRenderTable			( lua_State* L );
	static int		_setRenderTable			( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAIDrawable_Draw		( int subPrimID );

public:

	DECL_LUA_FACTORY ( MOAIRenderPass )

	//----------------------------------------------------------------//
						MOAIRenderPass			();
						~MOAIRenderPass			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
