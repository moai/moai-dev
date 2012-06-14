// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIGfxQuadListDeck2D.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITextureBase.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	reserveLists
	@text	Reserve quad lists.
	
	@in		MOAIGfxQuadListDeck2D self
	@in		number nLists
	@out	nil
*/
int MOAIGfxQuadListDeck2D::_reserveLists ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadListDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveLists ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	reservePairs
	@text	Reserve pairs.
	
	@in		MOAIGfxQuadListDeck2D self
	@in		number nPairs
	@out	nil
*/
int MOAIGfxQuadListDeck2D::_reservePairs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadListDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReservePairs ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserveQuads
	@text	Reserve quads.
	
	@in		MOAIGfxQuadListDeck2D self
	@in		number nQuads
	@out	nil
*/
int MOAIGfxQuadListDeck2D::_reserveQuads ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadListDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveQuads ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserveUVQuads
	@text	Reserve UV quads.
	
	@in		MOAIGfxQuadListDeck2D self
	@in		number nUVQuads
	@out	nil
*/
int MOAIGfxQuadListDeck2D::_reserveUVQuads ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadListDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveUVQuads ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setList
	@text	Initializes quad pair list at index. A list starts at the index
			of a pair and then continues sequentially for n pairs after. So
			a list with base 3 and a run of 4 would display pair 3, 4, 5,
			and 6.
	
	@in		MOAIGfxQuadListDeck2D self
	@in		number idx
	@in		number basePairID	The base pair of the list.
	@in		number totalPairs	The run of the list - total pairs to display (including base).
	@out	nil
*/
int MOAIGfxQuadListDeck2D::_setList ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadListDeck2D, "UNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 basePairID = state.GetValue < u32 >( 3, 1 ) - 1;
	u32 totalPairs = state.GetValue < u32 >( 4, 0 );

	self->SetList ( idx, basePairID, totalPairs );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPair
	@text	Associates a quad with its UV coordinates.
	
	@in		MOAIGfxQuadListDeck2D self
	@in		number idx
	@in		number uvQuadID
	@in		number quadID
	@out	nil
