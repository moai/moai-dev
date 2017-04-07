
#include "pch.h"
#include <zl-util/ZLBone.h>

//================================================================//
// ZLBone
//================================================================//

//----------------------------------------------------------------//
float ZLBone::ClampEuler ( float r ) {

	if ( r >= 360.0f ) {
		r = ( float )fmod ( r, 360.0f );
	}
	else if ( r < 0.0f ) {
		r = 360.0f + ( float )fmod ( r, 360.0f );
	}
	return r;
}

//----------------------------------------------------------------//
void ZLBone::Compose ( ZLAffine3D& mtx ) {

	float scaleX = this->mScale.mX;
	float scaleY = this->mScale.mY;
	float scaleZ = this->mScale.mZ;

	if ( this->mRotationMode != QUATERNION ) {
		
		float rx = ClampEuler ( this->mEuler.mX ) * ( float )D2R;
		float ry = ClampEuler ( this->mEuler.mY ) * ( float )D2R;
		float rz = ClampEuler ( this->mEuler.mZ ) * ( float )D2R;

		float cosX = Cos ( rx );
		float sinX = Sin ( rx );
		float cosY = Cos ( ry );
		float sinY = Sin ( ry );
		float cosZ = Cos ( rz );
		float sinZ = Sin ( rz );
		
		switch ( this->mRotationMode ) {
		
			case EULER_XYZ:
			
				mtx.m [ ZLAffine3D::C0_R0 ] = cosZ * cosY * scaleX;
				mtx.m [ ZLAffine3D::C0_R1 ] = sinZ * cosY * scaleX;
				mtx.m [ ZLAffine3D::C0_R2 ] = -sinY * scaleX;

				mtx.m [ ZLAffine3D::C1_R0 ] = (( cosZ * sinY * sinX ) + ( -sinZ * cosX )) * scaleY;
				mtx.m [ ZLAffine3D::C1_R1 ] = (( sinZ * sinY * sinX ) + ( cosZ * cosX )) * scaleY;
				mtx.m [ ZLAffine3D::C1_R2 ] = cosY * sinX * scaleY;

				mtx.m [ ZLAffine3D::C2_R0 ] = (( cosZ * sinY * cosX ) + ( -sinZ * -sinX )) * scaleZ;
				mtx.m [ ZLAffine3D::C2_R1 ] = (( sinZ * sinY * cosX ) + ( cosZ * -sinX )) * scaleZ;
				mtx.m [ ZLAffine3D::C2_R2 ] = cosY * cosX * scaleZ;
				
				break;

			case EULER_XZY:

				mtx.m [ ZLAffine3D::C0_R0 ] = cosY * cosZ * scaleX;
				mtx.m [ ZLAffine3D::C0_R1 ] = sinZ * scaleX;
				mtx.m [ ZLAffine3D::C0_R2 ] = -sinY * cosZ * scaleX;

				mtx.m [ ZLAffine3D::C1_R0 ] = (( cosY * -sinZ * cosX ) + ( sinY * sinX )) * scaleY;
				mtx.m [ ZLAffine3D::C1_R1 ] = cosZ * cosX * scaleY;
				mtx.m [ ZLAffine3D::C1_R2 ] = (( -sinY * -sinZ * cosX ) + ( cosY * sinX )) * scaleY;

				mtx.m [ ZLAffine3D::C2_R0 ] = (( cosY * -sinZ * -sinX ) + ( sinY * cosX )) * scaleZ;
				mtx.m [ ZLAffine3D::C2_R1 ] = cosZ * -sinX * scaleZ;
				mtx.m [ ZLAffine3D::C2_R2 ] = (( -sinY * -sinZ * -sinX ) + ( cosY * cosX )) * scaleZ;

				break;

			case EULER_YXZ:
			

				mtx.m [ ZLAffine3D::C0_R0 ] = (( cosZ * cosY ) + ( -sinZ * -sinX * -sinY )) * scaleX;
				mtx.m [ ZLAffine3D::C0_R1 ] = (( sinZ * cosY ) + ( cosZ * -sinX * -sinY )) * scaleX;
				mtx.m [ ZLAffine3D::C0_R2 ] = cosX * -sinY * scaleX;

				mtx.m [ ZLAffine3D::C1_R0 ] = -sinZ * cosX * scaleY;
				mtx.m [ ZLAffine3D::C1_R1 ] = cosZ * cosX * scaleY;
				mtx.m [ ZLAffine3D::C1_R2 ] = sinX * scaleY;

				mtx.m [ ZLAffine3D::C2_R0 ] = (( cosZ * sinY ) + ( -sinZ * -sinX * cosY )) * scaleZ;
				mtx.m [ ZLAffine3D::C2_R1 ] = (( sinZ * sinY ) + ( cosZ * -sinX * cosY )) * scaleZ;
				mtx.m [ ZLAffine3D::C2_R2 ] = cosX * cosY * scaleZ;

				break;

			case EULER_YZX:

				mtx.m [ ZLAffine3D::C0_R0 ] = cosZ * cosY * scaleX;
				mtx.m [ ZLAffine3D::C0_R1 ] = (( cosX * sinZ * cosY ) + ( -sinX * -sinY )) * scaleX;
				mtx.m [ ZLAffine3D::C0_R2 ] = (( sinX * sinZ * cosY ) + ( cosX * -sinY )) * scaleX;

				mtx.m [ ZLAffine3D::C1_R0 ] = -sinZ * scaleY;
				mtx.m [ ZLAffine3D::C1_R1 ] = cosX * cosZ * scaleY;
				mtx.m [ ZLAffine3D::C1_R2 ] = sinX * cosZ * scaleY;

				mtx.m [ ZLAffine3D::C2_R0 ] = cosZ * sinY * scaleZ;
				mtx.m [ ZLAffine3D::C2_R1 ] = (( cosX * sinZ * sinY ) + ( -sinX * cosY )) * scaleZ;
				mtx.m [ ZLAffine3D::C2_R2 ] = (( sinX * sinZ * sinY ) + ( cosX * cosY )) * scaleZ;

				break;

			case EULER_ZXY:
			
				mtx.m [ ZLAffine3D::C0_R0 ] = (( cosY * cosZ) + (sinY * sinX * sinZ )) * scaleX;
				mtx.m [ ZLAffine3D::C0_R1 ] = cosX * sinZ * scaleX;
				mtx.m [ ZLAffine3D::C0_R2 ] = ( -sinY * cosZ + (cosY * sinX * sinZ )) * scaleX;

				mtx.m [ ZLAffine3D::C1_R0 ] = (( cosY * -sinZ) + (sinY * sinX * cosZ )) * scaleY;
				mtx.m [ ZLAffine3D::C1_R1 ] = cosX * cosZ * scaleY;
				mtx.m [ ZLAffine3D::C1_R2 ] = ( -sinY * -sinZ + (cosY * sinX * cosZ )) * scaleY;

				mtx.m [ ZLAffine3D::C2_R0 ] = ( sinY * cosX * scaleZ );
				mtx.m [ ZLAffine3D::C2_R1 ] = -sinX * scaleZ;
				mtx.m [ ZLAffine3D::C2_R2 ] = ( cosY * cosX * scaleZ );

				break;

			case EULER_ZYX:

				mtx.m [ ZLAffine3D::C0_R0 ] = cosY * cosZ * scaleX;
				mtx.m [ ZLAffine3D::C0_R1 ] = (( cosX * sinZ ) + ( -sinX * -sinY * cosZ )) * scaleX;
				mtx.m [ ZLAffine3D::C0_R2 ] = (( sinX * sinZ ) + ( cosX * -sinY * cosZ )) * scaleX;

				mtx.m [ ZLAffine3D::C1_R0 ] = cosY * -sinZ * scaleY;
				mtx.m [ ZLAffine3D::C1_R1 ] = (( cosX * cosZ ) + ( -sinX * -sinY * -sinZ )) * scaleY;
				mtx.m [ ZLAffine3D::C1_R2 ] = (( sinX * cosZ ) + ( cosX * -sinY * -sinZ )) * scaleY;

				mtx.m [ ZLAffine3D::C2_R0 ] = sinY * scaleZ;
				mtx.m [ ZLAffine3D::C2_R1 ] = ( -sinX * cosY * scaleZ );
				mtx.m [ ZLAffine3D::C2_R2 ] = ( cosX * cosY * scaleZ );
				
				break;
			
			case QUATERNION:
			
				this->mQuaternion.Get ( mtx );
			
				mtx.m [ ZLAffine3D::C0_R0 ] *= scaleX;
				mtx.m [ ZLAffine3D::C0_R1 ] *= scaleX;
				mtx.m [ ZLAffine3D::C0_R2 ] *= scaleX;

				mtx.m [ ZLAffine3D::C1_R0 ] *= scaleY;
				mtx.m [ ZLAffine3D::C1_R1 ] *= scaleY;
				mtx.m [ ZLAffine3D::C1_R2 ] *= scaleY;

				mtx.m [ ZLAffine3D::C2_R0 ] *= scaleZ;
				mtx.m [ ZLAffine3D::C2_R1 ] *= scaleZ;
				mtx.m [ ZLAffine3D::C2_R2 ] *= scaleZ;
			
			default:
			
				mtx.Scale ( scaleX, scaleY, scaleZ );
		}
	}
	
	mtx.m [ ZLAffine3D::C3_R0 ] = this->mLocation.mX;
	mtx.m [ ZLAffine3D::C3_R1 ] = this->mLocation.mY;
	mtx.m [ ZLAffine3D::C3_R2 ] = this->mLocation.mZ;
	
	if (( this->mShearYX != 0.0f ) || ( this->mShearZX != 0.0f ) ||
		( this->mShearXY != 0.0f ) || ( this->mShearZY != 0.0f ) ||
		( this->mShearXZ != 0.0f ) || ( this->mShearYZ != 0.0f )) {
		
		ZLAffine3D shear;
		shear.Shear ( this->mShearYX, this->mShearZX, this->mShearXY, this->mShearZY, this->mShearXZ, this->mShearYZ );
		mtx.Prepend ( shear );
	}
	
	if (( this->mPivot.mX != 0.0f ) || ( this->mPivot.mY != 0.0f ) || ( this->mPivot.mZ != 0.0f )) {
		
		ZLAffine3D pivot;
		pivot.Translate ( -this->mPivot.mX, -this->mPivot.mY, -this->mPivot.mZ );
		mtx.Prepend ( pivot );
	}
}

