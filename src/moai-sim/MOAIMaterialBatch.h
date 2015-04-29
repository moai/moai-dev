// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALBATCH_H
#define	MOAIMATERIALBATCH_H

class MOAIGfxState;
class MOAIShader;

//================================================================//
// MOAIMaterial
//================================================================//
// TODO: doxygen
class MOAIMaterial {
private:
	
	friend class MOAIMaterialBatch;
	
	MOAIShader*			mShader;
	MOAIGfxState*		mTexture;

public:

	//----------------------------------------------------------------//
	void				LoadGfxState			( MOAIMaterial* fallback, u32 defaultShader );
						MOAIMaterial			();
	virtual				~MOAIMaterial			();
};

//================================================================//
// MOAIMaterialBatch
//================================================================//
// TODO: doxygen
class MOAIMaterialBatch :
	public virtual MOAILuaObject {
private:
	
	u32								mIndexBatchSize;
	ZLLeanArray < MOAIMaterial >	mMaterials;
	
	//----------------------------------------------------------------//
	static int			_getIndexBatchSize			( lua_State* L );
	static int			_getShader					( lua_State* L );
	static int			_getTexture					( lua_State* L );
	static int			_reserve					( lua_State* L );
	static int			_setIndexBatchSize			( lua_State* L );
	static int			_setShader					( lua_State* L );
	static int			_setTexture					( lua_State* L );

public:

	static const u32 UNKNOWN = 0xffffffff;

	GET_SET ( u32, IndexBatchSize, mIndexBatchSize )

	DECL_LUA_FACTORY ( MOAIMaterialBatch )

	//----------------------------------------------------------------//
	void				Clear						();
	MOAIMaterial*		GetMaterial					( u32 idx );
	MOAIMaterial*		GetMaterial					( u32 materialID, u32 deckIndex );
	void				LoadGfxState				( MOAIMaterialBatch* fallback, u32 idx, u32 defaultShader );
	void				LoadGfxState				( MOAIMaterialBatch* fallback, u32 materialID, u32 deckIndex, u32 defaultShader );
						MOAIMaterialBatch			();
	virtual				~MOAIMaterialBatch			();
	MOAIMaterial*		RawGetMaterial				( u32 idx );
	MOAIShader*			RawGetShader				( u32 idx );
	MOAIGfxState*		RawGetTexture				( u32 idx );
	void				RawLoadGfxState				( u32 idx, u32 defaultShader );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				Reserve						( u32 n );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetShader					( u32 idx, u32 shaderID );
	void				SetShader					( u32 idx, MOAIShader* shader );
	MOAIShader*			SetShader					( MOAILuaState& state, u32 idx );
	void				SetTexture					( u32 idx, MOAIGfxState* texture );
	MOAIGfxState*		SetTexture					( MOAILuaState& state, u32 idx );
	size_t				Size						();
};

#endif
