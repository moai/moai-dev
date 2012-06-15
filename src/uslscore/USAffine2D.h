// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USAFFINE2D_H
#define USAFFINE2D_H

#include <uslscore/USMathConsts.h>
#include <uslscore/USMatrix.h>
#include <uslscore/USRect.h>
#include <uslscore/USTrig.h>
#include <uslscore/USVec2D.h>

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
		
		SIZE,
	};

	TYPE	m [ SIZE ];

	//----------------------------------------------------------------//
	void Append ( const USMetaAffine2D < TYPE >& mtx ) {

		USMetaAffine2D < TYPE > temp;
		temp.Multiply (	*this, mtx );
		this->Init ( temp );
	}

	//----------------------------------------------------------------//
	TYPE Determinant () const {

		return	m [ C0_R0 ] * m [ C1_R1 ] - m [ C0_R1 ] * m [ C1_R0 ];
	}

	//----------------------------------------------------------------//
	void GetBasis ( USMetaVec2D < TYPE >& xAxis, USMetaVec2D < TYPE >& yAxis ) const {
	
		xAxis.mX = m [ C0_R0 ];
		xAxis.mY = m [ C0_R1 ];
		
		yAxis.mX = m [ C1_R0 ];
		yAxis.mY = m [ C1_R1 ];
	}

	//----------------------------------------------------------------//
	inline TYPE GetElement ( int c, int r ) const {

		return m [ ( c * 3 ) + r ];
	}

	//----------------------------------------------------------------//
	USMetaVec2D < TYPE > GetHeading () const {

		USMetaVec2D < TYPE > heading;
	
		heading.mX = m [ C0_R0 ];
		heading.mY = m [ C0_R1 ];

		heading.NormSafe ();
		
		return heading;
	}

	//----------------------------------------------------------------//
	TYPE GetRot () const {

		float rot = ( TYPE )( atan2 ( m [ C0_R0 ], m [ C0_R1 ]) * R2D );
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

		memcpy ( m, mtx.m, sizeof ( m ));
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const USMetaMatrix3x3 < PARAM_TYPE >& mtx ) {

		m[C0_R0]	= ( TYPE )mtx.m[MatrixElem3x3::C0_R0];
		m[C0_R1]	= ( TYPE )mtx.m[MatrixElem3x3::C0_R1];
		
		m[C1_R0]	= ( TYPE )mtx.m[MatrixElem3x3::C1_R0];
		m[C1_R1]	= ( TYPE )mtx.m[MatrixElem3x3::C1_R1];
		
		m[C2_R0]	= ( TYPE )mtx.m[MatrixElem3x3::C2_R0];
		m[C2_R1]	= ( TYPE )mtx.m[MatrixElem3x3::C2_R1];
	}

	//----------------------------------------------------------------//
	template < typename PARAM_TYPE >
	void Init ( const USMetaMatrix4x4 < PARAM_TYPE >& mtx ) {

		m [ C0_R0 ]	= ( TYPE )mtx.m[MatrixElem4x4::C0_R0];
		m [ C0_R1 ]	= ( TYPE )mtx.m[MatrixElem4x4::C0_R1];
		
		m [ C1_R0 ]	= ( TYPE )mtx.m[MatrixElem4x4::C1_R0];
		m [ C1_R1 ]	= ( TYPE )mtx.m[MatrixElem4x4::C1_R1];
		
		m [ C2_R0 ]	= ( TYPE )mtx.m[MatrixElem4x4::C3_R0];
		m [ C2_R1 ]	= ( TYPE )mtx.m[MatrixElem4x4::C3_R1];
	}

	//----------------------------------------------------------------//
	bool Inverse () {

		USMetaAffine2D < TYPE > temp = *this;
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
	bool IsIdent () const {
		
		if ( !(( m [ C0_R0 ] == 1.0f ) && ( m [ C1_R1 ] == 1.0f ))) return false;
		if ( !(( m [ C1_R0 ] == 0.0f ) && ( m [ C2_R0 ] == 0.0f ))) return false;
		if ( !(( m [ C0_R1 ] == 0.0f ) && ( m [ C2_R1 ] == 0.0f ))) return false;
		
		return true;
	}

	//----------------------------------------------------------------//
	bool IsSame ( const USMetaAffine2D < TYPE >& compare ) const {
		
		return (
			( m [ C0_R0 ] == compare.m [ C0_R0 ]) &&
			( m [ C0_R1 ] == compare.m [ C0_R1 ]) &&
			( m [ C1_R0 ] == compare.m [ C1_R0 ]) &&
			( m [ C1_R1 ] == compare.m [ C1_R1 ]) &&
			( m [ C2_R0 ] == compare.m [ C2_R0 ]) &&
			( m [ C2_R1 ] == compare.m [ C2_R1 ])
		);
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
	
	#ifdef ARM6_IPHONE_3G
		TYPE src_vec[2] = { ( TYPE )point.mX, ( TYPE )point.mY};
		
		asm volatile (
				
              "fldmias  %0, {s8-s13}		\n\t"                                
              "fldmias  %1, {s0-s1}			\n\t"
                				
				// x
              "fmuls s3, s8, s0				\n\t"
			  "fmacs s3, s10, s1			\n\t"
			  "fadds s3, s3, s12			\n\t"
				
				//y
              "fmuls s4, s9, s0				\n\t"
			  "fmacs s4, s11, s1			\n\t"
			  "fadds s4, s4, s13			\n\t"
				                
              // Save vector.
              "fstmias  %1, {s3-s4}			\n\t"  
                
              : 
              : "r" (m), "r" (src_vec)
              : "memory","cc", "s0", "s1", "s2", "s3", "s4", "s8", "s9", "s10", "s11", "s12", "s13" 
		);  
		point.mX = src_vec[0];
		point.mY = src_vec[1];
	#else

		TYPE x =	( m [ C0_R0 ] * ( TYPE )point.mX ) +
					( m [ C1_R0 ] * ( TYPE )point.mY ) +
					( m [ C2_R0 ]);
		
		TYPE y =	( m [ C0_R1 ] * ( TYPE )point.mX ) +
					( m [ C1_R1 ] * ( TYPE )point.mY ) +
					( m [ C2_R1 ]); 
		
		point.mX = ( PARAM_TYPE )x;
		point.mY = ( PARAM_TYPE )y;
	#endif
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
	void TransformVec ( USMetaVec2D < PARAM_TYPE >& point ) const {
		
		TYPE x =	( m [ C0_R0 ] * ( TYPE )point.mX ) +
					( m [ C1_R0 ] * ( TYPE )point.mY );
		
		TYPE y =	( m [ C0_R1 ] * ( TYPE )point.mX ) +
					( m [ C1_R1 ] * ( TYPE )point.mY );
		
		point.mX = ( PARAM_TYPE )x;
		point.mY = ( PARAM_TYPE )y;
	}

	//----------------------------------------------------------------//
	inline TYPE TransformX ( TYPE x ) const {
		
		return ( m [ C0_R0 ] * x ) + ( m [ C2_R0 ]);
	}

	//----------------------------------------------------------------//
	inline TYPE TransformY ( TYPE y ) const {
		
		return ( m [ C1_R1 ] * y ) + ( m [ C2_R1 ]);
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
