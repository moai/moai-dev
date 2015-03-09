// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLVEC2D_H
#define	ZLVEC2D_H

#include <zl-util/ZLMathConsts.h>
#include <zl-util/ZLTrig.h>
#include <zl-util/ZLVec3D.h>

template < typename TYPE > class tVec3;

// TODO: I'm on the fence about these macros
// If we keep them, move to someplace more general and standardize across all vector classes

#define DECLARE_BINARY(t,f)						\
	static t f ( t v0, const t& v1 ) {			\
		v0.f ( v1 );							\
		return v0;								\
	}

#define DECLARE_BINARY_WITH_SCALAR(t,f)			\
	static t f ( t v0, const t& v1, TYPE s ) {	\
		v0.f ( v1, s );							\
		return v0;								\
	}

#define DECLARE_UNARY(t,f)						\
	static t f ( t v0 ) {						\
		v0.f ();								\
		return v0;								\
	}

#define DECLARE_UNARY_WITH_SCALAR(t,f)			\
	static t f ( t v0, TYPE s ) {				\
		v0.f ( s );								\
		return v0;								\
	}

//================================================================//
// ZLMetaVec2D
//================================================================//
template < typename TYPE >
class ZLMetaVec2D {
public:

	TYPE	mX;
	TYPE	mY;

	//----------------------------------------------------------------//
	void Abs () {
		mX = mX < 0.0f ? -mX : mX;
		mY = mY < 0.0f ? -mY : mY;
	}

	//----------------------------------------------------------------//
	// V = V + point
	void Add ( const ZLMetaVec2D < TYPE >& point ) {
		mX += point.mX;
		mY += point.mY;
	}
	
	//----------------------------------------------------------------//
	// V = V + ( point * scale )
	void Add ( const ZLMetaVec2D < TYPE >& point, TYPE scale ) {
		mX += point.mX * scale;
		mY += point.mY * scale;
	}
	
	//----------------------------------------------------------------//
	// Clamps in positive and negative
	void Clamp ( const ZLMetaVec2D < TYPE >& clamp ) {
		if ( mX > clamp.mX ) mX = clamp.mX;
		else if ( mX < -clamp.mX ) mX = -clamp.mX;
		if ( mY > clamp.mY ) mY = clamp.mY;
		else if ( mY < -clamp.mY ) mY = -clamp.mY;
	}
	
	//----------------------------------------------------------------//
	// V = V x vec
	TYPE Cross ( const ZLMetaVec2D < TYPE >& vec ) const {
	
		return ( mX * vec.mY ) - ( mY * vec.mX );
	}
	
	//----------------------------------------------------------------//
	bool Compare ( const ZLMetaVec2D < TYPE >& point ) {
	
		if ((( mX != point.mX ) || ( mX != point.mX )) ||
			(( mY != point.mY ) || ( mY != point.mY ))) return false;

		return true;
	}
	
	//----------------------------------------------------------------//
	// Is V within res of point?
	bool Compare ( const ZLMetaVec2D < TYPE >& point, TYPE res ) {
	
		if ((( mX < ( point.mX - res )) || ( mX > ( point.mX + res ))) ||
			(( mY < ( point.mY - res )) || ( mY > ( point.mY + res )))) return false;

		return true;
	}
	
	//----------------------------------------------------------------//
	// |V| -= damp
	void Dampen ( const ZLMetaVec2D < TYPE >& damp ) {
		if ( mX > ( TYPE )0 ) {
			mX -= damp.mX;
			if ( mX < ( TYPE )0 ) mX = ( TYPE )0;
		}
		else if ( mX < ( TYPE )0 ) {
			mX += damp.mX;
			if ( mX > ( TYPE )0 ) mX = ( TYPE )0;
		}
		
		if ( mY > ( TYPE )0 ) {
			mY -= damp.mY;
			if ( mY < ( TYPE )0 ) mY = ( TYPE )0;
		}
		else if ( mY < ( TYPE )0 ) {
			mY += damp.mY;
			if ( mY > ( TYPE )0 ) mY = ( TYPE )0;
		}
	}
	
	//----------------------------------------------------------------//
	float Dist ( const ZLMetaVec2D < TYPE >& point ) {
		
		TYPE x = this->mX - point.mX;
		TYPE y = this->mY - point.mY;

		return sqrtf (( x * x ) + ( y * y ));
	}
	
	//----------------------------------------------------------------//
	float DistSqrd ( const ZLMetaVec2D < TYPE >& point ) const {
	
		TYPE x = this->mX - point.mX;
		TYPE y = this->mY - point.mY;
		
		return  (( x * x ) + ( y * y ));
	}
	
