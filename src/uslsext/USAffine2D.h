// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USAFFINE2D_H
#define USAFFINE2D_H

#include <uslsext/USMathConsts.h>
#include <uslsext/USMatrix.h>
#include <uslsext/USTrig.h>

//================================================================//
// USMetaAffine2D
//================================================================//
template < typename TYPE >
class USMetaAffine2D {
public:

	enum {
		C0_R0	=	0,
		C0_R1,
		
		C1_R0,
		C1_R1,
		
		C2_R0,
		C2_R1,
	};

	TYPE	m [ 6 ];

	//----------------------------------------------------------------//
	void Append ( const USMetaAffine2D < TYPE >& mtx ) {

		USMetaAffine2D < TYPE > temp;
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

		return	m [ C0_R0 ] * m [ C1_R1 ] - m [ C0_R1 ] * m [ C1_R0 ];
	}

	//----------------------------------------------------------------//
	TYPE GetElement ( int c, int r ) const {

		return m [ ( c * 3 ) + r ];
	}

	//----------------------------------------------------------------//
	USMetaVec2D < TYPE > GetHeading () const {

		USVec2D heading;
	
		heading.mX = m [ USAffine2D::C0_R0 ];
		heading.mY = m [ USAffine2D::C0_R1 ];

		heading.NormSafe ();
		
		return heading;
	}

	//----------------------------------------------------------------//
	TYPE GetRot () const {

		float rot = ( TYPE )( atan2 ( m [ USAffine2D::C0_R0 ], m [ USAffine2D::C0_R1 ]) * R2D );
		return rot;
	}

	//----------------------------------------------------------------//
	USMetaVec2D < TYPE > GetStretch () const {

		USMetaVec2D < TYPE > stretch;
		USMetaVec2D < TYPE > axis;

		// x axis
		axis.mX =	m [ C0_R0 ];
		axis.mY =	m [ C0_R1 ];
		
		stretch.mX = axis.Length ();

		// y axis
		axis.mX =	m [ C1_R0 ];
		axis.mY =	m [ C1_R1 ];

		stretch.mY = axis.Length ();
		
		return stretch;
	}

	//----------------------------------------------------------------//
	USMetaVec2D < TYPE > GetScale () const {

		USMetaVec2D < TYPE > vec;

		vec.mX = ( TYPE )m [ C0_R0 ];
		vec.mY = ( TYPE )m [ C1_R1 ];
		
		return vec;
	}

	//----------------------------------------------------------------//
	USMetaVec2D < TYPE > GetTranslation () const {

		USMetaVec2D < TYPE > vec;

		vec.mX = ( TYPE )m [ C2_R0 ];
		vec.mY = ( TYPE )m [ C2_R1 ];
		
		return vec;
	}

	//----------------------------------------------------------------//
	void Ident () {

		m [ C0_R0 ]	= 1;
		m [ C0_R1 ]	= 0;
		
		m [ C1_R0 ]	= 0;
		m [ C1_R1 ]	= 1;
		
		m [ C2_R0 ]	= 0;
		m [ C2_R1 ]	= 0;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const USMetaAffine2D < PARAM_TYPE >& mtx ) {

		m [ C0_R0 ]	= ( TYPE )mtx.m [ C0_R0 ];
		m [ C0_R1 ]	= ( TYPE )mtx.m [ C0_R1 ];
		
		m [ C1_R0 ]	= ( TYPE )mtx.m [ C1_R0 ];
		m [ C1_R1 ]	= ( TYPE )mtx.m [ C1_R1 ];
		
		m [ C2_R0 ]	= ( TYPE )mtx.m [ C2_R0 ];
		m [ C2_R1 ]	= ( TYPE )mtx.m [ C2_R1 ];
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const USMetaMatrix2D < PARAM_TYPE >& mtx ) {

		m[C0_R0]	= ( TYPE )mtx.m[MatrixElem2D::C0_R0];
		m[C0_R1]	= ( TYPE )mtx.m[MatrixElem2D::C0_R1];
		
		m[C1_R0]	= ( TYPE )mtx.m[MatrixElem2D::C1_R0];
		m[C1_R1]	= ( TYPE )mtx.m[MatrixElem2D::C1_R1];
		
		m[C2_R0]	= ( TYPE )mtx.m[MatrixElem2D::C2_R0];
		m[C2_R1]	= ( TYPE )mtx.m[MatrixElem2D::C2_R1];
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const USMetaMatrix3D < PARAM_TYPE >& mtx ) {

		m [ C0_R0 ]	= ( TYPE )mtx.m[MatrixElem3D::C0_R0];
		m [ C0_R1 ]	= ( TYPE )mtx.m[MatrixElem3D::C0_R1];
		
		m [ C1_R0 ]	= ( TYPE )mtx.m[MatrixElem3D::C1_R0];
		m [ C1_R1 ]	= ( TYPE )mtx.m[MatrixElem3D::C1_R1];
		
		m [ C2_R0 ]	= ( TYPE )mtx.m[MatrixElem3D::C3_R0];
		m [ C2_R1 ]	= ( TYPE )mtx.m[MatrixElem3D::C3_R1];
	}

