// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAISpriteDeck2D.h>
#include <moai-sim/MOAIMaterialStackMgr.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getQuad
	@text	Get model space quad given a deck index. 
			Vertex order is clockwiese from upper left.
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@out	number x0
	@out	number y0
	@out	number x1
	@out	number y1
	@out	number x2
	@out	number y2
	@out	number x3
	@out	number y3
*/
int MOAISpriteDeck2D::_getQuad ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "U" )
	
	u32 quadID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 10 ) {
		quadID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	return quadID < self->mQuads.Size () ? state.Push ( self->mQuads [ quadID ]) : 0;
}

//----------------------------------------------------------------//
/**	@lua	getRect
	@text	Set model space quad given a valid deck index and a rect.
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAISpriteDeck2D::_getRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UN" )
	
	u32 rectID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 10 ) {
		rectID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	return rectID < self->mQuads.Size () ? state.Push ( self->mQuads [ rectID ].GetBounds ()) : 0;
}

//----------------------------------------------------------------//
/**	@lua	getUVQuad
	@text	Get UV model space quad given a valid deck index.
			Vertex order is clockwise from upper left.

	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@out	number u0
	@out	number v0
	@out	number u1
	@out	number v1
	@out	number u2
	@out	number v2
	@out	number u3
	@out	number v3
*/
int MOAISpriteDeck2D::_getUVQuad ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UN" )
	
	u32 quadID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 10 ) {
		quadID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	return quadID < self->mQuads.Size () ? state.Push ( self->mUVQuads [ quadID ]) : 0;
}

//----------------------------------------------------------------//
/**	@lua	getUVRect
	@text	Get UV model space quad given a valid deck index.
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAISpriteDeck2D::_getUVRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "U" )
	
	u32 rectID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 10 ) {
		rectID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	return rectID < self->mUVQuads.Size () ? state.Push ( self->mUVQuads [ rectID ].GetBounds ()) : 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveQuads
	@text	Reserve quads.
	
	@in		MOAISpriteDeck2D self
	@in		number nQuads
	@out	nil
*/
int MOAISpriteDeck2D::_reserveQuads ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveQuads ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveSprites
	@text	Reserve sprites.
	
	@in		MOAISpriteDeck2D self
	@in		number nPairs
	@out	nil
*/
int MOAISpriteDeck2D::_reserveSprites ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReservePairs ( total );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveSpriteLists
	@text	Reserve quad lists.
	
	@in		MOAISpriteDeck2D self
	@in		number nLists
	@out	nil
*/
int MOAISpriteDeck2D::_reserveSpriteLists ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveLists ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveUVQuads
	@text	Reserve UV quads.
	
	@in		MOAISpriteDeck2D self
	@in		number nUVQuads
	@out	nil
*/
int MOAISpriteDeck2D::_reserveUVQuads ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveUVQuads ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setQuad
	@text	Set model space quad given a valid deck index. Vertex order is
			clockwise from upper left (xMin, yMax)
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		number x3
	@in		number y3
	@out	nil
*/
int MOAISpriteDeck2D::_setQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "U" )

	u32 quadID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 10 ) {
		quadID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	ZLQuad quad = state.GetQuad ( idx );

	self->SetQuad ( quadID, quad );
	self->SetBoundsDirty ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRect
	@text	Set model space quad given a valid deck index and a rect.
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAISpriteDeck2D::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "U" )

	u32 rectID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 6 ) {
		rectID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	ZLRect rect = state.GetRect < float >( idx );

	self->SetRect ( rectID, rect );
	self->SetBoundsDirty ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSprite
	@text	Associates a quad with its UV coordinates.
	
	@in		MOAISpriteDeck2D self
	@in		number idx
	@in		number uvQuadID
	@in		number quadID
	@opt	number materialID
	@out	nil
*/
int MOAISpriteDeck2D::_setSprite ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UNNN" )

	u32 idx			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 uvQuadID	= state.GetValue < u32 >( 3, 1 ) - 1;
	u32 quadID		= state.GetValue < u32 >( 4, 1 ) - 1;
	u32 materialID	= state.GetValue < u32 >( 5, 1 ) - 1;
	
	self->SetPair ( idx, uvQuadID, quadID, materialID );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSpriteList
	@text	Initializes quad pair list at index. A list starts at the index
			of a pair and then continues sequentially for n pairs after. So
			a list with base 3 and a run of 4 would display pair 3, 4, 5,
			and 6.
	
	@in		MOAISpriteDeck2D self
	@in		number idx
	@in		number basePairID	The base pair of the list.
	@in		number totalPairs	The run of the list - total pairs to display (including base).
	@out	nil
*/
int MOAISpriteDeck2D::_setSpriteList ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UNNN" )

	u32 idx			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 basePairID	= state.GetValue < u32 >( 3, 1 ) - 1;
	u32 totalPairs	= state.GetValue < u32 >( 4, 0 );

	self->SetList ( idx, basePairID, totalPairs );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setUVQuad
	@text	Set UV space quad given a valid deck index. Vertex order is
			clockwise from upper left (xMin, yMax)
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		number x3
	@in		number y3
	@out	nil
*/
int MOAISpriteDeck2D::_setUVQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "U" )

	u32 quadID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 10 ) {
		quadID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	ZLQuad quad = state.GetQuad ( idx );

	self->SetUVQuad ( quadID, quad );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setUVRect
	@text	Set UV space quad given a valid deck index and a rect.
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAISpriteDeck2D::_setUVRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "U" )

	u32 rectID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 6 ) {
		rectID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	ZLRect rect = state.GetRect < float >( idx );

	self->SetUVRect ( rectID, rect );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	transform
	@text	Apply the given MOAITransform to all the vertices in the deck.
	
	@in		MOAISpriteDeck2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAISpriteDeck2D::_transform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UU" )
	
	MOAITransformBase* transform = state.GetLuaObject < MOAITransformBase >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->Transform ( transform->GetLocalToWorldMtx ());
		self->SetBoundsDirty ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	transformUV
	@text	Apply the given MOAITransform to all the uv coordinates in the deck.
	
	@in		MOAISpriteDeck2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAISpriteDeck2D::_transformUV ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UU" )
	
	MOAITransformBase* transform = state.GetLuaObject < MOAITransformBase >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->TransformUV ( transform->GetLocalToWorldMtx ());
	}
	return 0;
}

