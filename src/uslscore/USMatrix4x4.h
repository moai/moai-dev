// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USMATRIX4X4_H
#define USMATRIX4X4_H

#include <uslscore/USMatrix.h>
#include <uslscore/USRect.h>
#include <uslscore/USTrig.h>
#include <uslscore/USVec3D.h>
#include <uslscore/USVec4D.h>

//================================================================//
// USMetaMatrix4x4
//================================================================//
template < typename TYPE >
class USMetaMatrix4x4 {
public:

	enum {
		C0_R0	=	0,
		C0_R1,
		C0_R2,
		C0_R3,
		
		C1_R0,
		C1_R1,
		C1_R2,
		C1_R3,
		
		C2_R0,
		C2_R1,
		C2_R2,
		C2_R3,
		
		C3_R0,
		C3_R1,
		C3_R2,
		C3_R3,
		
		SIZE,
	};

	TYPE m [ SIZE ];

	//----------------------------------------------------------------//
	void Append	( const USMetaMatrix4x4 < TYPE >& mtx ) {

		USMetaMatrix4x4 < TYPE > temp;
		temp.Multiply (	*this, mtx );
		this->Init ( temp );
	}

	//----------------------------------------------------------------//
	TYPE Determinant ( void ) const {

		// 2x2 determinants
		TYPE fA0 = m[C0_R0]*m[C1_R1] - m[C1_R0]*m[C0_R1];
		TYPE fA1 = m[C0_R0]*m[C2_R1] - m[C2_R0]*m[C0_R1];
		TYPE fA2 = m[C0_R0]*m[C3_R1] - m[C3_R0]*m[C0_R1];
		TYPE fA3 = m[C1_R0]*m[C2_R1] - m[C2_R0]*m[C1_R1];
		TYPE fA4 = m[C1_R0]*m[C3_R1] - m[C3_R0]*m[C1_R1];
		TYPE fA5 = m[C2_R0]*m[C3_R1] - m[C3_R0]*m[C2_R1];
		TYPE fB0 = m[C0_R2]*m[C1_R3] - m[C1_R2]*m[C0_R3];
		TYPE fB1 = m[C0_R2]*m[C2_R3] - m[C2_R2]*m[C0_R3];
		TYPE fB2 = m[C0_R2]*m[C3_R3] - m[C3_R2]*m[C0_R3];
		TYPE fB3 = m[C1_R2]*m[C2_R3] - m[C2_R2]*m[C1_R3];
		TYPE fB4 = m[C1_R2]*m[C3_R3] - m[C3_R2]*m[C1_R3];
		TYPE fB5 = m[C2_R2]*m[C3_R3] - m[C3_R2]*m[C2_R3];

		// 3x3 determinants
		return fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
	}