*/
int MOAIGfxQuadListDeck2D::_setPair ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadListDeck2D, "UNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	if ( !MOAILogMessages::CheckIndexPlusOne ( idx, self->mPairs.Size (), L )) {
		return 0;
	}
	
	u32 uvQuadID = state.GetValue < u32 >( 3, 1 ) - 1;
	if ( !MOAILogMessages::CheckIndexPlusOne ( uvQuadID, self->mUVQuads.Size (), L )) {
		return 0;
	}
	
	u32 quadID = state.GetValue < u32 >( 4, 1 ) - 1;
	if ( !MOAILogMessages::CheckIndexPlusOne ( quadID, self->mQuads.Size (), L )) {
		return 0;
	}
	
	self->SetPair ( idx, uvQuadID, quadID );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setQuad
	@text	Set model space quad given a valid deck index. Vertex order is
			clockwise from upper left (xMin, yMax)
	
	@in		MOAIGfxQuadListDeck2D self
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
int MOAIGfxQuadListDeck2D::_setQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadListDeck2D, "UNNNNNNNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	if ( MOAILogMessages::CheckIndexPlusOne ( idx, self->mQuads.Size (), L )) {
	
		USQuad quad;
		
		quad.mV [ 0 ].mX = state.GetValue < float >( 3, 0.0f );
		quad.mV [ 0 ].mY = state.GetValue < float >( 4, 0.0f );
		quad.mV [ 1 ].mX = state.GetValue < float >( 5, 0.0f );
		quad.mV [ 1 ].mY = state.GetValue < float >( 6, 0.0f );
		quad.mV [ 2 ].mX = state.GetValue < float >( 7, 0.0f );
		quad.mV [ 2 ].mY = state.GetValue < float >( 8, 0.0f );
		quad.mV [ 3 ].mX = state.GetValue < float >( 9, 0.0f );
		quad.mV [ 3 ].mY = state.GetValue < float >( 10, 0.0f );

		self->SetQuad ( idx, quad );
		self->SetBoundsDirty ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRect
	@text	Set model space quad given a valid deck index and a rect.
	
	@in		MOAIGfxQuadListDeck2D self
	@in		number idx	Index of the quad.
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIGfxQuadListDeck2D::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadListDeck2D, "UNNNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	if ( MOAILogMessages::CheckIndexPlusOne ( idx, self->mQuads.Size (), L )) {
	
		USRect rect;
		
		rect.mXMin = state.GetValue < float >( 3, 0.0f );
		rect.mYMin = state.GetValue < float >( 4, 0.0f );
		rect.mXMax = state.GetValue < float >( 5, 0.0f );
		rect.mYMax = state.GetValue < float >( 6, 0.0f );

		self->SetRect ( idx, rect );
		self->SetBoundsDirty ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUVQuad
	@text	Set UV space quad given a valid deck index. Vertex order is
			clockwise from upper left (xMin, yMax)
	
	@in		MOAIGfxQuadListDeck2D self
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
int MOAIGfxQuadListDeck2D::_setUVQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadListDeck2D, "UNNNNNNNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	if ( MOAILogMessages::CheckIndexPlusOne ( idx, self->mUVQuads.Size (), L )) {
	
		USQuad quad;
		
		quad.mV [ 0 ].mX = state.GetValue < float >( 3, 0.0f );
		quad.mV [ 0 ].mY = state.GetValue < float >( 4, 0.0f );
		quad.mV [ 1 ].mX = state.GetValue < float >( 5, 0.0f );
		quad.mV [ 1 ].mY = state.GetValue < float >( 6, 0.0f );
		quad.mV [ 2 ].mX = state.GetValue < float >( 7, 0.0f );
		quad.mV [ 2 ].mY = state.GetValue < float >( 8, 0.0f );
		quad.mV [ 3 ].mX = state.GetValue < float >( 9, 0.0f );
		quad.mV [ 3 ].mY = state.GetValue < float >( 10, 0.0f );

		self->SetUVQuad ( idx, quad );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUVRect
	@text	Set UV space quad given a valid deck index and a rect.
	
	@in		MOAIGfxQuadListDeck2D self
	@in		number idx	Index of the quad.
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIGfxQuadListDeck2D::_setUVRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadListDeck2D, "UNNNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	if ( MOAILogMessages::CheckIndexPlusOne ( idx, self->mUVQuads.Size (), L )) {
	
		USRect rect;
		
		rect.mXMin = state.GetValue < float >( 3, 0.0f );
		rect.mYMin = state.GetValue < float >( 4, 0.0f );
		rect.mXMax = state.GetValue < float >( 5, 0.0f );
		rect.mYMax = state.GetValue < float >( 6, 0.0f );

		self->SetUVRect ( idx, rect );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	transform
	@text	Apply the given MOAITransform to all the vertices in the deck.
	
	@in		MOAIGfxQuadListDeck2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAIGfxQuadListDeck2D::_transform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadListDeck2D, "UU" )
	
	MOAITransform* transform = state.GetLuaObject < MOAITransform >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->Transform ( transform->GetLocalToWorldMtx ());
		self->SetBoundsDirty ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	transformUV
	@text	Apply the given MOAITransform to all the uv coordinates in the deck.
	
	@in		MOAIGfxQuadListDeck2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAIGfxQuadListDeck2D::_transformUV ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadListDeck2D, "UU" )
	
	MOAITransform* transform = state.GetLuaObject < MOAITransform >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->TransformUV ( transform->GetLocalToWorldMtx ());
	}
	return 0;
}

//================================================================//
// MOAIGfxQuadListDeck2D
//================================================================//

//----------------------------------------------------------------//
USBox MOAIGfxQuadListDeck2D::ComputeMaxBounds () {

	USRect rect;
	rect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );

	u32 size = this->mQuads.Size ();
	for ( u32 i = 0; i < size; ++i ) {
		rect.Grow ( this->mQuads [ i ].GetBounds ());
	}
	
	USBox bounds;
	bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
	return bounds;
}

