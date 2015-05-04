// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALBATCH_H
#define	MOAIMATERIALBATCH_H

class MOAIGfxState;
class MOAIImage;
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
	MOAIImage*			mHitMask;

	u32		mHitColorScalar;
	u32		mHitColorThreshold;
	

public:

	//----------------------------------------------------------------//
	void				LoadGfxState			( MOAIMaterial* fallback, u32 defaultShader );
						MOAIMaterial			();
	virtual				~MOAIMaterial			();
	bool				TestHit					( float x, float y ); // in local (uv) space of the mask
	
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
	static int			_reserveMaterials			( lua_State* L );
	static int			_setHitMask					( lua_State* L );
	static int			_setHitMaskScalar			( lua_State* L );
	static int			_setHitMaskThreshold		( lua_State* L );
	static int			_setIndexBatchSize			( lua_State* L );
	static int			_setShader					( lua_State* L );
	static int			_setTexture					( lua_State* L );

	//----------------------------------------------------------------//
	inline u32 GetRawIndex ( u32 idx ) {
	
		size_t totalMaterials = this->mMaterials.Size ();
		return ( totalMaterials && ( this->mIndexBatchSize > 0 )) ? ((( u32 )( idx / this->mIndexBatchSize )) % totalMaterials ) : 0;
	}

public:

	static const u32 UNKNOWN = 0xffffffff;

	GET_SET ( u32, IndexBatchSize, mIndexBatchSize )

	DECL_LUA_FACTORY ( MOAIMaterialBatch )

	//----------------------------------------------------------------//
	MOAIMaterial&		AffirmMaterial				( u32 idx );
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
	
	void				SetHitMask					( u32 idx, MOAIImage* mask );
	MOAIImage*			SetHitMask					( MOAILuaState& state, u32 idx );
	void				SetHitMaskScalar			( u32 idx, u32 scalar );
	void				SetHitMaskScalar			( MOAILuaState& state, u32 idx );
	void				SetHitMaskThreshold			( u32 idx, u32 threshold );
	void				SetHitMaskThreshold			( MOAILuaState& state, u32 idx );
	
	void				SetShader					( u32 idx, u32 shaderID );
	void				SetShader					( u32 idx, MOAIShader* shader );
	MOAIShader*			SetShader					( MOAILuaState& state, u32 idx );
	void				SetTexture					( u32 idx, MOAIGfxState* texture );
	MOAIGfxState*		SetTexture					( MOAILuaState& state, u32 idx );
	size_t				Size						();
	bool				TestHit						( MOAIMaterialBatch* fallback, u32 idx, float x, float y );
	bool				TestHit						( MOAIMaterialBatch* fallback, u32 materialID, u32 deckIndex, float x, float y );
	bool				TestHit						( MOAIMaterialBatch* fallback, u32 idx, u32 granularity, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ); // in local (model) space of the quad
	bool				TestHit						( MOAIMaterialBatch* fallback, u32 materialID, u32 deckIndex, u32 granularity, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ); // in local (model) space of the quad
};

#endif
