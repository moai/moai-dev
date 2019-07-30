// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALBATCH_H
#define	MOAIMATERIALBATCH_H

#include <moai-gfx/MOAIMaterialBatchInterface.h>

class MOAIImage;
class MOAIMaterial;
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
	public virtual MOAIMaterialBatchInterface {
private:

	friend class MOAIMaterialBatchHolder;
	friend class MOAIMaterialBatchInterface;

	ZLSize											mIndexBatchSize;
	ZLLeanArray < ZLStrongPtr < MOAIMaterial > >	mMaterials;

	//----------------------------------------------------------------//
	void					MOAILuaObject_RegisterLuaClass						( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs						( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_SerializeIn							( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void					MOAILuaObject_SerializeOut							( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
	MOAIMaterial&			MOAIMaterialBatchInterface_AffirmMaterial			( ZLIndex index );
	MOAIMaterialBatch&		MOAIMaterialBatchInterface_AffirmMaterialBatch		();
	MOAIMaterial*			MOAIMaterialBatchInterface_GetMaterial				( ZLIndex index );
	MOAIMaterialBatch*		MOAIMaterialBatchInterface_GetMaterialBatch			();

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
	void				Clear						();
						MOAIMaterialBatch			();
	virtual				~MOAIMaterialBatch			();
	MOAIMaterial*		RawGetMaterial				( ZLIndex idx );
	void				Reserve						( ZLSize n );
	size_t				Size						();
};

#endif
