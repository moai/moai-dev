// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USAFFINE3D_H
#define USAFFINE3D_H

#include <uslscore/USMathConsts.h>
#include <uslscore/USMatrix.h>
#include <uslscore/USRect.h>
#include <uslscore/USTrig.h>
#include <uslscore/USVec2D.h>
#include <uslscore/USVec3D.h>

//================================================================//
// USMetaAffine3D
//================================================================//
template < typename TYPE >
class USMetaAffine3D {
public:

	enum {
		C0_R0	=	0,
		C0_R1,
		C0_R2,
		
		C1_R0,
		C1_R1,
		C1_R2,
		
		C2_R0,
		C2_R1,
		C2_R2,
		
		C3_R0,
		C3_R1,
		C3_R2,
		
		SIZE,
	};

	TYPE	m [ SIZE ];

	//----------------------------------------------------------------//
	void Append ( const USMetaAffine3D < TYPE >& mtx ) {

		USMetaAffine3D < TYPE > temp;
		temp.Multiply (	*this, mtx );
		this->Init ( temp );
	}

	//----------------------------------------------------------------//
	void AppendOffsetScale ( const USMetaVec3D < TYPE >& offset, const USMetaVec3D < TYPE >& scale ) {
		
		USMetaAffine3D < TYPE > temp;
		
		temp.m[C0_R0]	=	m[C0_R0] * scale.mX;
		temp.m[C0_R1]	=	m[C0_R1] * scale.mX;
		temp.m[C0_R2]	=	m[C0_R2] * scale.mX;
		
		temp.m[C1_R0]	=	m[C1_R0] * scale.mY;
		temp.m[C1_R1]	=	m[C1_R1] * scale.mY;
		temp.m[C1_R2]	=	m[C1_R2] * scale.mY;
		
		temp.m[C2_R0]	=	m[C2_R0] * scale.mZ;
		temp.m[C2_R1]	=	m[C2_R1] * scale.mZ;
		temp.m[C2_R2]	=	m[C2_R2] * scale.mZ;
		
		temp.m[C3_R0]	=	m[C3_R0] + offset.mX;
		temp.m[C3_R1]	=	m[C3_R1] + offset.mY;
		temp.m[C3_R2]	=	m[C3_R2] + offset.mZ;
		
		this->Init ( temp );
	}

	//----------------------------------------------------------------//
	void GetBasis ( USMetaVec3D < TYPE >& xAxis, USMetaVec3D < TYPE >& yAxis, USMetaVec3D < TYPE >& zAxis ) const {
	
		xAxis.mX = m [ C0_R0 ];
		xAxis.mY = m [ C0_R1 ];
		xAxis.mZ = m [ C0_R2 ];
		
		yAxis.mX = m [ C1_R0 ];
		yAxis.mY = m [ C1_R1 ];
		yAxis.mZ = m [ C1_R2 ];
		
		zAxis.mX = m [ C2_R0 ];
		zAxis.mY = m [ C2_R1 ];
		zAxis.mZ = m [ C2_R2 ];
	}

	//----------------------------------------------------------------//
	inline TYPE GetElement ( int c, int r ) const {

		return m [ ( c * 3 ) + r ];
	}

	//----------------------------------------------------------------//
	USMetaVec3D < TYPE > GetHeading () const {

		USMetaVec3D < TYPE > heading;
	
		heading.mX = m [ C0_R0 ];
		heading.mY = m [ C0_R1 ];
		heading.mZ = m [ C0_R1 ];

		heading.NormSafe ();
		
		return heading;
	}

	//----------------------------------------------------------------//
	TYPE GetRot () const {

		float rot = ( TYPE )( atan2 ( m [ C0_R0 ], m [ C0_R1 ]) * R2D );
		return rot;
	}

	//----------------------------------------------------------------//
	USMetaVec3D < TYPE > GetStretch () const {

		USMetaVec3D < TYPE > stretch;
		USMetaVec3D < TYPE > axis;

		// x axis
		axis.mX =	m [ C0_R0 ];
		axis.mY =	m [ C0_R1 ];
		axis.mZ =	m [ C0_R2 ];
		
		stretch.mX = axis.Length ();

		// y axis
		axis.mX =	m [ C1_R0 ];
		axis.mY =	m [ C1_R1 ];
		axis.mZ =	m [ C1_R2 ];

		stretch.mY = axis.Length ();
		
		// z axis
		axis.mX =	m [ C2_R0 ];
		axis.mY =	m [ C2_R1 ];
		axis.mZ =	m [ C2_R2 ];

		stretch.mZ = axis.Length ();
		
		return stretch;
	}

