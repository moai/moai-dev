// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIMaterialBatch.h>
#include <moai-gfx/MOAIMaterialBatchHolder.h>


//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatchHolder::_getBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	return self->mMaterialBatch ? self->mMaterialBatch->GetBlendMode ( state, 2 ) : 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatchHolder::_getCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	return self->mMaterialBatch ? self->mMaterialBatch->GetCullMode ( state, 2 ) : 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatchHolder::_getDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	return self->mMaterialBatch ? self->mMaterialBatch->GetDepthMask ( state, 2 ) : 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatchHolder::_getDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	return self->mMaterialBatch ? self->mMaterialBatch->GetDepthTest ( state, 2 ) : 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatchHolder::_getLight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	return self->mMaterialBatch ? self->mMaterialBatch->GetLight ( state, 2 ) : 0;
}

//----------------------------------------------------------------//
/**	@name	getMaterialBatch
	@text	Return the material batch attached to the prop.
	
	@in		MOAIMaterialBatchHolder self
	@out	MOAIMaterialBatch materialBatch
*/
int MOAIMaterialBatchHolder::_getMaterialBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	self->mMaterialBatch.PushRef ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getShader
	@text	Get the shader at the given index in the prop's material batch,
			ignoring the material's index batch size. If no material batch is
			attached to the prop then nil will be returned.
	
	@in		MOAIMaterialBatchHolder self
	@opt	number idx				Default value is 1.
	@out	MOAIShaderGL shader		Or nil if none exists.
*/
int MOAIMaterialBatchHolder::_getShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	return self->mMaterialBatch ? self->mMaterialBatch->GetShader ( state, 2 ) : 0;
}

//----------------------------------------------------------------//
/**	@name	getTexture
	@text	Get the texture at the given index in the prop's material batch,
			ignoring the material's index batch size. If no material batch is
			attached to the prop then nil will be returned.
	
	@in		MOAIMaterialBatchHolder self
	@opt	number idx				Default value is 1.
	@out	MOAITextureGL texture		Or nil if none exists.
*/
int MOAIMaterialBatchHolder::_getTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	return self->mMaterialBatch ? self->mMaterialBatch->GetTexture ( state, 2 ) : 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveMaterials
	@text	Reserve materials in the prop's material batch (and creates a
			material batch if none exists).
	
	@in		MOAIMaterialBatchHolder self
	@in		number count
	@out	nil
*/
int MOAIMaterialBatchHolder::_reserveMaterials ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	self->AffirmMaterialBatch ()->Reserve ( state.GetValue < u32 >( 2, 0 ));
	
	return 0;
}

//----------------------------------------------------------------//
/** @lua	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAIMaterialBatchHolder.
			See the OpenGL documentation for an explanation of blending constants.

	@in		MOAIMaterialBatchHolder self
	@in		number srcFactor
	@in		number dstFactor
	@out	nil
*/
int MOAIMaterialBatchHolder::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )

	self->AffirmMaterialBatch ()->SetBlendMode ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCullMode
	@text	Sets and enables face culling.
	
	@in		MOAIMaterialBatchHolder self
	@opt	number cullMode			Default value is MOAIMaterialBatchHolder.CULL_NONE.
	@out	nil
*/
int MOAIMaterialBatchHolder::_setCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	self->AffirmMaterialBatch ()->SetCullMode ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthMask
	@text	Disables or enables depth writing.
	
	@in		MOAIMaterialBatchHolder self
	@opt	boolean depthMask		Default value is true.
	@out	nil
*/
int MOAIMaterialBatchHolder::_setDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	self->AffirmMaterialBatch ()->SetDepthMask ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthTest
	@text	Sets and enables depth testing (assuming depth buffer is present).
	
	@in		MOAIMaterialBatchHolder self
	@opt	number depthFunc		Default value is MOAIMaterialBatchHolder.DEPTH_TEST_DISABLE.
	@out	nil
*/
int MOAIMaterialBatchHolder::_setDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	self->AffirmMaterialBatch ()->SetDepthTest ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setIndexBatchSize
	@text	Sets and index batch size of the associated matrial batch (and creates
			a material batch if none exists).
	
	@in		MOAIMaterialBatchHolder self
	@opt	number indexBatchSize
	@out	nil