	//----------------------------------------------------------------//
	void EulerRPH (	USMetaVec3D < TYPE >& rot ) {

		TYPE cr = Cos ( rot.mZ	);
		TYPE sr = Sin ( rot.mZ	);
		TYPE cp = Cos ( rot.mX	);
		TYPE sp = Sin ( rot.mX	);
		TYPE ch = Cos ( rot.mY	);
		TYPE sh = Sin ( rot.mY	);
		
		m[C0_R0]	= (	ch * cr	) +	( sh * sp *	sr );
		m[C0_R1]	= (	cp * sr	);
		m[C0_R2]	= (	ch * sp	* sr ) - ( sh *	cr );
		m[C0_R3]	= 0;
		
		m[C1_R0]	= (	sh * sp	* cr ) - ( ch *	sr );
		m[C1_R1]	= (	cp * cr	);
		m[C1_R2]	= (	sh * sr	) +	( ch * sp *	cr );
		m[C1_R3]	= 0;
		
		m[C2_R0]	= (	sh * cp	);
		m[C2_R1]	= -sp;
		m[C2_R2]	= (	ch * cp	);
		m[C2_R3]	= 0;
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	void EulerXYZ (	TYPE x, TYPE y, TYPE z )	{

		TYPE cx = Cos ( x );
		TYPE sx = Sin ( x );
		TYPE cy = Cos ( y );
		TYPE sy = Sin ( y );
		TYPE cz = Cos ( z );
		TYPE sz = Sin ( z );

		m[C0_R0]	= cz*cy;
		m[C0_R1]	= sz*cy;
		m[C0_R2]	= -sy;
		m[C0_R3]	= 0;

		m[C1_R0]	= (cz*sy*sx)+(-sz*cx);
		m[C1_R1]	= (sz*sy*sx)+(cz*cx);
		m[C1_R2]	= cy*sx;
		m[C1_R3]	= 0;

		m[C2_R0]	= (cz*sy*cx)+(-sz*-sx);
		m[C2_R1]	= (sz*sy*cx)+(cz*-sx);
		m[C2_R2]	= cy*cx;
		m[C2_R3]	= 0;

		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	TYPE GetElement ( int c, int r ) const {

		return m [ ( c * 4 ) + r ];
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
	void GetTranslation ( USMetaVec3D < TYPE >& tr ) const {

		tr.mX = m[C3_R0];
		tr.mY = m[C3_R1];
		tr.mZ = m[C3_R2];
	}

	//----------------------------------------------------------------//
	void Ident () {

		m[C0_R0]	= 1;
		m[C0_R1]	= 0;
		m[C0_R2]	= 0;
		m[C0_R3]	= 0;
		
		m[C1_R0]	= 0;
		m[C1_R1]	= 1;
		m[C1_R2]	= 0;
		m[C1_R3]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= 0;
		m[C2_R2]	= 1;
		m[C2_R3]	= 0;
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init (	const USMetaAffine2D < PARAM_TYPE >& mtx ) {

		m[C0_R0]	= ( TYPE )mtx.m[AffineElem2D::C0_R0];
		m[C0_R1]	= ( TYPE )mtx.m[AffineElem2D::C0_R1];
		m[C0_R2]	= 0;
		m[C0_R3]	= 0;
		
		m[C1_R0]	= ( TYPE )mtx.m[AffineElem2D::C1_R0];
		m[C1_R1]	= ( TYPE )mtx.m[AffineElem2D::C1_R1];
		m[C1_R2]	= 0;
		m[C1_R3]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= 0;
		m[C2_R2]	= 1;
		m[C2_R3]	= 0;
		
		m[C3_R0]	= ( TYPE )mtx.m[AffineElem2D::C2_R0];
		m[C3_R1]	= ( TYPE )mtx.m[AffineElem2D::C2_R1];
		m[C3_R2]	= 0;
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init (	const USMetaAffine3D < PARAM_TYPE >& mtx ) {

		m[C0_R0]	= ( TYPE )mtx.m[AffineElem3D::C0_R0];
		m[C0_R1]	= ( TYPE )mtx.m[AffineElem3D::C0_R1];
		m[C0_R2]	= ( TYPE )mtx.m[AffineElem3D::C0_R2];
		m[C0_R3]	= 0;
		
		m[C1_R0]	= ( TYPE )mtx.m[AffineElem3D::C1_R0];
		m[C1_R1]	= ( TYPE )mtx.m[AffineElem3D::C1_R1];
		m[C1_R2]	= ( TYPE )mtx.m[AffineElem3D::C1_R2];
		m[C1_R3]	= 0;
		
		m[C2_R0]	= ( TYPE )mtx.m[AffineElem3D::C2_R0];
		m[C2_R1]	= ( TYPE )mtx.m[AffineElem3D::C2_R1];
		m[C2_R2]	= ( TYPE )mtx.m[AffineElem3D::C2_R2];
		m[C2_R3]	= 0;
		
		m[C3_R0]	= ( TYPE )mtx.m[AffineElem3D::C3_R0];
		m[C3_R1]	= ( TYPE )mtx.m[AffineElem3D::C3_R1];
		m[C3_R2]	= ( TYPE )mtx.m[AffineElem3D::C3_R2];
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init (	const USMetaMatrix3x3 < PARAM_TYPE >& mtx ) {

		m[C0_R0]	= ( TYPE )mtx.m[MatrixElem3x3::C0_R0];
		m[C0_R1]	= ( TYPE )mtx.m[MatrixElem3x3::C0_R1];
		m[C0_R2]	= 0;
		m[C0_R3]	= 0;
		
		m[C1_R0]	= ( TYPE )mtx.m[MatrixElem3x3::C1_R0];
		m[C1_R1]	= ( TYPE )mtx.m[MatrixElem3x3::C1_R1];
		m[C1_R2]	= 0;
		m[C1_R3]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= 0;
		m[C2_R2]	= 1;
		m[C2_R3]	= 0;
		
		m[C3_R0]	= ( TYPE )mtx.m[MatrixElem3x3::C2_R0];
		m[C3_R1]	= ( TYPE )mtx.m[MatrixElem3x3::C2_R1];
		m[C3_R2]	= 0;
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const USMetaMatrix4x4 < PARAM_TYPE >& mtx ) {

		m[C0_R0]	= ( TYPE )mtx.m[C0_R0];
		m[C0_R1]	= ( TYPE )mtx.m[C0_R1];
		m[C0_R2]	= ( TYPE )mtx.m[C0_R2];
		m[C0_R3]	= ( TYPE )mtx.m[C0_R3];
		
		m[C1_R0]	= ( TYPE )mtx.m[C1_R0];
		m[C1_R1]	= ( TYPE )mtx.m[C1_R1];
		m[C1_R2]	= ( TYPE )mtx.m[C1_R2];
		m[C1_R3]	= ( TYPE )mtx.m[C1_R3];
		
		m[C2_R0]	= ( TYPE )mtx.m[C2_R0];
		m[C2_R1]	= ( TYPE )mtx.m[C2_R1];
		m[C2_R2]	= ( TYPE )mtx.m[C2_R2];
		m[C2_R3]	= ( TYPE )mtx.m[C2_R3];
		
		m[C3_R0]	= ( TYPE )mtx.m[C3_R0];
		m[C3_R1]	= ( TYPE )mtx.m[C3_R1];
		m[C3_R2]	= ( TYPE )mtx.m[C3_R2];
		m[C3_R3]	= ( TYPE )mtx.m[C3_R3];
	}

	//----------------------------------------------------------------//
	bool Inverse ( void	) {

		USMetaMatrix4x4 < TYPE > temp	= *this;
		return Inverse ( temp );
	}

	//----------------------------------------------------------------//
	bool Inverse ( const USMetaMatrix4x4 < TYPE >& mtx ) {

		// 2x2 determinants
		TYPE fA0 = mtx.m[C0_R0]*mtx.m[C1_R1] - mtx.m[C1_R0]*mtx.m[C0_R1];
		TYPE fA1 = mtx.m[C0_R0]*mtx.m[C2_R1] - mtx.m[C2_R0]*mtx.m[C0_R1];
		TYPE fA2 = mtx.m[C0_R0]*mtx.m[C3_R1] - mtx.m[C3_R0]*mtx.m[C0_R1];
		TYPE fA3 = mtx.m[C1_R0]*mtx.m[C2_R1] - mtx.m[C2_R0]*mtx.m[C1_R1];
		TYPE fA4 = mtx.m[C1_R0]*mtx.m[C3_R1] - mtx.m[C3_R0]*mtx.m[C1_R1];
		TYPE fA5 = mtx.m[C2_R0]*mtx.m[C3_R1] - mtx.m[C3_R0]*mtx.m[C2_R1];
		TYPE fB0 = mtx.m[C0_R2]*mtx.m[C1_R3] - mtx.m[C1_R2]*mtx.m[C0_R3];
		TYPE fB1 = mtx.m[C0_R2]*mtx.m[C2_R3] - mtx.m[C2_R2]*mtx.m[C0_R3];
		TYPE fB2 = mtx.m[C0_R2]*mtx.m[C3_R3] - mtx.m[C3_R2]*mtx.m[C0_R3];
		TYPE fB3 = mtx.m[C1_R2]*mtx.m[C2_R3] - mtx.m[C2_R2]*mtx.m[C1_R3];
		TYPE fB4 = mtx.m[C1_R2]*mtx.m[C3_R3] - mtx.m[C3_R2]*mtx.m[C1_R3];
		TYPE fB5 = mtx.m[C2_R2]*mtx.m[C3_R3] - mtx.m[C3_R2]*mtx.m[C2_R3];

		// 3x3 determinants
		TYPE det = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;

		// If determinant == 0, there is no inverse
		if ( det  == 0 ) {
			*this = mtx;
			return false;
		}

		TYPE invDet = 1/det;

		// Transpose & 3x3 determinants * inverse determinant
		m[C0_R0] = ( + mtx.m[C1_R1]*fB5 - mtx.m[C2_R1]*fB4 + mtx.m[C3_R1]*fB3 ) * invDet;
		m[C0_R1] = ( - mtx.m[C0_R1]*fB5 + mtx.m[C2_R1]*fB2 - mtx.m[C3_R1]*fB1 ) * invDet;
		m[C0_R2] = ( + mtx.m[C0_R1]*fB4 - mtx.m[C1_R1]*fB2 + mtx.m[C3_R1]*fB0 ) * invDet;
		m[C0_R3] = ( - mtx.m[C0_R1]*fB3 + mtx.m[C1_R1]*fB1 - mtx.m[C2_R1]*fB0 ) * invDet;
		m[C1_R0] = ( - mtx.m[C1_R0]*fB5 + mtx.m[C2_R0]*fB4 - mtx.m[C3_R0]*fB3 ) * invDet;
		m[C1_R1] = ( + mtx.m[C0_R0]*fB5 - mtx.m[C2_R0]*fB2 + mtx.m[C3_R0]*fB1 ) * invDet;
		m[C1_R2] = ( - mtx.m[C0_R0]*fB4 + mtx.m[C1_R0]*fB2 - mtx.m[C3_R0]*fB0 ) * invDet;
		m[C1_R3] = ( + mtx.m[C0_R0]*fB3 - mtx.m[C1_R0]*fB1 + mtx.m[C2_R0]*fB0 ) * invDet;
		m[C2_R0] = ( + mtx.m[C1_R3]*fA5 - mtx.m[C2_R3]*fA4 + mtx.m[C3_R3]*fA3 ) * invDet;
		m[C2_R1] = ( - mtx.m[C0_R3]*fA5 + mtx.m[C2_R3]*fA2 - mtx.m[C3_R3]*fA1 ) * invDet;
		m[C2_R2] = ( + mtx.m[C0_R3]*fA4 - mtx.m[C1_R3]*fA2 + mtx.m[C3_R3]*fA0 ) * invDet;
		m[C2_R3] = ( - mtx.m[C0_R3]*fA3 + mtx.m[C1_R3]*fA1 - mtx.m[C2_R3]*fA0 ) * invDet;
		m[C3_R0] = ( - mtx.m[C1_R2]*fA5 + mtx.m[C2_R2]*fA4 - mtx.m[C3_R2]*fA3 ) * invDet;
		m[C3_R1] = ( + mtx.m[C0_R2]*fA5 - mtx.m[C2_R2]*fA2 + mtx.m[C3_R2]*fA1 ) * invDet;
		m[C3_R2] = ( - mtx.m[C0_R2]*fA4 + mtx.m[C1_R2]*fA2 - mtx.m[C3_R2]*fA0 ) * invDet;
		m[C3_R3] = ( + mtx.m[C0_R2]*fA3 - mtx.m[C1_R2]*fA1 + mtx.m[C2_R2]*fA0 ) * invDet;

		// Inverse found!
		return true;
	}

	//----------------------------------------------------------------//
	bool IsIdent () const {
		
		if ( !(( m [ C0_R0 ] == 1.0f ) && ( m [ C1_R1 ] == 1.0f ) && ( m [ C2_R2 ] == 1.0f ) && ( m [ C3_R3 ] == 1.0f ))) return false;
		if ( !(( m [ C1_R0 ] == 0.0f ) && ( m [ C2_R0 ] == 0.0f ) && ( m [ C3_R0 ] == 0.0f ))) return false;
		if ( !(( m [ C0_R1 ] == 0.0f ) && ( m [ C2_R1 ] == 0.0f ) && ( m [ C3_R1 ] == 0.0f ))) return false;
		if ( !(( m [ C0_R2 ] == 0.0f ) && ( m [ C1_R2 ] == 0.0f ) && ( m [ C3_R2 ] == 0.0f ))) return false;
		if ( !(( m [ C0_R3 ] == 0.0f ) && ( m [ C1_R3 ] == 0.0f ) && ( m [ C2_R3 ] == 0.0f ))) return false;
		
		return true;
	}

	//----------------------------------------------------------------//
	bool IsSame ( const USMetaMatrix4x4 < TYPE >& compare ) const {
		
		return ( memcmp ( m, compare.m, sizeof ( m )) == 0.0f );
	}

	//----------------------------------------------------------------//
	void Multiply (	const USMetaMatrix4x4 < TYPE >& mtx2, const USMetaMatrix4x4 < TYPE >& mtx1 ) {

		m[C0_R0]	=	( mtx1.m[C0_R0] * mtx2.m[C0_R0] )	+
						( mtx1.m[C1_R0] * mtx2.m[C0_R1] )	+
						( mtx1.m[C2_R0] * mtx2.m[C0_R2] )	+
						( mtx1.m[C3_R0] * mtx2.m[C0_R3] );
		
		m[C0_R1]	=	( mtx1.m[C0_R1] * mtx2.m[C0_R0] )	+
						( mtx1.m[C1_R1] * mtx2.m[C0_R1] )	+
						( mtx1.m[C2_R1] * mtx2.m[C0_R2] )	+
						( mtx1.m[C3_R1] * mtx2.m[C0_R3] );
		
		m[C0_R2]	=	( mtx1.m[C0_R2] * mtx2.m[C0_R0] )	+
						( mtx1.m[C1_R2] * mtx2.m[C0_R1] )	+
						( mtx1.m[C2_R2] * mtx2.m[C0_R2] )	+
						( mtx1.m[C3_R2] * mtx2.m[C0_R3] );
		
		m[C0_R3]	=	( mtx1.m[C0_R3] * mtx2.m[C0_R0] )	+
						( mtx1.m[C1_R3] * mtx2.m[C0_R1] )	+
						( mtx1.m[C2_R3] * mtx2.m[C0_R2] )	+
						( mtx1.m[C3_R3] * mtx2.m[C0_R3] );

		m[C1_R0]	=	( mtx1.m[C0_R0] * mtx2.m[C1_R0] )	+
						( mtx1.m[C1_R0] * mtx2.m[C1_R1] )	+
						( mtx1.m[C2_R0] * mtx2.m[C1_R2] )	+
						( mtx1.m[C3_R0] * mtx2.m[C1_R3] );
		
		m[C1_R1]	=	( mtx1.m[C0_R1] * mtx2.m[C1_R0] )	+
						( mtx1.m[C1_R1] * mtx2.m[C1_R1] )	+
						( mtx1.m[C2_R1] * mtx2.m[C1_R2] )	+
						( mtx1.m[C3_R1] * mtx2.m[C1_R3] );
		
		m[C1_R2]	=	( mtx1.m[C0_R2] * mtx2.m[C1_R0] )	+
						( mtx1.m[C1_R2] * mtx2.m[C1_R1] )	+
						( mtx1.m[C2_R2] * mtx2.m[C1_R2] )	+
						( mtx1.m[C3_R2] * mtx2.m[C1_R3] );
		
		m[C1_R3]	=	( mtx1.m[C0_R3] * mtx2.m[C1_R0] )	+
						( mtx1.m[C1_R3] * mtx2.m[C1_R1] )	+
						( mtx1.m[C2_R3] * mtx2.m[C1_R2] )	+
						( mtx1.m[C3_R3] * mtx2.m[C1_R3] );
		
		m[C2_R0]	=	( mtx1.m[C0_R0] * mtx2.m[C2_R0] )	+
						( mtx1.m[C1_R0] * mtx2.m[C2_R1] )	+
						( mtx1.m[C2_R0] * mtx2.m[C2_R2] )	+
						( mtx1.m[C3_R0] * mtx2.m[C2_R3] );
		
		m[C2_R1]	=	( mtx1.m[C0_R1] * mtx2.m[C2_R0] )	+
						( mtx1.m[C1_R1] * mtx2.m[C2_R1] )	+
						( mtx1.m[C2_R1] * mtx2.m[C2_R2] )	+
						( mtx1.m[C3_R1] * mtx2.m[C2_R3] );
		
		m[C2_R2]	=	( mtx1.m[C0_R2] * mtx2.m[C2_R0] )	+
						( mtx1.m[C1_R2] * mtx2.m[C2_R1] )	+
						( mtx1.m[C2_R2] * mtx2.m[C2_R2] )	+
						( mtx1.m[C3_R2] * mtx2.m[C2_R3] );
		
		m[C2_R3]	=	( mtx1.m[C0_R3] * mtx2.m[C2_R0] )	+
						( mtx1.m[C1_R3] * mtx2.m[C2_R1] )	+
						( mtx1.m[C2_R3] * mtx2.m[C2_R2] )	+
						( mtx1.m[C3_R3] * mtx2.m[C2_R3] );
		
		m[C3_R0]	=	( mtx1.m[C0_R0] * mtx2.m[C3_R0] )	+
						( mtx1.m[C1_R0] * mtx2.m[C3_R1] )	+
						( mtx1.m[C2_R0] * mtx2.m[C3_R2] )	+
						( mtx1.m[C3_R0] * mtx2.m[C3_R3] );
		
		m[C3_R1]	=	( mtx1.m[C0_R1] * mtx2.m[C3_R0] )	+
						( mtx1.m[C1_R1] * mtx2.m[C3_R1] )	+
						( mtx1.m[C2_R1] * mtx2.m[C3_R2] )	+
						( mtx1.m[C3_R1] * mtx2.m[C3_R3] );
		
		m[C3_R2]	=	( mtx1.m[C0_R2] * mtx2.m[C3_R0] )	+
						( mtx1.m[C1_R2] * mtx2.m[C3_R1] )	+
						( mtx1.m[C2_R2] * mtx2.m[C3_R2] )	+
						( mtx1.m[C3_R2] * mtx2.m[C3_R3] );
		
		m[C3_R3]	=	( mtx1.m[C0_R3] * mtx2.m[C3_R0] )	+
						( mtx1.m[C1_R3] * mtx2.m[C3_R1] )	+
						( mtx1.m[C2_R3] * mtx2.m[C3_R2] )	+
						( mtx1.m[C3_R3] * mtx2.m[C3_R3] );
	}

	//----------------------------------------------------------------//
	void Ortho ( TYPE xs, TYPE ys, TYPE zn, TYPE zf ) {
		
		m [ C0_R0 ] = xs;
		m [ C0_R1 ] = 0;
		m [ C0_R2 ] = 0;
		m [ C0_R3 ] = 0;
		
		m [ C1_R0 ] = 0;
		m [ C1_R1 ] = ys;
		m [ C1_R2 ] = 0;
		m [ C1_R3 ] = 0;
		
		m [ C2_R0 ] = 0;
		m [ C2_R1 ] = 0;
		m [ C2_R2 ] = -2 / ( zf - zn );
		m [ C2_R3 ] = 0;
		
		m [ C3_R0 ] = 0;
		m [ C3_R1 ] = 0;
		m [ C3_R2 ] = -( zf + zn ) / ( zf - zn );
		m [ C3_R3 ] = 1;
	}

	//----------------------------------------------------------------//
	void Prepend ( const USMetaMatrix4x4 < TYPE >& mtx ) {

		USMetaMatrix4x4 < TYPE > temp;
		temp.Multiply (	mtx, *this );
		this->Init ( temp );
	}

	//----------------------------------------------------------------//
	void Perspective ( TYPE xs, TYPE ys, TYPE zn, TYPE zf ) {
		
		m [ C0_R0 ] = xs;
		m [ C0_R1 ] = 0;
		m [ C0_R2 ] = 0;
		m [ C0_R3 ] = 0;
		
		m [ C1_R0 ] = 0;
		m [ C1_R1 ] = ys;
		m [ C1_R2 ] = 0;
		m [ C1_R3 ] = 0;
		
		m [ C2_R0 ] = 0;
		m [ C2_R1 ] = 0;
		m [ C2_R2 ] = ( zf + zn ) / ( zn - zf );
		m [ C2_R3 ] = -1;
		
		m [ C3_R0 ] = 0;
		m [ C3_R1 ] = 0;
		m [ C3_R2 ] = ( 2 * zn * zf ) / ( zn - zf );
		m [ C3_R3 ] = 0;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Project ( USMetaVec3D < PARAM_TYPE >& vec ) const {
		
		PARAM_TYPE w =	(( PARAM_TYPE )m[C0_R3] * vec.mX ) +
						(( PARAM_TYPE )m[C1_R3] * vec.mY ) +
						(( PARAM_TYPE )m[C2_R3] * vec.mZ ) +
						(( PARAM_TYPE )m[C3_R3]);
		
		this->Transform ( vec );
		
		vec.mX /= w;
		vec.mY /= w;
		vec.mZ /= w;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Project ( USMetaVec4D < PARAM_TYPE >& vec ) const {

		this->Transform ( vec );
		
		PARAM_TYPE w = vec.mW;
		
		vec.mX /= w;
		vec.mY /= w;
		vec.mZ /= w;
		vec.mW /= w;
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
		m[C0_R3]	= 0;

		m[C1_R0]	= (	c2 * x * y ) - ( z * s );
		m[C1_R1]	= c	+ (	c2 * y * y );
		m[C1_R2]	= (	c2 * y * z ) + ( x * s );
		m[C1_R3]	= 0;
		
		m[C2_R0]	= (	c2 * x * z ) + ( y * s );
		m[C2_R1]	= (	c2 * y * z ) - ( x * s );
		m[C2_R2]	= c	* (	c2 * z * z );
		m[C2_R3]	= 0;
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
		m[C3_R3]	= 1;
		}

	//----------------------------------------------------------------//
	void RotateX ( TYPE radians ) {

		m[C0_R0]	= 1;
		m[C0_R1]	= 0;
		m[C0_R2]	= 0;
		m[C0_R3]	= 0;
		
		m[C1_R0]	= 0;
		m[C1_R1]	= Cos (	radians	);
		m[C1_R2]	= Sin (	radians	);
		m[C1_R3]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= -m[C1_R2];	// -sin
		m[C2_R2]	= m[C1_R1];	// cos
		m[C2_R3]	= 0;
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	void RotateY ( TYPE radians ) {

		m[C0_R0]	= Cos (	radians	);
		m[C0_R1]	= 0;
		m[C0_R2]	= -Sin ( radians );
		m[C0_R3]	= 0;
		
		m[C1_R0]	= 0;
		m[C1_R1]	= 1;
		m[C1_R2]	= 0;
		m[C1_R3]	= 0;
		
		m[C2_R0]	= -m[C0_R2];	// sin
		m[C2_R1]	= 0;
		m[C2_R2]	= m[C0_R0];	// cos
		m[C2_R3]	= 0;
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	void RotateZ ( TYPE radians ) {

		m[C0_R0]	= Cos (	radians	);
		m[C0_R1]	= Sin (	radians	);
		m[C0_R2]	= 0;
		m[C0_R3]	= 0;
		
		m[C1_R0]	= -m[C0_R1];	// -sin
		m[C1_R1]	= m[C0_R0];	// cos
		m[C1_R2]	= 0;
		m[C1_R3]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= 0;
		m[C2_R2]	= 1;
		m[C2_R3]	= 0;
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	void Scale ( USMetaVec3D < TYPE >& scl ) {

		Scale (	scl.mX,	scl.mY,	scl.mZ );
	}

	//----------------------------------------------------------------//
	void Scale ( TYPE x, TYPE y, TYPE z ) {

		m[C0_R0]	= x;
		m[C0_R1]	= 0;
		m[C0_R2]	= 0;
		m[C0_R3]	= 0;
		
		m[C1_R0]	= 0;
		m[C1_R1]	= y;
		m[C1_R2]	= 0;
		m[C1_R3]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= 0;
		m[C2_R2]	= z;
		m[C2_R3]	= 0;
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	void ScRoTr	( USMetaVec3D < TYPE >& sc, USMetaVec3D < TYPE >& ro,	USMetaVec3D < TYPE >& tr ) {

		TYPE cx = Cos ( ro.mX );
		TYPE sx = Sin ( ro.mX );
		TYPE cy = Cos ( ro.mY );
		TYPE sy = Sin ( ro.mY );
		TYPE cz = Cos ( ro.mZ );
		TYPE sz = Sin ( ro.mZ );

		m[C0_R0]	= cz*cy*sc.mX;
		m[C0_R1]	= sz*cy*sc.mX;
		m[C0_R2]	= -sy*sc.mX;
		m[C0_R3]	= 0;

		m[C1_R0]	= ((cz*sy*sx)+(-sz*cx))*sc.mY;
		m[C1_R1]	= ((sz*sy*sx)+(cz*cx))*sc.mY;
		m[C1_R2]	= cy*sx*sc.mY;
		m[C1_R3]	= 0;

		m[C2_R0]	= ((cz*sy*cx)+(-sz*-sx))*sc.mZ;
		m[C2_R1]	= ((sz*sy*cx)+(cz*-sx))*sc.mZ;
		m[C2_R2]	= cy*cx*sc.mZ;
		m[C2_R3]	= 0;

		m[C3_R0]	= tr.mX;
		m[C3_R1]	= tr.mY;
		m[C3_R2]	= tr.mZ;
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	void ScRoTrInv ( USMetaVec3D < TYPE >& sc, USMetaVec3D < TYPE >& ro, USMetaVec3D < TYPE >&	tr ) {

		TYPE cx = Cos ( -ro.mX	);
		TYPE sx = Sin ( -ro.mX	);
		TYPE cy = Cos ( -ro.mY	);
		TYPE sy = Sin ( -ro.mY	);
		TYPE cz = Cos ( -ro.mZ	);
		TYPE sz = Sin ( -ro.mZ	);

		TYPE x	= -tr.mX;
		TYPE y	= -tr.mY;
		TYPE z	= -tr.mZ;

		USMetaVec3D < TYPE >	invScl;
		invScl.mX =	1 / sc.mX;
		invScl.mY =	1 / sc.mY;
		invScl.mZ =	1 / sc.mZ;

		m[C0_R0]	= invScl.mX*cy*cz;
		m[C0_R1]	= invScl.mY*((cx*sz)+(-sx*-sy*cz));
		m[C0_R2]	= invScl.mZ*((sx*sz)+(cx*-sy*cz));
		m[C0_R3]	= 0;

		m[C1_R0]	= invScl.mX*cy*-sz;
		m[C1_R1]	= invScl.mY*((cx*cz)+(-sx*-sy*-sz));
		m[C1_R2]	= invScl.mZ*((sx*cz)+(cx*-sy*-sz));
		m[C1_R3]	= 0;

		m[C2_R0]	= invScl.mX*sy;
		m[C2_R1]	= invScl.mY*-sx*cy;
		m[C2_R2]	= invScl.mZ*cx*cy;
		m[C2_R3]	= 0;

		m[C3_R0]	= invScl.mX*((cy*cz*x)+(cy*-sz*y)+(sy*z));
		m[C3_R1]	= invScl.mY*((((cx*sz)+(-sx*-sy*cz))*x)+(((cx*cz)+(-sx*-sy*-sz))*y)+(-sx*cy*z));
		m[C3_R2]	= invScl.mZ*((((sx*sz)+(cx*-sy*cz))*x)+(((sx*cz)+(cx*-sy*-sz))*y)+(cx*cy*z));
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	void Shear ( TYPE yx, TYPE zx, TYPE xy, TYPE zy, TYPE xz, TYPE yz ) {
		
		m[C0_R0]	= 1;
		m[C0_R1]	= yx;
		m[C0_R2]	= zx;
		m[C0_R3]	= 0;
		
		m[C1_R0]	= xy;
		m[C1_R1]	= 1;
		m[C1_R2]	= zy;
		m[C1_R3]	= 0;
		
		m[C2_R0]	= xz;
		m[C2_R1]	= yz;
		m[C2_R2]	= 1;
		m[C2_R3]	= 0;
		
		m[C3_R0]	= 0;
		m[C3_R1]	= 0;
		m[C3_R2]	= 0;
		m[C3_R3]	= 1;
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
					(( PARAM_TYPE )m[C3_R0] );
		
		y =			(( PARAM_TYPE )m[C0_R1] *	vec.mX ) +
					(( PARAM_TYPE )m[C1_R1] *	vec.mY ) +
					(( PARAM_TYPE )m[C2_R1] *	vec.mZ ) +
					(( PARAM_TYPE )m[C3_R1] );
		
		vec.mZ =	(( PARAM_TYPE )m[C0_R2] *	vec.mX ) +
					(( PARAM_TYPE )m[C1_R2] *	vec.mY ) +
					(( PARAM_TYPE )m[C2_R2] *	vec.mZ ) +
					(( PARAM_TYPE )m[C3_R2] );
		
		vec.mX = x;
		vec.mY = y;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Transform ( USMetaVec4D < PARAM_TYPE >& vec ) const {

		PARAM_TYPE x;
		PARAM_TYPE y;
		PARAM_TYPE z;
		PARAM_TYPE w;
		
		x =		(( PARAM_TYPE )m[C0_R0] * vec.mX ) +
				(( PARAM_TYPE )m[C1_R0] * vec.mY ) +
				(( PARAM_TYPE )m[C2_R0] * vec.mZ ) +
				(( PARAM_TYPE )m[C3_R0] * vec.mW );
		
		y =		(( PARAM_TYPE )m[C0_R1] * vec.mX ) +
				(( PARAM_TYPE )m[C1_R1] * vec.mY ) +
				(( PARAM_TYPE )m[C2_R1] * vec.mZ ) +
				(( PARAM_TYPE )m[C3_R1] * vec.mW );
		
		z =		(( PARAM_TYPE )m[C0_R2] * vec.mX ) +
				(( PARAM_TYPE )m[C1_R2] * vec.mY ) +
				(( PARAM_TYPE )m[C2_R2] * vec.mZ ) +
				(( PARAM_TYPE )m[C3_R2] * vec.mW );
		
		w =		(( PARAM_TYPE )m[C0_R3] * vec.mX ) +
				(( PARAM_TYPE )m[C1_R3] * vec.mY ) +
				(( PARAM_TYPE )m[C2_R3] * vec.mZ ) +
				(( PARAM_TYPE )m[C3_R3] * vec.mW );
		
		vec.mX = x;
		vec.mY = y;
		vec.mZ = z;
		vec.mW = w;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Transform ( USMetaRect < PARAM_TYPE >& rect ) const {

		USVec2D point;
		USRect newRect;
		
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
		
			TYPE xform_mat[16] = { m[C0_R0], m[C0_R1], m[C0_R2], m[C0_R3], //column 1
								   m[C1_R0], m[C1_R1], m[C1_R2], m[C1_R3], //column 2
								   m[C2_R0], m[C2_R1], m[C2_R2], m[C2_R3], //column 3
								   m[C3_R0], m[C3_R1], m[C3_R2], m[C3_R3] };			 //column 4
								   
			TYPE point_mat[16] = { quad[0].mX, quad[0].mY, 0, 1,   //column 1
								   quad[1].mX, quad[1].mY, 0, 1,   //column 2
								   quad[2].mX, quad[2].mY, 0, 1,   //column 3
								   quad[3].mX, quad[3].mY, 0, 1 }; //column 4
								   
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
	template < typename PARAM_TYPE>
	void TransformQuad ( USMetaVec4D < PARAM_TYPE >* quad ) const {
	
		this->Transform ( quad [ 0 ]);
		this->Transform ( quad [ 1 ]);
		this->Transform ( quad [ 2 ]);
		this->Transform ( quad [ 3 ]);
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
	void Translate ( USMetaVec3D < TYPE >& trn ) {

		Translate (	trn.mX,	trn.mY,	trn.mZ );
	}

	//----------------------------------------------------------------//
	void Translate ( TYPE x, TYPE y, TYPE z ) {

		m[C0_R0]	= 1;
		m[C0_R1]	= 0;
		m[C0_R2]	= 0;
		m[C0_R3]	= 0;
		
		m[C1_R0]	= 0;
		m[C1_R1]	= 1;
		m[C1_R2]	= 0;
		m[C1_R3]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= 0;
		m[C2_R2]	= 1;
		m[C2_R3]	= 0;
		
		m[C3_R0]	= x;
		m[C3_R1]	= y;
		m[C3_R2]	= z;
		m[C3_R3]	= 1;
	}

	//----------------------------------------------------------------//
	void Transpose () {

		TYPE swap;
		
		swap		= m[C0_R1];
		m[C0_R1]	= m[C1_R0];
		m[C1_R0]	= swap;
		
		swap		= m[C0_R2];
		m[C0_R2]	= m[C2_R0];
		m[C2_R0]	= swap;
		
		swap		= m[C0_R3];
		m[C0_R3]	= m[C3_R0];
		m[C3_R0]	= swap;

		swap		= m[C1_R2];
		m[C1_R2]	= m[C2_R1];
		m[C2_R1]	= swap;
		
		swap		= m[C1_R3];
		m[C1_R3]	= m[C3_R1];
		m[C3_R1]	= swap;
		
		swap		= m[C2_R3];
		m[C2_R3]	= m[C3_R2];
		m[C3_R2]	= swap;
	}
	
	//----------------------------------------------------------------//
	USMetaMatrix4x4 () {
	}
};

typedef USMetaMatrix4x4 < float > USMatrix4x4;

#endif