	//----------------------------------------------------------------//
	USMetaVec3D < TYPE > GetScale () const {

		USMetaVec3D < TYPE > vec;

		vec.mX = ( TYPE )m [ C0_R0 ];
		vec.mY = ( TYPE )m [ C1_R1 ];
		vec.mZ = ( TYPE )m [ C2_R2 ];
		
		return vec;
	}

	//----------------------------------------------------------------//
	USMetaVec3D < TYPE > GetTranslation () const {

		USMetaVec3D < TYPE > vec;

		vec.mX = ( TYPE )m [ C3_R0 ];
		vec.mY = ( TYPE )m [ C3_R1 ];
		vec.mZ = ( TYPE )m [ C3_R2 ];
		
		return vec;
	}

	//----------------------------------------------------------------//
	void Ident () {

		m [ C0_R0 ]	= 1;
		m [ C0_R1 ]	= 0;
		m [ C0_R2 ]	= 0;
		
		m [ C1_R0 ]	= 0;
		m [ C1_R1 ]	= 1;
		m [ C1_R2 ]	= 0;
		
		m [ C2_R0 ]	= 0;
		m [ C2_R1 ]	= 0;
		m [ C2_R2 ]	= 1;
		
		m [ C3_R0 ]	= 0;
		m [ C3_R1 ]	= 0;
		m [ C3_R2 ]	= 0;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const USMetaAffine3D < PARAM_TYPE >& mtx ) {

		memcpy ( m, mtx.m, sizeof ( m ));
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const USMetaMatrix4x4 < PARAM_TYPE >& mtx ) {
		
		m[C0_R0]	= ( TYPE )mtx.m[MatrixElem4x4::C0_R0];
		m[C0_R1]	= ( TYPE )mtx.m[MatrixElem4x4::C0_R1];
		m[C0_R2]	= ( TYPE )mtx.m[MatrixElem4x4::C0_R2];
		
		m[C1_R0]	= ( TYPE )mtx.m[MatrixElem4x4::C1_R0];
		m[C1_R1]	= ( TYPE )mtx.m[MatrixElem4x4::C1_R1];
		m[C1_R2]	= ( TYPE )mtx.m[MatrixElem4x4::C1_R2];
		
		m[C2_R0]	= ( TYPE )mtx.m[MatrixElem4x4::C2_R0];
		m[C2_R1]	= ( TYPE )mtx.m[MatrixElem4x4::C2_R1];
		m[C2_R2]	= ( TYPE )mtx.m[MatrixElem4x4::C2_R2];
		
		m[C3_R0]	= ( TYPE )mtx.m[MatrixElem4x4::C3_R0];
		m[C3_R1]	= ( TYPE )mtx.m[MatrixElem4x4::C3_R1];
		m[C3_R2]	= ( TYPE )mtx.m[MatrixElem4x4::C3_R2];
	}

	//----------------------------------------------------------------//
	bool Inverse () {

		USMetaAffine3D < TYPE > temp = *this;
		return Inverse ( temp );
	}

