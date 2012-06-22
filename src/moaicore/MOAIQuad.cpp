//[MOAI LICENSE]

#include "pch.h"
#include <moaicore/MOAICollisionMgr.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIMultiShape2D.h>
#include <moaicore/MOAIQuad.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>inside (mX, mY)</tt>\n
	\n
	Returns true if a point is inside of this sprite.
	@param mX The X coordinate of the point to be checked.
	@param mY The Y coordinate of the point to be checked.
	@return True if the point is inside, otherwise false.
*/
int	MOAIQuad::_inside ( lua_State* L ) {
	LUA_SETUP ( MOAIQuad, "UNN" )

	USVec2D vec;
	vec.mX	= state.GetValue < float >( 2, 0.0f );
	vec.mY	= state.GetValue < float >( 3, 0.0f );
	
	bool result = self->Inside ( vec );
	lua_pushboolean ( state, result );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int	MOAIQuad::_setQuad ( lua_State* L ) {
	LUA_SETUP ( MOAIQuad, "UNNNNNNNN" )

	self->mQuad.mV [ 0 ].mX	= state.GetValue < float >( 2, 0.0f );
	self->mQuad.mV [ 0 ].mY	= state.GetValue < float >( 3, 0.0f );
	
	self->mQuad.mV [ 1 ].mX	= state.GetValue < float >( 4, 0.0f );
	self->mQuad.mV [ 1 ].mY	= state.GetValue < float >( 5, 0.0f );
	
	self->mQuad.mV [ 2 ].mX	= state.GetValue < float >( 6, 0.0f );
	self->mQuad.mV [ 2 ].mY	= state.GetValue < float >( 7, 0.0f );
	
	self->mQuad.mV [ 3 ].mX	= state.GetValue < float >( 8, 0.0f );
	self->mQuad.mV [ 3 ].mY	= state.GetValue < float >( 9, 0.0f ); 
	
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
int MOAIQuad::_setRect ( lua_State* L ) {
	LUA_SETUP ( MOAIQuad, "UNNNN" )

	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float x1	= state.GetValue < float >( 4, 0.0f );
	float y1	= state.GetValue < float >( 5, 0.0f );
	
	self->mQuad.Init ( x0, y0, x1, y1 );

	self->ScheduleUpdate ();

	return 0;
}

//================================================================//
// MOAIQuad
//================================================================//

//----------------------------------------------------------------//
void MOAIQuad::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	MOAITransform2D::ApplyAttrOp ( attrID, attrOp );
}

//----------------------------------------------------------------//
u32 MOAIQuad::CountAttributes () {

	return MOAIQuad::TOTAL_ATTR;
}

//----------------------------------------------------------------//
void MOAIQuad::DrawDebug () {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	if ( debugLines.Bind ( MOAIDebugLines::COLLISION_PRIMS )) {
	
		debugLines.SetWorldMtx ( this->GetLocalToWorldMtx ());
		debugLines.SetPenSpace ( USDebugLines::MODEL_SPACE );
		debugLines.DrawQuad ( this->mQuad );
	}
}

//----------------------------------------------------------------//
u32 MOAIQuad::GetLocalFrame ( USRect& frame ) {

	this->mQuad.GetBounds ( frame );
	return FRAME_OK;
}

//----------------------------------------------------------------//
void MOAIQuad::GetShape ( MOAIMultiShape2D& shape ) {

	USQuad quad = this->mQuad;
	const USMatrix2D& matrix = this->GetLocalToWorldMtx ();
	quad.Transform ( matrix );

	shape.SetQuad ( quad );
}

//----------------------------------------------------------------//
bool MOAIQuad::Inside ( USVec2D vec ) {
	UNUSED ( vec );

	//if ( !this->mGfxSource ) return false;
	//
	//const USMatrix2D& worldToLocal = this->GetWorldToLocalMtx ();
	//worldToLocal.Transform ( vec );
	//
	//return this->mGfxSource->Contains ( this->mContentAddr, vec );
	
	return false;
}

//----------------------------------------------------------------//
MOAIOverlapPrim2D* MOAIQuad::IsOverlapPrim2D () {
	return this;
}

//----------------------------------------------------------------//
MOAIQuad::MOAIQuad () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIOverlapPrim2D )
	RTTI_END
	
	//this->SetQueryMask ( MOAIPrim::CAN_GET_OVERLAP_PRIM | MOAIPrim::CAN_DRAW_DEBUG );
}

//----------------------------------------------------------------//
MOAIQuad::~MOAIQuad () {
}

//----------------------------------------------------------------//
void MOAIQuad::OnDepNodeUpdate () {

	MOAIPrim::OnDepNodeUpdate ();
	MOAICollisionMgr::Get ().ScheduleOverlap ( *this );
}

//----------------------------------------------------------------//
void MOAIQuad::RegisterLuaClass ( USLuaState& state ) {
	
	MOAIPrim::RegisterLuaClass ( state );
	MOAIOverlapPrim2D::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIQuad::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIPrim::RegisterLuaFuncs ( state );
	MOAIOverlapPrim2D::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "inside",					_inside },
		{ "setQuad",				_setQuad },
		{ "setRect",				_setRect },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIQuad::ToString () {

	STLString repr( MOAIPrim::ToString () );

	//PRETTY_PRINT ( repr, mUVTransform )

	//if ( mGfxSource ) {

	//	PRETTY_PRINT ( repr, mGfxSource )
	//	PRETTY_PRINT ( repr, mContentAddr )
	//}

	return repr;
}

