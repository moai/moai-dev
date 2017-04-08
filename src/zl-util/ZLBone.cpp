
#include "pch.h"
#include <zl-util/ZLBone.h>
#include <zl-util/ZLInterpolate.h>

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
void ZLBone::Compose ( ZLAffine3D& mtx ) const {

	ZLMatrix3x3 rot;
	this->ComposeRotation ( rot );

	float scaleX = this->mScale.mX;
	float scaleY = this->mScale.mY;
	float scaleZ = this->mScale.mZ;

	mtx.m [ ZLAffine3D::C0_R0 ] = rot.m [ ZLMatrix3x3::C0_R0 ] * scaleX;
	mtx.m [ ZLAffine3D::C0_R1 ] = rot.m [ ZLMatrix3x3::C0_R1 ] * scaleX;
	mtx.m [ ZLAffine3D::C0_R2 ] = rot.m [ ZLMatrix3x3::C0_R2 ] * scaleX;

	mtx.m [ ZLAffine3D::C1_R0 ] = rot.m [ ZLMatrix3x3::C1_R0 ] * scaleY;
	mtx.m [ ZLAffine3D::C1_R1 ] = rot.m [ ZLMatrix3x3::C1_R1 ] * scaleY;
	mtx.m [ ZLAffine3D::C1_R2 ] = rot.m [ ZLMatrix3x3::C1_R2 ] * scaleY;

	mtx.m [ ZLAffine3D::C2_R0 ] = rot.m [ ZLMatrix3x3::C2_R0 ] * scaleZ;
	mtx.m [ ZLAffine3D::C2_R1 ] = rot.m [ ZLMatrix3x3::C2_R1 ] * scaleZ;
	mtx.m [ ZLAffine3D::C2_R2 ] = rot.m [ ZLMatrix3x3::C2_R2 ] * scaleZ;
	
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
void ZLBone::Compose ( ZLAffine3D& mtx, ZLAffine3D& inv ) const {

	this->Compose ( mtx );
	inv.Inverse ( mtx );
}

//----------------------------------------------------------------//
void ZLBone::ComposeRotation ( ZLMatrix3x3& mtx ) const {

	if ( this->mRotationMode == QUATERNION ) {
	
		this->mQuaternion.Get ( mtx );
	}
	else {

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
			
				mtx.m [ ZLMatrix3x3::C0_R0 ] = cosZ * cosY;
				mtx.m [ ZLMatrix3x3::C0_R1 ] = sinZ * cosY;
				mtx.m [ ZLMatrix3x3::C0_R2 ] = -sinY;

				mtx.m [ ZLMatrix3x3::C1_R0 ] = (( cosZ * sinY * sinX ) + ( -sinZ * cosX ));
				mtx.m [ ZLMatrix3x3::C1_R1 ] = (( sinZ * sinY * sinX ) + ( cosZ * cosX ));
				mtx.m [ ZLMatrix3x3::C1_R2 ] = cosY * sinX;

				mtx.m [ ZLMatrix3x3::C2_R0 ] = (( cosZ * sinY * cosX ) + ( -sinZ * -sinX ));
				mtx.m [ ZLMatrix3x3::C2_R1 ] = (( sinZ * sinY * cosX ) + ( cosZ * -sinX ));
				mtx.m [ ZLMatrix3x3::C2_R2 ] = cosY * cosX;
				
				break;

			case EULER_XZY:

				mtx.m [ ZLMatrix3x3::C0_R0 ] = cosY * cosZ;
				mtx.m [ ZLMatrix3x3::C0_R1 ] = sinZ;
				mtx.m [ ZLMatrix3x3::C0_R2 ] = -sinY * cosZ;

				mtx.m [ ZLMatrix3x3::C1_R0 ] = (( cosY * -sinZ * cosX ) + ( sinY * sinX ));
				mtx.m [ ZLMatrix3x3::C1_R1 ] = cosZ * cosX;
				mtx.m [ ZLMatrix3x3::C1_R2 ] = (( -sinY * -sinZ * cosX ) + ( cosY * sinX ));

				mtx.m [ ZLMatrix3x3::C2_R0 ] = (( cosY * -sinZ * -sinX ) + ( sinY * cosX ));
				mtx.m [ ZLMatrix3x3::C2_R1 ] = cosZ * -sinX;
				mtx.m [ ZLMatrix3x3::C2_R2 ] = (( -sinY * -sinZ * -sinX ) + ( cosY * cosX ));

				break;

			case EULER_YXZ:
			

				mtx.m [ ZLMatrix3x3::C0_R0 ] = (( cosZ * cosY ) + ( -sinZ * -sinX * -sinY ));
				mtx.m [ ZLMatrix3x3::C0_R1 ] = (( sinZ * cosY ) + ( cosZ * -sinX * -sinY ));
				mtx.m [ ZLMatrix3x3::C0_R2 ] = cosX * -sinY;

				mtx.m [ ZLMatrix3x3::C1_R0 ] = -sinZ * cosX;
				mtx.m [ ZLMatrix3x3::C1_R1 ] = cosZ * cosX;
				mtx.m [ ZLMatrix3x3::C1_R2 ] = sinX;

				mtx.m [ ZLMatrix3x3::C2_R0 ] = (( cosZ * sinY ) + ( -sinZ * -sinX * cosY ));
				mtx.m [ ZLMatrix3x3::C2_R1 ] = (( sinZ * sinY ) + ( cosZ * -sinX * cosY ));
				mtx.m [ ZLMatrix3x3::C2_R2 ] = cosX * cosY;

				break;

			case EULER_YZX:

				mtx.m [ ZLMatrix3x3::C0_R0 ] = cosZ * cosY;
				mtx.m [ ZLMatrix3x3::C0_R1 ] = (( cosX * sinZ * cosY ) + ( -sinX * -sinY ));
				mtx.m [ ZLMatrix3x3::C0_R2 ] = (( sinX * sinZ * cosY ) + ( cosX * -sinY ));

				mtx.m [ ZLMatrix3x3::C1_R0 ] = -sinZ;
				mtx.m [ ZLMatrix3x3::C1_R1 ] = cosX * cosZ;
				mtx.m [ ZLMatrix3x3::C1_R2 ] = sinX * cosZ;

				mtx.m [ ZLMatrix3x3::C2_R0 ] = cosZ * sinY;
				mtx.m [ ZLMatrix3x3::C2_R1 ] = (( cosX * sinZ * sinY ) + ( -sinX * cosY ));
				mtx.m [ ZLMatrix3x3::C2_R2 ] = (( sinX * sinZ * sinY ) + ( cosX * cosY ));

				break;

			case EULER_ZXY:
			
				mtx.m [ ZLMatrix3x3::C0_R0 ] = (( cosY * cosZ) + (sinY * sinX * sinZ ));
				mtx.m [ ZLMatrix3x3::C0_R1 ] = cosX * sinZ;
				mtx.m [ ZLMatrix3x3::C0_R2 ] = ( -sinY * cosZ + (cosY * sinX * sinZ ));

				mtx.m [ ZLMatrix3x3::C1_R0 ] = (( cosY * -sinZ) + (sinY * sinX * cosZ ));
				mtx.m [ ZLMatrix3x3::C1_R1 ] = cosX * cosZ;
				mtx.m [ ZLMatrix3x3::C1_R2 ] = ( -sinY * -sinZ + (cosY * sinX * cosZ ));

				mtx.m [ ZLMatrix3x3::C2_R0 ] = ( sinY * cosX );
				mtx.m [ ZLMatrix3x3::C2_R1 ] = -sinX;
				mtx.m [ ZLMatrix3x3::C2_R2 ] = ( cosY * cosX );

				break;

			case EULER_ZYX:

				mtx.m [ ZLMatrix3x3::C0_R0 ] = cosY * cosZ;
				mtx.m [ ZLMatrix3x3::C0_R1 ] = (( cosX * sinZ ) + ( -sinX * -sinY * cosZ ));
				mtx.m [ ZLMatrix3x3::C0_R2 ] = (( sinX * sinZ ) + ( cosX * -sinY * cosZ ));

				mtx.m [ ZLMatrix3x3::C1_R0 ] = cosY * -sinZ;
				mtx.m [ ZLMatrix3x3::C1_R1 ] = (( cosX * cosZ ) + ( -sinX * -sinY * -sinZ ));
				mtx.m [ ZLMatrix3x3::C1_R2 ] = (( sinX * cosZ ) + ( cosX * -sinY * -sinZ ));

				mtx.m [ ZLMatrix3x3::C2_R0 ] = sinY;
				mtx.m [ ZLMatrix3x3::C2_R1 ] = ( -sinX * cosY );
				mtx.m [ ZLMatrix3x3::C2_R2 ] = ( cosX * cosY );
				
				break;
			
			default:
			
				mtx.Ident ();
		}
	}
}

