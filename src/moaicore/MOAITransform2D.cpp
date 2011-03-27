// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAITransform2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>addLoc (x, y)</tt>\n
\n
	Adds location values to the transform's point of origin.
	@param x Location's X coordinate.
	@param y Location's Y coordinate.
*/
int MOAITransform2D::_addLoc ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNN" )
	
	USVec2D loc = self->GetLoc ();
	
	loc.mX += state.GetValue < float >( 2, 0.0f );
	loc.mY += state.GetValue < float >( 3, 0.0f );
	
	self->SetLoc ( loc );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/** @brief <tt>addRot (rotation)</tt>\n
\n
	Adds rotation value to the transform.
	@param rotation Degrees of rotation.
*/
int MOAITransform2D::_addRot ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UN" )
	
	float d = self->GetRot ();
	d += state.GetValue < float >( 2, 0.0f );
	
	self->SetRot ( d );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>addScl (sx, sy)</tt>\n
\n
	Adds scaling values to the transform.
	@param sx Horizontal scaling value.
	@param sy Vertical scaling value.
*/
int MOAITransform2D::_addScl ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNN" )
	
	USVec2D scl = self->GetScl ();
	
	scl.mX += state.GetValue < float >( 2, 0.0f );
	scl.mY += state.GetValue < float >( 3, 0.0f );
	
	self->SetScl ( scl );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>getLoc ( )</tt>\n
\n
	Returns the x and y coordinates of this object's location.
	@return X and y coordinates in worldspace.
*/
int	MOAITransform2D::_getLoc ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "U" )
	
	lua_pushnumber ( state, self->mLoc.mX );
	lua_pushnumber ( state, self->mLoc.mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>getRot ( )</tt>\n
\n
	Returns the rotation angle of this object.
	@return Rotation angle value.
*/
int	MOAITransform2D::_getRot ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "U" )
	
	lua_pushnumber ( state, self->mDegrees );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>getScl ( )</tt>\n
\n
	Returns the x and y scaling values of this object.
	@return X and y scaling values.
*/
int	MOAITransform2D::_getScl ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "U" )
	
	lua_pushnumber ( state, self->mScale.mX );
	lua_pushnumber ( state, self->mScale.mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAITransform2D::_modelToWorld ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNN" )

	USVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );

	USAffine2D modelToWorld = self->GetLocalToWorldMtx ();
	modelToWorld.Transform ( loc );

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>move (x, y, rot, sx, sy, delay [, mode])</tt>\n
\n
	Transitions various values of the transform to other values within a certain time period.
	@param x Horizontal distance to move.
	@param y Vertical distance to move.
	@param rot New rotation value.
	@param sx New horizontal scaling value.
	@param sy New vertical scaling value.
	@param delay Time period for the transition.
	@param mode (optional) Interpolation mode.  Default: SMOOTH 
*/
int MOAITransform2D::_move ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNNNNNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveForces ( 5 );
	
	float xLoc		= state.GetValue < float >( 2, 0.0f );
	float yLoc		= state.GetValue < float >( 3, 0.0f );
	float zRot		= state.GetValue < float >( 4, 0.0f );
	float xScl		= state.GetValue < float >( 5, 0.0f );
	float yScl		= state.GetValue < float >( 6, 0.0f );
	float delay		= state.GetValue < float >( 7, 0.0f );
	u32 mode		= state.GetValue < u32 >( 8, USInterpolate::kSmooth );
	
	action->SetForce ( 0, self, MOAITransform2D::ATTR_X_LOC, xLoc, mode );
	action->SetForce ( 1, self, MOAITransform2D::ATTR_Y_LOC, yLoc, mode );
	action->SetForce ( 2, self, MOAITransform2D::ATTR_Z_ROT, zRot, mode );
	action->SetForce ( 3, self, MOAITransform2D::ATTR_X_SCL, xScl, mode );
	action->SetForce ( 4, self, MOAITransform2D::ATTR_Y_SCL, yScl, mode );
	
	action->SetDelay ( delay );
	action->Start ();
	action->PushLuaInstance ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>moveLoc (x, y, delay [, mode])</tt>\n
\n
	Moves only the transform origin within a certain time period.
	@param x Horizontal distance.
	@param y Vertical distance.
	@param delay Time period to move.
	@param mode (optional) Interpolation mode.  Default: SMOOTH
