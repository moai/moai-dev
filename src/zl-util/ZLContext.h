// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLCONTEXT_H
#define	ZLCONTEXT_H

#include <zl-common/zl_types.h>
#include <zl-util/STLSet.h>
#include <zl-util/ZLLeanArray.h>
#include <zl-util/ZLRtti.h>
#include <zl-vfs/ZLThreadLocalPtr.h>

//================================================================//
// ZLContextClassIDBase
//================================================================//
class ZLContextClassIDBase {
protected:

	//----------------------------------------------------------------//
	static ZLIndex GetUniqueID ()	{
		static ZLIndex counter = 0;
		return counter++;
	};
};

//================================================================//
// ZLContextClassID
//================================================================//
template < typename	TYPE >
class ZLContextClassID :
	public ZLContextClassIDBase {
public:
	
	//----------------------------------------------------------------//
	static ZLIndex GetID ( void	) {
	
		static ZLIndex type	= GetUniqueID ();
		return type;
	};
};

//================================================================//
// ZLContextClassBase
//================================================================//
class ZLContextClassBase {
protected:

	friend class ZLContext;
	
	//----------------------------------------------------------------//
	virtual void		ZLContextClass_Finalize		();
	virtual void		ZLContextClass_Initialize	();

	//----------------------------------------------------------------//
	void				Finalize					();
	void				Initialize					();
						ZLContextClassBase			();
	virtual				~ZLContextClassBase			();
};

//================================================================//
// ZLContextPair
//================================================================//
class ZLContextPair {
public:

	ZLContextClassBase*			mGlobalBase;
	void*						mGlobal;
	bool						mIsValid;

	ZLContextPair*				mAliasOf;
	ZLIndex						mAliasID;

	//----------------------------------------------------------------//
	ZLContextPair () :
		mGlobalBase ( NULL ),
		mGlobal ( NULL ),
		mIsValid ( false ),
		mAliasOf ( NULL ),
		mAliasID ( 0) {
	}
};

//================================================================//
// ZLContext
//================================================================//
class ZLContext {
private:

	friend class ZLContextMgr;

	enum {
		CHUNK_SIZE = 32,
	};

	ZLLeanArray < ZLContextPair >	mGlobals;