	//----------------------------------------------------------------//
	// V . point
	float Dot ( const ZLMetaVec2D < TYPE >& point ) const {
		return ( mX * point.mX ) + ( mY * point.mY );
	}
	
	//----------------------------------------------------------------//
	bool Equals ( const ZLMetaVec2D < TYPE >& point  ) {

		if ( mX != point.mX ) return false;
		if ( mY != point.mY ) return false;
		
		return true;
	}
	
	//----------------------------------------------------------------//
	void Init ( TYPE x, TYPE y ) {
		mX = x;
		mY = y;
	}
	
	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const ZLMetaVec2D < PARAM_TYPE >& point ) {
		this->mX = ( TYPE )point.mX;
		this->mY = ( TYPE )point.mY;
	}
	
	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const tVec3 < PARAM_TYPE >& vec ) {
		this->mX = ( TYPE )vec.mX;
		this->mY = ( TYPE )vec.mY;
	}
	
	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const ZLMetaVec2D < PARAM_TYPE >& v0, const ZLMetaVec2D < PARAM_TYPE >& v1 ) {
		this->mX = ( TYPE )v1.mX - ( TYPE )v0.mX;
		this->mY = ( TYPE )v1.mY - ( TYPE )v0.mY;
	}
	
	//----------------------------------------------------------------//
	// V = 1 / V
	void Invert () {
		mX = -mX;
		mY = -mY;
	}
	
	//----------------------------------------------------------------//
	// |V|
	float Length () {
		return sqrtf (( mX * mX ) + ( mY * mY ));
	}
	
	//----------------------------------------------------------------//
	float LengthSquared () const {
		return ( mX * mX ) + ( mY * mY );
	}
	
	//----------------------------------------------------------------//
	// V = V + ( time * point )
	void Lerp ( const ZLMetaVec2D < TYPE >& point, TYPE time ) {

		this->mX = this->mX + (( point.mX - this->mX ) * time );
		this->mY = this->mY + (( point.mY - this->mY ) * time );
	}
	
	//----------------------------------------------------------------//
	// V = ( V0 + V1 ) / 2
	void Mid ( const ZLMetaVec2D < TYPE >& point ) {

		this->mX = ( this->mX + point.mX ) * 0.5f;
		this->mY = ( this->mY + point.mY ) * 0.5f;
	}
	
	//----------------------------------------------------------------//
	// V *= point
	void Mul ( const ZLMetaVec2D < TYPE >& point ) {
		mX = mX * point.mX;
		mY = mY * point.mY;
	}
	
	//----------------------------------------------------------------//
	// |V|, V /= |V|
	TYPE Norm () {

		TYPE length;
		
		length = this->Length ();

		this->mX = this->mX / length;
		this->mY = this->mY / length;

		return length;
	}
	
	//----------------------------------------------------------------//
	// Normalize vector; test for 0-length vector.
	TYPE NormSafe () {

		TYPE length;
		
		length = this->Length ();

		if ( length != ( TYPE )0.0 ) {
			this->mX = this->mX / length;
			this->mY = this->mY / length;
		}

		return length;
	}
	
	//----------------------------------------------------------------//
	// V += point * d
	void Offset ( const ZLMetaVec2D < TYPE >& point, TYPE d ) {
		mX += point.mX * d;
		mY += point.mY * d;
	}
	
	//----------------------------------------------------------------//
	// Project V onto plane of norm along norm
	void PerpProject ( const ZLMetaVec2D < TYPE >& norm ) {

		TYPE dot;
		
		dot = this->Dot ( norm );

		this->mX -= norm.mX * dot;
		this->mY -= norm.mY * dot;
	}
	
	//----------------------------------------------------------------//
	// Project V onto plane of norm along axis
	void PerpProject ( const ZLMetaVec2D < TYPE >& norm, const ZLMetaVec2D < TYPE >& axis ) {

		TYPE project = norm.Dot ( axis ) / Dot ( norm );

		this->mX -= axis.mX * project;
		this->mY -= axis.mY * project;
	}
	
	//----------------------------------------------------------------//
	// Project onto vec
	void Project ( const ZLMetaVec2D < TYPE >& vec ) {
		
		TYPE p = this->Dot ( vec ) / vec.LengthSquared ();

		this->mX = p * vec.mX;
		this->mY = p * vec.mY;
	}
	
	//----------------------------------------------------------------//
	void Quantize ( TYPE decimalPlace ) {

		TYPE reciprocal = 1.0f / decimalPlace;
		
		this->mX = (( s32 )this->mX * reciprocal ) * decimalPlace;
		this->mY = (( s32 )this->mY * reciprocal ) * decimalPlace;
	}
	
	//----------------------------------------------------------------//
	// angle between vector and (1.0f,0.0f) in radians
	float Radians () const {
		
		float r = ASin ( ABS ( this->mY ));

		if ( this->mX < 0.0f ) {
			r = ( float )PI - r;
		}
		return this->mY < 0.0f ? ( float )TWOPI - r : r;
	}
	
	//----------------------------------------------------------------//
	// angle between vectors in radians
	float Radians ( const ZLMetaVec2D < TYPE >& v ) const {
		
		float dot = this->Dot ( v );
		
		if ( dot <= -1.0f ) return ( float )PI;
		if ( dot >= 1.0f ) return 0.0f;
		
		return ACos ( dot );
	}
	
	//----------------------------------------------------------------//
	// Reflect V off of plane of norm
	void Reflect ( const ZLMetaVec2D < TYPE >& norm ) {

		TYPE dot;
		
		dot = ( TYPE )2.0 * this->Dot ( norm );

		this->mX -= norm.mX * dot;
		this->mY -= norm.mY * dot;
	}
	
	//----------------------------------------------------------------//
	// Ceiling V components to nearest res
	void Res ( TYPE res ) {

		this->mX = ((( s32 )( this->mX / res )) * res );
		this->mY = ((( s32 )( this->mY / res )) * res );
	}
	
	//----------------------------------------------------------------//
	// V = -V
	void Reverse () {
		mX = -mX;
		mY = -mY;
	}
	
	//----------------------------------------------------------------//
	void Rotate90Anticlockwise () {
	
		float temp = this->mX;
		this->mX = this->mY;
		this->mY = -temp;
	}
	
	//----------------------------------------------------------------//
	void Rotate90Clockwise () {
	
		float temp = this->mX;
		this->mX = -this->mY;
		this->mY = temp;
	}
	
	//----------------------------------------------------------------//
	// V *= scale
	void Scale ( TYPE scale ) {
		mX *= scale;
		mY *= scale;
	}
	
	//----------------------------------------------------------------//
	// V *= scale
	void Scale ( TYPE xSc, TYPE ySc ) {
		mX *= xSc;
		mY *= ySc;
	}
	
	//----------------------------------------------------------------//
	// length, V / |V| * TYPE
	float SetLength ( float length ) {

		TYPE scale;
		scale = this->Length () / length;

		this->mX = this->mX / scale;
		this->mY = this->mY / scale;

		return length;
	}
	
	//----------------------------------------------------------------//
	//  V = V - point
	void Sub ( const ZLMetaVec2D < TYPE >& point ) { 
		mX -= point.mX;
		mY -= point.mY;
	}

	//----------------------------------------------------------------//
	// V = V - ( point * scale )
	void Sub ( const ZLMetaVec2D < TYPE >& point, TYPE scale ) { 
		mX -= point.mX * scale;
		mY -= point.mY * scale;
	}

	//----------------------------------------------------------------//
	ZLMetaVec2D () {
	}

	//----------------------------------------------------------------//
	ZLMetaVec2D ( TYPE x, TYPE y ) :
		mX ( x ),
		mY ( y ) {
	}
	
	//----------------------------------------------------------------//
	~ZLMetaVec2D () {
	}
	
	DECLARE_BINARY ( ZLMetaVec2D < TYPE >, Add )
	DECLARE_BINARY ( ZLMetaVec2D < TYPE >, Mid )
	DECLARE_BINARY ( ZLMetaVec2D < TYPE >, Mul )
	DECLARE_BINARY ( ZLMetaVec2D < TYPE >, Sub )
	
	DECLARE_BINARY_WITH_SCALAR ( ZLMetaVec2D < TYPE >, Add )
	DECLARE_BINARY_WITH_SCALAR ( ZLMetaVec2D < TYPE >, Lerp )
	DECLARE_BINARY_WITH_SCALAR ( ZLMetaVec2D < TYPE >, Sub )
	
	DECLARE_UNARY_WITH_SCALAR ( ZLMetaVec2D < TYPE >, Scale )
};

typedef ZLMetaVec2D < int > ZLIntVec2D;
typedef ZLMetaVec2D < float > ZLVec2D;
typedef ZLMetaVec2D < double > ZLVec2D64;

#endif