	//----------------------------------------------------------------//
	bool Inverse () {

		USMetaAffine2D < TYPE > temp	= *this;
		return Inverse ( temp );
	}

	//----------------------------------------------------------------//
	bool Inverse ( const USMetaAffine2D < TYPE >& mtx ) {

		TYPE det = mtx.Determinant ();
		
		// If determinant == 0, there is no inverse
		if ( det == 0 ) {
			*this = mtx;
			return false;
		}

		TYPE invDet = 1 / det;
	   
		m [ C0_R0 ] = invDet * ( mtx.m [ C1_R1 ]);
		m [ C0_R1 ] = invDet * ( -mtx.m [ C0_R1 ]);
	   
		m [ C1_R0 ] = invDet * ( -mtx.m [ C1_R0 ]);
		m [ C1_R1 ] = invDet * ( mtx.m [ C0_R0 ]);
	   
		m [ C2_R0 ] = invDet * ( mtx.m [ C1_R0 ] * mtx.m [ C2_R1 ] - mtx.m [ C1_R1 ] * mtx.m [ C2_R0 ]);
		m [ C2_R1 ] = invDet * ( mtx.m [ C0_R1 ] * mtx.m [ C2_R0 ] - mtx.m [ C0_R0 ] * mtx.m [ C2_R1 ]);

		return true;
	}

	//----------------------------------------------------------------//
	void Multiply ( const USMetaAffine2D < TYPE >& mtx2, const USMetaAffine2D < TYPE >& mtx1 ) {

		m [ C0_R0 ]	=	( mtx1.m [ C0_R0 ] * mtx2.m [ C0_R0 ])	+
						( mtx1.m [ C1_R0 ] * mtx2.m [ C0_R1 ]);
		
		m [ C0_R1 ]	=	( mtx1.m [ C0_R1 ] * mtx2.m [ C0_R0 ])	+
						( mtx1.m [ C1_R1 ] * mtx2.m [ C0_R1 ]);

		m [ C1_R0 ]	=	( mtx1.m [ C0_R0 ] * mtx2.m [ C1_R0 ])	+
						( mtx1.m [ C1_R0 ] * mtx2.m [ C1_R1 ]);
		
		m [ C1_R1 ]	=	( mtx1.m [ C0_R1 ] * mtx2.m [ C1_R0 ])	+
						( mtx1.m [ C1_R1 ] * mtx2.m [ C1_R1 ]);
		
		m [ C2_R0 ]	=	( mtx1.m [ C0_R0 ] * mtx2.m [ C2_R0 ])	+
						( mtx1.m [ C1_R0 ] * mtx2.m [ C2_R1 ])	+
						( mtx1.m [ C2_R0 ]);
		
		m [ C2_R1 ]	=	( mtx1.m [ C0_R1 ] * mtx2.m [ C2_R0 ])	+
						( mtx1.m [ C1_R1 ] * mtx2.m [ C2_R1 ])	+
						( mtx1.m [ C2_R1 ]);
	}

	//----------------------------------------------------------------//
	void Prepend ( const USMetaAffine2D < TYPE >& mtx ) {

		USMetaAffine2D < TYPE > temp;
		temp.Multiply (	mtx, *this );
		this->Init ( temp );
	}

