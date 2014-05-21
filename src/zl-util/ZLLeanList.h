// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLLEANLIST_H
#define	ZLLEANLIST_H

template < typename TYPE > class ZLLeanList;
template < typename TYPE > class ZLList;

//================================================================//
// ZLLeanLink
//================================================================//
template < typename TYPE >
class ZLLeanLink {
private:

	TYPE					mData;
	ZLLeanLink < TYPE >*	mPrev;
	ZLLeanLink < TYPE >*	mNext;
	ZLLeanList < TYPE >*	mList;

public:

	friend class ZLLeanList < TYPE >;
	friend class ZLList < TYPE >;

	//----------------------------------------------------------------//
	inline TYPE& Data () {
		return this->mData;
	}

	//----------------------------------------------------------------//
	inline void Data ( const TYPE& data ) {
		this->mData = data;
	}

	//----------------------------------------------------------------//
	inline ZLLeanList < TYPE >* List () {
		return this->mList;
	}

	//----------------------------------------------------------------//
	inline ZLLeanLink < TYPE >* Next () {
		return this->mNext;
	}
	
	//----------------------------------------------------------------//
	inline ZLLeanLink < TYPE >* Prev () {
		return this->mPrev;
	}
	
	//----------------------------------------------------------------//
	inline void Remove () {
		if ( this->mList ) {
			this->mList->Remove ( *this );
		}
	}
	
	//----------------------------------------------------------------//
	ZLLeanLink () :
		mPrev ( 0 ),
		mNext ( 0 ),
		mList ( 0 ) {
	}

	//----------------------------------------------------------------//
	ZLLeanLink ( const TYPE& data ) :
		mData ( data ),
		mPrev ( 0 ),
		mNext ( 0 ),
		mList ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~ZLLeanLink ()  {
		this->Remove ();
	}
};

//================================================================//
// ZLLeanList
//================================================================//
template < typename TYPE >
class ZLLeanList {
private:

	ZLLeanLink < TYPE >*	mHead;
	ZLLeanLink < TYPE >*	mTail;
	size_t					mCount;

public:

	typedef ZLLeanLink < TYPE >* Iterator;

	//----------------------------------------------------------------//
	inline TYPE& Back () {
		assert ( this->mTail );
		return this->mTail->Data ();
	}

	//----------------------------------------------------------------//
	void Clear () {
		
		Iterator iterator = this->Head ();
		while ( iterator ) {
			Iterator clear = iterator;
			iterator = iterator->Next ();
			clear->mPrev = 0;
			clear->mNext = 0;
			clear->mList = 0;
		}
		
		this->mCount = 0;
		this->mHead = 0;
		this->mTail = 0;
	}

	//----------------------------------------------------------------//
	size_t Count () {
		return this->mCount;
	}

	//----------------------------------------------------------------//
	ZLLeanLink < TYPE >* FindFirst ( const TYPE& type ) {
	
		Iterator iterator = this->Head ();
		for ( ; iterator ; iterator = iterator->Next ()) {
			if ( iterator->Data () == type ) {
				return iterator;
			}
		}
		return 0;
	}

	//----------------------------------------------------------------//
	ZLLeanLink < TYPE >* FindLast ( const TYPE& type ) {
	
		Iterator iterator = this->Tail ();
		for ( ; iterator ; iterator = iterator->Prev ()) {
			if ( iterator->Data () == type ) {
				return iterator;
			}
		}
		return 0;
	}

	//----------------------------------------------------------------//
	inline TYPE& Front () {
		assert ( this->mHead );
		return this->mHead->Data ();
	}

