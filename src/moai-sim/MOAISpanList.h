// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISPANLIST_H
#define	MOAISPANLIST_H

//================================================================//
// MOAISpan
//================================================================//
template < typename TYPE >
class MOAISpan {
public:

	size_t		mBase;
	size_t		mSize;
	bool		mOccupied;

	MOAISpan < TYPE >*	mPrev;	
	MOAISpan < TYPE >*	mNext;
	
	TYPE		mData;
	
	//----------------------------------------------------------------//
	MOAISpan () {
	}
};

//================================================================//
// MOAISpanList
//================================================================//
template < typename TYPE >
class MOAISpanList {
public:

	typedef MOAISpan < TYPE > Span;

	Span*	mHead;
	Span*	mTail;
	
	size_t	mSize;

	//----------------------------------------------------------------//
	Span* Alloc ( size_t size ) {

		Span* span = this->mHead;
		
		// find the first span large enough to accomodate the request
		// as allocations schemes go this is pretty naive, yo
		// but glyphs will more or less be roughly the same size...
		// could also speed this up by keeping a free span list or sorted rb tree
		for ( ; span; span = span->mNext ) {
			
			if ( span->mOccupied || ( span->mSize < size )) continue;
			
			if ( span->mSize == size ) {
				span->mOccupied = true;
				return span;
			}
			
			Span* snip = new Span ();
			snip->mPrev = span;
			snip->mNext = span->mNext;
			snip->mBase = span->mBase + size;
			snip->mSize = span->mSize - size;
			snip->mOccupied = false;
			
			if ( span->mNext ) {
				span->mNext->mPrev = snip;
			}
			else {
				this->mTail = snip;
			}
			
			span->mOccupied = true;
			span->mNext = snip;
			span->mSize = size;
			
			return span;
		}

		return 0;
	}

	//----------------------------------------------------------------//
	void Expand ( size_t size ) {

		if ( size < this->mSize ) return;
		size_t more = size - this->mSize;

		if ( this->mTail ) {
		
			if ( this->mTail->mOccupied ) {
				
				Span* span = new Span ();
			
				span->mPrev = this->mTail;
				span->mNext = 0;
				span->mBase = this->mSize;
				span->mSize = more;
				span->mOccupied = false;
				
				this->mTail->mNext = span;
				this->mTail = span;
			}
			else {
				this->mTail->mSize += more;
			}
		}
		else {
			
			Span* span = new Span ();
			
			span->mPrev = 0;
			span->mNext = 0;
			span->mBase = 0;
			span->mSize = size;
			span->mOccupied = false;
			
			this->mHead = span;
			this->mTail = span;
		}
		
		this->mSize = size;
	}

	//----------------------------------------------------------------//
	void Free ( Span* span ) {
		
		if ( !span ) return;
		
		// should span remain at all it will not be occupied
		span->mOccupied = false;

		// get prev and next for easy access later
		Span* prev = span->mPrev;
		Span* next = span->mNext;

		// bail if span is alone
		if ( !( prev || next )) return;

		// check occupancy on either side of span
		bool prevOccupied = !prev || ( prev && prev->mOccupied );
		bool nextOccupied = !next || ( next && next->mOccupied );
		
		// both neighbors are occupied; nothing more to do
		if ( prevOccupied && nextOccupied ) return;

		// only prev is occupied or missing; merge with next
		if ( prevOccupied && !nextOccupied ) {
			
			// span is tail; nothing to do
			if ( !next ) return;
			
			next->mPrev = prev;
			next->mBase = span->mBase;
			next->mSize += span->mSize;
			
			if ( prev ) {
				prev->mNext = next;
			}
			else {
				this->mHead = next;
			}
			
			delete span;
			return;
		}
		
		// only next is occupied or missing; merge with prev
		if ( nextOccupied && !prevOccupied ) {
			
			// span is head, nothing to do
			if ( !prev ) return;
			
			prev->mNext = next;
			prev->mSize += span->mSize;
			
			if ( next ) {
				next->mPrev = prev;
			}
			else {
				this->mTail = prev;
			}
			
			delete span;
			return;
		}
		
		// both sides exist and are not occupied
		// remove span and merge neighbors
		
		prev->mSize += span->mSize + next->mSize;
		prev->mNext = next->mNext;
		
		if ( next->mNext ) {
			next->mNext->mPrev = prev;
		}
		else {
			this->mTail = prev;
		}
		
		delete span;
		delete next;
	}

	//----------------------------------------------------------------//
	bool HasRoom ( size_t size ) {
		
		Span* span = this->mHead;
		for ( ; span; span = span->mNext ) {
			if ( !span->mOccupied && ( size <= span->mSize )) return true;
		}
		return false;
	}

	//----------------------------------------------------------------//
	MOAISpanList () :
		mHead ( 0 ),
		mTail ( 0 ),
		mSize ( 0 ) {
	}

	//----------------------------------------------------------------//
	~MOAISpanList () {
	
		Span* cursor = this->mHead;
		while ( cursor ) {
			Span* span = cursor;
			cursor = cursor->mNext;
			delete span;
		}
	}
};

#endif
