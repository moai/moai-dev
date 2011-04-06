// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <chipmunk/chipmunk.h>
#include <moaicore/MOAICpArbiter.h>
#include <moaicore/MOAICpBody.h>
#include <moaicore/MOAICpConstraint.h>
#include <moaicore/MOAICpDebugDraw.h>
#include <moaicore/MOAICpShape.h>
#include <moaicore/MOAICpSpace.h>
#include <moaicore/MOAILogMessages.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_CHIPMUNK

//================================================================//
// MOAICpPrim
//================================================================//
class MOAICpCollisionHandler  {
public:

	friend class MOAICpSpace;

	cpCollisionType		mTypeA;
	cpCollisionType		mTypeB;
	USLuaRef			mHandler;
	u32					mMask;
	
	MOAICpSpace* mSpace;
	MOAICpCollisionHandler* mNext;
};

//================================================================//
// MOAICpPrim
//================================================================//

//----------------------------------------------------------------//
void MOAICpPrim::DoRemove () {
}

//----------------------------------------------------------------//
MOAICpPrim::MOAICpPrim () :
	mSpace ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaObject )
	RTTI_END
	
	this->mLinkInSpace.Data ( this );
}

//----------------------------------------------------------------//
MOAICpPrim::~MOAICpPrim () {

	this->Remove ();
}

//----------------------------------------------------------------//
void MOAICpPrim::Remove () {

	if ( this->mSpace ) {
		this->mSpace->RemovePrim ( *this );
	}
}

//================================================================//
// collision handlers
//================================================================//

//----------------------------------------------------------------//
static int _cpCollisionFunc ( cpArbiter* arb, void* data, u32 eventType, bool checkResult ) {

	MOAICpCollisionHandler* handler = ( MOAICpCollisionHandler* )data;
	if ( handler->mMask & eventType ) {

		// this can be called during shutdown, so make sure the runtime is still valid
		if ( USLuaRuntime::IsValid ()) {

			USLuaStateHandle state = USLuaRuntime::Get ().State ();
			if ( handler->mHandler.PushRef ( state )) {
				
				cpShape* a;
				cpShape* b;
				cpArbiterGetShapes ( arb, &a, &b );
				
				if ( a && b ) {
					
					state.Push ( eventType );
					(( MOAICpShape* )a->data )->PushLuaUserdata ( state );
					(( MOAICpShape* )b->data )->PushLuaUserdata ( state );
					
					MOAICpArbiter* arbiter = handler->mSpace->GetArbiter ();
					arbiter->SetArbiter ( arb );
					arbiter->PushLuaUserdata ( state );
					
					state.DebugCall ( 4, 1 );
					
					//lua_call ( state, 4, 1 );
					
					if ( checkResult ) {
						bool result = state.GetValue < bool >( -1, true );
						return result ? cpTrue : cpFalse;
					}
				}
			}
		}
	}
	return cpTrue;
}

//----------------------------------------------------------------//
static int _cpCollisionBeginFunc ( cpArbiter* arb, cpSpace* space, void *data ) {
	UNUSED ( space );
	
	if ( cpArbiterIsFirstContact ( arb )) {
		return _cpCollisionFunc ( arb, data, MOAICpSpace::BEGIN, true );
	}
	return true;
}

//----------------------------------------------------------------//
static int _cpCollisionPreSolveFunc ( cpArbiter* arb, cpSpace* space, void *data ) {
	UNUSED ( space );
	return _cpCollisionFunc ( arb, data, MOAICpSpace::PRE_SOLVE, true );
}

//----------------------------------------------------------------//
static void _cpCollisionPostSolveFunc ( cpArbiter* arb, cpSpace* space, void *data ) {
	UNUSED ( space );
	_cpCollisionFunc ( arb, data, MOAICpSpace::POST_SOLVE, false );
}

//----------------------------------------------------------------//
static void _cpCollisionSeparateFunc ( cpArbiter* arb, cpSpace* space, void *data ) {
	UNUSED ( space );
	_cpCollisionFunc ( arb, data, MOAICpSpace::SEPARATE, false );
}