	//----------------------------------------------------------------//
	bool Inverse ( const USMetaAffine3D < TYPE >& mtx ) {

		// 2x2 determinants
		TYPE fA0 = mtx.m[C0_R0]*mtx.m[C1_R1] - mtx.m[C1_R0]*mtx.m[C0_R1];
		TYPE fA1 = mtx.m[C0_R0]*mtx.m[C2_R1] - mtx.m[C2_R0]*mtx.m[C0_R1];
		TYPE fA2 = mtx.m[C0_R0]*mtx.m[C3_R1] - mtx.m[C3_R0]*mtx.m[C0_R1];
		TYPE fA3 = mtx.m[C1_R0]*mtx.m[C2_R1] - mtx.m[C2_R0]*mtx.m[C1_R1];
		TYPE fA4 = mtx.m[C1_R0]*mtx.m[C3_R1] - mtx.m[C3_R0]*mtx.m[C1_R1];
		TYPE fA5 = mtx.m[C2_R0]*mtx.m[C3_R1] - mtx.m[C3_R0]*mtx.m[C2_R1];
		TYPE fB2 = mtx.m[C0_R2];
		TYPE fB4 = mtx.m[C1_R2];
		TYPE fB5 = mtx.m[C2_R2];

		// 3x3 determinants
		TYPE det = fA0*fB5-fA1*fB4+fA3*fB2;

		// If determinant == 0, there is no inverse
		if ( det  == 0 ) {
			*this = mtx;
			return false;
		}

		TYPE invDet = 1/det;

		// Transpose & 3x3 determinants * inverse determinant
		m[C0_R0] = ( + mtx.m[C1_R1]*fB5 - mtx.m[C2_R1]*fB4 ) * invDet;
		m[C0_R1] = ( - mtx.m[C0_R1]*fB5 + mtx.m[C2_R1]*fB2 ) * invDet;
		m[C0_R2] = ( + mtx.m[C0_R1]*fB4 - mtx.m[C1_R1]*fB2 ) * invDet;
		
		m[C1_R0] = ( - mtx.m[C1_R0]*fB5 + mtx.m[C2_R0]*fB4 ) * invDet;
		m[C1_R1] = ( + mtx.m[C0_R0]*fB5 - mtx.m[C2_R0]*fB2 ) * invDet;
		m[C1_R2] = ( - mtx.m[C0_R0]*fB4 + mtx.m[C1_R0]*fB2 ) * invDet;
		
		m[C2_R0] = fA3 * invDet;
		m[C2_R1] = -fA1 * invDet;
		m[C2_R2] = fA0 * invDet;
		
		m[C3_R0] = ( - mtx.m[C1_R2]*fA5 + mtx.m[C2_R2]*fA4 - mtx.m[C3_R2]*fA3 ) * invDet;
		m[C3_R1] = ( + mtx.m[C0_R2]*fA5 - mtx.m[C2_R2]*fA2 + mtx.m[C3_R2]*fA1 ) * invDet;
		m[C3_R2] = ( - mtx.m[C0_R2]*fA4 + mtx.m[C1_R2]*fA2 - mtx.m[C3_R2]*fA0 ) * invDet;

		// Inverse found!
		return true;
	}

	//----------------------------------------------------------------//
	bool IsIdent () const {
		
		if ( !(( m [ C0_R0 ] == 1.0f ) && ( m [ C1_R1 ] == 1.0f ) && ( m [ C2_R2 ] == 1.0f ))) return false;
		if ( !(( m [ C1_R0 ] == 0.0f ) && ( m [ C2_R0 ] == 0.0f ) && ( m [ C3_R0 ] == 0.0f ))) return false;
		if ( !(( m [ C0_R1 ] == 0.0f ) && ( m [ C2_R1 ] == 0.0f ) && ( m [ C3_R1 ] == 0.0f ))) return false;
		if ( !(( m [ C0_R2 ] == 0.0f ) && ( m [ C1_R2 ] == 0.0f ) && ( m [ C3_R2 ] == 0.0f ))) return false;
		
		return true;
	}

	//----------------------------------------------------------------//
	bool IsSame ( const USMetaAffine3D < TYPE >& compare ) const {
		
		return ( memcmp ( m, compare.m, sizeof ( m )));
	}

