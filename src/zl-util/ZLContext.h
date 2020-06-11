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
// ZLThreadLocal
//================================================================//
template < typename TYPE >
class ZLLocalPtr {
private:

	TYPE*	mPtr;

public:

	//----------------------------------------------------------------//
	TYPE* Get () {
		return this->mPtr;
	}

	//----------------------------------------------------------------//
	void Set ( TYPE* assign ) {
		this->mPtr = assign;
	}
	
	//----------------------------------------------------------------//
	ZLLocalPtr () :
		mPtr ( NULL ) {
	}
};

//================================================================//
// ZLContextClassIDBase
//================================================================//
class ZLContextClassIDBase {
protected:

	//----------------------------------------------------------------//
	static u32 GetUniqueID ()	{
		static u32 counter = 0;	
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
	static u32 GetID ( void	) {
	
		static u32 type	= GetUniqueID ();
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
	virtual void			OnGlobalsFinalize			();
	virtual void			OnGlobalsInitialize			();
							ZLContextClassBase			();
	virtual					~ZLContextClassBase			();
};

//================================================================//
// ZLContextPair
//================================================================//
class ZLContextPair {
protected:
	friend class ZLContext;

	ZLContextClassBase*			mGlobalBase;
	void*						mGlobal;
	void*						mProxy;
	bool						mIsValid;
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
				ZLContext			();
				~ZLContext			();

public:
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* AffirmGlobal () {
		
		u32 id = ZLContextClassID < TYPE >::GetID ();
		
		if ( this->mGlobals.Size () <= id ) {
		
			ZLContextPair pair;
			pair.mGlobalBase	= 0;
			pair.mGlobal		= 0;
			pair.mProxy			= 0;
			pair.mIsValid		= false;
			
			this->mGlobals.GrowChunked ( id + 1, CHUNK_SIZE, pair );
		}
		
		if ( !this->mGlobals [ id ].mGlobal ) {
			
			// NOTE: other (new) globals may be accessed in the constructor, particularly
			// if a Lua binding is created via LuaRetain. this may trigger a reallocation
			// of the globals array, which will invalidate pointers and references. for this
			// reason, we need to get 'pair' *after* the constructor.
			
			TYPE* global = new TYPE;
			
			ZLContextPair& pair = this->mGlobals [ id ];
			
			pair.mGlobalBase	= global;
			pair.mGlobal		= global;
			pair.mProxy			= 0;
			pair.mIsValid		= true;
			
			pair.mGlobalBase->OnGlobalsInitialize ();
		}
		
		if ( !this->mGlobals [ id ].mIsValid ) {
			return 0;
		}
		
		return ( TYPE* )this->mGlobals [ id ].mGlobal;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* GetGlobal () {
		
		u32 id = ZLContextClassID < TYPE >::GetID ();
		if ( id < this->mGlobals.Size ()) {
			ZLContextPair& pair = this->mGlobals [ id ];
			if ( pair.mIsValid ) {
				return ( TYPE* )( pair.mProxy ? pair.mProxy : pair.mGlobal );
			}
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void Invalidate () {
		
		u32 id = ZLContextClassID < TYPE >::GetID ();
		
		if ( id < this->mGlobals.Size ()) {
			this->mGlobals [ id ].mIsValid = false;
		}
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool IsValid () {
		
		u32 id = ZLContextClassID < TYPE >::GetID ();
		
		if ( id < this->mGlobals.Size ()) {
			return this->mGlobals [ id ].mIsValid;
		}
		return false;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void ProxyGlobal ( TYPE& proxy ) {
		
		u32 id = ZLContextClassID < TYPE >::GetID ();
		if ( id < this->mGlobals.Size ()) {
			this->mGlobals [ id ].mProxy = &proxy;
		}
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
	
	#ifdef ZL_USE_CONTEXT_THREADLOCAL
		static ZLThreadLocalPtr < GlobalsSet >		sGlobalsSet;
		static ZLThreadLocalPtr < ZLContext >		sInstance;
	#else
		static ZLLocalPtr < GlobalsSet >			sGlobalsSet;
		static ZLLocalPtr < ZLContext >				sInstance;
	#endif

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
template < typename TYPE, typename SUPER = RTTIBase >
class ZLContextClass :
	public virtual ZLContextClassBase,
	public virtual SUPER {
public:
	
	//----------------------------------------------------------------//
	inline static TYPE& Affirm () {
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
	inline static void Proxy ( TYPE& proxy ) {
		assert ( ZLContextMgr::Get ());
		ZLContextMgr::Get ()->ProxyGlobal < TYPE >( proxy );
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