//================================================================//
// MOAISpriteDeck2D
//================================================================//

//----------------------------------------------------------------//
MOAIDeck* MOAISpriteDeck2D::AffirmDeck ( MOAILuaState& state, int idx ) {

	MOAIDeck* deck = state.GetLuaObject < MOAIDeck >( idx, false );
	if ( deck ) return deck;
	
	MOAITextureBase* texture = MOAITexture::AffirmTexture ( state, idx );
		
	if ( texture ) {
		
		MOAISpriteDeck2D* quadDeck = new MOAISpriteDeck2D ();
		MOAIMaterialBatch* batch = quadDeck->AffirmMaterialBatch ();
	
		assert ( quadDeck );
		assert ( batch );
		
		batch->SetTexture ( 0, texture );
		
		int hWidth = ( int )( texture->GetWidth () / 2 );
		int hHeight = ( int )( texture->GetHeight () / 2 );
		
		ZLRect rect;
		rect.Init ( -hWidth, -hHeight, hWidth, hHeight );
		quadDeck->SetRect( 0, rect );
		
		return quadDeck;
	}
	return 0;
}

//----------------------------------------------------------------//
//bool MOAISpriteDeck2D::Contains ( u32 idx, const ZLVec2D& vec ) {
//	
//	size_t size = this->mSpriteLists.Size ();
//	if ( size ) {
//		
//		idx = ( idx - 1 ) % size;
//		MOAISpriteList& brush = this->mSpriteLists [ idx ];
//		
//		for ( size_t i = 0; i < brush.mTotalSprites; ++i ) {
//			MOAISprite& prim = this->mSprites [ brush.mBaseSprite + i ];
//			if ( this->mQuads [ prim.mQuadID ].Contains ( vec.mX, vec.mY )) {
//				return true;
//			} 
//		}
//	}
//	return false;
//}

//----------------------------------------------------------------//
//bool MOAISpriteDeck2D::Inside ( u32 idx, MOAIMaterialBatch* materials, u32 granularity, ZLVec3D vec, float pad ) {
//	UNUSED ( pad );
//
//	u32 size = ( u32 )this->mSpriteLists.Size (); // TODO: cast
//	if ( size ) {
//	
//		idx = ( idx - 1 ) % size;
//		
//		MOAISpriteList& sprite = this->mSpriteLists [ idx ];
//		
//		for ( u32 i	 = 0; i < sprite.mTotalSprites; ++i ) {
//			MOAISprite& prim = this->mSprites [ sprite.mBaseSprite + i ];
//			if ( this->TestHit ( materials, prim.mMaterialID, idx, granularity, this->mQuads [ prim.mQuadID ], this->mUVQuads [ prim.mUVQuadID ], vec.mX, vec.mY )) return true;
//		}
//	}
//	return false;
//}