	//----------------------------------------------------------------//
	void Join ( ZLLeanList < TYPE >& a, ZLLeanList < TYPE >& b ) {
		
		ZLLeanLink < TYPE >* headA = a.mHead;
		ZLLeanLink < TYPE >* tailA = a.mTail;
		
		ZLLeanLink < TYPE >* headB = b.mHead;
		ZLLeanLink < TYPE >* tailB = b.mTail;
		
		size_t count = 0;
		
		if ( headA == headB ) {
			count = a.mCount;
			headB = 0;
			tailB = 0;
		}
		else {
			count = a.mCount + b.mCount;
		}
		
		a.mHead = 0;
		a.mTail = 0;
		a.mCount = 0;
		
		b.mHead = 0;
		b.mTail = 0;
		b.mCount = 0;
		
		// set 'this' last in case it is same as 'a' or 'b'
		this->mHead = 0;
		this->mTail = 0;
		this->mCount = count;
		
		if ( count ) {

			if ( headA && headB ) {
				
				tailA->mNext = headB;
				headB->mPrev = tailA;
				
				this->mHead = headA;
				this->mTail = tailB;
			}
			else if ( headA ) {
				this->mHead = headA;
				this->mTail = tailA;
			}
			else {
				this->mHead = headB;
				this->mTail = tailB;
			}
			
			ZLLeanLink < TYPE >* cursor = this->mHead;
			for ( ; cursor; cursor = cursor->mNext ) {
				cursor->mList = this;
			}
		}
	}

	//----------------------------------------------------------------//
	inline ZLLeanLink < TYPE >* Head () {
		return ( this->mHead ) ? this->mHead : 0;
	}

	//----------------------------------------------------------------//
	void InsertAfter ( ZLLeanLink < TYPE >& cursor, ZLLeanLink < TYPE >& link ) {

		link.Remove ();

		if ( cursor.mNext ) {
		
			link.mPrev = &cursor;
			link.mNext = cursor.mNext;
		
			link.mPrev->mNext = &link;
			link.mNext->mPrev = &link;
		}
		else {
			this->PushBack ( link );
		}
		
		link.mList = this;
		++this->mCount;
	}

	//----------------------------------------------------------------//
	void InsertBefore ( ZLLeanLink < TYPE >& cursor, ZLLeanLink < TYPE >& link ) {

		link.Remove ();

		if ( cursor.mPrev ) {
			
			link.mPrev = cursor.mPrev;
			link.mNext = &cursor;
		
			link.mPrev->mNext = &link;
			link.mNext->mPrev = &link;
		}
		else {
			this->PushFront ( link );
		}
		
		link.mList = this;
		++this->mCount;
	}

	//----------------------------------------------------------------//
	void PopBack () {
	
		if ( this->mTail ) {
			this->Remove ( *this->mTail );
		}
	}

	//----------------------------------------------------------------//
	void PopFront () {
	
		if ( this->mHead ) {
			this->Remove ( *this->mHead );
		}
	}

	//----------------------------------------------------------------//
	void PushBack ( ZLLeanLink < TYPE >& link ) {

		link.Remove ();

		if ( !mHead ) {
			this->mHead = &link;
			this->mTail = &link;
			link.mPrev = 0;
			link.mNext = 0;
		}
		else {
			this->mTail->mNext = &link;
			link.mPrev = mTail;
			link.mNext = 0;
			this->mTail = &link;
		}

		link.mList = this;
		++this->mCount;
	}

	//----------------------------------------------------------------//
	void PushFront ( ZLLeanLink < TYPE >& link ) {

		link.Remove ();

		if ( !this->mHead ) {
			this->mHead = &link;
			this->mTail = &link;
			link.mPrev = 0;
			link.mNext = 0;
		}
		else {
			this->mHead->mPrev = &link;
			link.mPrev = 0;
			link.mNext = this->mHead;
			this->mHead = &link;
		}
		
		link.mList = this;
		++this->mCount;
	}

	//----------------------------------------------------------------//
	void Remove ( ZLLeanLink < TYPE >& link ) {

		if ( link.mList != this ) return;

		if ( link.mNext ) {
			link.mNext->mPrev = link.mPrev;
		}
		else {
			this->mTail = link.mPrev;
		}

		if ( link.mPrev ) {
			link.mPrev->mNext = link.mNext;
		}
		else {
			this->mHead = link.mNext;
		}
		
		link.mPrev = 0;
		link.mNext = 0;
		link.mList = 0;
		
		--this->mCount;
	}
	
	//----------------------------------------------------------------//
	inline ZLLeanLink < TYPE >* Tail () {
		return ( this->mTail ) ? this->mTail : 0;
	}
	
	//----------------------------------------------------------------//
	ZLLeanList () :
		mHead ( 0 ),
		mTail ( 0 ),
		mCount ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~ZLLeanList () {
		this->Clear ();
	}
};

#endif
