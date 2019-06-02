// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILIGHTFORMAT_H
#define	MOAILIGHTFORMAT_H

#include <moai-gfx/MOAIShaderUniform.h>

//================================================================//
// MOAILightFormatUniform
//================================================================//
class MOAILightFormatUniform :
	public MOAIShaderUniform {
public:

	size_t				mBase;		// base in uniform buffer
};

//================================================================//
// MOAILightFormat
//================================================================//
class MOAILightFormat :
	public virtual MOAILuaObject {
private:

	friend class MOAILight;

	u32											mTextures;
	ZLLeanArray < MOAILightFormatUniform >		mUniforms;
	bool										mDirty;
	ZLSize										mBufferSize;

	//----------------------------------------------------------------//
	static int					_reserveTextures			( lua_State* L );
	static int					_reserveUniform				( lua_State* L );
	static int					_setUniform					( lua_State* L );

	//----------------------------------------------------------------//
	void						Bless						();

public:

	DECL_LUA_FACTORY ( MOAILightFormat )

	//----------------------------------------------------------------//
	MOAILightFormatUniform*		GetUniform					( ZLIndex uniformID );
								MOAILightFormat				();
								~MOAILightFormat			();
	void						MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void						MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
};

#endif
