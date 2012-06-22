//[MOAI LICENSE]

#include "pch.h"
#include <moaicore/MOAIContentLibrary2D.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIFeeler2D.h>
#include <moaicore/MOAIMarioFsm2D.h>
#include <moaicore/MOAISurfaceSampler2D.h>


//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( standing ) isStanding ( self )</tt>\n
\n
	Returns 'true' if the feeler is at rest on a surface.
	@param self (in)
	@param standing (out)
*/
int MOAIFeeler2D::_isStanding ( lua_State* L ) {
	LUA_SETUP ( MOAIFeeler2D, "U" );
	
	bool standing = self->mIsStanding;
	
	lua_pushboolean ( L, standing );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setMove ( self, xMove, yMove )</tt>\n
\n
	Sets the move vector to be evaluated by the feeler.
	This should be the entire move the feeler is expected
	to make in a single frame - the move vector is *not*
	scaled by  the simulation's time step.
	@param self (in)
	@param xMove (in)
	@param yMove (in)
*/
int MOAIFeeler2D::_setMove ( lua_State* L ) {
	LUA_SETUP ( MOAIFeeler2D, "U" );
	
	self->mMove.mX = state.GetValue < float >( 2, self->mMove.mX );
	self->mMove.mY = state.GetValue < float >( 3, self->mMove.mY );
	
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setSkirt ( self, skirtSize )</tt>\n
\n
	Set's the skirt size for the feeler. The skirt size is the
	feeler's 'foot.' This is a small portion of the feeler that
	may be evalted differently from the body.
	@param self (in)
	@param skirtSize (in)
*/
int MOAIFeeler2D::_setSkirt ( lua_State* L ) {
	LUA_SETUP ( MOAIFeeler2D, "UN" );
	
	self->mSkirt = state.GetValue < float >( 2, self->mSkirt );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setSize ( self, hRad, vRad )</tt>\n
\n
	Set the width and height of the collision ellipse.
	@param self (in)
	@param hRad (in)
	@param vRad (in)
*/
int MOAIFeeler2D::_setSize ( lua_State* L ) {
	LUA_SETUP ( MOAIFeeler2D, "U" );
	
	self->mHRad = state.GetValue < float >( 2, self->mHRad );
	self->mVRad = state.GetValue < float >( 3, self->mVRad );
	
	self->ScheduleUpdate ();
	
	return 0;
}

//================================================================//
// MOAIFeeler2D
//================================================================//

//----------------------------------------------------------------//
void MOAIFeeler2D::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	switch( attrID ) {
		case ATTR_X_VELOCITY:
			this->mMove.mX = attrOp.Op ( this->mMove.mX );
			break;
		case ATTR_Y_VELOCITY:
			this->mMove.mY = attrOp.Op ( this->mMove.mY );
			break;
		default:
			MOAITransform2D::ApplyAttrOp ( attrID, attrOp );
	}
}

//----------------------------------------------------------------//
void MOAIFeeler2D::Attach ( MOAITransform2D* transform ) {

	this->Detach ();

	if ( transform ) {
		
		this->mAttachTarget = transform;
		
		this->mAttachPoint.mX = this->mLoc.mX;
		this->mAttachPoint.mY = this->mLoc.mY - this->mVRad - this->mSkirt;
		
		const USMatrix2D& worldToLocalMtx = this->mAttachTarget->GetWorldToLocalMtx ();
		worldToLocalMtx.Transform ( this->mAttachPoint );
	}
}

//----------------------------------------------------------------//
u32 MOAIFeeler2D::CountAttributes () {

	return MOAIFeeler2D::TOTAL_ATTR;
}

//----------------------------------------------------------------//
void MOAIFeeler2D::Detach () {

	if ( this->mAttachTarget ) {
		
		const USMatrix2D& localToWorldMtx = this->mAttachTarget->GetLocalToWorldMtx ();
		localToWorldMtx.Transform ( this->mAttachPoint );
		
		this->mLoc.mX = this->mAttachPoint.mX;
		this->mLoc.mY = this->mAttachPoint.mY + this->mVRad + this->mSkirt;
		
		this->mAttachTarget = 0;
	}
}

//----------------------------------------------------------------//
void MOAIFeeler2D::DrawDebug () {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	debugLines.SetPenColor ( 0xffffffff );
	debugLines.SetPenWidth ( 2 );
	
	debugLines.SetPenSpace ( MOAIDebugLines::WORLD_SPACE );
	//debugLines.DrawRect ( this->GetWorldBounds ());
	
	debugLines.DrawEllipse ( this->mLoc, this->mHRad, this->mVRad, 16 );
	
	USVec2D foot = this->mLoc;
	foot.mY -= ( this->mVRad + this->mSkirt );
	
	USVec2D head = this->mLoc;
	head.mY += ( this->mVRad + this->mHat );
	
	debugLines.DrawLine ( head, foot );
}

//----------------------------------------------------------------//
u32 MOAIFeeler2D::GetLocalFrame ( USRect& frame ) {
	
	frame.mXMin = -this->mHRad;
	frame.mYMin = -this->mVRad;
	frame.mXMax = this->mHRad;
	frame.mYMax = this->mVRad;
	
	return FRAME_OK;
}

//----------------------------------------------------------------//
void MOAIFeeler2D::GatherSurfacesForMove ( MOAISurfaceSampler2D& sampler, USVec2D& move ) {

	u32 results = 0;
	
	USPartition < MOAIPrim >* partition = this->GetPartition ();
	if ( !partition ) return;
	
	USRect sweptRect;
	this->GetSweptRect ( move, sweptRect );
	sweptRect.Inflate ( this->mHRad * 0.5f ); // TODO
	
	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	debugLines.SetPenColor ( 0xff00ff00 );
	debugLines.SetPenWidth ( 1 );
	debugLines.SetPenSpace ( MOAIDebugLines::WORLD_SPACE );
	debugLines.DrawRect ( sweptRect );
	
	results = partition->GatherPrims ( sweptRect, 0, MOAIContentLibrary2D::CAN_GATHER_SURFACES );
	
	if ( results ) {
		
		USMatrix2D worldToSampleMtx;
		this->GetWorldMtxInv ( worldToSampleMtx );
		
		sampler.Init ( worldToSampleMtx, sweptRect );
		
		for ( u32 i = 0; i < results; ++i ) {
			MOAIPrim* prim = partition->GetResult ( i );
			prim->GatherSurfaces ( sampler );
		}
	}
	
	//USDebugHUD::Get ().DrawRect ( sweptRect, 0xff00ff00, 2 );
	
	//USMatrix2D invWorldMtx;
	//this->GetWorldMtxInv ( invWorldMtx );
	
	//sampler.Reset ( invWorldMtx );
	//this->mGroup->SendTileFunc ( sampler, sweptRect );
}

//----------------------------------------------------------------//
void MOAIFeeler2D::GetRect ( USRect& rect ) {
	
	rect.mXMin = this->mLoc.mX - this->mHRad;
	rect.mYMin = this->mLoc.mY - ( this->mVRad + this->mSkirt );
	rect.mXMax = this->mLoc.mX + this->mHRad;
	rect.mYMax = this->mLoc.mY + ( this->mVRad + this->mHat );
}

//----------------------------------------------------------------//
void MOAIFeeler2D::GetSweptRect ( USVec2D& move, USRect& rect ) {

	USRect bounds;
	this->GetRect ( bounds );

	rect = bounds;
	bounds.Offset ( move.mX, move.mY );
	rect.Grow ( bounds );
}

//----------------------------------------------------------------//
void MOAIFeeler2D::GetWorldMtx ( USMatrix2D& transform ) {
	
	transform.Scale (
		this->mHRad,
		this->mVRad
	);
}

//----------------------------------------------------------------//
void MOAIFeeler2D::GetWorldMtxInv ( USMatrix2D& transform ) {
	
	transform.Scale (
		1.0f / this->mHRad,
		1.0f / this->mVRad
	);
}

//----------------------------------------------------------------//
MOAIFeeler2D::MOAIFeeler2D () :
	mTouchFeet ( false ),
	mTouchHead ( false ),
	mTouchRight ( false ),
	mTouchLeft ( false ),
	mIsStanding ( false ),
	mSkirt ( 0.0f ),
	mHat ( 0.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIPrim )
	RTTI_END
	
	this->SetQueryMask ( MOAIContentLibrary2D::CAN_DRAW_DEBUG );
	
	this->mMove.Init ( 0.0f, 0.0f );
	
	this->mHRad = 32.0f;
	this->mVRad = 32.0f;
	
	this->SetAngles ( 60.5f, 120.0f );
}

//----------------------------------------------------------------//
MOAIFeeler2D::~MOAIFeeler2D () {
}

//----------------------------------------------------------------//
void MOAIFeeler2D::Move () {
	
	//MOAITransform2D* platform = this->mAttachTarget;
	
	this->Detach ();
	
	MOAIMarioFsm2D state;

	// Gather all the surfaces we think we might collide with
	this->GatherSurfacesForMove ( state, this->mMove );

	USMatrix2D worldMtx;
	this->GetWorldMtx ( worldMtx );
	state.DrawSurfaces ( worldMtx );

	state.Init ( 8, this->mFloorCos, this->mCeilCos );
	
	state.SetSkirt ( this->mSkirt / this->mVRad );
	state.SetHat ( this->mHat / this->mVRad );

	USVec2D loc = this->mLoc;
	USVec2D move = this->mMove;
	
	this->TransformToLocal ( loc );
	this->TransformToLocal ( move );
	
	loc = state.Move ( loc, move );

	this->TransformToWorld ( loc );

	this->mLoc = loc;
	
	//this->mTouchFeet = state.IsStanding ();
	//this->mTouchHead = state.IsStanding ();
	//this->mTouchRight = state.IsStanding ();
	//this->mTouchLeft = state.IsStanding ();
	this->mIsStanding = state.IsStanding ();
	
	this->mMove.Init ( 0.0f, 0.0f );
	
	this->Attach ( state.GetPlatform ());
}

//----------------------------------------------------------------//
void MOAIFeeler2D::OnDepNodeUpdate () {
	
	this->Move ();
	MOAIPrim::OnDepNodeUpdate ();
}

//----------------------------------------------------------------//
void MOAIFeeler2D::RegisterLuaClass ( USLuaState& state ) {
	
	MOAIPrim::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_X_VELOCITY", ( u32 )ATTR_X_VELOCITY );
	state.SetField ( -1, "ATTR_Y_VELOCITY", ( u32 )ATTR_Y_VELOCITY );
}

//----------------------------------------------------------------//
void MOAIFeeler2D::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIPrim::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "isStanding",			_isStanding },
		{ "setMove",			_setMove },
		{ "setSkirt",			_setSkirt },
		{ "setSize",			_setSize },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFeeler2D::SetAngles ( float floorAngle, float ceilAngle ) {

	this->mFloorCos = Cos ( floorAngle * ( float )D2R );
	this->mCeilCos = Cos ( ceilAngle * ( float )D2R );
}

//----------------------------------------------------------------//
void MOAIFeeler2D::SetMove ( float x, float y ) {

	this->mMove.Init ( x, y );
}

//----------------------------------------------------------------//
STLString MOAIFeeler2D::ToString () {

	STLString repr( MOAITransform2D::ToString ());

	return repr;
}

//----------------------------------------------------------------//
void MOAIFeeler2D::TransformToLocal ( USVec2D& vec ) {

	vec.Scale ( 1.0f / this->mHRad, 1.0f / this->mVRad );
}

//----------------------------------------------------------------//
void MOAIFeeler2D::TransformToWorld ( USVec2D& vec ) {

	vec.Scale ( this->mHRad, this->mVRad );
}