*/
int MOAIMaterialBatchHolder::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )

	self->AffirmMaterialBatch ()->SetIndexBatchSize ( state.GetValue < u32 >( 2, 1 ));
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatchHolder::_setLight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	state.Push ( self->AffirmMaterialBatch ()->SetLight ( state, 2 ));
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setMaterialBatch
	@text	Sets the prop's material batch.
	
	@in		MOAIMaterialBatchHolder self
	@opt	MOAIMaterialBatch materialBatch
	@out	nil
*/
int MOAIMaterialBatchHolder::_setMaterialBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	self->mMaterialBatch.Set ( *self, state.GetLuaObject < MOAIMaterialBatch >( 2, true ));
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setShader
	@text	Sets a shader in the associated material batch. Creates a
			material batch if none exists. Index batch size is ignored.
			
			If no value for 'idx' is provided, then the shader or shader
			preset is expected as the first paramater, and idx defaults to 1.
	
	@overload
	
		@in		MOAIMaterialBatchHolder self
		@in		number idx
		@in		variant shader			Overloaded to accept a MOAIShaderGL or a shader preset.
		@out	MOAIShaderGL shader		The shader that was set or created.
	
	@overload
	
		@in		MOAIMaterialBatchHolder self
		@in		variant shader			Overloaded to accept a MOAIShaderGL or a shader preset.
		@out	MOAIShaderGL shader		The shader that was set or created.
*/
int MOAIMaterialBatchHolder::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	state.Push ( self->AffirmMaterialBatch ()->SetShader ( state, 2 ));
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setTexture
	@text	Sets a texture in the associated material batch. Creates a
			material batch is none exists. Index batch size is ignored.
			
			If no value for 'idx' is provided, then the texture or filename
			is expected as the first paramater, and idx defaults to 1.
	
	@overload
	
		@in		MOAIMaterialBatchHolder self
		@in		number idx
		@in		variant texture			Overloaded to accept a filename, MOAITextureGL, MOAIImage, MOAIStream or MOAIDataBuffer.
		@out	MOAITextureGL texture		The texture that was set or created.
	
	@overload
	
		@in		MOAIMaterialBatchHolder self
		@in		variant texture			Overloaded to accept a filename, MOAITextureGL, MOAIImage, MOAIStream or MOAIDataBuffer.
		@out	MOAITextureGL texture		The texture that was set or created.
*/
int MOAIMaterialBatchHolder::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	state.Push ( self->AffirmMaterialBatch ()->SetTexture ( state, 2 ));
	
	return 1;
}

//================================================================//
// MOAIMaterialBatchHolder
//================================================================//

//----------------------------------------------------------------//
MOAIMaterialBatch* MOAIMaterialBatchHolder::AffirmMaterialBatch () {

	if ( !this->mMaterialBatch ) {
		this->mMaterialBatch.Set ( *this, new MOAIMaterialBatch );
	}
	return this->mMaterialBatch;
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatchHolder::GetMaterial ( ZLIndex idx ) {

	return this->mMaterialBatch ? this->mMaterialBatch->GetMaterial ( idx ) : NULL;
}

//----------------------------------------------------------------//
MOAIMaterialBatchHolder::MOAIMaterialBatchHolder () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMaterialBatchHolder::~MOAIMaterialBatchHolder () {

	this->mMaterialBatch.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIMaterialBatchHolder::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIMaterialBatchHolder::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getBlendMode",			_getBlendMode },
		{ "getCullMode",			_getCullMode },
		{ "getDepthMask",			_getDepthMask },
		{ "getDepthTest",			_getDepthTest },
		{ "getLight",				_getLight },
		{ "getMaterialBatch",		_getMaterialBatch },
		{ "getShader",				_getShader },
		{ "getTexture",				_getTexture },
		{ "reserveMaterials",		_reserveMaterials },
		{ "setBlendMode",			_setBlendMode },
		{ "setCullMode",			_setCullMode },
		{ "setDepthMask",			_setDepthMask },
		{ "setDepthTest",			_setDepthTest },
		{ "setIndexBatchSize",		_setIndexBatchSize },
		{ "setLight",				_setLight },
		{ "setMaterialBatch",		_setMaterialBatch },
		{ "setShader",				_setShader },
		{ "setTexture",				_setTexture },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMaterialBatchHolder::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMaterialBatchHolder::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}