//================================================================//
// query callbacks
//================================================================//

//----------------------------------------------------------------//
static void _shapeListForPointCallback ( cpShape *shape, void *data ) {

	USLuaState& state = *( USLuaState* )data;
	(( MOAICpShape* )shape->data )->PushLuaUserdata ( state );
}

//----------------------------------------------------------------//
static void _shapeListForRectCallback ( cpShape *shape, void *data ) {

	USLuaState& state = *( USLuaState* )data;
	(( MOAICpShape* )shape->data )->PushLuaUserdata ( state );
}

//----------------------------------------------------------------//
static void _shapeListForSegmentCallback ( cpShape *shape, cpFloat t, cpVect n, void *data ) {

	USLuaState& state = *( USLuaState* )data;
	(( MOAICpShape* )shape->data )->PushLuaUserdata ( state );
	
	lua_pushnumber ( state, t );
	lua_pushnumber ( state, n.x );
	lua_pushnumber ( state, n.y );
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name activateShapesTouchingShape
	@param1 self @type userdata
	@param2 other @type userdata
	@text Activates shapes that are currently touching the specified shape.
	@return nil
*/
int MOAICpSpace::_activateShapesTouchingShape ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UU" )
	
	MOAICpShape* shape = state.GetLuaObject < MOAICpShape >( 2 );
	if ( shape && shape->mShape ) {
		cpSpaceActivateShapesTouchingShape ( self->mSpace, shape->mShape );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name getDamping
	@param1 self @type userdata
	@text Returns the current damping in the space.
	@return The damping. @type number
*/
int MOAICpSpace::_getDamping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "U" )
	
	lua_pushnumber ( L, self->mSpace->damping );
	return 1;
}

//----------------------------------------------------------------//
/**	@name getGravity
	@param1 self @type userdata
	@text Returns the current gravity as two return values (x grav, y grav).
	@return The gravity in the X and Y dimensions. @type (number,number)
*/
int MOAICpSpace::_getGravity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "U" )
	
	lua_pushnumber ( L, self->mSpace->gravity.x );
	lua_pushnumber ( L, self->mSpace->gravity.y );
	return 1;
}

//----------------------------------------------------------------//
/**	@name getIdleSpeedThreshold
	@param1 self @type userdata
	@text Returns the speed threshold which indicates whether a body is idle (less than or equal to threshold) or in motion (greater than threshold).
	@return The idle speed threshold. @type number
*/
int MOAICpSpace::_getIdleSpeedThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "U" )
	
	lua_pushnumber ( L, self->mSpace->idleSpeedThreshold );
	return 1;
}

//----------------------------------------------------------------//
/**	@name getIterations
	@param1 self @type userdata
	@text Description of method coming soon.
	@return The iterations. @type number
*/
int MOAICpSpace::_getIterations ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "U" )
	
	lua_pushnumber ( L, self->mSpace->iterations );
	return 1;
}

//----------------------------------------------------------------//
/**	@name getSleepTimeThreshold
	@param1 self @type userdata
	@text Description of method coming soon.
	@return The iterations. @type number
*/
int MOAICpSpace::_getSleepTimeThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "U" )
	
	lua_pushnumber ( L, self->mSpace->sleepTimeThreshold );
	return 1;
}

//----------------------------------------------------------------//
/**	@name getStaticBody
	@param1 self @type userdata
	@text Returns the static body representing this space.
	@return The static body. @type number
*/
int MOAICpSpace::_getStaticBody ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "U" )
	
	if ( !self->mStaticBody ) {
		self->mStaticBody = new MOAICpBody ();
		self->mStaticBody->mBody = &self->mSpace->staticBody;
		self->mStaticBody->mBody->data = self->mStaticBody;
	}
	
	self->mStaticBody->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name insertProp
	@param1 self @type userdata
	@text Inserts a new prop into the world (can be used as a body, joint, etc.)
	@return The new prop. @type number
