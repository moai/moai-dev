// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLRAMESIZE_H
#define	ZLRAMESIZE_H

//================================================================//
// ZLFrameSize
//================================================================//
// TODO: doxygen
class ZLFrameSize {
protected:
	
	u32				mWidth;
	u32				mHeight;

public:
	
	GET_CONST	( u32, Width, mWidth )
	GET_CONST	( u32, Height, mHeight )
	
	//----------------------------------------------------------------//
	ZLRect GetRect () {

		return ZLRect ( this->mWidth, this->mHeight );
	}
	
	//----------------------------------------------------------------//
	ZLFrameSize () :
		mWidth ( 0 ),
		mHeight ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~ZLFrameSize () {
	}
	
	//----------------------------------------------------------------//
	void SetFrameSize ( u32 width, u32 height ) {

		this->mWidth = width;
		this->mHeight = height;
	}
};

#endif