	//----------------------------------------------------------------//
	void Multiply ( const USMetaAffine3D < TYPE >& mtx2, const USMetaAffine3D < TYPE >& mtx1 ) {

		m[C0_R0]	=	( mtx1.m[C0_R0] * mtx2.m[C0_R0] )	+
						( mtx1.m[C1_R0] * mtx2.m[C0_R1] )	+
						( mtx1.m[C2_R0] * mtx2.m[C0_R2] );
		
		m[C0_R1]	=	( mtx1.m[C0_R1] * mtx2.m[C0_R0] )	+
						( mtx1.m[C1_R1] * mtx2.m[C0_R1] )	+
						( mtx1.m[C2_R1] * mtx2.m[C0_R2] );
		
		m[C0_R2]	=	( mtx1.m[C0_R2] * mtx2.m[C0_R0] )	+
						( mtx1.m[C1_R2] * mtx2.m[C0_R1] )	+
						( mtx1.m[C2_R2] * mtx2.m[C0_R2] );

		m[C1_R0]	=	( mtx1.m[C0_R0] * mtx2.m[C1_R0] )	+
						( mtx1.m[C1_R0] * mtx2.m[C1_R1] )	+
						( mtx1.m[C2_R0] * mtx2.m[C1_R2] );
		
		m[C1_R1]	=	( mtx1.m[C0_R1] * mtx2.m[C1_R0] )	+
						( mtx1.m[C1_R1] * mtx2.m[C1_R1] )	+
						( mtx1.m[C2_R1] * mtx2.m[C1_R2] );
		
		m[C1_R2]	=	( mtx1.m[C0_R2] * mtx2.m[C1_R0] )	+
						( mtx1.m[C1_R2] * mtx2.m[C1_R1] )	+
						( mtx1.m[C2_R2] * mtx2.m[C1_R2] );
		
		m[C2_R0]	=	( mtx1.m[C0_R0] * mtx2.m[C2_R0] )	+
						( mtx1.m[C1_R0] * mtx2.m[C2_R1] )	+
						( mtx1.m[C2_R0] * mtx2.m[C2_R2] );
		
		m[C2_R1]	=	( mtx1.m[C0_R1] * mtx2.m[C2_R0] )	+
						( mtx1.m[C1_R1] * mtx2.m[C2_R1] )	+
						( mtx1.m[C2_R1] * mtx2.m[C2_R2] );
		
		m[C2_R2]	=	( mtx1.m[C0_R2] * mtx2.m[C2_R0] )	+
						( mtx1.m[C1_R2] * mtx2.m[C2_R1] )	+
						( mtx1.m[C2_R2] * mtx2.m[C2_R2] );
		
		m[C3_R0]	=	( mtx1.m[C0_R0] * mtx2.m[C3_R0] )	+
						( mtx1.m[C1_R0] * mtx2.m[C3_R1] )	+
						( mtx1.m[C2_R0] * mtx2.m[C3_R2] )	+
						( mtx1.m[C3_R0] );
		
		m[C3_R1]	=	( mtx1.m[C0_R1] * mtx2.m[C3_R0] )	+
						( mtx1.m[C1_R1] * mtx2.m[C3_R1] )	+
						( mtx1.m[C2_R1] * mtx2.m[C3_R2] )	+
						( mtx1.m[C3_R1] );
		
		m[C3_R2]	=	( mtx1.m[C0_R2] * mtx2.m[C3_R0] )	+
						( mtx1.m[C1_R2] * mtx2.m[C3_R1] )	+
						( mtx1.m[C2_R2] * mtx2.m[C3_R2] )	+
						( mtx1.m[C3_R2] );
	}

	//----------------------------------------------------------------//
	void Prepend ( const USMetaAffine3D < TYPE >& mtx ) {

		USMetaAffine3D < TYPE > temp;
		temp.Multiply (	mtx, *this );
		this->Init ( temp );
	}

	//----------------------------------------------------------------//
	void PrependOffsetScale ( const USMetaVec3D < TYPE >& offset, const USMetaVec3D < TYPE >& scale ) {
		
		USMetaAffine3D < TYPE > temp;
		
		temp.m[C0_R0]	=	scale.mX * m[C0_R0];
		temp.m[C0_R1]	=	scale.mY * m[C0_R1];
		temp.m[C0_R2]	=	scale.mZ * m[C0_R2];

		temp.m[C1_R0]	=	scale.mX * m[C1_R0];
		temp.m[C1_R1]	=	scale.mY * m[C1_R1];
		temp.m[C1_R2]	=	scale.mZ * m[C1_R2];
		
		temp.m[C2_R0]	=	scale.mX * m[C2_R0];
		temp.m[C2_R1]	=	scale.mY * m[C2_R1];
		temp.m[C2_R2]	=	scale.mZ * m[C2_R2];
		
		temp.m[C3_R0]	=	offset.mX + m[C3_R0];
		temp.m[C3_R1]	=	offset.mY + m[C3_R1];
		temp.m[C3_R2]	=	offset.mZ + m[C3_R2];
		
		this->Init ( temp );
	}