//----------------------------------------------------------------//
bool ZLBone::GetEuler ( ZLVec3D& euler ) const {

	if ( this->mRotationMode != QUATERNION ) {
		euler = this->mEuler;
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
ZLQuaternion ZLBone::GetQuaternion () const {

	if ( this->mRotationMode == QUATERNION ) {
		return this->mQuaternion;
	}
	ZLMatrix3x3 mtx;
	this->ComposeRotation ( mtx );
	return ZLQuaternion ( mtx );
}

//----------------------------------------------------------------//
void ZLBone::Interpolate ( const ZLBone& b0, const ZLBone& b1, float t ) {

	this->mShearYX = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, b0.mShearYX, b1.mShearXY, t );
	this->mShearZX = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, b0.mShearZX, b1.mShearZX, t );

	this->mShearXY = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, b0.mShearXY, b1.mShearXY, t );
	this->mShearZY = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, b0.mShearZY, b1.mShearZY, t );
	
	this->mShearXZ = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, b0.mShearXZ, b1.mShearXZ, t );
	this->mShearYZ = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, b0.mShearYZ, b1.mShearYZ, t );

	this->mPivot.Lerp ( b0.mPivot, b1.mPivot, t );
	this->mLocation.Lerp ( b0.mLocation, b1.mLocation, t );
	this->mScale.Lerp ( b0.mScale, b1.mScale, t );

	if ((( b0.mRotationMode == QUATERNION ) || ( b1.mRotationMode == QUATERNION )) || ( b0.mRotationMode != b1.mRotationMode )) {
	
		ZLQuaternion q0 = b0.GetQuaternion ();
		ZLQuaternion q1 = b1.GetQuaternion ();
	
		this->mQuaternion.Slerp ( q0, q1, t );
		this->mRotationMode = QUATERNION;
	}
	else {
	
		this->mEuler.Lerp ( b0.mEuler, b1.mEuler, t );
		this->mRotationMode = b0.mRotationMode;
	}
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