//----------------------------------------------------------------//
MOAISpriteDeck2D::MOAISpriteDeck2D () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIMaterialBatchHolder )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISpriteDeck2D::~MOAISpriteDeck2D () {
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIDeck::RegisterLuaClass ( state );
	MOAIMaterialBatchHolder::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );
	MOAIMaterialBatchHolder::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getQuad",				_getQuad },
		{ "getRect",				_getRect },
		{ "getUVQuad",				_getUVQuad },
		{ "getUVRect",				_getUVRect },
		{ "reserveQuads",			_reserveQuads },
		{ "reserveSprites",			_reserveSprites },
		{ "reserveSpriteLists",		_reserveSpriteLists },
		{ "reserveUVQuads",			_reserveUVQuads },
		{ "setQuad",				_setQuad },
		{ "setRect",				_setRect },
		{ "setSprite",				_setSprite },
		{ "setSpriteList",			_setSpriteList },
		{ "setUVQuad",				_setUVQuad },
		{ "setUVRect",				_setUVRect },
		{ "transform",				_transform },
		{ "transformUV",			_transformUV },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::ReserveLists ( u32 total ) {

	this->mSpriteLists.Init ( total );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::ReservePairs ( u32 total ) {

	this->mSprites.Init ( total );
	
	MOAISprite zero;
	zero.mQuadID		= 0;
	zero.mUVQuadID		= 0;
	
	this->mSprites.Fill ( zero );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::ReserveQuads ( u32 total ) {

	this->mQuads.Init ( total );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::ReserveUVQuads ( u32 total ) {

	this->mUVQuads.Init ( total );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIDeck::SerializeIn ( state, serializer );
	MOAIMaterialBatchHolder::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIDeck::SerializeOut ( state, serializer );
	MOAIMaterialBatchHolder::SerializeOut ( state, serializer );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SetList ( u32 idx, u32 basePairID, u32 totalPairs ) {
	
	this->mSpriteLists.Grow ( idx + 1 );
	MOAISpriteList& sprite = this->mSpriteLists [ idx ];
	
	sprite.mBaseSprite = basePairID;
	sprite.mTotalSprites = totalPairs;
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SetPair ( u32 idx, u32 uvQuadID, u32 quadID, u32 materialID ) {
	
	this->mSprites.Grow ( idx + 1 );
	MOAISprite& spritePair = this->mSprites [ idx ];
	
	spritePair.mUVQuadID = uvQuadID;
	spritePair.mQuadID = quadID;
	spritePair.mMaterialID = materialID;
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SetQuad ( u32 idx, ZLQuad& quad ) {

	this->mQuads.Grow ( idx + 1 );
	this->mQuads [ idx ] = quad;
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SetRect ( u32 idx, ZLRect& rect ) {

	this->mQuads.Grow ( idx + 1 );
	this->mQuads [ idx ].Init ( rect );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SetUVQuad ( u32 idx, ZLQuad& quad ) {

	this->mUVQuads.Grow ( idx + 1 );
	this->mUVQuads [ idx ] = quad;
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SetUVRect ( u32 idx, ZLRect& rect ) {

	this->mUVQuads.Grow ( idx + 1 );
	this->mUVQuads [ idx ].Init ( rect );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::Transform ( const ZLAffine3D& mtx ) {

	size_t total = this->mQuads.Size ();
	for ( size_t i = 0; i < total; ++i ) {
		this->mQuads [ i ].Transform ( mtx );
	}
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::TransformUV ( const ZLAffine3D& mtx ) {

	size_t total = this->mQuads.Size ();
	for ( size_t i = 0; i < total; ++i ) {
		this->mUVQuads [ i ].Transform ( mtx );
	}
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
ZLBounds MOAISpriteDeck2D::MOAIDeck_ComputeMaxBounds () {

	size_t size = this->mQuads.Size ();
	
	if ( size ) {
	
		ZLRect rect = this->mQuads [ 0 ].GetBounds ();
		
		for ( size_t i = 1; i < size; ++i ) {
			rect.Grow ( this->mQuads [ i ].GetBounds ());
		}
		
		ZLBounds bounds;
		bounds.Init ( rect );
		bounds.mStatus = ZLBounds::ZL_BOUNDS_OK;
		return bounds;
	}
	return ZLBounds::ZL_BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::MOAIDeck_Draw ( u32 idx ) {

	MOAIMaterialStackMgr& materialStack = MOAIMaterialStackMgr::Get ();

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAIQuadBrush::BindVertexFormat ();
	
	gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::MODEL_TO_DISPLAY_MTX ));
	gfxMgr.mVertexCache.SetUVTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::UV_TO_MODEL_MTX ));

	size_t totalSprites			= this->mSprites.Size ();
	size_t totalSpriteLists		= this->mSpriteLists.Size ();
	size_t totalQuads			= this->mQuads.Size ();
	
	if ( totalSprites ) {

		u32 base;
		u32 top;
		
		if ( totalSpriteLists ) {
		
			MOAISpriteList& spriteList = this->mSpriteLists [ idx % totalSpriteLists ];
			base = spriteList.mBaseSprite;
			top = base + spriteList.mTotalSprites;
		}
		else {
		
			base = idx % totalSprites;
			top = base + 1;
		}
		
		u32 materialID = MOAIMaterialBatch::UNKNOWN;
		materialStack.Push (); // push an empty stack frame to facilitate "swap" below
		
		for ( size_t i = base; i < top; ++i ) {
			
			MOAISprite spritePair = this->mSprites [ i % totalSprites ];
			
			if (( i == base ) || ( materialID != spritePair.mMaterialID )) {
			
				materialID = spritePair.mMaterialID;
				
				MOAIMaterial* spriteMaterial = this->GetMaterial ( materialID );
				if ( spriteMaterial ) {
					
					materialStack.Pop ();
					materialStack.Push ( spriteMaterial );
					materialStack.SetShader ( MOAIShaderMgr::DECK2D_SHADER );
					materialStack.LoadGfxState ();
				}
			}
			
			MOAIQuadBrush glQuad;
			glQuad.mUVQuad = this->mUVQuads [ spritePair.mUVQuadID ];
			glQuad.mModelQuad = this->mQuads [ spritePair.mQuadID ];
			glQuad.Draw ();
		}
		
		materialStack.Pop ();
	}
	else {
		
		MOAIQuadBrush quadBrush;
		MOAIMaterial* material = 0;
		
		if ( totalQuads ) {
		
			size_t itemIdx = idx % totalQuads;
			material = this->GetMaterial ( itemIdx );

			quadBrush.mModelQuad = this->mQuads [ itemIdx ];

			if ( itemIdx < this->mUVQuads.Size ()) {
				quadBrush.mUVQuad = this->mUVQuads [ itemIdx ];
			}
			else {
				quadBrush.mUVQuad.Init ( 0.0f, 1.0f, 1.0f, 0.0f );
			}
		}
		else {
		
			material = this->GetMaterial ();
			
			quadBrush.mModelQuad.Init ( -0.5f, -0.5f, 0.5f, 0.5f );
			quadBrush.mUVQuad.Init ( 0.0f, 1.0f, 1.0f, 0.0f );
		}
		
		materialStack.Push ( material );
		materialStack.SetShader ( MOAIShaderMgr::DECK2D_SHADER );
		materialStack.LoadGfxState ();
		
		quadBrush.Draw ();
		
		materialStack.Pop ();
	}
}

//----------------------------------------------------------------//
ZLBounds MOAISpriteDeck2D::MOAIDeck_GetBounds ( u32 idx ) {

	ZLBounds bounds = ZLBounds::EMPTY;

	size_t totalSprites			= this->mSprites.Size ();
	size_t totalSpriteLists		= this->mSpriteLists.Size ();
	size_t totalQuads			= this->mQuads.Size ();
	
	if ( totalQuads ) {
	
		if ( totalSprites ) {

			u32 base;
			u32 top;
			
			if ( totalSpriteLists ) {
			
				MOAISpriteList& spriteList = this->mSpriteLists [ idx % totalSpriteLists ];
				base = spriteList.mBaseSprite;
				top = base + spriteList.mTotalSprites;
			}
			else {
			
				base = idx % totalSprites;
				top = base + 1;
			}

			ZLRect rect;

			for ( size_t i = base ; i < top; ++i ) {
				
				MOAISprite sprite = this->mSprites [ i % totalSprites ];
				rect.Grow ( this->mQuads [ sprite.mQuadID ].GetBounds (), i > 0 );
			}
			bounds.Init ( rect );
		}
		else {
		
			ZLRect rect = this->mQuads [ idx % totalQuads ].GetBounds ();
			bounds.Init ( rect );
		}
	}
	else {
	
		bounds.Init ( -0.5f, 0.5f, 0.5f, -0.5f );
	}
	
	return bounds;
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAISpriteDeck2D::MOAIDeck_GetCollisionShape ( u32 idx ) {

	return 0;
}

//----------------------------------------------------------------//
bool MOAISpriteDeck2D::MOAIDeck_Overlap ( u32 idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {

	// TODO: handle granularity

	//return this->GetMaterialBatch ()->TestHit ( this, idx, granularity, this->mQuad.mModelQuad, this->mQuad.mUVQuad, vec.mX, vec.mY );
	
	return true;
}

//----------------------------------------------------------------//
bool MOAISpriteDeck2D::MOAIDeck_Overlap ( u32 idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {

	// TODO: handle granularity

	return (( vec.mZ == 0.0f ) && this->MOAIDeck_Overlap ( idx, vec, granularity, result ));
}