	//----------------------------------------------------------------//
	void Rotate	( USMetaVec3D < TYPE >& axis, TYPE radians ) {
	
		TYPE c	= Cos (	radians	);
		TYPE c2 = 1	- c;
		TYPE s	= Sin (	radians	);

		TYPE x	= axis.mX;
		TYPE y	= axis.mY;
		TYPE z	= axis.mZ;
		
		m[C0_R0]	= c	+ (c2 *	x *	x );
		m[C0_R1]	= (	c2 * x * y ) + ( z * s );
		m[C0_R2]	= (	c2 * x * z ) - ( y * s ) ;

		m[C1_R0]	= (	c2 * x * y ) - ( z * s );
		m[C1_R1]	= c	+ (	c2 * y * y );
		m[C1_R2]	= (	c2 * y * z ) + ( x * s );
		
		m[C2_R0]	= (	c2 * x * z ) + ( y * s );
		m[C2_R1]	= (	c2 * y * z ) - ( x * s );
		m[C2_R2]	= c	* (	c2 * z * z );
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
	}

	//----------------------------------------------------------------//
	void RotateX ( TYPE radians ) {

		m[C0_R0]	= 1;
		m[C0_R1]	= 0;
		m[C0_R2]	= 0;
		
		m[C1_R0]	= 0;
		m[C1_R1]	= Cos (	radians	);
		m[C1_R2]	= Sin (	radians	);
		
		m[C2_R0]	= 0;
		m[C2_R1]	= -m[C1_R2];	// -sin
		m[C2_R2]	= m[C1_R1];	// cos
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
	}

	//----------------------------------------------------------------//
	void RotateY ( TYPE radians ) {

		m[C0_R0]	= Cos (	radians	);
		m[C0_R1]	= 0;
		m[C0_R2]	= -Sin ( radians );
		
		m[C1_R0]	= 0;
		m[C1_R1]	= 1;
		m[C1_R2]	= 0;
		
		m[C2_R0]	= -m[C0_R2];	// sin
		m[C2_R1]	= 0;
		m[C2_R2]	= m[C0_R0];	// cos
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
	}

	//----------------------------------------------------------------//
	void RotateZ ( TYPE radians ) {

		m[C0_R0]	= Cos (	radians	);
		m[C0_R1]	= Sin (	radians	);
		m[C0_R2]	= 0;
		
		m[C1_R0]	= -m[C0_R1];	// -sin
		m[C1_R1]	= m[C0_R0];	// cos
		m[C1_R2]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= 0;
		m[C2_R2]	= 1;
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
	}

	//----------------------------------------------------------------//
	void Scale ( const USMetaVec3D < TYPE >& scl ) {

		this->Scale ( scl.mX, scl.mY, scl.mZ );
	}

	//----------------------------------------------------------------//
	void Scale ( TYPE x, TYPE y, TYPE z ) {

		m [ C0_R0 ]	= x;
		m [ C0_R1 ]	= 0;
		m [ C0_R2 ]	= 0;
		
		m [ C1_R0 ]	= 0;
		m [ C1_R1 ]	= y;
		m [ C1_R2 ]	= 0;
		
		m [ C2_R0 ]	= 0;
		m [ C2_R1 ]	= 0;
		m [ C2_R2 ]	= z;
		
		m [ C3_R0 ]	= 0;
		m [ C3_R1 ]	= 0;
		m [ C3_R2 ]	= 0;
	}

	//----------------------------------------------------------------//
	void ScRoTr ( USMetaVec3D < TYPE >& sc, USMetaVec3D < TYPE >& ro, USMetaVec3D < TYPE >&	tr ) {

		this->ScRoTr ( sc.mX, sc.mY, sc.mZ, ro.mX, ro.mY, ro.mZ, tr.mX, tr.mY, tr.mZ );
	}

	//----------------------------------------------------------------//
	void ScRoTr	( float scx, float scy, float scz, float rx, float ry, float rz, float tx, float ty, float tz ) {

		TYPE cx = Cos ( rx );
		TYPE sx = Sin ( rx );
		TYPE cy = Cos ( ry );
		TYPE sy = Sin ( ry );
		TYPE cz = Cos ( rz );
		TYPE sz = Sin ( rz );

		m[C0_R0]	= cz*cy*scx;
		m[C0_R1]	= sz*cy*scx;
		m[C0_R2]	= -sy*scx;

		m[C1_R0]	= ((cz*sy*sx)+(-sz*cx))*scy;
		m[C1_R1]	= ((sz*sy*sx)+(cz*cx))*scy;
		m[C1_R2]	= cy*sx*scy;

		m[C2_R0]	= ((cz*sy*cx)+(-sz*-sx))*scz;
		m[C2_R1]	= ((sz*sy*cx)+(cz*-sx))*scz;
		m[C2_R2]	= cy*cx*scz;

		m[C3_R0]	= tx;
		m[C3_R1]	= ty;
		m[C3_R2]	= tz;
	}

