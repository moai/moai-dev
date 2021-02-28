// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLAPPLICATION_H
#define	ZLAPPLICATION_H

#include <zl-core/STLSet.h>
#include <zl-core/ZLLeanArray.h>
#include <zl-core/ZLMutex.h>
#include <zl-core/ZLSingleton.h>

class ZLApplication;

//================================================================//
// ZLApplicationGlobalBase
//================================================================//
class ZLApplicationGlobalBase {
protected:

	friend class ZLApplication;
	
	ZLMutex				mMutex;
	void*				mSelf;
	
	//----------------------------------------------------------------//
	virtual void		ZLApplicationGlobalBase_Finalize		();
	virtual void		ZLApplicationGlobalBase_Initialize		();

	//----------------------------------------------------------------//
	void				Finalize					();
	void				Initialize					();
	bool				IsValid						();
						ZLApplicationGlobalBase		();
	virtual				~ZLApplicationGlobalBase	();
};

//================================================================//
// ZLApplication
//================================================================//
class ZLApplication :
	public ZLSingleton < ZLApplication > {
private:

	enum {
		CHUNK_SIZE = 32,
	};

	ZLLeanArray < ZLApplicationGlobalBase* > 	mGlobals;

	//----------------------------------------------------------------//
	static ZLIndex GetUniqueID () {
	
		static ZLIndex counter = 0;
		return counter++;
	};

	//----------------------------------------------------------------//
	template < typename TYPE >
	static ZLIndex GetClassID () {
	
		static ZLIndex type	= ZLApplication::GetUniqueID ();
		return type;
	};

	//----------------------------------------------------------------//
	void		AffirmSize			( ZLSize size );

public:
	
	//----------------------------------------------------------------//
				ZLApplication		();
				~ZLApplication		();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	ZLScopedRef < TYPE > AffirmGlobal () {
		
		ZLScopedLock lock ( this->mMutex );
		ZLLeanArray < ZLApplicationGlobalBase* >& globals = this->mGlobals;
		
		ZLIndex globalID = this->GetClassID < TYPE >();
		ZLApplication::AffirmSize (( ZLSize )globalID + 1 );
		
		if ( globals [ globalID ] == NULL ) {
			
			TYPE* global = new TYPE;
			ZLApplicationGlobalBase* base = global;
			base->mSelf = global;
			
			globals [ globalID ] = global;
			
			base->Initialize ();
		}
		
		ZLApplicationGlobalBase* base = globals [ globalID ];
		
		assert ( base->mSelf );
		
		return ZLScopedRef < TYPE& >( base->mMutex, *( TYPE* )base->mSelf  );
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	ZLScopedRef < TYPE > GetGlobal () {
		
		ZLScopedLock lock ( this->mMutex );
		ZLLeanArray < ZLApplicationGlobalBase* >& globals = this->mGlobals;
		
		ZLApplicationGlobalBase* base = NULL;
		
		ZLIndex globalID = this->GetClassID < TYPE >();
		if ( globalID < globals.Size ()) {
			base = globals [ globalID ];
			if ( base && base->mSelf ) {
				base = ( TYPE* )base->mSelf;
			}
		}
		assert ( base && base->mSelf );
		return ZLScopedRef < TYPE& >( base->mMutex, *( TYPE* )base->mSelf );
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool IsValid () {
		
		ZLScopedLock lock ( this->mMutex );
		ZLLeanArray < ZLApplicationGlobalBase* >& globals = this->mGlobals;
		
		ZLIndex globalID = this->GetClassID < TYPE >();
		
		if ( globalID < globals.Size ()) {
			ZLApplicationGlobalBase* base = globals [ globalID ];
			return ( base && base->mSelf );
		}
		return false;
	}
};

//================================================================//
// ZLApplicationGlobalBase
//================================================================//
// TODO: doxygen
template < typename TYPE >
class ZLApplicationGlobal :
	public virtual ZLApplicationGlobalBase {
public:

	typedef ZLScopedRef < TYPE > Ref;

	//----------------------------------------------------------------//
	static Ref Affirm () {
		return ZLApplication::Get ()->AffirmGlobal < TYPE >();
	}

	//----------------------------------------------------------------//
	static Ref Get () {
		return ZLApplication::Get ()->GetGlobal < TYPE >();
	}

	//----------------------------------------------------------------//
	static bool IsValid () {
		return ZLApplication::Get ()->IsValid < TYPE >();
	}

	//----------------------------------------------------------------//
	ZLApplicationGlobal () {
	}

	//----------------------------------------------------------------//
	~ZLApplicationGlobal () {
	}
};

#endif
