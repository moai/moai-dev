// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USMATRIX_H
#define USMATRIX_H

template < typename TYPE > class tMatrix2D;
template < typename TYPE > class tMatrix3D;

//================================================================//
// AffineElem2D
//================================================================//
namespace AffineElem2D {

	enum {
		C0_R0	=	0,
		C0_R1,
		
		C1_R0,
		C1_R1,
		
		C2_R0,
		C2_R1,
	};
}

//================================================================//
// AffineElem3D
//================================================================//
namespace AffineElem3D {

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
	};
}

//================================================================//
// MatrixElem2D
//================================================================//
namespace MatrixElem2D {

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
}

//================================================================//
// MatrixElem3D
//================================================================//
namespace MatrixElem3D {

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
		C3_R3
	};
}

template < typename TYPE > class USMetaAffine2D;
template < typename TYPE > class USMetaAffine3D;
template < typename TYPE > class USMetaMatrix3x3;
template < typename TYPE > class USMetaMatrix4x4;

#endif
