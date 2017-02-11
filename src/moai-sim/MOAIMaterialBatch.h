// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALBATCH_H
#define	MOAIMATERIALBATCH_H

#include <moai-sim/MOAIMaterial.h>

class MOAIImage;
class MOAIShader;
class MOAITextureBase;

//================================================================//
// MOAIMaterialBatch
//================================================================//
/**	@lua	MOAIMaterialBatch
	@text	This is an indexed set of materials. A material is a shader, a texture
			and an optional hit mask. A material batch may be attached to a prop
			or a deck to change the materials used when rendering deck indices.
			Some decks also allow multiple material batch indicies to be specified
			per deck item. This is useful when rendering compound objects that
			need elements form multiple shaders and textures.
*/
class MOAIMaterialBatch :
	public virtual MOAILuaObject {
private:
		
	u32								mIndexBatchSize;
	ZLLeanArray < MOAIMaterial >	mMaterials;
	
	//----------------------------------------------------------------//
	static int			_clearBlendMode				( lua_State* L );
	static int			_clearCullMode				( lua_State* L );
	static int			_clearDepthMask				( lua_State* L );
	static int			_clearDepthTest				( lua_State* L );
	static int			_clearShader				( lua_State* L );
	static int			_clearTexture				( lua_State* L );
	static int			_getBlendMode				( lua_State* L );
	static int			_getCullMode				( lua_State* L );
	static int			_getDepthMask				( lua_State* L );
	static int			_getDepthTest				( lua_State* L );
	static int			_getIndexBatchSize			( lua_State* L );
	static int			_getLight					( lua_State* L );
	static int			_getShader					( lua_State* L );
	static int			_getTexture					( lua_State* L );
	static int			_reserveMaterials			( lua_State* L );
	static int			_setBlendMode				( lua_State* L );
	static int			_setCullMode				( lua_State* L );
	static int			_setDepthMask				( lua_State* L );
	static int			_setDepthTest				( lua_State* L );
	static int			_setIndexBatchSize			( lua_State* L );
	static int			_setLight					( lua_State* L );
	static int			_setShader					( lua_State* L );
	static int			_setTexture					( lua_State* L );

	//----------------------------------------------------------------//
	static u32			GetMaterialID				( MOAILuaState& state, int& idx );
	static u32			GetMaterialID				( MOAILuaState& state, int& idx, int stackSizeWithMaterialID );
	static u32			GetNamedGlobalID			( MOAILuaState& state, int& idx, u32& globalID );

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
	void				ClearBlendMode				( u32 idx );
	int					ClearBlendMode				( MOAILuaState& state, int idx );
	void				ClearCullMode				( u32 idx );
	int					ClearCullMode				( MOAILuaState& state, int idx );
	void				ClearDepthMask				( u32 idx );
	int					ClearDepthMask				( MOAILuaState& state, int idx );
	void				ClearDepthTest				( u32 idx );
	int					ClearDepthTest				( MOAILuaState& state, int idx );
	void				ClearShader					( u32 idx );
	int					ClearShader					( MOAILuaState& state, int idx );
	void				ClearTexture				( u32 idx );
	int					ClearTexture				( MOAILuaState& state, int idx );
	int					GetBlendEquation			( MOAILuaState& state, int idx );
	int					GetBlendMode				( MOAILuaState& state, int idx );
	int					GetCullMode					( MOAILuaState& state, int idx );
	int					GetDepthMask				( MOAILuaState& state, int idx );
	int					GetDepthTest				( MOAILuaState& state, int idx );
	int					GetLight					( MOAILuaState& state, int idx );
	MOAIMaterial*		GetMaterial					( u32 idx );
	int					GetShader					( MOAILuaState& state, int idx );
	int					GetTexture					( MOAILuaState& state, int idx );
						MOAIMaterialBatch			();
	virtual				~MOAIMaterialBatch			();
	MOAIMaterial*		RawGetMaterial				( u32 idx );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				Reserve						( u32 n );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetBlendMode				( u32 idx, const MOAIBlendMode& blendMode );
	void				SetBlendMode				( MOAILuaState& state, int idx );
	void				SetCullMode					( u32 idx, int cullMode );
	void				SetCullMode					( MOAILuaState& state, int idx );
	void				SetDepthMask				( u32 idx, bool depthMask );
	void				SetDepthMask				( MOAILuaState& state, int idx );
	void				SetDepthTest				( u32 idx, int depthTest );
	void				SetDepthTest				( MOAILuaState& state, int idx );
	void				SetLight					( u32 idx, u32 globalID, MOAILight* light );
	MOAILight*			SetLight					( MOAILuaState& state, int idx );
	void				SetShader					( u32 idx, u32 shaderID );
	void				SetShader					( u32 idx, MOAIShader* shader );
	MOAIShader*			SetShader					( MOAILuaState& state, int idx );
	void				SetTexture					( u32 idx, MOAITextureBase* texture );
	void				SetTexture					( u32 idx, u32 name, MOAITextureBase* texture );
	MOAITextureBase*	SetTexture					( MOAILuaState& state, int idx );
	size_t				Size						();

	
	//bool				TestHit						( MOAIMaterialBatch* override, u32 idx, float x, float y );
	//bool				TestHit						( MOAIMaterialBatch* override, u32 materialID, u32 deckIndex, float x, float y );
	//bool				TestHit						( MOAIMaterialBatch* override, u32 idx, u32 granularity, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ); // in local (model) space of the quad
	//bool				TestHit						( MOAIMaterialBatch* override, u32 materialID, u32 deckIndex, u32 granularity, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ); // in local (model) space of the quad
};

#endif
