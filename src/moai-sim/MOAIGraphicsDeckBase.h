// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSDECKBASE_H
#define	MOAIGRAPHICSDECKBASE_H

#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIMaterialBatch.h>

//================================================================//
// MOAIGraphicsDeckBase
//================================================================//
class MOAIGraphicsDeckBase :
	public virtual MOAIDeck,
	public MOAIMaterialBatch {
protected:

	//----------------------------------------------------------------//
	virtual MOAIMaterialBatch*		MOAIDeck_GetMaterialBatch		();

public:
	
	//----------------------------------------------------------------//
				MOAIGraphicsDeckBase		();
				~MOAIGraphicsDeckBase		();
	void		RegisterLuaClass			( MOAILuaState& state );
	void		RegisterLuaFuncs			( MOAILuaState& state );
	void		SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void		SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