*/
int MOAICpSpace::_insertProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UU" )
	
	MOAICpPrim* prim = state.GetLuaObject < MOAICpPrim >( 2 );
	if ( prim ) {
		self->InsertPrim ( *prim );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name rehashShape
	@param1 self @type userdata
	@param1 other @type userdata
	@text Description of method coming soon.
	@return nil
*/
int MOAICpSpace::_rehashShape ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UU" )
	
	MOAICpShape* shape = state.GetLuaObject < MOAICpShape >( 2 );
	if ( shape && shape->mShape ) {
		cpSpaceRehashShape ( self->mSpace, shape->mShape );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name rehashStatic
	@param1 self @type userdata
	@text Description of method coming soon.
	@return nil
*/
int MOAICpSpace::_rehashStatic ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "U" )
	
	cpSpaceRehashStatic ( self->mSpace );
	return 0;
}

//----------------------------------------------------------------//
/**	@name removeProp
	@param1 self @type userdata
	@param2 prop @type userdata
	@text Removes a prop (body, joint, etc.) from the space.
	@return nil
*/
int MOAICpSpace::_removeProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UU" )
	
	MOAICpPrim* prim = state.GetLuaObject < MOAICpPrim >( 2 );
	if ( prim ) {
		self->RemovePrim ( *prim );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name resizeActiveHash
	@param1 self @type userdata
	@param2 dim @type number
	@param3 count @type number
	@text Description of method coming soon.
	@return nil
*/
int MOAICpSpace::_resizeActiveHash ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UNN" )
	
	cpFloat dim = state.GetValue < cpFloat >( 2, 0 );
	int count = state.GetValue < int >( 3, 0 );
	cpSpaceResizeActiveHash ( self->mSpace, dim, count );

	return 0;
}

//----------------------------------------------------------------//
/**	@name resizeStaticHash
	@param1 self @type userdata
	@param2 dim @type number
	@param3 count @type number
	@text Description of method coming soon.
	@return nil
*/
int MOAICpSpace::_resizeStaticHash ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UNN" )
	
	cpFloat dim = state.GetValue < cpFloat >( 2, 0 );
	int count = state.GetValue < int >( 3, 0 );
	cpSpaceResizeStaticHash ( self->mSpace, dim, count );

	return 0;
}

//----------------------------------------------------------------//
/**	@name setCollisionHandler
	@param1 self @type userdata
	@param2 type @type number
	@param3 subtype @type number
	@param4 handler @type function|nil
	@text Sets a function to handle the specific collision type on this object.  If nil is passed as the handler, the collision handler is unset.
	@return nil
*/
int MOAICpSpace::_setCollisionHandler ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UNN" )
	
	cpCollisionType typeA = state.GetValue < cpCollisionType >( 2, 0 );
	cpCollisionType typeB = state.GetValue < cpCollisionType >( 3, 0 );
	
	if ( typeB < typeA ) {
		cpCollisionType temp = typeA;
		typeA = typeB;
		typeB = temp;
	}
	
	if ( state.IsType ( 5, LUA_TFUNCTION )) {
		
		MOAICpCollisionHandler* handler = self->mCollisionHandlers;
		for ( ; handler; handler = handler->mNext ) {
			if (( handler->mTypeA == typeA ) && ( handler->mTypeB == typeB )) break;
		}
		
		if ( !handler ) {
			
			handler = new MOAICpCollisionHandler ();
			handler->mTypeA = typeA;
			handler->mTypeB = typeB;
			handler->mSpace = self;
			
			cpSpaceAddCollisionHandler (
				self->mSpace,
				typeA,
				typeB,
				_cpCollisionBeginFunc,
				_cpCollisionPreSolveFunc,
				_cpCollisionPostSolveFunc,
				_cpCollisionSeparateFunc,
				handler
			);
			
			handler->mNext = self->mCollisionHandlers;
			self->mCollisionHandlers = handler;
		}
		
		handler->mMask = state.GetValue < u32 >( 4, ALL );
		handler->mHandler.SetRef ( state, 5, false );
	}
	else {
	
		MOAICpCollisionHandler* handler = self->mCollisionHandlers;
		self->mCollisionHandlers = 0;
		
		while ( handler ) {
			MOAICpCollisionHandler* temp = handler;
			handler = handler->mNext;
		
			if (( temp->mTypeA == typeA ) && ( temp->mTypeB == typeB )) {
				cpSpaceRemoveCollisionHandler ( self->mSpace, typeA, typeB );
				delete temp;
				continue;
			}
			temp->mNext = self->mCollisionHandlers;
			self->mCollisionHandlers = temp;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name setDamping
	@param1 self @type userdata
	@param2 damping @type number
	@text Sets the current damping in the space.
	@return nil
*/
int MOAICpSpace::_setDamping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UN" )
	
	self->mSpace->damping = state.GetValue < cpFloat >( 2, 0 );
	return 0;
}

//----------------------------------------------------------------//
/**	@name setGravity
	@param1 self @type userdata
	@param2 xgrav @type number
	@param3 ygrav @type number
	@text Sets the current gravity in the space.
	@return nil
*/
int MOAICpSpace::_setGravity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UNN" )
	
	self->mSpace->gravity.x = state.GetValue < cpFloat >( 2, 0 );
	self->mSpace->gravity.y = state.GetValue < cpFloat >( 3, 0 );

	return 0;
}