//----------------------------------------------------------------//
bool MOAIGfxQuadListDeck2D::Contains ( u32 idx, MOAIDeckRemapper* remapper, const USVec2D& vec ) {
	
	u32 size = this->mSprites.Size ();
	if ( size ) {
		
		idx = remapper ? remapper->Remap ( idx ) : idx;
		idx = ( idx - 1 ) % size;
		USSprite& brush = this->mSprites [ idx ];
		
		for ( u32 i = 0; i < brush.mTotalPairs; ++i ) {
			USSpritePair& prim = this->mPairs [ brush.mBasePair + i ];
			if ( this->mQuads [ prim.mQuadID ].Contains ( vec.mX, vec.mY )) {
				return true;
			} 
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::DrawIndex ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	UNUSED ( zScl );

	u32 size = this->mSprites.Size ();
	if ( size ) {

		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		MOAIQuadBrush::BindVertexFormat ( gfxDevice );
		
		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		
		idx = ( idx - 1 ) % size;

		USSprite& sprite = this->mSprites [ idx ];
		MOAIQuadBrush glQuad;
		
		u32 base = sprite.mBasePair;
		u32 top = base + sprite.mTotalPairs;
		
		u32 totalSpritePairs = this->mPairs.Size ();
		
		for ( u32 i = base; i < top; ++i ) {
			
			USSpritePair spritePair = this->mPairs [ i % totalSpritePairs ];
			
			USQuad& uvQuad = this->mUVQuads [ spritePair.mUVQuadID ]; 
			USQuad& quad = this->mQuads [ spritePair.mQuadID ];
			
			glQuad.SetUVs ( uvQuad.mV [ 0 ], uvQuad.mV [ 1 ], uvQuad.mV [ 2 ], uvQuad.mV [ 3 ] );
			glQuad.SetVerts ( quad.mV [ 0 ], quad.mV [ 1 ], quad.mV [ 2 ], quad.mV [ 3 ]);
			glQuad.Draw ( xOff, yOff, zOff, xScl, yScl );
		}
	}
}

//----------------------------------------------------------------//
USBox MOAIGfxQuadListDeck2D::GetItemBounds ( u32 idx ) {
	
	USBox bounds;

	u32 size = this->mSprites.Size ();
	if ( size ) {

		idx = ( idx - 1 ) % size;

		USRect rect;
		USSprite& sprite = this->mSprites [ idx ];
		
		if ( sprite.mTotalPairs ) {
			
			USSpritePair prim = this->mPairs [ sprite.mBasePair ];
			USQuad& baseQuad = this->mQuads [ prim.mQuadID ];
			
			rect = baseQuad.GetBounds ();
			
			for ( u32 i	 = 1; i < sprite.mTotalPairs; ++i ) {
				
				prim = this->mPairs [ sprite.mBasePair + i ];
				rect.Grow ( this->mQuads [ prim.mQuadID ].GetBounds ());
			}
			
			bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
			return bounds;
		}
		
		
	}

	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );	
	return bounds;
}

//----------------------------------------------------------------//
MOAIGfxQuadListDeck2D::MOAIGfxQuadListDeck2D () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
	
	this->SetContentMask ( MOAIProp::CAN_DRAW );
}

//----------------------------------------------------------------//
MOAIGfxQuadListDeck2D::~MOAIGfxQuadListDeck2D () {

	this->mTexture.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "reserveLists",		_reserveLists },
		{ "reservePairs",		_reservePairs },
		{ "reserveQuads",		_reserveQuads },
		{ "reserveUVQuads",		_reserveUVQuads },
		{ "setList",			_setList },
		{ "setPair",			_setPair },
		{ "setQuad",			_setQuad },
		{ "setRect",			_setRect },
		{ "setUVQuad",			_setUVQuad },
		{ "setUVRect",			_setUVRect },
		{ "transform",			_transform },
		{ "transformUV",		_transformUV },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::ReserveLists ( u32 total ) {

	this->mSprites.Init ( total );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::ReservePairs ( u32 total ) {

	this->mPairs.Init ( total );
	
	USSpritePair zero;
	zero.mQuadID		= 0;
	zero.mUVQuadID		= 0;
	
	this->mPairs.Fill ( zero );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::ReserveQuads ( u32 total ) {

	this->mQuads.Init ( total );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::ReserveUVQuads ( u32 total ) {

	this->mUVQuads.Init ( total );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::SetList ( u32 idx, u32 basePairID, u32 totalPairs ) {

	if ( !this->mSprites.Size ()) return;
	if ( !this->mPairs.Size ()) return;
	
	USSprite& sprite = this->mSprites [ idx % this->mSprites.Size ()];
	
	sprite.mBasePair = basePairID % this->mPairs.Size ();
	sprite.mTotalPairs = totalPairs;
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::SetPair ( u32 idx, u32 uvQuadID, u32 quadID ) {
	
	if ( !this->mPairs.Size ()) return;
	if ( !this->mUVQuads.Size ()) return;
	if ( !this->mQuads.Size ()) return;
	
	USSpritePair& spritePair = this->mPairs [ idx % this->mPairs.Size ()];
	
	spritePair.mUVQuadID = uvQuadID % this->mUVQuads.Size ();
	spritePair.mQuadID = quadID % this->mQuads.Size ();
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::SetQuad ( u32 idx, USQuad& quad ) {

	if ( idx > this->mQuads.Size ()) return;
	this->mQuads [ idx ] = quad;
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::SetRect ( u32 idx, USRect& rect ) {

	if ( idx > this->mQuads.Size ()) return;
	this->mQuads [ idx ].Init ( rect );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::SetUVQuad ( u32 idx, USQuad& quad ) {

	if ( idx > this->mUVQuads.Size ()) return;
	this->mUVQuads [ idx ] = quad;
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::SetUVRect ( u32 idx, USRect& rect ) {

	if ( idx > this->mUVQuads.Size ()) return;
	this->mUVQuads [ idx ].Init ( rect );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::Transform ( const USAffine3D& mtx ) {

	u32 total = this->mQuads.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		this->mQuads [ i ].Transform ( mtx );
	}
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::TransformUV ( const USAffine3D& mtx ) {

	u32 total = this->mQuads.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		this->mUVQuads [ i ].Transform ( mtx );
	}
}