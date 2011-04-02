// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIGfxQuadListDeck2D.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>bind ( self )</tt>\n
\n
	Forces the texture to load.
	@param self (in)
*/
int MOAIGfxQuadListDeck2D::_bind ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "U" )

	self->Bind ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>releaseTexture ( self )</tt>\n
\n
	Releases the memory used for the brushlib's textures.  Will have to be reloaded if the brushlib is displayed again.
	@param self (in)
*/
int MOAIGfxQuadListDeck2D::_releaseTexture ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "U" )

	self->ReleaseTexture ();

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>reserveSprites ( self, total )</tt>\n
\n
	Reserves brushes for a brush library.
	@param self (in)
	@param total (in) The number of brushes to reserve.
*/
int MOAIGfxQuadListDeck2D::_reserveLists ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveLists ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>reservePrims ( self, total )</tt>\n
\n
	Reserves brush prims for a brush library.
	@param self (in)
	@param total (in) The number of brush prims to reserve.
*/
int MOAIGfxQuadListDeck2D::_reservePrims ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReservePrims ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>reserveQuads ( self, total )</tt>\n
\n
	Reserves rects for a brush library.
	@param self (in)
	@param total (in) The number of screen rects to reserve.
*/
int MOAIGfxQuadListDeck2D::_reserveQuads ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveQuads ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>reserveUVRects ( self, total )</tt>\n
\n
	Reserves UV rects for a brush library.
	@param self (in)
	@param total (in) The number of UV rects to reserve.
*/
int MOAIGfxQuadListDeck2D::_reserveUVRects ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveUVRects ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setSprite ( self, id, basePrimID, totalPrims )</tt>\n
\n
	Sets a brush in the brush library.
	@param self (in)
	@param id (in) ID of this brush.
	@param basePrimID (in) ID of the brushprim for this brush.
	@param totalPrims (in) Total number of brushprims up to this declaration(?).
*/
int MOAIGfxQuadListDeck2D::_setList ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "UNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 basePrimID = state.GetValue < u32 >( 3, 1 ) - 1;
	u32 totalPrims = state.GetValue < u32 >( 4, 0 );

	self->SetList ( idx, basePrimID, totalPrims );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setPrim ( self, id, uvRectID, screenRectID )</tt>\n
\n
	Sets a prim in the sprite library.
	@param self (in)
	@param id (in) ID of this prim.
	@param uvRectID (in) ID of the uvRect assigned to this prim.
	@param screenRectID (in) ID of the quad assigned to this prim.
*/
int MOAIGfxQuadListDeck2D::_setPrim ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "UNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 uvRectID = state.GetValue < u32 >( 3, 1 ) - 1;
	u32 screenRectID = state.GetValue < u32 >( 4, 1 ) - 1;

	self->SetPrim ( idx, uvRectID, screenRectID );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setQuad ( self, id, x0, y0, x1, y1, x2, y2, x3, y3 )</tt>\n
\n
	Sets a screen rect in the brush library.
	@param self (in)
	@param id (in) ID of this quad.
	@param x0 (in)
	@param y0 (in)
	@param x1 (in)
	@param y1 (in)
	@param x2 (in)
	@param y2 (in)
	@param x3 (in)
	@param y3 (in)
*/
int MOAIGfxQuadListDeck2D::_setQuad ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "UNNNNNNNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
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
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setRect ( self, id, xMin, yMin, xMax, yMax )</tt>\n
\n
	Sets a screen rect in the brush library.
	@param self (in)
	@param id (in) ID of this screen rect.
	@param xMin (in) Upper-left X coordinate of screen rect.
	@param yMin (in) Upper-left Y coordinate of screen rect.
	@param xMax (in) Lower-right X coordinate of screen rect.
	@param yMax (in) Lower-right Y coordinate of screen rect.
*/
int MOAIGfxQuadListDeck2D::_setRect ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "UNNNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	USRect rect;
	
	rect.mXMin = state.GetValue < float >( 3, 0.0f );
	rect.mYMin = state.GetValue < float >( 4, 0.0f );
	rect.mXMax = state.GetValue < float >( 5, 0.0f );
	rect.mYMax = state.GetValue < float >( 6, 0.0f );

	self->SetRect ( idx, rect );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setTexture ( self, id, filename )</tt>\n
