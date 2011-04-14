// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USCanvas.h>
#include <uslsext/USViewQuad.h>

//================================================================//
// USViewQuad
//================================================================//

//----------------------------------------------------------------//
void USViewQuad::Init () {

	USAffine2D viewProjMtx;
	USCanvas::GetViewProjMtx ( viewProjMtx );
	this->Init ( viewProjMtx );
}

//----------------------------------------------------------------//
void USViewQuad::Init ( USAffine2D& viewProjMtx ) {

	USAffine2D invMtx;
	invMtx.Inverse ( viewProjMtx );
	
	this->mPoints [ 0 ].Init ( -1.0f, 1.0f );
	this->mPoints [ 1 ].Init ( 1.0f, 1.0f );
	this->mPoints [ 2 ].Init ( 1.0f, -1.0f );
	this->mPoints [ 3 ].Init ( -1.0f, -1.0f );
	
	this->Transform ( invMtx );
}

//----------------------------------------------------------------//
void USViewQuad::Transform ( const USAffine2D& mtx ) {

	mtx.Transform ( this->mPoints [ 0 ]);
	mtx.Transform ( this->mPoints [ 1 ]);
	mtx.Transform ( this->mPoints [ 2 ]);
	mtx.Transform ( this->mPoints [ 3 ]);
	
	this->mBounds.Init ( this->mPoints [ 0 ]);
	this->mBounds.Grow ( this->mPoints [ 1 ]);
	this->mBounds.Grow ( this->mPoints [ 2 ]);
	this->mBounds.Grow ( this->mPoints [ 3 ]);
}
