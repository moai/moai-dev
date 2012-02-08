// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISPANLIST_H
#define	MOAISPANLIST_H

//================================================================//
// MOAISpan
//================================================================//
class MOAISpan {
public:

	size_t		mBase;
	size_t		mSize;
	bool		mOccupied;

	MOAISpan*	mPrev;	
	MOAISpan*	mNext;
};

//================================================================//
// MOAISpanList
//================================================================//
class MOAISpanList {
private:

	MOAISpan*	mHead;
	MOAISpan*	mTail;
	
	size_t		mSize;

public:

	//----------------------------------------------------------------//
	MOAISpan*		Alloc				( size_t size );
	void			Expand				( size_t size );
	void			Free				( MOAISpan* span );
					MOAISpanList		();
					~MOAISpanList		();
};

#endif
