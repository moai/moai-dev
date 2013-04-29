// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLLIST_H
#define	ZLLIST_H

#include <zl-util/ZLLeanList.h>

//================================================================//
// ZLList
//================================================================//
template < typename TYPE >
class ZLList {
private:

	static const u32 CHUNK_SIZE = 32;

	u32 mTopLink;
	ZLLeanArray < ZLLeanLink < TYPE >* > mChunks;

	ZLLeanList < TYPE > mFreeLinks;
	ZLLeanList < TYPE > mList;

	//----------------------------------------------------------------//
	ZLLeanLink < TYPE >* NextFreeLink () {
	
		if ( !this->mFreeLinks.Count ()) {
			
			u32 chunkID = ( this->mTopLink / CHUNK_SIZE );
			u32 totalChunks = chunkID + 1;
			
			if ( totalChunks > this->mChunks.Size ()) {
				
				u32 i = this->mChunks.Size ();
				this->mChunks.Resize ( totalChunks );
				
				for ( ; i < totalChunks; ++i ) {
				
					u32 chunkSize = sizeof ( ZLLeanLink < TYPE >) * CHUNK_SIZE;
					ZLLeanLink < TYPE >* chunk = ( ZLLeanLink < TYPE >* )malloc ( chunkSize );
					this->mChunks [ i ] = chunk;
				}
			}
			
			ZLLeanLink < TYPE >* link = &this->mChunks [ chunkID ][ this->mTopLink % CHUNK_SIZE ];
			
			link->mPrev = 0;
			link->mNext = 0;
			link->mList = 0;
			
			this->mFreeLinks.PushFront ( *link );
			this->mTopLink++;
		}
		
		return this->mFreeLinks.Head ();
	}

	//----------------------------------------------------------------//
	ZLLeanLink < TYPE >* NextFreeLink ( const TYPE& element ) {
		
		ZLLeanLink < TYPE >* link = this->NextFreeLink ();
		
		ZLLeanLink < TYPE >* prev = link->mPrev;
		ZLLeanLink < TYPE >* next = link->mNext;
		ZLLeanList < TYPE >* list = link->mList;
		
		// call the copy constructor
		new ( link ) ZLLeanLink < TYPE >( element );
		
		link->mPrev = prev;
		link->mNext = next;
		link->mList = list;
		
		return link;
	}

public:

	typedef ZLLeanLink < TYPE >* Iterator;

	//----------------------------------------------------------------//
	inline TYPE& Back () {
	
		return this->mList.Back ();
	}

	//----------------------------------------------------------------//
	void Clear () {
		
		Iterator iterator = this->Head ();
		for ( ; iterator; iterator = iterator->Next ()) {
			iterator->ZLLeanLink < TYPE >::~ZLLeanLink ();
		}
		
		this->mList.Clear ();
		this->mFreeLinks.Clear ();
		
		for ( u32 i = 0; i < this->mChunks.Size (); ++i ) {
			free ( this->mChunks [ i ]);
			this->mChunks [ i ] = 0;
		}
		
		this->mChunks.Clear ();
	}

	//----------------------------------------------------------------//
	bool Contains ( const TYPE& element ) {
	
		Iterator iterator = this->Head ();
		for ( ; iterator ; iterator = iterator->Next ()) {
			if ( iterator->Data () == element ) {
				return true;
			}
		}
		return false;
	}

	//----------------------------------------------------------------//
	u32 Count () {
	
		return this->mList.Count ();
	}

	//----------------------------------------------------------------//
	inline TYPE& Front () {
	
		this->mList.Front ();
	};

	//----------------------------------------------------------------//
	inline Iterator Head () {
		return this->mList.Head ();
	};

	//----------------------------------------------------------------//
	void InsertAfter ( ZLLeanLink < TYPE >& cursor, TYPE& element ) {
	
		this->mList.InsertAfter ( cursor, this->AssignLink ( element ));
	}

	//----------------------------------------------------------------//
	void InsertBefore ( ZLLeanLink < TYPE >& cursor, TYPE& element ) {
	
		this->mList.InsertBefore ( cursor, this->AssignLink ( element ));
	}

	//----------------------------------------------------------------//
	void PopBack () {
		
		this->Remove ( this->mList.Tail ());
	}

	//----------------------------------------------------------------//
	void PopFront () {
		
		this->Remove ( this->mList.Head ());
	}

	//----------------------------------------------------------------//
	void PushBack ( const TYPE& element ) {
	
		this->mList.PushBack ( *this->NextFreeLink ( element ));
	}

	//----------------------------------------------------------------//
	void PushFront ( const TYPE& element ) {
	
		this->mList.PushFront ( *this->NextFreeLink ( element ));
	}

	//----------------------------------------------------------------//
	void Remove ( Iterator cursor ) {
	
		if ( cursor ) {
			cursor->ZLLeanLink < TYPE >::~ZLLeanLink ();
			this->mFreeLinks.PushBack ( *cursor );
		}
	}
	
	//----------------------------------------------------------------//
	void Remove ( const TYPE& element ) {
	
		Iterator iterator = this->Head ();
		for ( ; iterator ; iterator = iterator->Next ()) {
			if ( iterator->Data () == element ) {
				this->Remove ( iterator );
				return;
			}
		}
	}
	
	//----------------------------------------------------------------//
	inline Iterator Tail () {
		return this->mList.Tail ();
	};
	
	//----------------------------------------------------------------//
	ZLList () :
		mTopLink ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~ZLList () {
		
		this->Clear ();
	}
};

#endif
