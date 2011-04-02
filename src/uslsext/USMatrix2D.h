// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USMATRIX2D_H
#define USMATRIX2D_H

#include <uslsext/USMatrix.h>
#include <uslsext/USTrig.h>

//================================================================//
// USMetaMatrix2D
//================================================================//
template < typename TYPE >
class USMetaMatrix2D {
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
	};

	TYPE	m [ 9 ];

	//----------------------------------------------------------------//
	void Append ( const USMetaMatrix2D < TYPE >& mtx ) {

		USMetaMatrix2D < TYPE > temp;
		temp.Multiply (	*this, mtx );
		this->Init ( temp );
	}

	//----------------------------------------------------------------//
	void GetBasis ( USMetaVec2D < TYPE >& xAxis, USMetaVec2D < TYPE >& yAxis ) const {
	
		xAxis.mX = m [ C0_R0 ];
		xAxis.mY = m [ C0_R1 ];
		
		yAxis.mX = m [ C1_R0 ];
		yAxis.mY = m [ C1_R1 ];
	}

	//----------------------------------------------------------------//
	TYPE Determinant () const {

		return	m[C0_R0] * ( m[C1_R1] * m[C2_R2] - m[C2_R1] * m[C1_R2] ) - 
				m[C0_R1] * ( m[C1_R0] * m[C2_R2] - m[C2_R0] * m[C1_R2] ) +
				m[C0_R2] * ( m[C1_R0] * m[C2_R1] - m[C2_R0] * m[C1_R1] );
	}

	//----------------------------------------------------------------//
	TYPE GetElement ( int c, int r ) const {

		return m [ ( c * 3 ) + r ];
	}

	//----------------------------------------------------------------//
	USMetaVec2D < TYPE > GetScale () const {

		USMetaVec2D < TYPE > vec;

		vec.mX = ( TYPE )m[C0_R0];
		vec.mY = ( TYPE )m[C1_R1];
		
		return vec;
	}

	//----------------------------------------------------------------//
	USMetaVec2D < TYPE > GetTranslation () const {

		USMetaVec2D < TYPE > vec;

		vec.mX = ( TYPE )m[C2_R0];
		vec.mY = ( TYPE )m[C2_R1];
		
		return vec;
	}

	//----------------------------------------------------------------//
	void Ident () {

		m[C0_R0]	= 1;
		m[C0_R1]	= 0;
		m[C0_R2]	= 0;
		
		m[C1_R0]	= 0;
		m[C1_R1]	= 1;
		m[C1_R2]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= 0;
		m[C2_R2]	= 1;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const USMetaAffine2D < PARAM_TYPE >& mtx ) {

		m[C0_R0]	= ( TYPE )mtx.m[AffineElem2D::C0_R0];
		m[C0_R1]	= ( TYPE )mtx.m[AffineElem2D::C0_R1];
		m[C0_R2]	= 0.0f;
		
		m[C1_R0]	= ( TYPE )mtx.m[AffineElem2D::C1_R0];
		m[C1_R1]	= ( TYPE )mtx.m[AffineElem2D::C1_R1];
		m[C1_R2]	= 0.0f;
		
		m[C2_R0]	= ( TYPE )mtx.m[AffineElem2D::C2_R0];
		m[C2_R1]	= ( TYPE )mtx.m[AffineElem2D::C2_R1];
		m[C2_R2]	= 1.0f;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const USMetaMatrix2D < PARAM_TYPE >& mtx ) {

		m[C0_R0]	= ( TYPE )mtx.m[C0_R0];
		m[C0_R1]	= ( TYPE )mtx.m[C0_R1];
		m[C0_R2]	= ( TYPE )mtx.m[C0_R2];
		
		m[C1_R0]	= ( TYPE )mtx.m[C1_R0];
		m[C1_R1]	= ( TYPE )mtx.m[C1_R1];
		m[C1_R2]	= ( TYPE )mtx.m[C1_R2];
		
		m[C2_R0]	= ( TYPE )mtx.m[C2_R0];
		m[C2_R1]	= ( TYPE )mtx.m[C2_R1];
		m[C2_R2]	= ( TYPE )mtx.m[C2_R2];
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const USMetaMatrix3D < PARAM_TYPE >& mtx ) {

		m[C0_R0]	= ( TYPE )mtx.m[MatrixElem3D::C0_R0];
		m[C0_R1]	= ( TYPE )mtx.m[MatrixElem3D::C0_R1];
		m[C0_R2]	= 0;
		
		m[C1_R0]	= ( TYPE )mtx.m[MatrixElem3D::C1_R0];
		m[C1_R1]	= ( TYPE )mtx.m[MatrixElem3D::C1_R1];
		m[C1_R2]	= 0;
		
		m[C2_R0]	= ( TYPE )mtx.m[MatrixElem3D::C3_R0];
		m[C2_R1]	= ( TYPE )mtx.m[MatrixElem3D::C3_R1];
		m[C2_R2]	= 1;
	}

	//----------------------------------------------------------------//
	bool Inverse () {

		USMetaMatrix2D < TYPE > temp	= *this;
		return Inverse ( temp );
	}

	//----------------------------------------------------------------//
	bool Inverse ( const USMetaMatrix2D < TYPE >& mtx ) {

		TYPE det = mtx.Determinant ();
		
		// If determinant == 0, there is no inverse
		if ( det == 0 ) {
			*this = mtx;
			return false;
		}

		TYPE invDet = 1 / det;
	   
		m[C0_R0] = invDet * ( mtx.m[C1_R1] * mtx.m[C2_R2] - mtx.m[C1_R2] * mtx.m[C2_R1] );
		m[C0_R1] = invDet * ( mtx.m[C0_R2] * mtx.m[C2_R1] - mtx.m[C0_R1] * mtx.m[C2_R2] );
		m[C0_R2] = invDet * ( mtx.m[C0_R1] * mtx.m[C1_R2] - mtx.m[C0_R2] * mtx.m[C1_R1] );
	   
		m[C1_R0] = invDet * ( mtx.m[C1_R2] * mtx.m[C2_R0] - mtx.m[C1_R0] * mtx.m[C2_R2] );
		m[C1_R1] = invDet * ( mtx.m[C0_R0] * mtx.m[C2_R2] - mtx.m[C0_R2] * mtx.m[C2_R0] );
		m[C1_R2] = invDet * ( mtx.m[C0_R2] * mtx.m[C1_R0] - mtx.m[C0_R0] * mtx.m[C1_R2] );
	   
		m[C2_R0] = invDet * ( mtx.m[C1_R0] * mtx.m[C2_R1] - mtx.m[C1_R1] * mtx.m[C2_R0] );
		m[C2_R1] = invDet * ( mtx.m[C0_R1] * mtx.m[C2_R0] - mtx.m[C0_R0] * mtx.m[C2_R1] );
		m[C2_R2] = invDet * ( mtx.m[C0_R0] * mtx.m[C1_R1] - mtx.m[C0_R1] * mtx.m[C1_R0] );

		return true;
	}

	//----------------------------------------------------------------//
	void Multiply ( const USMetaMatrix2D < TYPE >& mtx2, const USMetaMatrix2D < TYPE >& mtx1 ) {

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
	}

	//----------------------------------------------------------------//
	void Prepend ( const USMetaMatrix2D < TYPE >& mtx ) {

		USMetaMatrix2D < TYPE > temp;
		temp.Multiply (	mtx, *this );
		this->Init ( temp );
	}

	//----------------------------------------------------------------//
	void Rotate ( TYPE radians ) {

		m[C0_R0]	= Cos (	radians	);
		m[C0_R1]	= Sin (	radians	);
		m[C0_R2]	= 0;
		
		m[C1_R0]	= -m[C0_R1];	// -sin
		m[C1_R1]	= m[C0_R0];		// cos
		m[C1_R2]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= 0;
		m[C2_R2]	= 1;
	}

	//----------------------------------------------------------------//
	void Scale ( const USMetaVec2D < TYPE >& scl ) {

		this->Scale ( scl.mX, scl.mY );
	}

	//----------------------------------------------------------------//
	void Scale ( TYPE x, TYPE y ) {

		m[C0_R0]	= x;
		m[C0_R1]	= 0;
		m[C0_R2]	= 0;
		
		m[C1_R0]	= 0;
		m[C1_R1]	= y;
		m[C1_R2]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= 0;
		m[C2_R2]	= 1;
	}

	//----------------------------------------------------------------//
	void ScRoTr ( const USMetaVec2D < TYPE >& sc, TYPE radians, const USMetaVec2D < TYPE >& tr ) {

		this->ScRoTr ( sc.mX, sc.mY, radians, tr.mX, tr.mY );
	}

	//----------------------------------------------------------------//
	void ScRoTr ( TYPE xSc, TYPE ySc, TYPE radians, TYPE xTr, TYPE yTr ) {

		TYPE cz = Cos ( radians );
		TYPE sz = Sin ( radians );

		m[C0_R0]	= cz * xSc;
		m[C0_R1]	= sz * xSc;
		m[C0_R2]	= 0;

		m[C1_R0]	= -sz * ySc;
		m[C1_R1]	= cz * ySc;
		m[C1_R2]	= 0;

		m[C2_R0]	= xTr;
		m[C2_R1]	= yTr;
		m[C2_R2]	= 1;
	}

	//----------------------------------------------------------------//
	void ScRoTrInv ( const USMetaVec2D < TYPE >& sc, TYPE radians, const USMetaVec2D < TYPE >& tr ) {

		this->ScRoTrInv ( sc.mX, sc.mY, radians, tr.mX, tr.mY );
	}

	//----------------------------------------------------------------//
	void ScRoTrInv ( TYPE xSc, TYPE ySc, TYPE radians, TYPE xTr, TYPE yTr ) {

		TYPE cz = -Cos ( radians );
		TYPE sz = -Sin ( radians );

		xSc = 1 / xSc;
		ySc = 1 / ySc;

		xTr = -xTr;
		yTr = -yTr;

		m[C0_R0]	= xSc * cz;
		m[C0_R1]	= ySc * sz;
		m[C0_R2]	= 0;

		m[C1_R0]	= xSc * -sz;
		m[C1_R1]	= ySc * cz;
		m[C1_R2]	= 0;

		m[C2_R0]	= ( xSc * cz * xTr ) + ( -sz * yTr );
		m[C2_R1]	= ( ySc * sz * xTr ) + ( cz * yTr );
		m[C2_R2]	= 1;
	}

	//----------------------------------------------------------------//
	void Shear ( TYPE yx, TYPE xy ) {

		m[C0_R0]	= 1;
		m[C0_R1]	= xy;
		m[C0_R2]	= 0;
		
		m[C1_R0]	= yx;
		m[C1_R1]	= 1;
		m[C1_R2]	= 0;
		
		m[C2_R0]	= 0;
		m[C2_R1]	= 0;
		m[C2_R2]	= 1;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Transform ( USMetaVec2D < PARAM_TYPE >& point ) const {
		
		TYPE x =	( m[C0_R0 ] * ( TYPE )point.mX ) +
					( m[C1_R0 ] * ( TYPE )point.mY ) +
					( m[C2_R0 ]);
		
		TYPE y =	( m[C0_R1 ] * ( TYPE )point.mX ) +
					( m[C1_R1 ] * ( TYPE )point.mY ) +
					( m[C2_R1 ]);
		
		point.mX = ( PARAM_TYPE )x;
		point.mY = ( PARAM_TYPE )y;
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
	// Transforms w/o translation
	template < typename PARAM_TYPE >
	void TransformVec ( USMetaVec2D < PARAM_TYPE >& point ) const {
		
		TYPE x =	( m[C0_R0] * ( TYPE )point.mX ) +
					( m[C1_R0] * ( TYPE )point.mY );
		
		TYPE y =	( m[C0_R1] * ( TYPE )point.mX ) +
					( m[C1_R1] * ( TYPE )point.mY );
		
		point.mX = ( PARAM_TYPE )x;
		point.mY = ( PARAM_TYPE )y;
	}

	//----------------------------------------------------------------//
	void Translate ( const USMetaVec2D < TYPE >& trn ) {

		this->Translate ( trn.mX, trn.mY );
	}

	//----------------------------------------------------------------//
	void Translate ( TYPE x, TYPE y ) {

		m[C0_R0]	= 1;
		m[C0_R1]	= 0;
		m[C0_R2]	= 0;
		
		m[C1_R0]	= 0;
		m[C1_R1]	= 1;
		m[C1_R2]	= 0;
		
		m[C2_R0]	= x;
		m[C2_R1]	= y;
		m[C2_R2]	= 1;
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

		swap		= m[C1_R2];
		m[C1_R2]	= m[C2_R1];
		m[C2_R1]	= swap;
	}
	
	//----------------------------------------------------------------//
	USMetaMatrix2D () {
	}
};

typedef USMetaMatrix2D < float > USMatrix2D;
typedef USMetaMatrix2D < double > USMatrix2D64;

#endif
