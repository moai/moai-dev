// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIPartitionHull.h>

SUPPRESS_EMPTY_FILE_WARNING

#if 0

//================================================================//
// MOAIMaterial
//================================================================//

//----------------------------------------------------------------//
bool MOAIMaterial::LoadGfxState ( MOAIMaterial* fallback, u32 defaultShader ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	MOAIShader* shader = this->mShader ? this->mShader : (( fallback && fallback->mShader ) ? fallback->mShader : MOAIGfxMgr::Get ().GetShaderPreset ( defaultShader ));
	MOAITexture* texture = this->mTexture ? this->mTexture : (( fallback && fallback->mTexture ) ? fallback->mTexture : 0 );
	
	return ( gfxState.SetShader ( shader ) && gfxState.SetTexture ( texture ));
}

//----------------------------------------------------------------//
MOAIMaterial::MOAIMaterial () :
	mShader ( 0 ),
	mTexture ( 0 ),
	mHitMask ( 0 ),
	mHitColorScalar ( 0xffffffff ),
	mHitColorThreshold ( 0x00000000 ) {
}

//----------------------------------------------------------------//
MOAIMaterial::~MOAIMaterial () {
}

//----------------------------------------------------------------//
bool MOAIMaterial::TestHit ( float x, float y ) {

	static const ZLColorVec defaultHitColor ( 0.0f, 0.0f, 0.0f, 1.0f );

	if ( this->mHitMask ) {
	
		float width = ( float )this->mHitMask->GetWidth ();
		float height = ( float )this->mHitMask->GetHeight ();
	
		ZLColorVec scalar ( this->mHitColorScalar );
		ZLColorVec threshold ( this->mHitColorThreshold );
	
		ZLColorVec maskColor ( this->mHitMask->GetColor (( u32 )( width * x ), ( u32 )( height * y ))); // TODO: wrap, clamp
		
		maskColor.Modulate ( scalar );
		
		return (
			( threshold.mR <= maskColor.mR ) &&
			( threshold.mG <= maskColor.mG ) &&
			( threshold.mB <= maskColor.mB ) &&
			( threshold.mA <= maskColor.mA )
		);
	}
	return true;
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGraphicsPropBase::_getBlendEquation ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
	state.Push ( self->mBlendMode.mEquation );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGraphicsPropBase::_getBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
	state.Push ( self->mBlendMode.mSourceFactor );
	state.Push ( self->mBlendMode.mDestFactor );
	return 2;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGraphicsPropBase::_getCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
	state.Push ( self->mCullMode );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGraphicsPropBase::_getDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
	state.Push ( self->mDepthMask );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGraphicsPropBase::_getDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
	state.Push ( self->mDepthTest );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getIndexBatchSize
	@text	Get the index batch size.
	
	@in		MOAIMaterialBatch self
	@out	number indexBatchSize
*/
int MOAIMaterialBatch::_getIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" );
	state.Push ( self->mIndexBatchSize );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getShader
	@text	Get the shader for the given index. Index batch size
			is ignored.
	
	@in		MOAIMaterialBatch self
	@opt	number idx					Default value is 1.
	@out	MOAIShader shader
*/
int MOAIMaterialBatch::_getShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	state.Push ( self->RawGetShader ( state.GetValue < u32 >( 2, 1 ) - 1 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getTexture
	@text	Get the texture for the given index. Index batch size
			is ignored.
	
	@in		MOAIMaterialBatch self
	@opt	number idx		Default value is 1.
	@out	MOAITexture 	texture
*/
int MOAIMaterialBatch::_getTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	state.Push ( self->RawGetTexture ( state.GetValue < u32 >( 2, 1 ) - 1 ));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	reserveMaterials
	@text	Reserve material indices.
	
	@in		MOAIMaterialBatch self
	@opt	number count
	@out	nil
*/
int MOAIMaterialBatch::_reserveMaterials ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->Reserve ( state.GetValue ( 2, 0 ));
	return 1;
}

//----------------------------------------------------------------//
/** @lua	setBlendEquation
	@text	Set the blend equation. This determines how the srcFactor and dstFactor values set with setBlendMode are interpreted.

	@overload	Reset the blend function to GL_FUNC_ADD.

		@in		MOAIGraphicsPropBase self
		@out	nil

	@overload	Set the blend equation.

		@in		MOAIGraphicsPropBase self
		@in		number equation				One of GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT.
		@out	nil
	
*/
int MOAIGraphicsPropBase::_setBlendEquation ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )

	if ( state.IsType ( 2, LUA_TNUMBER )) {
		u32 equation = state.GetValue < u32 >( 2, ZLGfxEnum::BLEND_MODE_ADD );
		self->mBlendMode.SetBlendEquation ( equation );
	}
	else {
		self->mBlendMode.SetBlendEquation ( ZLGfxEnum::BLEND_MODE_ADD );
	}
	
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/** @lua	setBlendMode
	@text	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAIGraphicsPropBase.
			See the OpenGL documentation for an explanation of blending constants.

	@in		MOAIGraphicsPropBase self
	@in		number srcFactor
	@in		number dstFactor
	@out	nil
*/
int MOAIGraphicsPropBase::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )

	if ( state.IsType ( 2, LUA_TNUMBER )) {
		if ( state.IsType ( 3, LUA_TNUMBER )) {
		
			u32 srcFactor = state.GetValue < u32 >( 2, 0 );
			u32 dstFactor = state.GetValue < u32 >( 3, 0 );
			self->mBlendMode.SetBlend ( srcFactor, dstFactor );
		}
	}
	else {
		self->mBlendMode = ZLBlendMode ();
	}
	
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCullMode
	@text	Sets and enables face culling.
	
	@in		MOAIGraphicsPropBase self
	@opt	number cullMode			Default value is MOAIGraphicsPropBase.CULL_NONE.
	@out	nil
