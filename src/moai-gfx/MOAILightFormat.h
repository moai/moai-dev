// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILIGHTFORMAT_H
#define	MOAILIGHTFORMAT_H

#include <moai-gfx/MOAIUniformDescriptor.h>

//================================================================//
// MOAILightFormatUniform
//================================================================//
class MOAILightFormatUniform :
	public MOAIUniformDescriptor {
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

	//----------------------------------------------------------------//
	void						_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	DECL_LUA_FACTORY ( MOAILightFormat )

	//----------------------------------------------------------------//
	MOAILightFormatUniform*		GetUniform					( ZLIndex uniformID );
								MOAILightFormat				();
								~MOAILightFormat			();
};

#endif
