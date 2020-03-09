// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-assimp/MOAIAssimpAnimation.h>
#include <moai-assimp/MOAIAssimpUtil.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAssimpAnimation::_countChannels ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpAnimation, "U" )

	lua_pushinteger ( state , self->mScene ? self->mAssimpAnimation->mNumChannels : 0 );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpAnimation::_countPositionKeys ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpAnimation, "UN" )

	aiNodeAnim* channel = self->GetChannel ( state.GetValue < u32 >( 2, 1 ) - 1 );
	lua_pushinteger ( state , channel ? channel->mNumPositionKeys : 0 );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpAnimation::_countRotationKeys ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpAnimation, "UN" )

	aiNodeAnim* channel = self->GetChannel ( state.GetValue < u32 >( 2, 1 ) - 1 );
	lua_pushinteger ( state , channel ? channel->mNumRotationKeys : 0 );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpAnimation::_getAnimCurve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpAnimation, "UN" )
	
	MOAIAnimCurveBone* curve = self->GetAnimCurve ( state.GetValue < u32 >( 2, 1 ) - 1 );
	state.Push ( curve );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpAnimation::_getChannelName ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpAnimation, "UN" )
	
	aiNodeAnim* channel = self->GetChannel ( state.GetValue < u32 >( 2, 1 ) - 1 );
	lua_pushstring ( state , channel->mNodeName.C_Str ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpAnimation::_countScaleKeys ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpAnimation, "UN" )

	aiNodeAnim* channel = self->GetChannel ( state.GetValue < u32 >( 2, 1 ) - 1 );
	lua_pushinteger ( state , channel ? channel->mNumScalingKeys : 0 );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpAnimation::_getDuration ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpAnimation, "U" )

	lua_pushnumber ( state , self->mScene ? ( self->mAssimpAnimation->mDuration / self->mAssimpAnimation->mTicksPerSecond ) : 0 );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpAnimation::_getPositionKey ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpAnimation, "UNN" )

	aiVectorKey* key = self->GetPositionKey ( state.GetValue < u32 >( 2, 1 ) - 1 , state.GetValue < u32 >( 3, 1 ) - 1 );
	
	if ( key ) {
		lua_pushnumber ( L, key->mTime );
		MOAIAssimpUtil::PushVector ( L, key->mValue );
		return 2;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIAssimpAnimation::_getRotationKey ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpAnimation, "U" )

	aiQuatKey* key = self->GetRotationKey ( state.GetValue < u32 >( 2, 1 ) - 1 , state.GetValue < u32 >( 3, 1 ) - 1 );
	
	if ( key ) {
		lua_pushnumber ( L, key->mTime );
		MOAIAssimpUtil::PushQuaternion ( L, key->mValue );
		return 2;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIAssimpAnimation::_getScaleKey ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpAnimation, "U" )

	aiVectorKey* key = self->GetScaleKey ( state.GetValue < u32 >( 2, 1 ) - 1 , state.GetValue < u32 >( 3, 1 ) - 1 );
	
	if ( key ) {
		lua_pushnumber ( L, key->mTime );
		MOAIAssimpUtil::PushVector ( L, key->mValue );
		return 2;
	}
	return 0;
}

//================================================================//
// MOAIAssimpAnimation
//================================================================//

//----------------------------------------------------------------//
aiNodeAnim* MOAIAssimpAnimation::GetChannel ( u32 channel ) {

	return this->mScene && ( channel < this->mAssimpAnimation->mNumChannels ) ? this->mAssimpAnimation->mChannels [ channel ] : 0;
}

