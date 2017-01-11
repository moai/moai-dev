// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALHOLDER_H
#define	MOAIMATERIALHOLDER_H

#include <moai-sim/MOAIMaterialHolderMixIn.h>

class MOAIImage;
class MOAIShader;
class MOAITextureBase;

//================================================================//
// MOAIMaterialHolder
//================================================================//
// TODO: doxygen
class MOAIMaterialHolder :
	public virtual MOAIMaterialHolderMixIn < MOAIMaterialHolder > {
public:

	DECL_LUA_FACTORY ( MOAIMaterialHolder )

	//----------------------------------------------------------------//
				MOAIMaterialHolder			();
				~MOAIMaterialHolder			();
	void		RegisterLuaClass			( MOAILuaState& state );
	void		RegisterLuaFuncs			( MOAILuaState& state );
	void		SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void		SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