	//----------------------------------------------------------------//
	void		AffirmSize			( ZLSize size );
				ZLContext			();
				~ZLContext			();

public:
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* AffirmGlobal () {
		
		ZLIndex globalID = ZLContextClassID < TYPE >::GetID ();
		this->AffirmSize (( ZLSize )globalID + 1 );
		
		if ( !this->mGlobals [ globalID ].mGlobal ) {
			
			TYPE* global = new TYPE;
			
			ZLContextPair& pair = this->mGlobals [ globalID ];
			
			pair.mGlobalBase	= global;
			pair.mGlobal		= global;
			pair.mAliasOf		= NULL;
			pair.mIsValid		= true;
			
			pair.mGlobalBase->Initialize ();
		}
		
		if ( !this->mGlobals [ globalID ].mIsValid ) {
			return 0;
		}
		
		return ( TYPE* )this->mGlobals [ globalID ].mGlobal;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* GetGlobal () {
		
		ZLIndex globalID = ZLContextClassID < TYPE >::GetID ();
		if ( globalID < this->mGlobals.Size ()) {
			ZLContextPair& pair = this->mGlobals [ globalID ];
			if ((( pair.mAliasOf == NULL ) && pair.mIsValid ) || ( pair.mAliasOf && pair.mAliasOf->mIsValid )) {
				return ( TYPE* )pair.mGlobal;
			}
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void Invalidate () {
		
		ZLIndex id = ZLContextClassID < TYPE >::GetID ();
		
		if ( id < this->mGlobals.Size ()) {
			this->mGlobals [ id ].mIsValid = false;
		}
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool IsValid () {
		
		ZLIndex globalID = ZLContextClassID < TYPE >::GetID ();
		
		if ( globalID < this->mGlobals.Size ()) {
			ZLContextPair& pair = this->mGlobals [ globalID ];
			return ((( pair.mAliasOf == NULL ) && pair.mIsValid ) || ( pair.mAliasOf && pair.mAliasOf->mIsValid ));
		}
		return false;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE, typename ALIAS_TYPE >
	ALIAS_TYPE* RegisterGlobalAlias () {
		
		ZLIndex globalID = ZLContextClassID < TYPE >::GetID ();
		assert ( globalID < this->mGlobals.Size ());
		ZLContextPair& globalPair = this->mGlobals [ globalID ];
		TYPE* global = ( TYPE* )globalPair.mGlobal;
		assert ( global );

		ZLIndex aliasID = ZLContextClassID < ALIAS_TYPE >::GetID ();
		this->AffirmSize (( ZLSize )aliasID + 1 );

		ALIAS_TYPE* alias = global;

		ZLContextPair& aliasPair = this->mGlobals [ aliasID ];
		aliasPair.mGlobalBase	= NULL;
		aliasPair.mGlobal		= alias;
		aliasPair.mAliasOf		= &globalPair;
		aliasPair.mIsValid		= true;

		return ( ALIAS_TYPE* )this->mGlobals [ aliasID ].mGlobal;
	}
};

//================================================================//
// ZLContextMgr
//================================================================//
class ZLContextMgr {
private:

	friend class ZLContextClassBase;

	typedef STLSet < ZLContext* >::iterator GlobalsSetIt;
	typedef STLSet < ZLContext* > GlobalsSet;

	// TODO: sGlobalsSet needs to be shared across all threads and wrapped in a mutex.
	// should no longer be a ZLThreadLocalPtr.
	static ZLThreadLocalPtr < GlobalsSet >		sGlobalsSet;
	
	static ZLThreadLocalPtr < ZLContext >		sInstance;

	//----------------------------------------------------------------//
							ZLContextMgr			();
							~ZLContextMgr			();

public:

	//----------------------------------------------------------------//
	static bool				Check					( ZLContext* globals );
	static u32				CountContexts			();
	static ZLContext*		Create					();
	static void				Delete					( ZLContext* globals );
	static void				Finalize				();
	static ZLContext*		Get						();
	static ZLContext*		Set						( ZLContext* globals );
};

//================================================================//
// ZLContextClass
//================================================================//
/**	@lua	ZLContextClass
	@text	Base class for Moai singletons.
*/
template < typename TYPE >
class ZLContextClass :
	public virtual ZLContextClassBase {
public:
	
	//----------------------------------------------------------------//
	static TYPE& Affirm () {
		assert ( ZLContextMgr::Get ());
		TYPE* global = ZLContextMgr::Get ()->AffirmGlobal < TYPE >();
		assert ( global );
		return *global;
	}
	
	//----------------------------------------------------------------//
	inline static TYPE& Get () {
		assert ( ZLContextMgr::Get ());
		TYPE* global = ZLContextMgr::Get ()->GetGlobal < TYPE >();
		assert ( global );
		return *global;
	}
	
	//----------------------------------------------------------------//
	void InvalidateContext () {
		assert ( ZLContextMgr::Get ());
		ZLContextMgr::Get ()->Invalidate < TYPE >();
	}
	
	//----------------------------------------------------------------//
	inline static bool IsValid () {
		assert ( ZLContextMgr::Get ());
		return ZLContextMgr::Get ()->IsValid < TYPE >();
	}

	//----------------------------------------------------------------//
	template < typename ALIAS_TYPE >
	static ALIAS_TYPE& RegisterAlias () {
		assert ( ZLContextMgr::Get ());
		ALIAS_TYPE* alias = ZLContextMgr::Get ()->RegisterGlobalAlias < TYPE, ALIAS_TYPE >();
		assert ( alias );
		return *alias;
	}
};

//================================================================//
// ZLContextClassAlias
//================================================================//
/**	@lua	ZLContextClassAlias
	@text	Base class for Moai singleton aliases.
*/
template < typename TYPE >
class ZLContextClassAlias :
	public virtual ZLContextClassBase {
public:
	
	//----------------------------------------------------------------//
	inline static TYPE& Get () {
		assert ( ZLContextMgr::Get ());
		TYPE* global = ZLContextMgr::Get ()->GetGlobal < TYPE >();
		assert ( global );
		return *global;
	}

	//----------------------------------------------------------------//
	inline static bool IsValid () {
		assert ( ZLContextMgr::Get ());
		return ZLContextMgr::Get ()->IsValid < TYPE >();
	}
};

//================================================================//
// ZLScopedContext
//================================================================//
class ZLScopedContext {
private:

	ZLContext*		mOriginalContext;

public:
	
	//----------------------------------------------------------------//
	void				Clear						();
						ZLScopedContext				();
						~ZLScopedContext			();
};

#endif