//----------------------------------------------------------------//
MOAIAnimCurveBone* MOAIAssimpAnimation::GetAnimCurve ( u32 channel ) {

	aiNodeAnim* nodeAnim = this->GetChannel ( channel );
	if ( !nodeAnim ) return 0;
	
	size_t nKeys = nodeAnim->mNumPositionKeys;
	nKeys = MAX ( nKeys, nodeAnim->mNumRotationKeys );
	nKeys = MAX ( nKeys, nodeAnim->mNumScalingKeys );
	
	size_t iPos = 0;
	size_t iRot = 0;
	size_t iScl = 0;
	
	MOAIAnimCurveBone* curve = new MOAIAnimCurveBone ();
	curve->ReserveKeys (( u32 )nKeys );
	
	ZLVec3D p0			= ZLVec3D::ORIGIN;
	ZLQuaternion q0		= ZLQuaternion::IDENT;
	ZLVec3D s0			= ZLVec3D::AXIS;
	
	for ( ZLIndex i = 0; i < nKeys; ++i ) {
	
		const aiVectorKey* posKey	= iPos < nodeAnim->mNumPositionKeys ? &nodeAnim->mPositionKeys [ iPos ] : 0;
		const aiQuatKey* rotKey		= iRot < nodeAnim->mNumRotationKeys ? &nodeAnim->mRotationKeys [ iRot ] : 0;
		const aiVectorKey* sclKey	= iScl < nodeAnim->mNumScalingKeys ? &nodeAnim->mScalingKeys [ iScl ] : 0;
	
		if ( !( posKey || rotKey || sclKey )) break;
		
		double t	= posKey ? posKey->mTime : ( rotKey ? rotKey->mTime : sclKey->mTime );
		
		t			= posKey ? MIN ( t, posKey->mTime ) : t;
		t			= rotKey ? MIN ( t, rotKey->mTime ) : t;
		t			= sclKey ? MIN ( t, sclKey->mTime ) : t;
		
		if ( posKey ) {
		
			p0 = ZLVec3D ( (float) posKey->mValue.x, (float) posKey->mValue.y, (float) posKey->mValue.z );
			
			if ( t == posKey->mTime ) {
				iPos++;
			}
			else {
				ZLVec3D p1 = p0;
				const aiVectorKey& posKeyPrev = nodeAnim->mPositionKeys [ iPos > 0 ? iPos - 1 : 0 ];
				p0 = ZLVec3D ((float) posKeyPrev.mValue.x, (float) posKeyPrev.mValue.y, (float) posKeyPrev.mValue.z );
				p0.Lerp ( p1, (float) (( t - posKeyPrev.mTime ) / ( posKey->mTime - posKeyPrev.mTime )));
			}
		}
		
		if ( rotKey ) {
		
			q0 = ZLQuaternion ((float) rotKey->mValue.w, (float) rotKey->mValue.x, (float)rotKey->mValue.y, (float) rotKey->mValue.z );
			
			if ( t == posKey->mTime ) {
				iRot++;
			}
			else {
				ZLQuaternion q1 = q0;
				const aiQuatKey& rotKeyPrev = nodeAnim->mRotationKeys [ iRot > 0 ? iRot - 1 : 0 ];
				q0 = ZLQuaternion ((float) rotKeyPrev.mValue.w, (float) rotKeyPrev.mValue.x, (float) rotKeyPrev.mValue.y, (float) rotKeyPrev.mValue.z );
				q0.Slerp ( q0, q1, (float) (( t - rotKeyPrev.mTime ) / ( rotKey->mTime - rotKeyPrev.mTime )));
			}
		}
		
		if ( sclKey ) {
		
			s0 = ZLVec3D ((float) sclKey->mValue.x, (float) sclKey->mValue.y, (float) sclKey->mValue.z );
			
			if ( t == posKey->mTime ) {
				iScl++;
			}
			else {
				ZLVec3D s1 = s0;
				const aiVectorKey& sclKeyPrev = nodeAnim->mScalingKeys [ iScl > 0 ? iScl - 1 : 0 ];
				s0 = ZLVec3D ((float) sclKeyPrev.mValue.x, (float) sclKeyPrev.mValue.y, (float) sclKeyPrev.mValue.z );
				s0.Lerp ( s1, (float) (( t - sclKeyPrev.mTime ) / ( sclKey->mTime - sclKeyPrev.mTime )));
			}
		}
		
		curve->SetKey ( i, (float) ( t / this->mAssimpAnimation->mTicksPerSecond ), ZLInterpolate::kLinear );
		curve->SetSamplePosition ( i, p0.mX, p0.mY, p0.mZ );
		curve->SetSampleRotation ( i, q0.mV.mX, q0.mV.mY, q0.mV.mZ, q0.mS );
		curve->SetSampleScale ( i, s0.mX, s0.mY, s0.mZ );
	}
	
	return curve;
}

//----------------------------------------------------------------//
aiVectorKey* MOAIAssimpAnimation::GetPositionKey ( u32 channel, u32 key ) {

	aiNodeAnim* nodeAnim = this->GetChannel ( channel );
	return nodeAnim && ( key < nodeAnim->mNumPositionKeys ) ? &nodeAnim->mPositionKeys [ key ] : 0;
}

//----------------------------------------------------------------//
aiQuatKey* MOAIAssimpAnimation::GetRotationKey ( u32 channel, u32 key ) {

	aiNodeAnim* nodeAnim = this->GetChannel ( channel );
	return nodeAnim && ( key < nodeAnim->mNumPositionKeys ) ? &nodeAnim->mRotationKeys [ key ] : 0;
}

//----------------------------------------------------------------//
aiVectorKey* MOAIAssimpAnimation::GetScaleKey ( u32 channel, u32 key ) {

	aiNodeAnim* nodeAnim = this->GetChannel ( channel );
	return nodeAnim && ( key < nodeAnim->mNumPositionKeys ) ? &nodeAnim->mScalingKeys [ key ] : 0;
}

//----------------------------------------------------------------//
MOAIAssimpAnimation::MOAIAssimpAnimation () {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIAssimpAnimation )
		RTTI_EXTEND ( MOAIAssimpSceneMember )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAssimpAnimation::~MOAIAssimpAnimation () {
}

//----------------------------------------------------------------//
void MOAIAssimpAnimation::SetAnimation ( aiAnimation* assimpAnimation ) {

	this->mAssimpAnimation = assimpAnimation;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAssimpAnimation::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIAssimpSceneMember, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIAssimpAnimation::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIAssimpSceneMember, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "countChannels",			_countChannels },
		{ "countPositionKeys",		_countPositionKeys },
		{ "countRotationKeys",		_countRotationKeys },
		{ "countScaleKeys",			_countScaleKeys },
		{ "getAnimCurve",			_getAnimCurve },
		{ "getChannelName",			_getChannelName },
		{ "getDuration",			_getDuration },
		{ "getPositionKey",			_getPositionKey },
		{ "getRotationKey",			_getRotationKey },
		{ "getScaleKey",			_getScaleKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
