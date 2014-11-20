// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLBARYCENTRIC_H
#define ZLBARYCENTRIC_H

#include <zl-util/ZLVec2D.h>
#include <zl-util/ZLVec3D.h>

//================================================================//
// ZLMetaBarycentric
//================================================================//
template < typename TYPE >
class ZLMetaBarycentric {
public:
	
	//----------------------------------------------------------------//
	static bool BarycentricIsInside ( const ZLMetaVec3D < TYPE >& bary ) {
		
		return (( bary.mX >= 0 ) && ( bary.mY >= 0 ) && ( bary.mZ >= 0 ));
	}
	
	//----------------------------------------------------------------//
	static ZLMetaVec2D < TYPE > BarycentricToCartesian ( const ZLMetaVec2D < TYPE >& v0, const ZLMetaVec2D < TYPE >& v1, const ZLMetaVec2D < TYPE >& v2, const ZLMetaVec3D < TYPE >& bary ) {
	
		ZLMetaVec2D < TYPE > cart;
		cart.mX = ( v0.mX * bary.mX ) + ( v1.mX * bary.mY ) + ( v2.mX * bary.mZ );
		cart.mY = ( v0.mY * bary.mX ) + ( v1.mY * bary.mY ) + ( v2.mY * bary.mZ );
		return cart;
	}
	
	//----------------------------------------------------------------//
	static ZLMetaVec3D < TYPE > BarycentricToCartesian ( const ZLMetaVec3D < TYPE >& v0, const ZLMetaVec3D < TYPE >& v1, const ZLMetaVec3D < TYPE >& v2, const ZLMetaVec3D < TYPE >& bary ) {
	
		ZLMetaVec3D < TYPE > cart;
		cart.mX = ( v0.mX * bary.mX ) + ( v1.mX * bary.mY ) + ( v2.mX * bary.mZ );
		cart.mY = ( v0.mY * bary.mX ) + ( v1.mY * bary.mY ) + ( v2.mY * bary.mZ );
		cart.mZ = ( v0.mZ * bary.mX ) + ( v1.mZ * bary.mY ) + ( v2.mZ * bary.mZ );
		return cart;
	}
	
	//----------------------------------------------------------------//
	static ZLMetaVec3D < TYPE > CartesianToBarycentric ( const ZLMetaVec2D < TYPE >& v0, const ZLMetaVec2D < TYPE >& v1, const ZLMetaVec2D < TYPE >& v2, const ZLMetaVec2D < TYPE >& cart ) {
	
		ZLMetaVec3D < TYPE > bary;
		TYPE invDenom = 1 / (( v1.mY - v2.mY ) * ( v0.mX - v2.mX ) + ( v2.mX - v1.mX ) * ( v0.mY - v2.mY ));

		bary.mX = (( v1.mY - v2.mY ) * ( cart.mX - v2.mX ) + ( v2.mX - v1.mX ) * ( cart.mY - v2.mY )) * invDenom;
		bary.mY = (( v2.mY - v0.mY ) * ( cart.mX - v2.mX ) + ( v0.mX - v2.mX ) * ( cart.mY - v2.mY )) * invDenom;
		bary.mZ = 1 - bary.mX - bary.mY;

		return bary;
	}
	
	//----------------------------------------------------------------//
	static ZLMetaVec3D < TYPE > CartesianToBarycentric ( const ZLMetaVec3D < TYPE >& v0, const ZLMetaVec3D < TYPE >& v1, const ZLMetaVec3D < TYPE >& v2, const ZLMetaVec2D < TYPE >& cart ) {
	
		ZLMetaVec3D < TYPE > bary;
	
		ZLMetaVec3D < TYPE > e0 ( v1.mX - v0.mX, v1.mY - v0.mY );
		ZLMetaVec3D < TYPE > e1 ( v2.mX - v0.mX, v2.mY - v0.mY );
		ZLMetaVec3D < TYPE > e2 ( cart.mX - v0.mX, cart.mY - v0.mY );
		
		TYPE d00 = e0.Dot ( e0 );
		TYPE d01 = e0.Dot ( e1 );
		TYPE d11 = e1.Dot ( e1 );
		TYPE d20 = e2.Dot ( e0 );
		TYPE d21 = e2.Dot ( e1 );
		
		TYPE invDenom = 1 / (( d00 * d11 ) - ( d01 * d01 ));
		
		bary.mX = (( d11 * d20 ) - ( d01 * d21 )) * invDenom;
		bary.mY = (( d00 * d21 ) - ( d01 * d20 )) * invDenom;
		bary.mZ = 1 - bary.mX - bary.mY;

		return bary;
	}
};

typedef ZLMetaBarycentric < float > ZLBarycentric;

#endif