\n
	Sets a texture in the brush library.
	@param self (in)
	@param id (in) ID of this texture.
	@param filename (in) Image filename to be loaded as a texture.
*/
int MOAIGfxQuadListDeck2D::_setTexture ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "U" )

	self->mTexture = MOAITexture::AffirmTexture ( state, 2 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setUVRect ( self, id, xMin, yMin, xMax, yMax, texID )</tt>\n
\n
	Sets a UV rect in the sprite library.
	@param self (in)
	@param id (in) ID of the uv rect.
	@param xMin (in) First X coordinate of UV rect.
	@param yMin (in) First Y coordinate of UV rect.
	@param xMax (in) Second X coordinate of UV rect.
	@param yMax (in) Second Y coordinate of UV rect.
*/
int MOAIGfxQuadListDeck2D::_setUVRect ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "UNNNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	USRect rect;
	
	rect.mXMin = state.GetValue < float >( 3, 0.0f );
	rect.mYMin = state.GetValue < float >( 4, 0.0f );
	rect.mXMax = state.GetValue < float >( 5, 0.0f );
	rect.mYMax = state.GetValue < float >( 6, 0.0f );

	self->SetUVRect ( idx, rect );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIGfxQuadListDeck2D::_setUVRectTransform ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuadListDeck2D, "UN" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 transform		= state.GetValue < u32 >( 3, UV_NONE );

	self->SetUVRectTransform ( idx, transform );

	return 0;
}