//----------------------------------------------------------------//
/**	@name setIdleSpeedThreshold
	@param1 self @type userdata
	@param2 threshold @type number @text The idle speed threshold.
	@text Sets the speed threshold which indicates whether a body is idle (less than or equal to threshold) or in motion (greater than threshold).
	@return nil
*/
int MOAICpSpace::_setIdleSpeedThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UN" )
	
	self->mSpace->idleSpeedThreshold = state.GetValue < cpFloat >( 2, 0 );
	return 0;
}

//----------------------------------------------------------------//
/**	@name setIterations
	@param1 self @type userdata
	@param2 iterations @type number
	@text Description of method coming soon.
	@return nil
*/
int MOAICpSpace::_setIterations ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UN" )
	
	self->mSpace->iterations = state.GetValue < int >( 2, 0 );
	return 0;
}

//----------------------------------------------------------------//
/**	@name setSleepTimeThreshold
	@param1 self @type userdata
	@param2 threshold @type number
	@text Description of method coming soon.
	@return nil
*/
int MOAICpSpace::_setSleepTimeThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UN" )
	
	self->mSpace->sleepTimeThreshold = state.GetValue < cpFloat >( 2, 0 );
	return 0;
}

//----------------------------------------------------------------//
/**	@name shapeForPoint
	@param1 self @type userdata
	@param2 x @type number @text The X position to check.
	@param3 y @type number @text The Y position to check.
	@param4 layers @type number|nil @text The layers to check (nil for all).
	@param5 group @type number|nil @text The groups to check (nil for all).
	@text Retrieves a shape located at the specified X and Y position, that exists on the specified layer (or any layer if nil) and is part of the specified group (or any group if nil).
	@return The shape at the specified position, layer and within the specified group.  Returns nil if there is no shape that matches. @type userdata|nil
*/
int MOAICpSpace::_shapeForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UNN" )
	
	cpVect point;
	point.x = state.GetValue < cpFloat >( 2, 0 );
	point.y = state.GetValue < cpFloat >( 3, 0 );
	
	cpLayers layers = state.GetValue < cpLayers >( 4, CP_ALL_LAYERS );
	cpLayers group = state.GetValue < cpGroup >( 5, CP_NO_GROUP );
	
	cpShape* shape = cpSpacePointQueryFirst ( self->mSpace, point, layers, group );
	
	if ( shape ) {
		(( MOAICpShape* )shape->data )->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name shapeForSegment
	@param1 self @type userdata
	@param2 x1 @type number @text The start-point X position to check.
	@param3 y1 @type number @text The start-point Y position to check.
	@param3 x2 @type number @text The end-point X position to check.
	@param4 y2 @type number @text The end-point Y position to check.
	@param5 layers @type number|nil @text The layers to check (nil for all).
	@param6 group @type number|nil @text The groups to check (nil for all).
	@text Retrieves a shape that crosses the segment specified, that exists on the specified layer (or any layer if nil) and is part of the specified group (or any group if nil).
	@return The shape within the specified rectangle, at the specified layer and within the specified group.  Returns nil if there is no shape that matches. @type (userdata,t,x,y)|nil
*/
int MOAICpSpace::_shapeForSegment ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UNNNN" )
	
	cpVect start;
	start.x = state.GetValue < cpFloat >( 2, 0 );
	start.y = state.GetValue < cpFloat >( 3, 0 );
	
	cpVect end;
	end.x = state.GetValue < cpFloat >( 4, 0 );
	end.y = state.GetValue < cpFloat >( 5, 0 );
	
	cpLayers layers = state.GetValue < cpLayers >( 6, CP_ALL_LAYERS );
	cpLayers group = state.GetValue < cpGroup >( 7, CP_NO_GROUP );
	
	cpSegmentQueryInfo info;
	
	cpShape* shape = cpSpaceSegmentQueryFirst (
		self->mSpace,
		start,
		end,
		layers,
		group,
		&info
	);
	
	if ( shape ) {
		(( MOAICpShape* )shape->data )->PushLuaUserdata ( state );
		lua_pushnumber ( state, info.t );
		lua_pushnumber ( state, info.n.x );
		lua_pushnumber ( state, info.n.y );
		return 4;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name shapeListForPoint
	@param1 self @type userdata
	@param2 x @type number @text The X position to check.
	@param3 y @type number @text The Y position to check.
	@param4 layers @type number|nil @text The layers to check (nil for all).
	@param5 group @type number|nil @text The groups to check (nil for all).
	@text Operates the same as shapeForPoint, except that it returns all shapes matched as return values rather than just the first match.
	@return The shapes that were matched as multiple return values.  If 3 shapes match, there will be 3 return values.  If 0 shapes match, there will be 0 return values (effectively nil). @type (userdata,...)|nil
*/
int MOAICpSpace::_shapeListForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UNN" )
	
	cpVect point;
	point.x = state.GetValue < cpFloat >( 2, 0 );
	point.y = state.GetValue < cpFloat >( 3, 0 );
	
	cpLayers layers = state.GetValue < cpLayers >( 4, CP_ALL_LAYERS );
	cpLayers group = state.GetValue < cpGroup >( 5, CP_NO_GROUP );
	
	int base = state.GetTop ();
	
	cpSpacePointQuery (
		self->mSpace,
		point,
		layers,
		group,
		_shapeListForPointCallback,
		&state
	);
	
	int results = state.GetTop () - base;
	return results;
}

//----------------------------------------------------------------//
/**	@name shapeListForRect
	@param1 self @type userdata
	@param2 x1 @type userdata @text The top-left X position to check.
	@param3 y1 @type number @text The top-left Y position to check.
	@param3 x2 @type number @text The bottom-right X position to check.
	@param4 y2 @type number @text The bottom-right Y position to check.
	@param5 layers @type number|nil @text The layers to check (nil for all).
	@param6 group @type number|nil @text The groups to check (nil for all).
	@text Operates the same as shapeForSegment, except that it returns all shapes matched as return values rather than just the first match.
	@return The shapes that were matched as multiple return values.  If 3 shapes match, there will be 3 return values.  If 0 shapes match, there will be 0 return values (effectively nil). @type (userdata,...)|nil
*/
int MOAICpSpace::_shapeListForRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UNNNN" )
	
	USMetaRect < cpFloat > rect = state.GetRect < cpFloat >( 2 ); 
	rect.Bless ();
	
	cpBB bb;
	bb.l = rect.mXMin;
	bb.b = rect.mYMin;
	bb.r = rect.mXMax;
	bb.t = rect.mYMax;
	
	cpLayers layers = state.GetValue < cpLayers >( 6, CP_ALL_LAYERS );
	cpLayers group = state.GetValue < cpGroup >( 7, CP_NO_GROUP );
	
	int base = state.GetTop ();
	
	cpSpaceBBQuery (
		self->mSpace,
		bb,
		layers,
		group,
		_shapeListForRectCallback,
		&state
	);
	
	int results = state.GetTop () - base;
	return results;
}

