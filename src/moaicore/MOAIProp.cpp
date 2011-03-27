// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAISurfaceSampler2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( priority ) getPriority ( self )</tt>\n
\n
	Returns current rendering priority of prop.
	@param self (in)
	@param priority (out)
*/
int MOAIProp::_getPriority ( lua_State* L ) {
	LUA_SETUP ( MOAIProp, "U" )
	
	if ( self->mPriority != UNKNOWN_PRIORITY ) {
		lua_pushnumber ( state, self->mPriority );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setPriority ( self, priority )</tt>\n
\n
	Sets the primitive's render priority.
	@param self (in)
	@param priority (in)
*/
int MOAIProp::_setPriority ( lua_State* L ) {
	LUA_SETUP ( MOAIProp, "U" )
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
		self->mPriority = ( s32 )state.GetValue < int >( 2, 0 );
	}
	else {
		self->mPriority = UNKNOWN_PRIORITY;
		if ( self->mPartition ) {
			self->mPartition->AffirmPriority ( *self );
		}
	}
	return 0;
}

//================================================================//
// MOAIProp
//================================================================//

//----------------------------------------------------------------//
void MOAIProp::Draw () {
}

//----------------------------------------------------------------//
void MOAIProp::DrawDebug () {
}

//----------------------------------------------------------------//
void MOAIProp::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
	UNUSED ( sampler );
}

//----------------------------------------------------------------//
bool MOAIProp::GetCellRect ( USRect* cellRect, USRect* paddedRect ) {

	if ( !( cellRect || paddedRect )) return false;
	
	if ( this->mLayer ) {
	
		USVec2D center;
		this->mBounds.GetCenter ( center );
		
		USTileCoord coord = this->mLayer->mGridSpace.GetTileCoord ( center );
		USRect rect = this->mLayer->mGridSpace.GetTileRect ( coord );
		
		if ( cellRect ) {
			*cellRect = rect;
		}
		
		if ( paddedRect ) {
			rect.Inflate ( this->mLayer->mCellSize * 0.5f );
			*paddedRect = rect;
		}
		return true;
	}
	
	if ( cellRect ) {
		cellRect->Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	
	if ( paddedRect ) {
		paddedRect->Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	
	return false;
}

//----------------------------------------------------------------//
USRect MOAIProp::GetBounds () {

	return this->mBounds;
}

//----------------------------------------------------------------//
void MOAIProp::RegisterLuaClass ( USLuaState& state ) {
	
	MOAITransform::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIProp::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAITransform::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
		{ "getPriority",		_getPriority },
		{ "setPriority",		_setPriority },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIProp::SetBounds () {

	this->mLayer = 0;
	this->mCellSize = 0.0f;
	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
void MOAIProp::SetBounds ( const USRect& bounds ) {

	this->mBounds = bounds;
	this->mBounds.Bless ();

	float width = this->mBounds.Width ();
	float height = this->mBounds.Height ();
	
	this->mCellSize = ( width > height ) ? width : height;
}

//----------------------------------------------------------------//
void MOAIProp::UpdateBounds ( u32 status ) {

	if ( this->mPartition ) {
		this->mPartition->UpdateProp ( *this, status );
	}
	else {
		this->SetBounds ();
	}
}

//----------------------------------------------------------------//
void MOAIProp::UpdateBounds ( const USRect& bounds, u32 status ) {

	if ( this->mPartition ) {
		this->mPartition->UpdateProp ( *this, bounds, status );
	}
	else {
		this->SetBounds ( bounds );
	}
}

//----------------------------------------------------------------//
MOAIProp::MOAIProp () :
	mPartition ( 0 ),
	mLayer ( 0 ),
	mCell ( 0 ),
	mNextResult ( 0 ),
	mMask ( 0xffffffff ),
	mCellSize ( 0.0f ),
	mPriority ( UNKNOWN_PRIORITY ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransform )
	RTTI_END
	
	this->mLinkInCell.Data ( this );
	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIProp::~MOAIProp () {

	if ( this->mCell ) {
		this->mCell->RemoveProp ( *this );
	}
}