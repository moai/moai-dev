// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITABLELAYER_H
#define	MOAITABLELAYER_H

#include <moai-sim/MOAIAbstractLayer.h>

class MOAIColor;
class MOAIFrameBuffer;
class MOAIPartition;

//================================================================//
// MOAITableLayer
//================================================================//
class MOAITableLayer :
	public virtual MOAIAbstractLayer {
protected:

	MOAILuaMemberRef	mRenderTable;

	//----------------------------------------------------------------//
	static int			_getRenderTable			( lua_State* L );
	static int			_setRenderTable			( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAIDrawable_Draw		( int subPrimID );

public:

	DECL_LUA_FACTORY ( MOAITableLayer )

	//----------------------------------------------------------------//
						MOAITableLayer			();
						~MOAITableLayer			();
};

#endif
