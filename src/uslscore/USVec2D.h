// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef VEC2D_H
#define	VEC2D_H

//#include <uslscore/USTrig.h>
#include <uslscore/USVec3D.h>

/** @addtogroup MathLib */

template < typename TYPE > class tVec3;

//================================================================//
// USMetaVec2D
//================================================================//
/**	@brief Template class for two-dimensional vector math.
	@tparam TYPE Type of elements.  Must implement all built-in math operators.
	@ingroup MathLib
*/
template < typename TYPE >
class USMetaVec2D {
public:

	TYPE	mX;		/**< X component. */
	TYPE	mY;		/**< Y component. */

	//----------------------------------------------------------------//
	void Abs () {
		mX = mX < 0.0f ? -mX : mX;
		mY = mY < 0.0f ? -mY : mY;
	}

	//----------------------------------------------------------------//
	/**	@brief V = V + point
	*/
	void Add ( const USMetaVec2D < TYPE >& point ) {
		mX += point.mX;
		mY += point.mY;
	}
	
	//----------------------------------------------------------------//
	/**	@brief V = V + ( point * scale )
	*/
	void Add ( const USMetaVec2D < TYPE >& point, TYPE scale ) {
		mX += point.mX * scale;
		mY += point.mY * scale;
	}
	
	//----------------------------------------------------------------//
	/**	@brief Clamps in positive and negative
	*/
	void Clamp ( const USMetaVec2D < TYPE >& clamp ) {
		if ( mX > clamp.mX ) mX = clamp.mX;
		else if ( mX < -clamp.mX ) mX = -clamp.mX;
		if ( mY > clamp.mY ) mY = clamp.mY;
		else if ( mY < -clamp.mY ) mY = -clamp.mY;
	}
	
	//----------------------------------------------------------------//
	/**	@brief V = V x vec
	*/
	TYPE Cross ( const USMetaVec2D < TYPE >& vec ) {
	
		return ( mX * vec.mY ) - ( mY * vec.mX );
	}
	
	//----------------------------------------------------------------//
	/**	@brief Is V within res of point?
	*/
	bool Compare ( const USMetaVec2D < TYPE >& point, TYPE res ) {
	
		if ((( mX <= ( point.mX + res )) && ( mX >= ( point.mX - res ))) &&
			(( mY <= ( point.mY + res )) && ( mY >= ( point.mY - res )))) return true;

		return false;
	}
	
