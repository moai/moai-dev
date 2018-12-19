// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITABLEVIEWLAYER_H
#define	MOAITABLEVIEWLAYER_H

#include <moai-sim/MOAITableLayer.h>
#include <moai-sim/MOAIViewLayer.h>

//================================================================//
// MOAITableViewLayer
//================================================================//
// TODO: doxygen
class MOAITableViewLayer :
	public virtual MOAITableLayer,
	public virtual MOAIViewLayer {
private:

	//----------------------------------------------------------------//
	void				MOAIAbstractDrawable_Draw	( int subPrimID );
	void				MOAIViewLayer_Draw			();

public:
	
	DECL_LUA_FACTORY ( MOAITableViewLayer )
	
	//----------------------------------------------------------------//
						MOAITableViewLayer			();
						~MOAITableViewLayer			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
