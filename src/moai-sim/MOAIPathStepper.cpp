// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPathStepper.h>
#include <moai-sim/MOAIPathStepper.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: path
int MOAIPathStepper::_getLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathStepper, "U" );
	
	state.Push ( self->GetLength ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: path
int MOAIPathStepper::_more ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathStepper, "U" );
	
	state.Push ( self->More ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: path
int MOAIPathStepper::_next ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathStepper, "U" );
	
	state.Push ( self->Next ());
	return 2;
}

//----------------------------------------------------------------//
// TODO: path
int MOAIPathStepper::_start ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathStepper, "U" );
	
	MOAIPath* path		= state.GetLuaObject < MOAIPath >( 2, true );
	float stepSize		= state.GetValue < float >( 3, 1.0f );
	
	self->Start ( *path, stepSize );
	
	return 0;
}

//================================================================//
// MOAIPathStepper
//================================================================//

//----------------------------------------------------------------//
MOAIPathStepper::MOAIPathStepper () :
	mStepCount ( 0 ),
	mTotalSteps ( 0 ),
	mStepSize ( 0.0f ),
	mEdgeBase ( 0.0f ),
	mEdgeLength ( 0.0f ),
	mSegmentIdx ( 0 ),
	mTotalSegments ( 0 ),
	mLength ( 0.0f ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIPathStepper::~MOAIPathStepper () {

	this->mPath.Set ( *this, 0 );
}

//----------------------------------------------------------------//
bool MOAIPathStepper::More () {

	return this->mStepCount <= this->mTotalSteps;
}

//----------------------------------------------------------------//
bool MOAIPathStepper::MoreVertices () {

	return this->mFlattener.More ();
}

//----------------------------------------------------------------//
ZLVec2D MOAIPathStepper::Next () {

	static int count = 0;

	float elapsedLength = ( float )this->mStepCount * this->mStepSize;

	while ((( this->mEdgeBase + this->mEdgeLength ) < elapsedLength ) && this->MoreVertices ()) {
	
		this->mEdge [ 0 ] = this->mEdge [ 1 ];
		this->mEdge [ 1 ] = this->mEdge [ 2 ];
		this->mEdge [ 2 ] = this->NextVertex ();
		
		this->mEdgeBase += this->mEdgeLength;
		this->mEdgeLength = this->mEdge [ 0 ].Dist ( this->mEdge [ 1 ]);
		
		if ((( this->mEdgeBase + this->mEdgeLength ) < elapsedLength ) && !this->MoreVertices ()) {
		
			this->mEdge [ 0 ] = this->mEdge [ 1 ];
			this->mEdge [ 1 ] = this->mEdge [ 2 ];
			
			this->mEdgeBase += this->mEdgeLength;
			this->mEdgeLength = this->mEdge [ 0 ].Dist ( this->mEdge [ 1 ]);
		}
	}

	float t = ZLFloat::Clamp (( elapsedLength - this->mEdgeBase ) / this->mEdgeLength, 0.0f, 1.0f );
	
	ZLVec2D vec = this->mEdge [ 0 ];
	vec.Lerp ( this->mEdge [ 1 ], t );
	
	this->mStepCount++;
	
	return vec;
}

//----------------------------------------------------------------//
ZLVec2D MOAIPathStepper::NextVertex () {

	ZLVec2D vec = this->mFlattener.Next ();
	
	if ( !this->mFlattener.More ()) {
	
		this->mSegmentIdx++;
	
		if ( this->mSegmentIdx < this->mTotalSegments ) {
			
			this->mFlattener.Init ( this->mPath->GetSegment ( this->mSegmentIdx ));
			
			if ( this->mFlattener.More ()) {
				this->mFlattener.Next (); // skip the first vertex
			}
		}
	}
	return vec;
}

//----------------------------------------------------------------//
void MOAIPathStepper::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIPathStepper::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getLength",			_getLength },
		{ "more",				_more },
		{ "next",				_next },
		{ "start",				_start },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPathStepper::Start ( MOAIPath& path, float stepSize ) {

	this->mPath.Set ( *this, &path );
	
	this->mTotalSegments = path.CountSegments ();
	this->mLength = path.GetLength ();
	
	this->mSegmentIdx = 0;
	this->mFlattener.Init ( this->mPath->GetSegment ( 0 ));
	
	this->mStepCount = 0;
	this->mTotalSteps = ( u32 )( this->mLength / stepSize );
	this->mStepSize = stepSize;
	
	this->mEdgeBase = 0.0f;
	this->mEdgeLength = 0.0f;
	
	if ( this->MoreVertices ()) {
	
		this->mEdge [ 0 ] = this->NextVertex ();
		this->mEdge [ 1 ] = this->NextVertex ();
		this->mEdge [ 2 ] = this->NextVertex ();
		
		this->mEdgeLength = this->mEdge [ 0 ].Dist ( this->mEdge [ 1 ]);
	}
}
