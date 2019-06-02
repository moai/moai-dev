// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALBATCH_H
#define	MOAIMATERIALBATCH_H

#include <moai-gfx/MOAIMaterial.h>

class MOAIImage;
class MOAIShaderGL;
class MOAITextureBaseGL;

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
		
	ZLSize							mIndexBatchSize;
	ZLLeanArray < MOAIMaterial >	mMaterials;
	
	//----------------------------------------------------------------//
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
	static ZLIndex		GetMaterialID				( MOAILuaState& state, int& idx );
	static ZLIndex		GetMaterialID				( MOAILuaState& state, int& idx, bool& set );
	static ZLIndex		GetNamedGlobalID			( MOAILuaState& state, int& idx, u32& globalID );

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_SerializeIn			( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void				MOAILuaObject_SerializeOut			( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );

	//----------------------------------------------------------------//
	inline ZLIndex GetRawIndex ( ZLIndex idx ) {
	
		ZLSize totalMaterials = this->mMaterials.Size ();
		ZLSize rawIndex = ( totalMaterials && ( this->mIndexBatchSize > 0 )) ? (( idx / this->mIndexBatchSize ) % totalMaterials ) : 0;
		return ZLIndexCast ( rawIndex );
	}

public:

	GET_SET ( ZLSize, IndexBatchSize, mIndexBatchSize )

	DECL_LUA_FACTORY ( MOAIMaterialBatch )

	//----------------------------------------------------------------//
	MOAIMaterial&		AffirmMaterial				( ZLIndex idx );
	void				Clear						();
	int					GetBlendEquation			( MOAILuaState& state, int idx );
	int					GetBlendMode				( MOAILuaState& state, int idx );
	int					GetCullMode					( MOAILuaState& state, int idx );
	int					GetDepthMask				( MOAILuaState& state, int idx );
	int					GetDepthTest				( MOAILuaState& state, int idx );
	int					GetLight					( MOAILuaState& state, int idx );
	MOAIMaterial*		GetMaterial					( ZLIndex idx );
	int					GetShader					( MOAILuaState& state, int idx );
	int					GetTexture					( MOAILuaState& state, int idx );
						MOAIMaterialBatch			();
	virtual				~MOAIMaterialBatch			();
	MOAIMaterial*		RawGetMaterial				( ZLIndex idx );
	void				Reserve						( ZLSize n );
	
	void				SetBlendMode				( ZLIndex idx );
	void				SetBlendMode				( ZLIndex idx, const MOAIBlendMode& blendMode );
	void				SetBlendMode				( MOAILuaState& state, int idx );
	
	void				SetCullMode					( ZLIndex idx );
	void				SetCullMode					( ZLIndex idx, int cullMode );
	void				SetCullMode					( MOAILuaState& state, int idx );
	
	void				SetDepthMask				( ZLIndex idx );
	void				SetDepthMask				( ZLIndex idx, bool depthMask );
	void				SetDepthMask				( MOAILuaState& state, int idx );
	
	void				SetDepthTest				( ZLIndex idx );
	void				SetDepthTest				( ZLIndex idx, int depthTest );
	void				SetDepthTest				( MOAILuaState& state, int idx );
	
	void				SetLight					( ZLIndex idx, u32 name );
	void				SetLight					( ZLIndex idx, u32 name, MOAILight* light );
	MOAILight*			SetLight					( MOAILuaState& state, int idx );

	void				SetShader					( ZLIndex idx );
	void				SetShader					( ZLIndex idx, u32 shaderID );
	void				SetShader					( ZLIndex idx, MOAIShader* shader );
	MOAIShader*			SetShader					( MOAILuaState& state, int idx );

	void				SetTexture					( ZLIndex idx );
	void				SetTexture					( ZLIndex idx, MOAITexture* texture );
	void				SetTexture					( ZLIndex idx, u32 name );
	void				SetTexture					( ZLIndex idx, u32 name, MOAITexture* texture );
	MOAITexture*		SetTexture					( MOAILuaState& state, int idx );

	size_t				Size						();

	
	//bool				TestHit						( MOAIMaterialBatch* override, u32 idx, float x, float y );
	//bool				TestHit						( MOAIMaterialBatch* override, u32 materialID, u32 deckIndex, float x, float y );
	//bool				TestHit						( MOAIMaterialBatch* override, u32 idx, u32 granularity, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ); // in local (model) space of the quad
	//bool				TestHit						( MOAIMaterialBatch* override, u32 materialID, u32 deckIndex, u32 granularity, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ); // in local (model) space of the quad
};

#endif