*/
int MOAITransform2D::_moveLoc ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveForces ( 2 );
	
	float xLoc		= state.GetValue < float >( 2, 0.0f );
	float yLoc		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, USInterpolate::kSmooth );
	
	action->SetForce ( 0, self, MOAITransform2D::ATTR_X_LOC, xLoc, mode );
	action->SetForce ( 1, self, MOAITransform2D::ATTR_Y_LOC, yLoc, mode );
	
	action->SetDelay ( delay );
	action->Start ();
	action->PushLuaInstance ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>moveRot (rotation, delay [, mode])</tt>\n
\n
	Changes only the transform rotation value within a certain time period.
	@param rotation New degree of rotation.
	@param delay Time to transition to new rotation value.
	@param mode (optional) Interpolation mode.  Default: SMOOTH
*/
int MOAITransform2D::_moveRot ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveForces ( 1 );
	
	float zRot		= state.GetValue < float >( 2, 0.0f );
	float delay		= state.GetValue < float >( 3, 0.0f );
	u32 mode		= state.GetValue < u32 >( 4, USInterpolate::kSmooth );
	
	action->SetForce ( 0, self, MOAITransform2D::ATTR_Z_ROT, zRot, mode );
	
	action->SetDelay ( delay );
	action->Start ();
	action->PushLuaInstance ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>moveScl (sx, sy, delay [, mode])</tt>\n
\n
	Changes only the scaling values of the transform within a certain time period.
	@param sx New horizontal scaling value.
	@param sy New vertical scaling value.
	@param delay Time to transition to new scaling values.
	@param mode (optional) Interpolation mode.  Default: SMOOTH
*/
int MOAITransform2D::_moveScl ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveForces ( 2 );
	
	float xScl		= state.GetValue < float >( 2, 0.0f );
	float yScl		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, USInterpolate::kSmooth );
	
	action->SetForce ( 0, self, MOAITransform2D::ATTR_X_SCL, xScl, mode );
	action->SetForce ( 1, self, MOAITransform2D::ATTR_Y_SCL, yScl, mode );
	
	action->SetDelay ( delay );
	action->Start ();
	action->PushLuaInstance ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>seek ( x, y, rot, sx, sy, delay [, mode] )</tt>\n
\n
	Seeks specifically to new values (like using set functions), but with a delay.
	@param x New x-location.
	@param y New y-location.
	@param rot New rotation angle.
	@param sx New horizontal scaling value.
	@param sy New vertical scaling value.
	@param delay Time to transition to new values.
	@param mode (optional) Interpolation mode.  Default: SMOOTH
*/
int MOAITransform2D::_seek ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNNNNNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveForces ( 5 );
	
	float xLoc		= state.GetValue < float >( 2, 0.0f );
	float yLoc		= state.GetValue < float >( 3, 0.0f );
	float zRot		= state.GetValue < float >( 4, 0.0f );
	float xScl		= state.GetValue < float >( 5, 0.0f );
	float yScl		= state.GetValue < float >( 6, 0.0f );
	float delay		= state.GetValue < float >( 7, 0.0f );
	u32 mode		= state.GetValue < u32 >( 8, USInterpolate::kSmooth );

	action->SetForce ( 0, self, MOAITransform2D::ATTR_X_LOC, xLoc - self->mLoc.mX, mode );
	action->SetForce ( 1, self, MOAITransform2D::ATTR_Y_LOC, yLoc - self->mLoc.mY, mode );
	action->SetForce ( 2, self, MOAITransform2D::ATTR_Z_ROT, zRot - self->mDegrees, mode );
	action->SetForce ( 3, self, MOAITransform2D::ATTR_X_SCL, xScl - self->mScale.mX, mode );
	action->SetForce ( 4, self, MOAITransform2D::ATTR_Y_SCL, yScl - self->mScale.mY, mode );
	
	action->SetDelay ( delay );
	action->Start ();
	action->PushLuaInstance ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>seekLoc ( x, y, delay [, mode] )</tt>\n
\n
	Seeks specifically to a new location (like using set functions), but with a delay.
	@param x New x-location.
	@param y New y-location.
	@param delay Time to transition to new values.
	@param mode (optional) Interpolation mode.  Default: SMOOTH
*/
int MOAITransform2D::_seekLoc ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveForces ( 2 );
	
	float xLoc		= state.GetValue < float >( 2, 0.0f );
	float yLoc		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, USInterpolate::kSmooth );

	action->SetForce ( 0, self, MOAITransform2D::ATTR_X_LOC, xLoc - self->mLoc.mX, mode );
	action->SetForce ( 1, self, MOAITransform2D::ATTR_Y_LOC, yLoc - self->mLoc.mY, mode );
	
	action->SetDelay ( delay );
	action->Start ();
	action->PushLuaInstance ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>seekRot ( rot, delay [, mode] )</tt>\n