//----------------------------------------------------------------//
void ZLBone::Compose ( ZLAffine3D& mtx, ZLAffine3D& inv ) {

	this->Compose ( mtx );
	inv.Inverse ( mtx );
}

//----------------------------------------------------------------//
bool ZLBone::GetEuler ( ZLVec3D& euler ) {

	if ( this->mRotationMode != QUATERNION ) {
		euler = this->mEuler;
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool ZLBone::GetQuaternion ( ZLQuaternion& quat ) {

	if ( this->mRotationMode == QUATERNION ) {
		quat = this->mQuaternion;
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void ZLBone::SetEuler ( float x, float y, float z ) {

	this->mEuler.mX = x;
	this->mEuler.mY = y;
	this->mEuler.mZ = z;
}

//----------------------------------------------------------------//
void ZLBone::SetLocation ( float x, float y, float z ) {

	this->mLocation.mX = x;
	this->mLocation.mY = y;
	this->mLocation.mZ = z;
}

//----------------------------------------------------------------//
void ZLBone::SetPivot ( float x, float y, float z ) {

	this->mPivot.mX = x;
	this->mPivot.mY = y;
	this->mPivot.mZ = z;
}

//----------------------------------------------------------------//
void ZLBone::SetRotationMode ( u32 mode ) {

	this->mRotationMode = mode;
}

//----------------------------------------------------------------//
void ZLBone::SetScale ( float x, float y, float z ) {

	this->mScale.mX = x;
	this->mScale.mY = y;
	this->mScale.mZ = z;
}

//----------------------------------------------------------------//
void ZLBone::Ident () {

	this->mShearYX = 0.0f;
	this->mShearZX = 0.0f;
	this->mShearXY = 0.0f;
	this->mShearZY = 0.0f;
	this->mShearXZ = 0.0f;
	this->mShearYZ = 0.0f;
	
	this->mPivot		= ZLVec3D::ORIGIN;
	this->mLocation		= ZLVec3D::ORIGIN;
	this->mScale		= ZLVec3D::AXIS;
	this->mEuler		= ZLVec3D::ORIGIN;
	
	this->mRotationMode = EULER_XYZ;
}

//----------------------------------------------------------------//
ZLBone::ZLBone () {
}

//----------------------------------------------------------------//
ZLBone::~ZLBone () {
}