//----------------------------------------------------------------//
/**	@name shapeListForSegment
	@param1 self @type userdata
	@param2 x1 @type number @text The start-point X position to check.
	@param3 y1 @type number @text The start-point Y position to check.
	@param3 x2 @type number @text The end-point X position to check.
	@param4 y2 @type number @text The end-point Y position to check.
	@param5 layers @type number|nil @text The layers to check (nil for all).
	@param6 group @type number|nil @text The groups to check (nil for all).
	@text Operates the same as shapeForSegment, except that it returns all shapes matched as return values rather than just the first match.
	@return The shapes that were matched as multiple return values.  If 3 shapes match, there will be 3 return values.  If 0 shapes match, there will be 0 return values (effectively nil). @type (userdata,...)|nil
*/
int MOAICpSpace::_shapeListForSegment ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpSpace, "UNNNN" )
	
	cpVect start;
	start.x = state.GetValue < cpFloat >( 2, 0 );
	start.y = state.GetValue < cpFloat >( 3, 0 );
	
	cpVect end;
	end.x = state.GetValue < cpFloat >( 4, 0 );
	end.y = state.GetValue < cpFloat >( 5, 0 );
	
	cpLayers layers = state.GetValue < cpLayers >( 6, CP_ALL_LAYERS );
	cpLayers group = state.GetValue < cpGroup >( 7, CP_NO_GROUP );
		
	int base = state.GetTop ();
	
	cpSpaceSegmentQuery (
		self->mSpace,
		start,
		end,
		layers,
		group,
		_shapeListForSegmentCallback,
		&state
	);
	
	int results = state.GetTop () - base;
	return results;
}

