// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-spine/MOAISpineBone.h>

//================================================================//
// lua
//================================================================//


//----------------------------------------------------------------//
/**	@name	clearLock
 
	@in		MOAISpineBone self
	@out	nil
*/
int MOAISpineBone::_clearLock ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpineBone, "U" );
	
	self->ClearLock ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	lockTransform
 
	@in		MOAISpineBone self
	@in		number	bitwise combination of lock flags
	@out	nil
*/
int MOAISpineBone::_lockTransform ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpineBone, "UN" );
	
	self->LockTransform( state.GetValue < u32 >( 2, 0 ) );
	return 0;
}


//================================================================//
// MOAISpine
//================================================================//

//----------------------------------------------------------------//
bool MOAISpineBone::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( mBone && MOAITransform::MOAITransformAttr::Check ( attrID )) {
		
		switch ( UNPACK_ATTR ( attrID )) {
			case MOAITransform::ATTR_X_LOC: {
				if ( mLockFlags & LOCK_LOC ) break;
                mBone->x = attrOp.Apply ( mBone->x, op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_FLOAT );
				spBone_updateWorldTransform ( mBone );
				return true;
			}
				
			case MOAITransform::ATTR_Y_LOC: {
				if ( mLockFlags & LOCK_LOC ) break;
				mBone->y = attrOp.Apply ( mBone->y, op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_FLOAT );
				spBone_updateWorldTransform ( mBone );
				return true;
			}
			
			case MOAITransform::ATTR_Z_ROT: {
				if ( mLockFlags & LOCK_ROT ) break;
				mBone->rotation = attrOp.Apply ( mBone->rotation, op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_FLOAT );
				spBone_updateWorldTransform ( mBone );
				return true;
			}
				
			case MOAITransform::ATTR_X_SCL: {
				if ( mLockFlags & LOCK_SCL ) break;
				mBone->scaleX = attrOp.Apply ( mBone->scaleX, op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_FLOAT );
				spBone_updateWorldTransform ( mBone );
				return true;
			}
				
			case MOAITransform::ATTR_Y_SCL: {
				if ( mLockFlags & LOCK_SCL ) break;
				mBone->scaleY = attrOp.Apply ( mBone->scaleY, op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_FLOAT );
				spBone_updateWorldTransform ( mBone );
				return true;
			}
		}
	}
	
	return MOAITransform::ApplyAttrOp (attrID, attrOp, op );
}

//----------------------------------------------------------------//
void MOAISpineBone::ClearLock() {
	mLockFlags = 0;
}


//----------------------------------------------------------------//
void MOAISpineBone::LockTransform ( u32 flags ) {
	mLockFlags |= flags;
	
	// Set inner transform values to current bone state.
	// In locked state bone reads are prohibited and current
	// transform values are used as bone transform
	if ( mBone ) {
		this->mLoc.mX = mBone->x;
		this->mLoc.mY = mBone->y;
		this->mRot.mZ = mBone->rotation;
		this->mScale.mX = mBone->scaleX;
		this->mScale.mY = mBone->scaleY;
	}
}

//----------------------------------------------------------------//
MOAISpineBone::MOAISpineBone ():
	mBone ( 0 ),
	mRootTransform ( 0 ),
	mLockFlags ( 0 ) {
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransform )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISpineBone::~MOAISpineBone () {
	
}

//----------------------------------------------------------------//
void MOAISpineBone::OnDepNodeUpdate () {
		
	if ( mBone ) {
		
		if ( mLockFlags & LOCK_LOC ) {
			mBone->x = mLoc.mX;
			mBone->y = mLoc.mY;
		}
		if ( mLockFlags & LOCK_ROT ) {
			mBone->rotation = mRot.mZ;
		}
		if ( mLockFlags & LOCK_SCL ) {
			mBone->scaleX = mScale.mX;
			mBone->scaleY = mScale.mY;
		}
		if ( mLockFlags ) {
			spBone_updateWorldTransform ( mBone );
		}
		
		float parentRot = 0.0f;
		float parentSclX = 1.0f;
		float parentSclY = 1.0f;
		
		if ( !mBone->data->inheritRotation && mBone->parent ) {
			parentRot = -mBone->parent->worldRotation;
		}
		if ( !mBone->data->inheritScale && mBone->parent ) {
			parentSclX = 1.f / mBone->parent->worldScaleX;
			parentSclY = 1.f / mBone->parent->worldScaleY;
		}
		
		mLocalToWorldMtx.ScRoTr (
			mBone->scaleX * parentSclX,
			mBone->scaleY * parentSclY,
			1,
			0,
			0,
			( mBone->rotation + parentRot ) * ( float )D2R,
			mBone->x,
			mBone->y,
			0
		);
		
		const ZLAffine3D* inherit = this->GetLinkedValue < ZLAffine3D* >( MOAITransformAttr::Pack ( INHERIT_TRANSFORM ), 0 );
		if ( inherit ) {
			mLocalToWorldMtx.Append ( *inherit );
		}
		
		if ( mRootTransform ) {
			if ( mBone->worldFlipX ) {
				mLocalToWorldMtx.m [ ZLAffine3D::C0_R0 ] *= -1;
				mLocalToWorldMtx.m [ ZLAffine3D::C0_R1 ] *= -1;
				mLocalToWorldMtx.m [ ZLAffine3D::C0_R2 ] *= -1;
			}
			if ( mBone->worldFlipY ) {
				mLocalToWorldMtx.m [ ZLAffine3D::C1_R0 ] *= -1;
				mLocalToWorldMtx.m [ ZLAffine3D::C1_R1 ] *= -1;
				mLocalToWorldMtx.m [ ZLAffine3D::C1_R2 ] *= -1;
			}
			mLocalToWorldMtx.Append ( mRootTransform->GetLocalToWorldMtx () );
		}
		
		mWorldToLocalMtx.Inverse ( mLocalToWorldMtx );
	}
}

//----------------------------------------------------------------//
void MOAISpineBone::RegisterLuaClass ( MOAILuaState &state ) {

	MOAITransform::RegisterLuaClass ( state );
	
	state.SetField ( -1, "LOCK_LOC", ( u32 )LOCK_LOC );
	state.SetField ( -1, "LOCK_SCL", ( u32 )LOCK_SCL );
	state.SetField ( -1, "LOCK_ROT", ( u32 )LOCK_ROT );
	state.SetField ( -1, "LOCK_ALL", ( u32 )LOCK_ALL );
}

//----------------------------------------------------------------//
void MOAISpineBone::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAITransform::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "clearLock",		_clearLock },
		{ "lockTransform",	_lockTransform },
		{ "new",			MOAILogMessages::_alertNewIsUnsupported },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISpineBone::SetAsRootBone ( MOAITransform* rootTransform ) {
	this->mRootTransform = rootTransform;
	this->ScheduleUpdate ();
}

//----------------------------------------------------------------//
void MOAISpineBone::SetBone ( spBone *bone ) {
	mBone = bone;
	this->ScheduleUpdate ();
}