*/
int MOAIGraphicsPropBase::_setCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
	self->mCullMode = state.GetValue < int >( 2, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthMask
	@text	Disables or enables depth writing.
	
	@in		MOAIGraphicsPropBase self
	@opt	boolean depthMask		Default value is true.
	@out	nil
*/
int MOAIGraphicsPropBase::_setDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
	self->mDepthMask = state.GetValue < bool >( 2, true );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthTest
	@text	Sets and enables depth testing (assuming depth buffer is present).
	
	@in		MOAIGraphicsPropBase self
	@opt	number depthFunc		Default value is MOAIGraphicsPropBase.DEPTH_TEST_DISABLE.
	@out	nil
*/
int MOAIGraphicsPropBase::_setDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGraphicsPropBase, "U" )
	
	self->mDepthTest = state.GetValue < int >( 2, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setHitMask
	@text	Set or load an image to act as the hit mask for this deck.
	
	@in		MOAIDeck self
	@in		variant mask		A MOAIImage or a path to an image file
	@out	MOAIImage mask
*/
int MOAIMaterialBatch::_setHitMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	state.Push ( self->SetHitMask ( state, 2 ));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setHitMaskScalar
	@text	Set a color to modulate hit mask samples.
	
			If no value for 'idx' is provided, the value of index is 1.
	
	@overload
	
		@in		MOAIDeck self
		@in		number idx
		@in		number r
		@in		number g
		@in		number b
		@in		number a
		@out	nil
	
	@overload
	
		@in		MOAIDeck self
		@in		number r
		@in		number g
		@in		number b
		@in		number a
		@out	nil
*/
int MOAIMaterialBatch::_setHitMaskScalar ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->SetHitMaskScalar ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setHitMaskThreshold
	@text	Set a color to act as a threshold for hit mask samples.
	
			If no value for 'idx' is provided, the value of index is 1.
	
	@overload
	
		@in		MOAIDeck self
		@in		number idx
		@in		number r
		@in		number g
		@in		number b
		@in		number a
		@out	nil
	
	@overload
	
		@in		MOAIDeck self
		@in		number r
		@in		number g
		@in		number b
		@in		number a
		@out	nil
*/
int MOAIMaterialBatch::_setHitMaskThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->SetHitMaskThreshold ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setIndexBatchSize
	@text	Set the index batch size. When a prop or deck is drawing,
			the index is divided by the material batch's index batch size
			to get the material index. In this way sets of deck indices
			may be assigned to material indices. For example, an index
			batch size of 256 for a set of 4 materials would distribute
			1024 deck indices across materials 1 though 4. An index batch
			size of 1 would create a 1 to 1 mapping between deck indices
			and materials.
	
	@in		MOAIMaterialBatch self
	@opt	number indexBatchSize		Default value is 1.
	@out	nil
*/
int MOAIMaterialBatch::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->mIndexBatchSize = state.GetValue < u32 >( 2, 1 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setShader
	@text	Sets a shader in the material batch. Index batch size is ignored.
			
			If no value for 'idx' is provided, then the shader or shader
			preset is expected as the first paramater, and idx defaults to 1.
	
	@overload
	
		@in		MOAIMaterialBatch self
		@in		number idx
		@in		variant shader			Overloaded to accept a MOAIShader or a shader preset.
		@out	MOAIShader shader		The shader that was set or created.
	
	@overload
	
		@in		MOAIMaterialBatch self
		@in		variant shader			Overloaded to accept a MOAIShader or a shader preset.
		@out	MOAIShader shader		The shader that was set or created.
*/
int MOAIMaterialBatch::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	state.Push ( self->SetShader ( state, 2 ));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setTexture
	@text	Sets a texture in material batch. Index batch size is ignored.
			
			If no value for 'idx' is provided, then the texture or filename
			is expected as the first paramater, and idx defaults to 1.
	
	@overload
	
		@in		MOAIMaterialBatch self
		@in		number idx
		@in		variant texture			Overloaded to accept a filename, MOAITexture, MOAIImage, MOAIStream or MOAIDataBuffer.
		@out	MOAITexture texture		The texture that was set or created.
	
	@overload
	
		@in		MOAIMaterialBatch self
		@in		variant texture			Overloaded to accept a filename, MOAITexture, MOAIImage, MOAIStream or MOAIDataBuffer.
		@out	MOAITexture texture		The texture that was set or created.
*/
int MOAIMaterialBatch::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	state.Push ( self->SetTexture ( state, 2 ));
	return 1;
}

//================================================================//
// MOAIMaterialBatch
//================================================================//

//----------------------------------------------------------------//
MOAIMaterial& MOAIMaterialBatch::AffirmMaterial ( u32 idx ) {

	this->mMaterials.Grow ( idx, 1 );
	return this->mMaterials [ idx ];
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::Clear () {

	for ( u32 i = 0; i < this->mMaterials.Size (); ++i ) {
		MOAIMaterial& material = this->mMaterials [ i ];
	
		this->LuaRelease ( material.mShader );
		this->LuaRelease ( material.mTexture );
		this->LuaRelease ( material.mHitMask );
	}
	this->mMaterials.Clear ();
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatch::GetMaterial ( u32 idx ) {

	return this->RawGetMaterial ( this->GetRawIndex ( idx ));
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatch::GetMaterial ( u32 materialID, u32 deckIndex ) {

	if ( materialID == UNKNOWN ) {
		return this->GetMaterial ( deckIndex );
	}
	return this->RawGetMaterial ( materialID );
}

//----------------------------------------------------------------//
bool MOAIMaterialBatch::LoadGfxState ( MOAIMaterialBatch* override, u32 idx, u32 defaultShader ) {

	return this->LoadGfxState ( override, UNKNOWN, idx, defaultShader );
}

//----------------------------------------------------------------//
bool MOAIMaterialBatch::LoadGfxState ( MOAIMaterialBatch* override, u32 materialID, u32 deckIndex, u32 defaultShader ) {

	MOAIMaterial* primary = ( override && ( this != override )) ? override->GetMaterial ( materialID, deckIndex ) : 0;
	MOAIMaterial* secondary = this->GetMaterial ( materialID, deckIndex );

	if ( primary ) {
		return primary->LoadGfxState ( secondary, defaultShader );
	}
	else if ( secondary ) {
		return secondary->LoadGfxState ( 0, defaultShader );
	}

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	gfxState.SetTexture ();
	return gfxState.SetShader ( MOAIGfxMgr::Get ().GetShaderPreset ( defaultShader ));
}

//----------------------------------------------------------------//
MOAIMaterialBatch::MOAIMaterialBatch () :
	mIndexBatchSize ( 1 ) {
	
	RTTI_BEGIN ( MOAIMaterialBatch )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIMaterialBatch >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMaterialBatch::~MOAIMaterialBatch () {

	this->Clear ();
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatch::RawGetMaterial ( u32 idx ) {

	//return ( idx < this->mMaterials.Size ()) ? &this->mMaterials [ idx ] : 0;
	size_t size = this->mMaterials.Size ();
	return size ? &this->mMaterials [ idx % size ] : 0;
}

//----------------------------------------------------------------//
MOAIShader* MOAIMaterialBatch::RawGetShader ( u32 idx ) {

	if ( idx < this->mMaterials.Size ()) {
		return this->mMaterials [ idx ].mShader;
	}
	return 0;
}

//----------------------------------------------------------------//
MOAITexture* MOAIMaterialBatch::RawGetTexture ( u32 idx ) {

	if ( idx < this->mMaterials.Size ()) {
		return this->mMaterials [ idx ].mTexture;
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetHitMask ( u32 idx, MOAIImage* mask ) {

	MOAIMaterial& material = this->AffirmMaterial ( idx );
	
	if ( material.mHitMask != mask ) {
	
		this->LuaRetain ( mask );
		this->LuaRelease ( material.mHitMask );
		material.mHitMask  = mask;
	}
}

//----------------------------------------------------------------//
MOAIImage* MOAIMaterialBatch::SetHitMask ( MOAILuaState& state, u32 idx ) {

	u32 materialIdx = 0;
	if ( state.IsType ( idx, LUA_TNUMBER )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}

	MOAIImage* mask = MOAIImage::AffirmImage ( state, idx );
	this->SetHitMask ( materialIdx, mask );
	return mask;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetHitMaskScalar ( u32 idx, u32 scalar ) {

	this->AffirmMaterial ( idx ).mHitColorScalar = scalar;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetHitMaskScalar ( MOAILuaState& state, u32 idx ) {

	u32 materialIdx = 0;
	if ( state.CheckParams ( idx, "NNNNN", false )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	ZLColorVec scalar = state.GetValue < ZLColorVec >( idx, ZLColorVec::WHITE );
	this->SetHitMaskScalar ( materialIdx, scalar.PackRGBA ());
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetHitMaskThreshold ( u32 idx, u32 threshold ) {

	this->AffirmMaterial ( idx ).mHitColorThreshold = threshold;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetHitMaskThreshold ( MOAILuaState& state, u32 idx ) {

	u32 materialIdx = 0;
	if ( state.CheckParams ( idx, "NNNNN", false )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	ZLColorVec threshold = state.GetValue < ZLColorVec >( idx, ZLColorVec ( 0.0f, 0.0f, 0.0f, 0.0f ));
	this->SetHitMaskThreshold ( materialIdx, threshold.PackRGBA ());
}

//----------------------------------------------------------------//
//void MOAIMaterialBatch::RawLoadGfxState ( u32 idx, u32 defaultShader ) {
//
//	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
//	
//	MOAIShader* shader = 0;
//	MOAITexture* texture = 0;
//	
//	if ( idx < this->mMaterials.Size ()) {
//		shader = this->mMaterials [ idx ].mShader;
//		texture = this->mMaterials [ idx ].mTexture;
//	}
//	
//	shader = shader ? shader : MOAIGfxMgr::Get ().GetShaderPreset ( defaultShader );
//	
//	gfxMgr.SetShader ( shader );
//	gfxMgr.SetTexture ( texture );
//}

//----------------------------------------------------------------//
void MOAIMaterialBatch::Reserve ( u32 n ) {

	this->Clear ();
	this->mMaterials.Init ( n );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetShader ( u32 idx, u32 shaderID ) {

	this->SetShader ( idx, MOAIGfxMgr::Get ().GetShaderPreset ( shaderID ));
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetShader ( u32 idx, MOAIShader* shader ) {

	MOAIMaterial& material = this->AffirmMaterial ( idx );
	if ( material.mShader != shader ) {
	
		this->LuaRetain ( shader );
		this->LuaRelease ( material.mShader );
		material.mShader  = shader;
	}
}

//----------------------------------------------------------------//
MOAIShader* MOAIMaterialBatch::SetShader ( MOAILuaState& state, u32 idx ) {
	
	u32 materialIdx = 0;
	if ( state.IsType ( idx, LUA_TNUMBER ) && ( state.AbsIndex ( idx ) < state.GetTop ())) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}

	MOAIShader* shader = MOAIShader::AffirmShader ( state, idx );
	this->SetShader ( materialIdx, shader );
	return shader;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetTexture ( u32 idx, MOAITexture* texture ) {

	MOAIMaterial& material = this->AffirmMaterial ( idx );
	if ( material.mTexture != texture ) {
	
		this->LuaRetain ( texture );
		this->LuaRelease ( material.mTexture );
		material.mTexture  = texture;
	}
}

//----------------------------------------------------------------//
MOAITexture* MOAIMaterialBatch::SetTexture ( MOAILuaState& state, u32 idx ) {
	
	u32 materialIdx = 0;
	if ( state.IsType ( idx, LUA_TNUMBER )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	MOAITexture* texture = MOAITexture::AffirmTexture ( state, idx );
	this->SetTexture ( materialIdx, texture );
	return texture;
}

//----------------------------------------------------------------//
size_t MOAIMaterialBatch::Size () {

	return this->mMaterials.Size ();
}

//----------------------------------------------------------------//
bool MOAIMaterialBatch::TestHit ( MOAIMaterialBatch* override, u32 idx, float x, float y ) {

	return this->TestHit ( override, UNKNOWN, idx, x, y );
}

//----------------------------------------------------------------//
bool MOAIMaterialBatch::TestHit ( MOAIMaterialBatch* override, u32 materialID, u32 deckIndex, float x, float y ) {

	MOAIMaterial* primary = ( override && ( this != override )) ? override->GetMaterial ( materialID, deckIndex ) : 0;
	if ( primary && primary->mHitMask ) {
		return primary->TestHit ( x, y );
	}
	
	MOAIMaterial* secondary = this->GetMaterial ( materialID, deckIndex );
	if ( secondary && secondary->mHitMask ) {
		return secondary->TestHit ( x, y );
	}
	
	return 0;
}

//----------------------------------------------------------------//
bool MOAIMaterialBatch::TestHit ( MOAIMaterialBatch* override, u32 idx, u32 granularity, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ) {

	return this->TestHit ( override, UNKNOWN, idx, granularity, modelQuad, uvQuad, x, y );
}

//----------------------------------------------------------------//
bool MOAIMaterialBatch::TestHit ( MOAIMaterialBatch* override, u32 materialID, u32 deckIndex, u32 granularity, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ) {

	if ( granularity == MOAIPartitionHull::HIT_TEST_COARSE ) return true;

	ZLVec2D uv;

	if ( ZLQuad::RemapCoord ( modelQuad, uvQuad, 0, x, y, uv)) {
		return granularity == MOAIPartitionHull::HIT_TEST_FINE ? this->TestHit ( override, materialID, deckIndex, uv.mX, uv.mY ) : true;
	}
	
	if ( ZLQuad::RemapCoord ( modelQuad, uvQuad, 1, x, y, uv)) {
		return granularity == MOAIPartitionHull::HIT_TEST_FINE ? this->TestHit ( override, materialID, deckIndex, uv.mX, uv.mY ) : true;
	}

	return false;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterialBatch::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getIndexBatchSize",		_getIndexBatchSize },
		{ "getShader",				_getShader },
		{ "getTexture",				_getTexture },
		{ "reserveMaterials",		_reserveMaterials },
		{ "setHitMask",				_setHitMask },
		{ "setHitMaskScalar",		_setHitMaskScalar },
		{ "setHitMaskThreshold",	_setHitMaskThreshold },
		{ "setIndexBatchSize",		_setIndexBatchSize },
		{ "setShader",				_setShader },
		{ "setTexture",				_setTexture },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif
