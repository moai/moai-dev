// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFER_H
#define	MOAIFRAMEBUFFER_H

//================================================================//
// MOAIFrameBuffer
//================================================================//
class MOAIFrameBuffer :
	public virtual MOAILuaObject,
	public virtual ZLFrameSize {
protected:

	float				mBufferScale;
	bool				mLandscape;

	//----------------------------------------------------------------//

public:

	GET_SET		( float, BufferScale, mBufferScale )
	GET_SET		( bool, Landscape, mLandscape )

	//----------------------------------------------------------------//
					MOAIFrameBuffer				();
					~MOAIFrameBuffer			();
	ZLRect			WndRectToDevice 			( ZLRect rect ) const;
};

#endif