//================================================================//
// MOAICpSpace
//================================================================//

//----------------------------------------------------------------//
void MOAICpSpace::DrawDebug () {

	if ( this->mSpace ) {

		MOAICpDebugDrawOptions options;

		options.drawHash = 0;
		options.drawBBs = 0;
		options.drawShapes = 1;
		options.collisionPointSize = 4.0f;
		options.bodyPointSize = 0.0f;
		options.lineThickness = 1.5f;

		USCanvas::SetTexture ();

		glEnableClientState ( GL_VERTEX_ARRAY );
		MOAICpDebugDraw::DrawSpace ( this->mSpace, &options );
	}
}

//----------------------------------------------------------------//
MOAICpArbiter* MOAICpSpace::GetArbiter () {

	if ( !this->mArbiter ) {
		this->mArbiter = new MOAICpArbiter ();
	}
	return this->mArbiter;
}

//----------------------------------------------------------------//
void MOAICpSpace::InsertPrim ( MOAICpPrim& prim ) {

	if ( prim.mSpace == this ) return;
	
	prim.Retain ();

	if ( prim.mSpace ) {
		prim.mSpace->RemovePrim ( prim );
	}

	prim.CpAddToSpace ( this->mSpace );
	this->mPrims.PushBack ( prim.mLinkInSpace );
	prim.mSpace = this;
}

//----------------------------------------------------------------//
bool MOAICpSpace::IsBusy () {
	return this->IsActive ();
}

//----------------------------------------------------------------//
MOAICpSpace::MOAICpSpace () :
	mSpace ( 0 ),
	mCollisionHandlers ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
	
	this->mSpace = cpSpaceNew ();
}