	//----------------------------------------------------------------//
	void ScRoTrInv ( USMetaVec3D < TYPE >& sc, USMetaVec3D < TYPE >& ro, USMetaVec3D < TYPE >&	tr ) {

		this->ScRoTrInv ( sc.mX, sc.mY, sc.mZ, ro.mX, ro.mY, ro.mZ, tr.mX, tr.mY, tr.mZ );
	}

	//----------------------------------------------------------------//
	void ScRoTrInv ( float scx, float scy, float scz, float rx, float ry, float rz, float tx, float ty, float tz ) {

		TYPE cx = Cos ( -rx	);
		TYPE sx = Sin ( -rx	);
		TYPE cy = Cos ( -ry	);
		TYPE sy = Sin ( -ry	);
		TYPE cz = Cos ( -rz	);
		TYPE sz = Sin ( -rz	);

		TYPE x	= -tx;
		TYPE y	= -ty;
		TYPE z	= -tz;

		USMetaVec3D < TYPE >	invScl;
		invScl.mX =	1 / scx;
		invScl.mY =	1 / scy;
		invScl.mZ =	1 / scz;

		m[C0_R0]	= invScl.mX*cy*cz;
		m[C0_R1]	= invScl.mY*((cx*sz)+(-sx*-sy*cz));
		m[C0_R2]	= invScl.mZ*((sx*sz)+(cx*-sy*cz));

		m[C1_R0]	= invScl.mX*cy*-sz;
		m[C1_R1]	= invScl.mY*((cx*cz)+(-sx*-sy*-sz));
		m[C1_R2]	= invScl.mZ*((sx*cz)+(cx*-sy*-sz));

		m[C2_R0]	= invScl.mX*sy;
		m[C2_R1]	= invScl.mY*-sx*cy;
		m[C2_R2]	= invScl.mZ*cx*cy;

		m[C3_R0]	= invScl.mX*((cy*cz*x)+(cy*-sz*y)+(sy*z));
		m[C3_R1]	= invScl.mY*((((cx*sz)+(-sx*-sy*cz))*x)+(((cx*cz)+(-sx*-sy*-sz))*y)+(-sx*cy*z));
		m[C3_R2]	= invScl.mZ*((((sx*sz)+(cx*-sy*cz))*x)+(((sx*cz)+(cx*-sy*-sz))*y)+(cx*cy*z));
	}

