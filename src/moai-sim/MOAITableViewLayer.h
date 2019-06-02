// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITABLEVIEWLAYER_H
#define	MOAITABLEVIEWLAYER_H

#include <moai-sim/MOAIAbstractViewLayer.h>
#include <moai-sim/MOAITableLayer.h>

//================================================================//
// MOAITableViewLayer
//================================================================//
// TODO: doxygen
class MOAITableViewLayer :
	public virtual MOAITableLayer,
	public virtual MOAIAbstractViewLayer {
private:

	//----------------------------------------------------------------//
	void				MOAIDrawable_Draw			( int subPrimID );
	void				MOAIAbstractViewLayer_Draw			();

public:
	
	DECL_LUA_FACTORY ( MOAITableViewLayer )
	
	//----------------------------------------------------------------//
						MOAITableViewLayer			();
						~MOAITableViewLayer			();
	void				MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_SerializeIn					( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void				MOAILuaObject_SerializeOut				( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
};

#endif
