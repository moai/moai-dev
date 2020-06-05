// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFER_H
#define	MOAIFRAMEBUFFER_H

//================================================================//
// MOAIFrameBuffer
//================================================================//
class MOAIFrameBuffer :
	public virtual MOAILuaObject {
protected:

	u32					mBufferWidth;
	u32					mBufferHeight;
	float				mBufferScale;
	bool				mLandscape;

	//----------------------------------------------------------------//

public:

	GET_CONST	( u32, BufferWidth, mBufferWidth )
	GET_CONST	( u32, BufferHeight, mBufferHeight )
	GET_SET		( float, BufferScale, mBufferScale )
	GET_SET		( bool, Landscape, mLandscape )

	//----------------------------------------------------------------//
					MOAIFrameBuffer				();
					~MOAIFrameBuffer			();
	void			SetBufferSize				( u32 width, u32 height );
	ZLRect			WndRectToDevice 			( ZLRect rect ) const;
};

#endif