	//----------------------------------------------------------------//
	void Shear ( TYPE yx, TYPE zx, TYPE xy, TYPE zy, TYPE xz, TYPE yz ) {

		m[C0_R0]	= 1;
		m[C0_R1]	= yx;
		m[C0_R2]	= zx;
		
		m[C1_R0]	= xy;
		m[C1_R1]	= 1;
		m[C1_R2]	= zy;
		
		m[C2_R0]	= xz;
		m[C2_R1]	= yz;
		m[C2_R2]	= 1;
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Transform ( USMetaVec2D < PARAM_TYPE >& point ) const {

		PARAM_TYPE x;
		PARAM_TYPE y;
		
		x =		(( PARAM_TYPE )m[C0_R0] *	point.mX ) +
				(( PARAM_TYPE )m[C1_R0] *	point.mY ) +
				(( PARAM_TYPE )m[C3_R0] );
		
		y =		(( PARAM_TYPE )m[C0_R1] *	point.mX ) +
				(( PARAM_TYPE )m[C1_R1] *	point.mY ) +
				(( PARAM_TYPE )m[C3_R1] );
		
		point.mX = x;
		point.mY = y;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Transform ( USMetaVec3D < PARAM_TYPE >& vec ) const {

		PARAM_TYPE x;
		PARAM_TYPE y;
		
		x =			(( PARAM_TYPE )m[C0_R0] *	vec.mX ) +
					(( PARAM_TYPE )m[C1_R0] *	vec.mY ) +
					(( PARAM_TYPE )m[C2_R0] *	vec.mZ ) +
					(( PARAM_TYPE )m[C3_R0]);
		
		y =			(( PARAM_TYPE )m[C0_R1] *	vec.mX ) +
					(( PARAM_TYPE )m[C1_R1] *	vec.mY ) +
					(( PARAM_TYPE )m[C2_R1] *	vec.mZ ) +
					(( PARAM_TYPE )m[C3_R1]);
		
		vec.mZ =	(( PARAM_TYPE )m[C0_R2] *	vec.mX ) +
					(( PARAM_TYPE )m[C1_R2] *	vec.mY ) +
					(( PARAM_TYPE )m[C2_R2] *	vec.mZ ) +
					(( PARAM_TYPE )m[C3_R2]);
		
		vec.mX = x;
		vec.mY = y;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Transform ( USMetaRect < PARAM_TYPE >& rect ) const {

		USMetaVec2D < TYPE > point;
		USMetaRect < TYPE > newRect;
		
		point.Init ( rect.mXMin, rect.mYMin );
		this->Transform ( point );
		newRect.Init ( point );
		
		point.Init ( rect.mXMax, rect.mYMin );
		this->Transform ( point );
		newRect.Grow ( point );
		
		point.Init ( rect.mXMax, rect.mYMax );
		this->Transform ( point );
		newRect.Grow ( point );
		
		point.Init ( rect.mXMin, rect.mYMax );
		this->Transform ( point );
		newRect.Grow ( point );
		
		rect.mXMin = ( PARAM_TYPE )newRect.mXMin;
		rect.mYMin = ( PARAM_TYPE )newRect.mYMin;
		rect.mXMax = ( PARAM_TYPE )newRect.mXMax;
		rect.mYMax = ( PARAM_TYPE )newRect.mYMax;
		
		rect.Bless ();
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE>
	void TransformQuad ( USMetaVec2D < PARAM_TYPE >* quad ) const {
	
		#ifdef MOAI_ARM7
		
			TYPE xform_mat[16] = { m[C0_R0], m[C0_R1], 0, 0, //column 1
								   m[C1_R0], m[C1_R1], 0, 0, //column 2
								   m[C2_R0], m[C2_R1], 0, 0, //column 3
								   0, 0, 0, 0 };			 //column 4
								   
			TYPE point_mat[16] = { quad[0].mX, quad[0].mY, 1, 0,   //column 1
								   quad[1].mX, quad[1].mY, 1, 0,   //column 2
								   quad[2].mX, quad[2].mY, 1, 0,   //column 3
								   quad[3].mX, quad[3].mY, 1, 0 }; //column 4
								   
   			TYPE outpt_mat[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
								   		
			asm volatile (
			
				// Store A & B leaving room at top of registers for result (q0-q3)			
				"vldmia		%0,		{ q4-q7 }		\n\t"
				"vldmia		%1,		{ q8-q11 }		\n\t"
				
				// result = first column of B x first row of A
				"vmul.f32	q0,		q8,		d8[0]	\n\t"
				"vmul.f32	q1,		q8,		d10[0]	\n\t"
				"vmul.f32	q2,		q8,		d12[0]	\n\t"
				"vmul.f32	q3,		q8,		d14[0]	\n\t"
	         
				// result += second column of B x second row of A
				"vmla.f32	q0,		q9,		d8[1]	\n\t"
				"vmla.f32	q1,		q9,		d10[1]	\n\t"
				"vmla.f32	q2,		q9,		d12[1]	\n\t"
				"vmla.f32	q3,		q9,		d14[1]	\n\t"
	         
				// result += third column of B x third row of A
				"vmla.f32	q0,		q10,	d9[0]	\n\t"
				"vmla.f32	q1,		q10,	d11[0]	\n\t"
				"vmla.f32	q2,		q10,	d13[0]	\n\t"
         		"vmla.f32	q3,		q10,	d15[0]	\n\t"
	         
				// result += last column of B x last row of A
				"vmla.f32	q0,		q11,	d9[1]	\n\t"
				"vmla.f32	q1,		q11,	d11[1]	\n\t"
				"vmla.f32	q2,		q11,	d13[1]	\n\t"
				"vmla.f32	q3,		q11,	d15[1]	\n\t"
	         
				// output = result registers
				"vstmia		%2,		{ q0-q3 }		\n\t"
				
				: // no output
				: "r" (point_mat), "r" (xform_mat), "r" (outpt_mat)  // input - note *value* of pointer doesn't change
				: "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q11" //clobber		
			);
			
			quad[0].mX = outpt_mat[0]; quad[0].mY = outpt_mat[1];
			quad[1].mX = outpt_mat[4]; quad[1].mY = outpt_mat[5];
			quad[2].mX = outpt_mat[8]; quad[2].mY = outpt_mat[9];
			quad[3].mX = outpt_mat[12]; quad[3].mY = outpt_mat[13];
			
		#else
		
			this->Transform ( quad [ 0 ]);
			this->Transform ( quad [ 1 ]);
			this->Transform ( quad [ 2 ]);
			this->Transform ( quad [ 3 ]);
		
		#endif
	}

	//----------------------------------------------------------------//
	// Transforms w/o translation
	template < typename PARAM_TYPE >
	void TransformVec (	USMetaVec2D < PARAM_TYPE >& point ) const {

		PARAM_TYPE x;
		PARAM_TYPE y;
		
		x =		(( PARAM_TYPE )m[C0_R0] *	point.mX ) +
				(( PARAM_TYPE )m[C1_R0] *	point.mY );
		
		y =		(( PARAM_TYPE )m[C0_R1] *	point.mX ) +
				(( PARAM_TYPE )m[C1_R1] *	point.mY );
		
		point.mX = x;
		point.mY = y;
	}

	//----------------------------------------------------------------//
	// Transforms w/o translation
	template < typename PARAM_TYPE >
	void TransformVec (	USMetaVec3D < PARAM_TYPE >& vec ) const {

		PARAM_TYPE x;
		PARAM_TYPE y;
		
		x =			(( PARAM_TYPE )m[C0_R0] *	vec.mX ) +
					(( PARAM_TYPE )m[C1_R0] *	vec.mY ) +
					(( PARAM_TYPE )m[C2_R0] *	vec.mZ );
		
		y =			(( PARAM_TYPE )m[C0_R1] *	vec.mX ) +
					(( PARAM_TYPE )m[C1_R1] *	vec.mY ) +
					(( PARAM_TYPE )m[C2_R1] *	vec.mZ );
		
		vec.mZ =	(( PARAM_TYPE )m[C0_R2] *	vec.mX ) +
					(( PARAM_TYPE )m[C1_R2] *	vec.mY ) +
					(( PARAM_TYPE )m[C2_R2] *	vec.mZ );
		
		vec.mX = x;
		vec.mY = y;
	}

	//----------------------------------------------------------------//
	inline TYPE TransformX ( TYPE x ) const {
		
		return ( m [ C0_R0 ] * x ) + ( m [ C3_R0 ]);
	}

	//----------------------------------------------------------------//
	inline TYPE TransformY ( TYPE y ) const {
		
		return ( m [ C1_R1 ] * y ) + ( m [ C3_R1 ]);
	}

	//----------------------------------------------------------------//
	inline TYPE TransformZ ( TYPE z ) const {
		
		return ( m [ C2_R2 ] * z ) + ( m [ C3_R2 ]);
	}

	//----------------------------------------------------------------//
	void Translate ( USMetaVec3D < TYPE >& trn ) {

		Translate (	trn.mX,	trn.mY,	trn.mZ );
	}

	//----------------------------------------------------------------//
	void Translate ( TYPE x, TYPE y, TYPE z ) {

		m[C0_R0]	= 1;
		m[C0_R1]	= 0;
		m[C0_R2]	= 0;
		
		m[C1_R0]	= 0;
		m[C1_R1]	= 1;
		m[C1_R2]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= 0;
		m[C2_R2]	= 1;
		
		m[C3_R0]	= x;
		m[C3_R1]	= y;
		m[C3_R2]	= z;
	}
	
	//----------------------------------------------------------------//
	USMetaAffine3D () {
	}
};

typedef USMetaAffine3D < float > USAffine3D;
typedef USMetaAffine3D < double > USAffine3D64;

#endif
