// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAITileFlags.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	reserve
	@text	The total number of indices to remap. Index remaps will be
			initialized from 1 to N.
	
	@in		MOAIDeckRemapper self
	@opt	number size		Default value is 0.
	@out	nil
*/
int MOAIDeckRemapper::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckRemapper, "U" )

	u32 size = state.GetValue < u32 >( 2, 0 );
	self->mRemap.Init ( size );
	
	for ( u32 i = 0; i < size; ++i ) {
		self->mRemap [ i ] = i + 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setBase
	@text	Set the base offset for the range of indices to remap.
			Used when remapping only a portion of the indices in
			the original deck.
	
	@in		MOAIDeckRemapper self
	@opt	number base		Default value is 0.
	@out	nil
*/
int MOAIDeckRemapper::_setBase ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckRemapper, "U" )
	
	self->mBase = state.GetValue < u32 >( 2, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setBounds
	@text	Set the dimensions of a bounding box at a given index.
	
	@in		MOAIBoundsDeck self
	@in		number idx
	@in		number xMin
	@in		number yMin
	@in		number zMin
	@in		number xMax
	@in		number yMax
	@in		number zMax
	@out	nil
*/
//int	MOAIBoundsDeck::_setBounds ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIBoundsDeck, "UN" )
//	
//	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
//	
//	if ( idx < self->mBoundsArray.Size ()) {
//		ZLBox bounds = state.GetBox ( 3 );
//		bounds.Bless ();
//		self->mBoundsArray [ idx ] = bounds;
//		self->SetBoundsDirty ();
//	}
//	return 0;
//}

//----------------------------------------------------------------//
/**	@lua	setIndex
	@text	Associate a deck index with a bounding box.
	
	@in		MOAIBoundsDeck self
	@in		number idx
	@in		number boundsID
	@out	nil
*/
//int	MOAIBoundsDeck::_setIndex ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIBoundsDeck, "UN" )
//	
//	u32 idx			= state.GetValue < u32 >( 2, 1 ) - 1;
//	u32 boundsID	= state.GetValue < u32 >( 3, 1 ) - 1;
//	
//	if ( idx < self->mIndexMap.Size ()) {
//		self->mIndexMap [ idx ] = boundsID;
//	}
//	return 0;
//}

//----------------------------------------------------------------//
/**	@lua	setRemap
	@text	Remap a single index to a new value.
	
	@in		MOAIDeckRemapper self
	@in		number index		Index to remap.
	@opt	number remap		New value for index. Default value is index (i.e. remove the remap).
	@out	nil
*/
int MOAIDeckRemapper::_setRemap ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckRemapper, "UN" )

	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 remap	= state.GetValue < u32 >( 3, idx );
	
	if ( idx < self->mRemap.Size ()) {
		self->mRemap [ idx ] = remap;
	}
	return 0;
}

//================================================================//
// MOAIDeckRemapper
//================================================================//

//----------------------------------------------------------------//
//ZLBox MOAIBoundsDeck::ComputeMaxBounds () {
//
//	ZLBox bounds;
//
//	size_t size = this->mBoundsArray.Size ();
//	if ( size == 0 ) {
//		bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
//	}
//	else {
//		this->mMaxBounds = this->mBoundsArray [ 0 ];
//		for ( size_t i = 1; i < size; ++i ) {
//			bounds.Grow ( this->mBoundsArray [ i ]);
//		}
//	}
//	return bounds;
//}

//----------------------------------------------------------------//
//ZLBox MOAIBoundsDeck::GetItemBounds ( u32 idx ) {
//
//	idx = ( idx - 1 ) % this->mIndexMap.Size ();
//	idx = this->mIndexMap [ idx ] % this->mBoundsArray.Size ();
//	return this->mBoundsArray [ idx ];
//}

//----------------------------------------------------------------//
MOAIDeckRemapper::MOAIDeckRemapper () :
	mBase ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeckRemapper::~MOAIDeckRemapper () {
}

//----------------------------------------------------------------//
void MOAIDeckRemapper::RegisterLuaClass ( MOAILuaState& state ) {

	MOAINode::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDeckRemapper::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserve",			_reserve },
		{ "setBase",			_setBase },
		{ "setRemap",			_setRemap },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
u32 MOAIDeckRemapper::Remap ( u32 idx ) const {

	u32 code = ( idx & MOAITileFlags::CODE_MASK ) - 1;

	if (( code >= this->mBase ) && ( code < this->mRemap.Size ())) {
	
		u32 flags = idx & MOAITileFlags::FLAGS_MASK;
		return ( this->mRemap [ code ] ^ ( flags & MOAITileFlags::FLIP_MASK )) | ( flags & MOAITileFlags::HIDDEN );
	}
	return idx;
}

//----------------------------------------------------------------//
u32 MOAIDeckRemapper::Remap ( const MOAIDeckRemapper* remapper, u32 idx ) {

	return remapper ? remapper->Remap ( idx ) : idx;
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIDeckRemapper::MOAINode_ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {

	attrID -=1;
	if (( attrID >= this->mBase ) && ( attrID < this->mRemap.Size ())) {
		this->mRemap [ attrID ] = ZLFloat::ToIndex ( attr.Apply (( float )this->mRemap [ attrID ], op, MOAIAttribute::ATTR_READ_WRITE ));
		return true;
	}
	return false;
}