	//----------------------------------------------------------------//
	/**	@brief |V| -= damp
	*/
	void Dampen ( const USMetaVec2D < TYPE >& damp ) {
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
	float Dist ( const USMetaVec2D < TYPE >& point ) {
		
		TYPE x = this->mX - point.mX;
		TYPE y = this->mY - point.mY;

		return sqrtf (( x * x ) + ( y * y ));
	}
	
	//----------------------------------------------------------------//
	float DistSqrd ( const USMetaVec2D < TYPE >& point ) const {
	
		TYPE x = this->mX - point.mX;
		TYPE y = this->mY - point.mY;
		
		return  (( x * x ) + ( y * y ));
	}
	
	//----------------------------------------------------------------//
	/**	@brief V . point
	*/
	float Dot ( const USMetaVec2D < TYPE >& point ) const {
		return ( mX * point.mX ) + ( mY * point.mY );
	}
	
	//----------------------------------------------------------------//
	bool Equals ( const USMetaVec2D < TYPE >& point  ) {

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
	void Init ( const USMetaVec2D < PARAM_TYPE >& point ) {
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
	void Init ( const USMetaVec2D < PARAM_TYPE >& v0, const USMetaVec2D < PARAM_TYPE >& v1 ) {
		this->mX = ( TYPE )v1.mX - ( TYPE )v0.mX;
		this->mY = ( TYPE )v1.mY - ( TYPE )v0.mY;
	}
	
	//----------------------------------------------------------------//
	/**	@brief V = -V
	*/
	void Invert () {
		mX = -mX;
		mY = -mY;
	}
	
	//----------------------------------------------------------------//
	/**	@brief |V|
	*/
	float Length () {
		return sqrtf (( mX * mX ) + ( mY * mY ));
	}
	
	//----------------------------------------------------------------//
	float LengthSquared () const {
		return ( mX * mX ) + ( mY * mY );
	}
	
	//----------------------------------------------------------------//
	/**	@brief V = V + ( time * point )
	*/
	void Lerp ( const USMetaVec2D& point, TYPE time ) {

		this->mX = this->mX + (( point.mX - this->mX ) * time );
		this->mY = this->mY + (( point.mY - this->mY ) * time );
	}
	
	//----------------------------------------------------------------//
	/**	@brief V *= point
	*/
	void Multiply ( const USMetaVec2D < TYPE >& point ) {
		mX = mX * point.mX;
		mY = mY * point.mY;
	}
	
	//----------------------------------------------------------------//
	/**	@brief |V|, V /= |V|
	*/
	TYPE Norm () {

		TYPE length;
		
		length = this->Length ();

		this->mX = this->mX / length;
		this->mY = this->mY / length;

		return length;
	}
	
	//----------------------------------------------------------------//
	/**	@brief Normalize vector; test for 0-length vector.
	*/
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
	/**	@brief V += point * d
	*/
	void Offset ( const USMetaVec2D < TYPE >& point, TYPE d ) {
		mX += point.mX * d;
		mY += point.mY * d;
	}
	
	//----------------------------------------------------------------//
	/**	@brief Project V onto plane of norm along norm
	*/
	void PerpProject ( const USMetaVec2D& norm ) {

		TYPE dot;
		
		dot = this->Dot ( norm );

		this->mX -= norm.mX * dot;
		this->mY -= norm.mY * dot;
	}
	
	//----------------------------------------------------------------//
	/**	@brief Project V onto plane of norm along axis
	*/
	void PerpProject ( const USMetaVec2D& norm, const USMetaVec2D& axis ) {

		TYPE project = norm.Dot ( axis ) / Dot ( norm );

		this->mX -= axis.mX * project;
		this->mY -= axis.mY * project;
	}
	
	//----------------------------------------------------------------//
	/**	@brief Project onto vec
	*/
	void Project ( const USMetaVec2D& vec ) {
		
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
	/**	@brief Reflect V off of plane of norm
	*/
	void Reflect ( const USMetaVec2D& norm ) {

		TYPE dot;
		
		dot = ( TYPE )2.0 * this->Dot ( norm );

		this->mX -= norm.mX * dot;
		this->mY -= norm.mY * dot;
	}
	
	//----------------------------------------------------------------//
	/**	@brief Ceiling V components to nearest res
	*/
	void Res ( TYPE res ) {

		this->mX = ((( s32 )( this->mX / res )) * res );
		this->mY = ((( s32 )( this->mY / res )) * res );
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
	/**	@brief V *= scale
	*/
	void Scale ( TYPE scale ) {
		mX *= scale;
		mY *= scale;
	}
	
	//----------------------------------------------------------------//
	/**	@brief V *= scale
	*/
	void Scale ( TYPE xSc, TYPE ySc ) {
		mX *= xSc;
		mY *= ySc;
	}
	
	//----------------------------------------------------------------//
	/**	@brief length, V / |V| * TYPE
	*/
	float SetLength ( float length ) {

		TYPE scale;
		scale = this->Length () / length;

		this->mX = this->mX / scale;
		this->mY = this->mY / scale;

		return length;
	}
	
	//----------------------------------------------------------------//
	/**	@brief  V = V - point
	*/
	void Sub ( const USMetaVec2D < TYPE >& point ) { 
		mX -= point.mX;
		mY -= point.mY;
	}

	//----------------------------------------------------------------//
	/**	@brief V = V - ( point * scale )
	*/
	void Sub ( const USMetaVec2D < TYPE >& point, TYPE scale ) { 
		mX -= point.mX * scale;
		mY -= point.mY * scale;
	}

	//----------------------------------------------------------------//
	USMetaVec2D () {
	}

	//----------------------------------------------------------------//
	USMetaVec2D ( TYPE x, TYPE y ) :
		mX ( x ),
		mY ( y ) {
	}
	
	//----------------------------------------------------------------//
	~USMetaVec2D () {
	}
};

//================================================================//
// USIntVec2D
//================================================================//
/**	@brief 2D integer vector.
	@ingroup MathLib
*/
class USIntVec2D :
	public USMetaVec2D < int > {
public:

	//----------------------------------------------------------------//
	/**	@brief Constructor.
	*/	
	USIntVec2D () {
	}

	//----------------------------------------------------------------//
	/**	@brief Constructor.
	*/	
	USIntVec2D ( int x, int y ) :
		USMetaVec2D < int > ( x, y ) {
	}
};

typedef USMetaVec2D < float > USVec2D;
typedef USMetaVec2D < double > USVec2D64;

//================================================================//
// USVec2D
//================================================================//
/**	@brief 2D float vector.
	@ingroup MathLib
*/

//class USVec2D :
//	public USMetaVec2D < float > {
//public:
//
//	//----------------------------------------------------------------//
//	/**	@brief Constructor.
//	*/	
//	USVec2D () {
//	}
//
//	//----------------------------------------------------------------//
//	/**	@brief Constructor.
//	*/	
//	USVec2D ( float x, float y ) :
//		USMetaVec2D < float > ( x, y ) {
//	}
//};

//================================================================//
// USVec2D64
//================================================================//
/**	@brief 2D double vector.
	@ingroup MathLib
*/
//class USVec2D64 :
//	public USMetaVec2D < double > {
//public:
//
//	//----------------------------------------------------------------//
//	/**	@brief Constructor.
//	*/	
//	USVec2D64 () {
//	}
//
//	//----------------------------------------------------------------//
//	/**	@brief Constructor.
//	*/	
//	USVec2D64 ( double x, double y ) :
//		USMetaVec2D < double > ( x, y ) {
//	}
//};

#endif
