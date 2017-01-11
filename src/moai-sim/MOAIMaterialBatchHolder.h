// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALBATCHHOLDER_H
#define	MOAIMATERIALBATCHHOLDER_H

#include <moai-sim/MOAIMaterialBatchHolderMixIn.h>

class MOAIImage;
class MOAIShader;
class MOAITextureBase;

//================================================================//
// MOAIMaterialBatchHolder
//================================================================//
// TODO: doxygen
class MOAIMaterialBatchHolder :
	public virtual MOAIMaterialBatchHolderMixIn < MOAIMaterialBatchHolder > {
public:

	DECL_LUA_FACTORY ( MOAIMaterialBatchHolder )

	//----------------------------------------------------------------//
				MOAIMaterialBatchHolder		();
				~MOAIMaterialBatchHolder	();
	void		RegisterLuaClass			( MOAILuaState& state );
	void		RegisterLuaFuncs			( MOAILuaState& state );
	void		SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void		SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