//================================================================//
// MOAIGfxQuadListDeck2D
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxQuadListDeck2D::Bind () {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	if ( !drawBuffer.BindTexture ( this->mTexture )) return false;
	USGLQuad::BindVertexFormat ( drawBuffer );

	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxQuadListDeck2D::Contains ( u32 idx, const USVec2D& vec ) {
	
	if ( idx < this->mSprites.Size ()) {
		USSprite& brush = this->mSprites [ idx ];
		
		for ( u32 i = 0; i < brush.mTotalPrims; ++i ) {
			USSpritePrim& prim = this->mPrims [ brush.mBasePrim + i ];
			if ( this->mQuads [ prim.mQuadID ].Contains ( vec.mX, vec.mY )) {
				return true;
			} 
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::Draw ( u32 idx, float xOff, float yOff, float xScale, float yScale ) {

	u32 size = this->mQuads.Size ();
	if ( size ) {

		idx = ( idx - 1 ) % size;

		USSprite& sprite = this->mSprites [ idx ];
		USGLQuad glQuad;
		
		u32 base = sprite.mBasePrim;
		u32 top = base + sprite.mTotalPrims;
		
		u32 totalSpritePrims = this->mPrims.Size ();
		
		for ( u32 i = base; i < top; ++i ) {
			
			USSpritePrim spritePrim = this->mPrims [ i % totalSpritePrims ];
			
			USSpriteUVRect& uvRect = this->mUVRects [ spritePrim.mUVRectID ]; 
			USQuad& quad = this->mQuads [ spritePrim.mQuadID ];
			
			if ( uvRect.mTransform == UV_ROTATE_90 ) {
				USRect& rect = uvRect.mRect;
				glQuad.SetUVs ( rect.mXMax, rect.mYMax, rect.mXMax, rect.mYMin, rect.mXMin, rect.mYMin, rect.mXMin, rect.mYMax );
			}
			else {
				glQuad.SetUVs ( uvRect.mRect );
			}
			glQuad.SetVerts ( quad.mV [ 0 ], quad.mV [ 1 ], quad.mV [ 2 ], quad.mV [ 3 ]);
			glQuad.Draw ( xOff, yOff, xScale, yScale );
		}
	}
}

//----------------------------------------------------------------//
USRect MOAIGfxQuadListDeck2D::GetBounds ( u32 idx ) {

	idx = idx - 1;

	USRect rect;
	rect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	if ( idx < this->mSprites.Size ()) {
		USSprite& sprite = this->mSprites [ idx ];
		
		if ( sprite.mTotalPrims ) {
			
			USSpritePrim prim = this->mPrims [ sprite.mBasePrim ];
			USQuad& baseQuad = this->mQuads [ prim.mQuadID ];
			
			baseQuad.GetBounds ( rect );
			
			for ( u32 i = 1; i < sprite.mTotalPrims; ++i ) {
				
				prim = this->mPrims [ sprite.mBasePrim + i ];
				USQuad& quad = this->mQuads [ prim.mQuadID ];
				
				rect.Grow ( quad.mV [ 0 ]);
				rect.Grow ( quad.mV [ 1 ]);
				rect.Grow ( quad.mV [ 2 ]);
				rect.Grow ( quad.mV [ 3 ]);
			}
		}
	}
	return rect;
}

//----------------------------------------------------------------//
MOAIGfxQuadListDeck2D::MOAIGfxQuadListDeck2D () {
	
	RTTI_SINGLE ( MOAIDeck2D )
	this->SetContentMask ( MOAIProp::CAN_DRAW );
}

//----------------------------------------------------------------//
MOAIGfxQuadListDeck2D::~MOAIGfxQuadListDeck2D () {
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::RegisterLuaClass ( USLuaState& state ) {
	
	MOAIDeck2D::RegisterLuaClass ( state );
	
	state.SetField ( -1, "UV_NONE", ( u32 )UV_NONE );
	state.SetField ( -1, "UV_ROTATE_90", ( u32 )UV_ROTATE_90 );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::RegisterLuaFuncs ( USLuaState& state ) {

	MOAIDeck2D::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
		{ "bind",					_bind },
		{ "releaseTexture",			_releaseTexture },
		{ "reserveLists",			_reserveLists },
		{ "reservePrims",			_reservePrims },
		{ "reserveQuads",			_reserveQuads },
		{ "reserveUVRects",			_reserveUVRects },
		{ "setList",				_setList },
		{ "setPrim",				_setPrim },
		{ "setQuad",				_setQuad },
		{ "setRect",				_setRect },
		{ "setUVRect",				_setUVRect },
		{ "setUVRectTransform",		_setUVRectTransform },
		{ "setTexture",				_setTexture },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::ReleaseTexture () {

	this->mTexture->USTexture::Release ();
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::ReserveLists ( u32 total ) {

	this->mSprites.Init ( total );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::ReservePrims ( u32 total ) {

	this->mPrims.Init ( total );
	
	USSpritePrim zero;
	zero.mQuadID		= 0;
	zero.mUVRectID		= 0;
	
	this->mPrims.Fill ( zero );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::ReserveQuads ( u32 total ) {

	this->mQuads.Init ( total );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::ReserveUVRects ( u32 total ) {

	this->mUVRects.Init ( total );
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::SetList ( u32 idx, u32 basePrimID, u32 totalPrims ) {

	if ( !this->mSprites.Size ()) return;
	if ( !this->mPrims.Size ()) return;
	
	USSprite& sprite = this->mSprites [ idx % this->mSprites.Size ()];
	
	sprite.mBasePrim = basePrimID % this->mPrims.Size ();
	sprite.mTotalPrims = totalPrims;
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::SetPrim ( u32 idx, u32 uvRectID, u32 screenRectID ) {
	
	if ( !this->mPrims.Size ()) return;
	if ( !this->mUVRects.Size ()) return;
	if ( !this->mQuads.Size ()) return;
	
	USSpritePrim& spritePrim = this->mPrims [ idx % this->mPrims.Size ()];
	
	spritePrim.mUVRectID = uvRectID % this->mUVRects.Size ();
	spritePrim.mQuadID = screenRectID % this->mQuads.Size ();
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
void MOAIGfxQuadListDeck2D::SetUVRect ( u32 idx, USRect& rect ) {

	if ( !this->mUVRects.Size ()) return;
	USSpriteUVRect& uvRect = this->mUVRects [ idx % this->mUVRects.Size ()];
	
	uvRect.mRect = rect;
	uvRect.mTransform = UV_NONE;
}

//----------------------------------------------------------------//
void MOAIGfxQuadListDeck2D::SetUVRectTransform ( u32 idx, u32 transform ) {

	if ( !this->mUVRects.Size ()) return;
	USSpriteUVRect& uvRect = this->mUVRects [ idx % this->mUVRects.Size ()];
	
	uvRect.mTransform = transform;
}

//----------------------------------------------------------------//
STLString MOAIGfxQuadListDeck2D::ToString () {

	STLString repr;

	//PRETTY_PRINT ( repr, mTexture )
	//PRETTY_PRINT ( repr, mUVRects )
	//PRETTY_PRINT ( repr, mQuads )
	//PRETTY_PRINT ( repr, mPrims )
	//PRETTY_PRINT ( repr, mSprites )

	return repr;
}