\n
	Seeks specifically to a new rotation value (like using set functions), but with a delay.
	@param rot New rotation angle.
	@param delay Time to transition to new values.
	@param mode (optional) Interpolation mode.  Default: SMOOTH
*/
int MOAITransform2D::_seekRot ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveForces ( 1 );
	
	float zRot		= state.GetValue < float >( 2, 0.0f );
	float delay		= state.GetValue < float >( 3, 0.0f );
	u32 mode		= state.GetValue < u32 >( 4, USInterpolate::kSmooth );

	action->SetForce ( 0, self, MOAITransform2D::ATTR_Z_ROT, zRot - self->mDegrees, mode );
	
	action->SetDelay ( delay );
	action->Start ();
	action->PushLuaInstance ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>seekScl ( sx, sy, delay [, mode] )</tt>\n
\n
	Seeks specifically to new scaling values (like using set functions), but with a delay.
	@param sx New horizontal scaling value.
	@param sy New vertical scaling value.
	@param delay Time to transition to new values.
	@param mode (optional) Interpolation mode.  Default: SMOOTH
*/
int MOAITransform2D::_seekScl ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveForces ( 2 );
	
	float xScl		= state.GetValue < float >( 2, 0.0f );
	float yScl		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, USInterpolate::kSmooth );

	action->SetForce ( 0, self, MOAITransform2D::ATTR_X_SCL, xScl - self->mScale.mX, mode );
	action->SetForce ( 1, self, MOAITransform2D::ATTR_Y_SCL, yScl - self->mScale.mY, mode );
	
	action->SetDelay ( delay );
	action->Start ();
	action->PushLuaInstance ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>setLoc (x, y)</tt>\n
\n
	Sets the location for the transform origin.
	@param x X coordinate of origin location.
	@param y Y coordinate of origin location.
*/
int MOAITransform2D::_setLoc ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNN" )
	
	USVec2D loc;
	
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	
	self->SetLoc ( loc );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAITransform2D::_setOffset ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UU" );
	
	MOAITransformBase2D* offset = state.GetLuaData < MOAITransformBase2D >( 2 );
	if ( !offset ) return 0;
	
	self->SetOffset ( offset );
	self->mFilter = INHERIT_LOC;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setParent (parent)</tt>\n
\n
	Sets the parent of the transform in the hiearchy.
	@param parent Transform to become parent.
*/
int MOAITransform2D::_setParent ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "U" )
	
	MOAITransformBase2D* parent = state.GetLuaData < MOAITransformBase2D >( 2 );
	self->SetParent ( parent );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setRot (rotation)</tt>\n
\n
	Sets the rotation value of the transform.
	@param rotation New rotation value in degrees.
*/
int MOAITransform2D::_setRot ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UN" )
	
	float d = state.GetValue < float >( 2, 0.0f );
	
	self->SetRot ( d );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setScl (sx, sy)</tt>\n
\n
	Sets the scaling values of the transform.
	@param sx New horizontal scaling value.
	@param sy New vertical scaling value.
*/
int MOAITransform2D::_setScl ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNN" )
	
	USVec2D scl;
	
	scl.mX = state.GetValue < float >( 2, 0.0f );
	scl.mY = state.GetValue < float >( 3, 0.0f );
	
	self->SetScl ( scl );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAITransform2D::_worldToModel ( lua_State* L ) {
	LUA_SETUP ( MOAITransform2D, "UNN" )

	USVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );

	USAffine2D worldToModel = self->GetWorldToLocalMtx ();
	worldToModel.Transform ( loc );

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );

	return 2;
}

//================================================================//
// MOAITransform2D
//================================================================//

//----------------------------------------------------------------//
void MOAITransform2D::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	switch ( attrID ) {
		case ATTR_X_LOC:
			this->mLoc.mX = attrOp.Op ( this->mLoc.mX );
			return;
		case ATTR_Y_LOC:
			this->mLoc.mY = attrOp.Op ( this->mLoc.mY );
			return;
		case ATTR_Z_ROT:
			this->mDegrees = attrOp.Op ( this->mDegrees );
			return;
		case ATTR_X_SCL:
			this->mScale.mX = attrOp.Op ( this->mScale.mX );
			return;
		case ATTR_Y_SCL:
			this->mScale.mY = attrOp.Op ( this->mScale.mY );
			return;
	}
}

