//[MOAI LICENSE]

#include "pch.h"
#include <moaicore/MOAICollisionMgr.h>
#include <moaicore/MOAIMultiShape2D.h>
#include <moaicore/MOAIOverlapPrim2D.h>

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
int MOAIOverlapPrim2D::_setActive ( lua_State* L ) {
	LUA_SETUP ( MOAIOverlapPrim2D, "UB" )
	
	self->mIsActive = state.GetValue < bool >( 2, false );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIOverlapPrim2D::_setCallback ( lua_State* L ) {
	LUA_SETUP ( MOAIOverlapPrim2D, "UF" )
	
	self->mOnOverlap.SetRef ( state, 2, false );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIOverlapPrim2D::_setCacheOverlaps ( lua_State* L ) {
	LUA_SETUP ( MOAIOverlapPrim2D, "UB" )
	
	self->mCacheOverlaps = state.GetValue < bool >( 2, false );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIOverlapPrim2D::_setReceiveFlags ( lua_State* L ) {
	LUA_SETUP ( MOAIOverlapPrim2D, "UN" )
	
	self->mReceiveFlags = state.GetValue < u32 >( 2, false );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIOverlapPrim2D::_setSendFlags ( lua_State* L ) {
	LUA_SETUP ( MOAIOverlapPrim2D, "UN" )
	
	self->mSendFlags = state.GetValue < u32 >( 2, false );
	return 0;
}

//================================================================//
// MOAIOverlapPrim2D
//================================================================//

//----------------------------------------------------------------//
void MOAIOverlapPrim2D::BreakLink ( MOAIOverlapLink2D* link ) {

	MOAICollisionMgr& collisionMgr = MOAICollisionMgr::Get ();

	// find the target prim
	MOAIOverlapPrim2D* target = link->mPrim;
	
	// release the link
	collisionMgr.FreeLink ( link );
	
	// rebuild the target prim's list w/o the link back
	MOAIOverlapLink2D* cursor = target->mOverlaps;
	target->mOverlaps = 0;
	
	while ( cursor ) {
		MOAIOverlapLink2D* temp = cursor;
		cursor = cursor->mNext;
		
		if ( temp->mPrim == this ) {
			collisionMgr.FreeLink ( temp );
			continue;
		}
		
		temp->mNext = target->mOverlaps;
		target->mOverlaps = temp;
	}
	
	// send the notification to any prims with cached overlaps
	
	u32 send = ( this->mSendFlags & target->mReceiveFlags );
	u32 receive = ( this->mReceiveFlags & target->mSendFlags );
		
	if ( send && target->mCacheOverlaps ) {
		target->OnOverlapEnd ( send );
	}
	
	if ( receive && this->mCacheOverlaps ) {
		this->OnOverlapEnd ( receive );
	}
}

//----------------------------------------------------------------//
void MOAIOverlapPrim2D::ClearLinks () {

	MOAIOverlapLink2D* cursor = this->mOverlaps;
	this->mOverlaps = 0;

	if ( MOAICollisionMgr::IsValid ()) {
		while ( cursor ) {
			MOAIOverlapLink2D* temp = cursor;
			cursor = cursor->mNext;
			
			this->BreakLink ( temp );
		}
	}
}

//----------------------------------------------------------------//
void MOAIOverlapPrim2D::GetShape ( MOAIMultiShape2D& shape ) {

	shape.Clear ();
}

//----------------------------------------------------------------//
void MOAIOverlapPrim2D::MakeLink ( MOAIOverlapPrim2D& target, USVec2D& loc ) {

	u32 send = ( this->mSendFlags & target.mReceiveFlags );
	u32 receive = ( this->mReceiveFlags & target.mSendFlags );
	if ( !( send || receive )) return; 
	
	// if either prim wants overlaps we have to cache them for both
	bool cacheOverlaps = ( this->mCacheOverlaps || target.mCacheOverlaps );
	
	bool isNew = true;
	if ( cacheOverlaps ) {
		
		MOAIOverlapLink2D* link = this->mOverlaps;
		for ( ; link; link = link->mNext ) {
			if ( link->mPrim == &target ) {
				isNew = false;
				link->mWeak = false;
			}
		}
	}
	
	// if there's no link OR nobody needs cached overlaps...
	if ( isNew == true ) {
		
		// if anyone wants cached overlaps, hook 'em up
		if ( cacheOverlaps ) {
		
			MOAICollisionMgr& collisionMgr = MOAICollisionMgr::Get ();
			
			MOAIOverlapLink2D* sendLink = collisionMgr.AllocLink ();
			MOAIOverlapLink2D* receiveLink = collisionMgr.AllocLink ();
			
			if (( sendLink && receiveLink )) {
				
				sendLink->mPrim = &target;
				sendLink->mNext = this->mOverlaps;
				sendLink->mWeak = false;
				
				this->mOverlaps = sendLink;
				
				receiveLink->mPrim = this;
				receiveLink->mNext = target.mOverlaps;
				receiveLink->mWeak = false;
				
				target.mOverlaps = receiveLink;
			}
		}
		
		// send the first contact message whether or not overlaps are cached
		
		if ( send ) {
			USVec2D local = loc;
			target.GetWorldToLocalMtx ().Transform ( local );
			target.OnOverlapBegin ( receive, local );
		}
		
		if ( receive ) {
			USVec2D local = loc;
			this->GetWorldToLocalMtx ().Transform ( local );
			this->OnOverlapBegin ( send, local );
		}
	}
	else {
		
		// not new, so send an update message to anyone w/o cached overlaps
		
		if ( send && ( target.mCacheOverlaps == false )) {
			USVec2D local = loc;
			target.GetWorldToLocalMtx ().Transform ( local );
			target.OnOverlapBegin ( receive, local );
		}
		
		if ( receive && ( this->mCacheOverlaps == false )) {
			USVec2D local = loc;
			this->GetWorldToLocalMtx ().Transform ( local );
			this->OnOverlapBegin ( send, local );
		}
	}
}

//----------------------------------------------------------------//
MOAIOverlapPrim2D::MOAIOverlapPrim2D () :
	mNext ( 0 ),
	mOverlapState ( OVERLAP_IDLE ),
	mSendFlags ( SEND_ALL ),
	mReceiveFlags ( RECEIVE_ALL ),
	mIsActive ( true ),
	mCacheOverlaps ( true ),
	mOverlaps ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIPrim )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIOverlapPrim2D::~MOAIOverlapPrim2D () {

	this->ClearLinks ();
}

//----------------------------------------------------------------//
void MOAIOverlapPrim2D::OnOverlapBegin ( u32 overlap, const USVec2D& loc ) {

	if ( this->mOnOverlap ) {
		USLuaStateHandle state = this->mOnOverlap.GetSelf ();
		lua_pushnumber ( state, overlap );
		lua_pushboolean ( state, true );
		lua_pushnumber ( state, loc.mX );
		lua_pushnumber ( state, loc.mY );
		state.DebugCall ( 4, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIOverlapPrim2D::OnOverlapEnd ( u32 overlap ) {

	if ( this->mOnOverlap ) {
		USLuaStateHandle state = this->mOnOverlap.GetSelf ();
		lua_pushnumber ( state, overlap );
		lua_pushboolean ( state, false );
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIOverlapPrim2D::PurgeWeakLinks () {

	MOAIOverlapLink2D* cursor = this->mOverlaps;
	this->mOverlaps = 0;

	while ( cursor ) {
		MOAIOverlapLink2D* temp = cursor;
		cursor = cursor->mNext;
		
		if ( temp->mWeak ) {
			this->BreakLink ( temp );
			continue;
		}
		
		temp->mNext = this->mOverlaps;
		this->mOverlaps = temp;
	}
}

//----------------------------------------------------------------//
void MOAIOverlapPrim2D::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIOverlapPrim2D::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "setActive",				_setActive },
		{ "setCallback",			_setCallback },
		{ "setCacheOverlaps",		_setCacheOverlaps },
		{ "setReceiveFlags",		_setReceiveFlags },
		{ "setSendFlags",			_setSendFlags },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIOverlapPrim2D::ToString () {

	STLString repr( MOAIPrim::ToString () );

	//PRETTY_PRINT ( repr, mUVTransform )

	//if ( mGfxSource ) {

	//	PRETTY_PRINT ( repr, mGfxSource )
	//	PRETTY_PRINT ( repr, mContentAddr )
	//}

	return repr;
}

//----------------------------------------------------------------//
void MOAIOverlapPrim2D::WeakenLinks () {

	MOAIOverlapLink2D* link = this->mOverlaps;
	for ( ; link; link = link->mNext ) {
	
		// if the link exists and the prim has already been processed this frame,
		// then it can't be weak
		if ( link->mPrim->mOverlapState == OVERLAP_DONE ) continue;
		
		// weaken link
		link->mWeak = true;
	}
}
