// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILIGHTFORMAT_H
#define	MOAILIGHTFORMAT_H

#include <moai-sim/MOAIShaderUniformHandle.h>

//================================================================//
// MOAILightFormatUniform
//================================================================//
class MOAILightFormatUniform :
	public MOAIShaderUniformBase {
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
	size_t										mBufferSize;

	//----------------------------------------------------------------//
	static int					_reserveTextures			( lua_State* L );
	static int					_reserveUniform				( lua_State* L );
	static int					_setUniform					( lua_State* L );

	//----------------------------------------------------------------//
	void						Bless						();

public:

	DECL_LUA_FACTORY ( MOAILightFormat )

	//----------------------------------------------------------------//
	MOAILightFormatUniform*		GetUniform					( u32 uniformID );
								MOAILightFormat				();
								~MOAILightFormat			();
	void						RegisterLuaClass			( MOAILuaState& state );
	void						RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
