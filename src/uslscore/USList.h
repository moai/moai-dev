// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USLIST_H
#define	USLIST_H

#include <uslscore/USLeanList.h>

//================================================================//
// USList
//================================================================//
template < typename TYPE >
class USList {
private:

	static const u32 CHUNK_SIZE = 32;

	u32 mTopLink;
	USLeanArray < USLeanLink < TYPE >* > mChunks;

	USLeanList < TYPE > mFreeLinks;
	USLeanList < TYPE > mList;

	//----------------------------------------------------------------//
	USLeanLink < TYPE >* NextFreeLink () {
	
		if ( !this->mFreeLinks.Count ()) {
			
			u32 chunkID = ( this->mTopLink / CHUNK_SIZE );
			u32 totalChunks = chunkID + 1;
			
			if ( totalChunks > this->mChunks.Size ()) {
				
				u32 i = this->mChunks.Size ();
				this->mChunks.Resize ( totalChunks );
				
				for ( ; i < totalChunks; ++i ) {
				
					u32 chunkSize = sizeof ( USLeanLink < TYPE >) * CHUNK_SIZE;
					USLeanLink < TYPE >* chunk = ( USLeanLink < TYPE >* )malloc ( chunkSize );
					this->mChunks [ i ] = chunk;
				}
			}
			
			USLeanLink < TYPE >* link = &this->mChunks [ chunkID ][ this->mTopLink % CHUNK_SIZE ];
			
			link->mPrev = 0;
			link->mNext = 0;
			link->mList = 0;
			
			this->mFreeLinks.PushFront ( *link );
			this->mTopLink++;
		}
		
		return this->mFreeLinks.Head ();
	}

	//----------------------------------------------------------------//
	USLeanLink < TYPE >* NextFreeLink ( const TYPE& element ) {
		
		USLeanLink < TYPE >* link = this->NextFreeLink ();
		
		USLeanLink < TYPE >* prev = link->mPrev;
		USLeanLink < TYPE >* next = link->mNext;
		USLeanList < TYPE >* list = link->mList;
		
		// call the copy constructor
		new ( link ) USLeanLink < TYPE >( element );
		
		link->mPrev = prev;
		link->mNext = next;
		link->mList = list;
		
		return link;
	}

public:

	typedef USLeanLink < TYPE >* Iterator;

	//----------------------------------------------------------------//
	inline TYPE& Back () {
	
		return this->mList.Back ();
	}

	//----------------------------------------------------------------//
	void Clear () {
		
		Iterator iterator = this->Head ();
		for ( ; iterator; iterator = iterator->Next ()) {
			iterator->USLeanLink < TYPE >::~USLeanLink ();
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
	void InsertAfter ( USLeanLink < TYPE >& cursor, TYPE& element ) {
	
		this->mList.InsertAfter ( cursor, this->AssignLink ( element ));
	}

	//----------------------------------------------------------------//
	void InsertBefore ( USLeanLink < TYPE >& cursor, TYPE& element ) {
	
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
			cursor->USLeanLink < TYPE >::~USLeanLink ();
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
	USList () :
		mTopLink ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~USList () {
		
		this->Clear ();
	}
};

#endif