//----------------------------------------------------------------//
MOAICpSpace::~MOAICpSpace () {

	PrimIt primIt;
	
	for ( primIt = this->mPrims.Head (); primIt; primIt = primIt->Next ()) {
		MOAICpPrim* prim = primIt->Data ();
		prim->CpRemoveFromSpace ( this->mSpace );
		prim->mSpace = 0;
	}

	primIt = this->mPrims.Head ();
	while ( primIt ) {
		MOAICpPrim* prim = primIt->Data ();
		primIt = primIt->Next ();
		prim->Release ();
	}

	this->mPrims.Clear ();

	if ( this->mStaticBody ) {
		this->mStaticBody->ClearShapes ();
		this->mStaticBody->mBody = 0;
	}

	// DO THIS LAST! cpShape hangs on to refs to its handler and will
	// attempt to call it as it is being release; if the handlers are
	// removed/deleted first, the app will crash
	while ( this->mCollisionHandlers ) {
	
		MOAICpCollisionHandler* handler = this->mCollisionHandlers;
		this->mCollisionHandlers = this->mCollisionHandlers->mNext;
		
		cpSpaceRemoveCollisionHandler ( this->mSpace, handler->mTypeA, handler->mTypeB );
		delete handler;
	}

	cpSpaceFree ( this->mSpace );
}

//----------------------------------------------------------------//
void MOAICpSpace::OnUpdate ( float step ) {
	
	USLeanList < MOAICpPrim* > removeList;
	
	if ( this->mSpace ) {
		
		cpSpaceStep ( this->mSpace, step );
		
		cpBody** bodies = ( cpBody** )this->mSpace->bodies->arr;
		int num = this->mSpace->bodies->num;
		
		for ( int i = 0; i < num; ++i ) {
			MOAICpBody* body = ( MOAICpBody* )bodies [ i ]->data;
			body->ScheduleUpdate ();
			
			if ( body->mRemoveFlag != MOAICpBody::NONE ) {
				body->mLinkInSpace.Remove ();
				removeList.PushBack ( body->mLinkInSpace );
			}
		}
		
		while ( removeList.Count ()) {
			MOAICpPrim* prim = removeList.Front ();
			removeList.PopFront ();
			prim->DoRemove ();
		}
	}
}

//----------------------------------------------------------------//
void MOAICpSpace::RegisterLuaClass ( USLuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
	
	state.SetField ( -1, "BEGIN", ( u32 )BEGIN );
	state.SetField ( -1, "PRE_SOLVE", ( u32 )PRE_SOLVE );
	state.SetField ( -1, "POST_SOLVE", ( u32 )POST_SOLVE );
	state.SetField ( -1, "SEPARATE", ( u32 )SEPARATE );
	state.SetField ( -1, "ALL", ( u32 )ALL );
}

//----------------------------------------------------------------//
void MOAICpSpace::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "activateShapesTouchingShape",	_activateShapesTouchingShape },
		{ "getDamping",						_getDamping },
		{ "getGravity",						_getGravity },
		{ "getIdleSpeedThreshold",			_getIdleSpeedThreshold },
		{ "getIterations",					_getIterations },
		{ "getSleepTimeThreshold",			_getSleepTimeThreshold },
		{ "getStaticBody",					_getStaticBody },
		{ "insertPrim",						_insertProp },
		{ "rehashShape",					_rehashShape },
		{ "rehashStatic",					_rehashStatic },
		{ "removePrim",						_removeProp },
		{ "resizeActiveHash",				_resizeActiveHash },
		{ "resizeStaticHash",				_resizeStaticHash },
		{ "setCollisionHandler",			_setCollisionHandler },
		{ "setDamping",						_setDamping },
		{ "setGravity",						_setGravity },
		{ "setIdleSpeedThreshold",			_setIdleSpeedThreshold },
		{ "setIterations",					_setIterations },
		{ "setSleepTimeThreshold",			_setSleepTimeThreshold },
		{ "shapeForPoint",					_shapeForPoint },
		{ "shapeForSegment",				_shapeForSegment },
		{ "shapeListForPoint",				_shapeListForPoint },
		{ "shapeListForRect",				_shapeListForRect },
		{ "shapeListForSegment",			_shapeListForSegment },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICpSpace::RemovePrim ( MOAICpPrim& prim ) {

	if ( prim.mSpace != this ) return;
	
	prim.CpRemoveFromSpace ( this->mSpace );
	this->mPrims.Remove ( prim.mLinkInSpace );
	
	prim.mSpace = 0;
	prim.Release ();
}

//----------------------------------------------------------------//
STLString MOAICpSpace::ToString () {

	STLString repr;
	return repr;
}

#endif