//----------------------------------------------------------------//
void MOAITransform2D::BuildTransforms ( float xOff, float yOff, float xStretch, float yStretch ) {
	
	this->mLocalToWorldMtx.ScRoTr (
		this->mScale.mX * xStretch,
		this->mScale.mY * yStretch,
		( float )D2R * this->mDegrees,
		this->mLoc.mX + xOff,
		this->mLoc.mY + yOff
	);
	
	if ( this->mParent || this->mOffset ) {
	
		USAffine2D inherit;
		inherit.Ident ();
		
		if ( this->mParent ) {
			inherit.Append ( this->mParent->GetLocalToWorldMtx ());
		}
		
		if ( this->mOffset ) {
			inherit.Append ( this->mOffset->GetLocalToWorldMtx ());
		}
		
		if ( this->mFilter == INHERIT_ALL ) {
			this->mLocalToWorldMtx.Append ( inherit );
		}
		else if ( this->mFilter == INHERIT_LOC ) {
			
			USVec2D loc ( 0.0f, 0.0f );
			inherit.Transform ( loc );
			
			USAffine2D mtx;
			mtx.Translate ( loc );
			
			this->mLocalToWorldMtx.Append ( mtx );
		}
	}
	
	this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
}

//----------------------------------------------------------------//
u32 MOAITransform2D::CountAttributes () {

	return MOAITransform2D::TOTAL_ATTR;
}

//----------------------------------------------------------------//
const USAffine2D& MOAITransform2D::GetLocalToWorldMtx () {

	return this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const USAffine2D& MOAITransform2D::GetWorldToLocalMtx () {

	return this->mWorldToLocalMtx;
}

//----------------------------------------------------------------//
MOAITransform2D::MOAITransform2D () :
	mLoc ( 0.0f, 0.0f ),
	mScale ( 1.0f, 1.0f ),
	mDegrees ( 0.0f ),
	mFilter ( INHERIT_ALL ) {
	
	RTTI_SINGLE ( MOAITransformBase2D )
	
	this->mLocalToWorldMtx.Ident ();
	this->mWorldToLocalMtx.Ident ();
}

//----------------------------------------------------------------//
MOAITransform2D::~MOAITransform2D () {

	this->mParent = 0;
}

//----------------------------------------------------------------//
void MOAITransform2D::OnDepNodeUpdate () {
	
	this->BuildTransforms ( 0.0f, 0.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
void MOAITransform2D::RegisterLuaClass ( USLuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_X_LOC", ( u32 )ATTR_X_LOC );
	state.SetField ( -1, "ATTR_Y_LOC", ( u32 )ATTR_Y_LOC );
	state.SetField ( -1, "ATTR_Z_ROT", ( u32 )ATTR_Z_ROT );
	state.SetField ( -1, "ATTR_X_SCL", ( u32 )ATTR_X_SCL );
	state.SetField ( -1, "ATTR_Y_SCL", ( u32 )ATTR_Y_SCL );
}

//----------------------------------------------------------------//
void MOAITransform2D::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "addLoc",				_addLoc },
		{ "addRot",				_addRot },
		{ "addScl",				_addScl },
		{ "getLoc",				_getLoc },
		{ "getRot",				_getRot },
		{ "getScl",				_getScl },
		{ "modelToWorld",		_modelToWorld },
		{ "move",				_move },
		{ "moveLoc",			_moveLoc },
		{ "moveRot",			_moveRot },
		{ "moveScl",			_moveScl },
		{ "seek",				_seek },
		{ "seekLoc",			_seekLoc },
		{ "seekRot",			_seekRot },
		{ "seekScl",			_seekScl },
		{ "setLoc",				_setLoc },
		{ "setOffset",			_setOffset },
		{ "setParent",			_setParent },
		{ "setRot",				_setRot },
		{ "setScl",				_setScl },
		{ "worldToModel",		_worldToModel },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITransform2D::SetLoc ( float x, float y ) {

	this->mLoc.mX = x;
	this->mLoc.mY = y;
}

//----------------------------------------------------------------//
void MOAITransform2D::SetOffset ( MOAITransformBase2D* offset ) {

	this->mOffset = offset;
	this->SetAttrLink ( ATTR_OFFSET, offset, NULL_ATTR );
	this->ScheduleUpdate ();
}

//----------------------------------------------------------------//
void MOAITransform2D::SetParent ( MOAITransformBase2D* parent ) {

	this->mParent = parent;
	this->SetAttrLink ( ATTR_PARENT, parent, NULL_ATTR );
	this->ScheduleUpdate ();
}

//----------------------------------------------------------------//
void MOAITransform2D::SetScl ( float x, float y ) {

	this->mScale.mX = x;
	this->mScale.mY = y;
}

//----------------------------------------------------------------//
STLString MOAITransform2D::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mLoc )
	PRETTY_PRINT ( repr, mScale )
	PRETTY_PRINT ( repr, mDegrees )

	return repr;
}