	//----------------------------------------------------------------//
	void Rotate ( TYPE radians ) {

		m [ C0_R0 ]	= Cos (	radians	);
		m [ C0_R1 ]	= Sin (	radians	);
		
		m [ C1_R0 ]	= -m [ C0_R1 ];		// -sin
		m [ C1_R1 ]	= m [ C0_R0 ];		// cos
		
		m [ C2_R0 ]	= 0;
		m [ C2_R1 ]	= 0;
	}

	//----------------------------------------------------------------//
	void Scale ( const USMetaVec2D < TYPE >& scl ) {

		this->Scale ( scl.mX, scl.mY );
	}

	//----------------------------------------------------------------//
	void Scale ( TYPE x, TYPE y ) {

		m [ C0_R0 ]	= x;
		m [ C0_R1 ]	= 0;
		
		m [ C1_R0 ]	= 0;
		m [ C1_R1 ]	= y;
		
		m [ C2_R0 ]	= 0;
		m [ C2_R1 ]	= 0;
	}

	//----------------------------------------------------------------//
	void ScRoTr ( const USMetaVec2D < TYPE >& sc, TYPE radians, const USMetaVec2D < TYPE >& tr ) {

		this->ScRoTr ( sc.mX, sc.mY, radians, tr.mX, tr.mY );
	}

	//----------------------------------------------------------------//
	void ScRoTr ( TYPE xSc, TYPE ySc, TYPE radians, TYPE xTr, TYPE yTr ) {

		TYPE cz = Cos ( radians );
		TYPE sz = Sin ( radians );

		m [ C0_R0 ]	= cz * xSc;
		m [ C0_R1 ]	= sz * xSc;

		m [ C1_R0 ]	= -sz * ySc;
		m [ C1_R1 ]	= cz * ySc;

		m [ C2_R0 ]	= xTr;
		m [ C2_R1 ]	= yTr;
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

		m [ C0_R0 ]	= xSc * cz;
		m [ C0_R1 ]	= ySc * sz;

		m [ C1_R0 ]	= xSc * -sz;
		m [ C1_R1 ]	= ySc * cz;

		m [ C2_R0 ]	= ( xSc * cz * xTr ) + ( -sz * yTr );
		m [ C2_R1 ]	= ( ySc * sz * xTr ) + ( cz * yTr );
	}

	//----------------------------------------------------------------//
	void Shear ( TYPE yx, TYPE xy ) {

		m [ C0_R0 ]	= 1;
		m [ C0_R1 ]	= xy;
		
		m [ C1_R0 ]	= yx;
		m [ C1_R1 ]	= 1;
		
		m [ C2_R0 ]	= 0;
		m [ C2_R1 ]	= 0;
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Transform ( USMetaVec2D < PARAM_TYPE >& point ) const {
		
		TYPE x =	( m [ C0_R0 ] * ( TYPE )point.mX ) +
					( m [ C1_R0 ] * ( TYPE )point.mY ) +
					( m [ C2_R0 ]);
		
		TYPE y =	( m [ C0_R1 ] * ( TYPE )point.mX ) +
					( m [ C1_R1 ] * ( TYPE )point.mY ) +
					( m [ C2_R1 ]);
		
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
		
		TYPE x =	( m [ C0_R0 ] * ( TYPE )point.mX ) +
					( m [ C1_R0 ] * ( TYPE )point.mY );
		
		TYPE y =	( m [ C0_R1 ] * ( TYPE )point.mX ) +
					( m [ C1_R1 ] * ( TYPE )point.mY );
		
		point.mX = ( PARAM_TYPE )x;
		point.mY = ( PARAM_TYPE )y;
	}

	//----------------------------------------------------------------//
	void Translate ( const USMetaVec2D < TYPE >& trn ) {

		this->Translate ( trn.mX, trn.mY );
	}

	//----------------------------------------------------------------//
	void Translate ( TYPE x, TYPE y ) {

		m [ C0_R0 ]	= 1;
		m [ C0_R1 ]	= 0;
		
		m [ C1_R0 ]	= 0;
		m [ C1_R1 ]	= 1;
		
		m [ C2_R0 ]	= x;
		m [ C2_R1 ]	= y;
	}
	
	//----------------------------------------------------------------//
	USMetaAffine2D () {
	}
};

typedef USMetaAffine2D < float > USAffine2D;
typedef USMetaAffine2D < double > USAffine2D64;

#endif
