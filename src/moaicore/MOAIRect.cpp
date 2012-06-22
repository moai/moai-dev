//[MOAI LICENSE]

#include "pch.h"
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIMultiShape2D.h>
#include <moaicore/MOAIRect.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int	MOAIRect::_inside ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UNN" )) return 0;
	
	MOAIRect* self = state.GetLuaData < MOAIRect >( 1 );
	if ( !self ) return 0;

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
int MOAIRect::_setRect( lua_State* L ) {
	
	USLuaState state ( L );
	if ( !state.CheckParams( 1, "UNNNN") ) return 0;
	
	MOAIRect* self = state.GetLuaData < MOAIRect >( 1 );
	if ( !self ) return 0;
	
	float left = state.GetValue < float >( 2, 0.0f );
	float top = state.GetValue < float >( 3, 0.0f );
	float bottom = state.GetValue < float >( 4, 0.0f );
	float right = state.GetValue < float >( 5, 0.0f );

	self->mRect.Init ( left, top, bottom, right );
	
	return 0;
}

//================================================================//
// MOAIRect
//================================================================//

//----------------------------------------------------------------//
void MOAIRect::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	MOAITransform2D::ApplyAttrOp ( attrID, attrOp );
}

//----------------------------------------------------------------//
u32 MOAIRect::CountAttributes () {

	return MOAIRect::TOTAL_ATTR;
}

//----------------------------------------------------------------//
void MOAIRect::DrawDebug () {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	if ( debugLines.Bind ( MOAIDebugLines::COLLISION_PRIMS )) {
	
		debugLines.SetWorldMtx ( this->GetLocalToWorldMtx ());
		debugLines.SetPenSpace ( USDebugLines::MODEL_SPACE );
		debugLines.DrawRect ( this->mRect );
	}
}

//----------------------------------------------------------------//
u32 MOAIRect::GetLocalFrame ( USRect& frame ) {

	frame = this->mRect;
	return FRAME_OK;
}

//----------------------------------------------------------------//
void MOAIRect::GetShape ( MOAIMultiShape2D& shape ) {

	USRect rect = this->GetWorldBounds ();
	shape.SetRect ( rect );
}

//----------------------------------------------------------------//
bool MOAIRect::Inside ( USVec2D vec ) {

	const USMatrix2D& worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( vec );
	
	return this->mRect.Contains ( vec );
}

//----------------------------------------------------------------//
MOAIOverlapPrim2D* MOAIRect::IsOverlapPrim2D () {
	return this;
}

//----------------------------------------------------------------//
MOAIRect::MOAIRect () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIOverlapPrim2D )
	RTTI_END
	
	//this->SetQueryMask ( MOAIPrim::CAN_GET_OVERLAP_PRIM | MOAIPrim::CAN_DRAW_DEBUG );
}

//----------------------------------------------------------------//
MOAIRect::~MOAIRect () {
}

//----------------------------------------------------------------//
void MOAIRect::RegisterLuaClass ( USLuaState& state ) {

	MOAIPrim::RegisterLuaClass ( state );
	MOAIOverlapPrim2D::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIRect::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIPrim::RegisterLuaFuncs ( state );
	MOAIOverlapPrim2D::RegisterLuaFuncs ( state );
	
	LuaReg regTable[] = {
		{ "inside",					_inside },
		{ "setRect",				_setRect },
		{ NULL, NULL }
	};
	
	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIRect::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mRect )

	return repr;
}

