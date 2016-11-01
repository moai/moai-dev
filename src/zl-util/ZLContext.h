// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLCONTEXT_H
#define	ZLCONTEXT_H

#include <zl-common/zl_types.h>
#include <zl-util/STLSet.h>
#include <zl-util/ZLLeanArray.h>
#include <zl-util/ZLRtti.h>
#include <zl-vfs/ZLThreadLocalPtr.h>

//================================================================//
// ZLContextIDBase
//================================================================//
class ZLContextIDBase {
protected:

	//----------------------------------------------------------------//
	static u32 GetUniqueID ()	{
		static u32 counter = 0;	
		return counter++;
	};
};

//================================================================//
// ZLContextID
//================================================================//
template < typename	TYPE >
class ZLContextID :
	public ZLContextIDBase {
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

	friend class ZLContexts;

	//----------------------------------------------------------------//
	virtual void			OnGlobalsFinalize			();
	virtual void			OnGlobalsInitialize			();
							ZLContextClassBase			();
	virtual					~ZLContextClassBase			();
};

//================================================================//
// ZLContextPairBase
//================================================================//
class ZLContextPair {
protected:
	friend class ZLContexts;

	ZLContextClassBase*			mGlobalBase;
	void*						mGlobal;
	void*						mProxy;
	bool						mIsValid;
};

//================================================================//
// ZLContexts
//================================================================//
class ZLContexts {
private:

	friend class ZLContextsMgr;

	enum {
		CHUNK_SIZE = 32,
	};

	ZLLeanArray < ZLContextPair >	mGlobals;

	//----------------------------------------------------------------//
				ZLContexts			();
				~ZLContexts			();

public:
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* AffirmGlobal () {
		
		u32 id = ZLContextID < TYPE >::GetID ();
		
		if ( this->mGlobals.Size () <= id ) {

			ZLContextPair pair;
			pair.mGlobalBase	= 0;
			pair.mGlobal		= 0;
			pair.mProxy			= 0;
			pair.mIsValid		= false;
			
			this->mGlobals.Grow ( id, CHUNK_SIZE, pair );
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
		
		u32 id = ZLContextID < TYPE >::GetID ();
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
		
		u32 id = ZLContextID < TYPE >::GetID ();
		
		if ( id < this->mGlobals.Size ()) {
			this->mGlobals [ id ].mIsValid = false;
		}
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool IsValid () {
		
		u32 id = ZLContextID < TYPE >::GetID ();
		
		if ( id < this->mGlobals.Size ()) {
			return this->mGlobals [ id ].mIsValid;
		}
		return false;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void ProxyGlobal ( TYPE& proxy ) {
		
		u32 id = ZLContextID < TYPE >::GetID ();
		if ( id < this->mGlobals.Size ()) {
			this->mGlobals [ id ].mProxy = &proxy;
		}
	}
};

//================================================================//
// ZLContextsMgr
//================================================================//
class ZLContextsMgr {
private:

	friend class ZLContextClassBase;

	typedef STLSet < ZLContexts* >::iterator GlobalsSetIt;
	typedef STLSet < ZLContexts* > GlobalsSet;

	static ZLThreadLocalPtr < GlobalsSet >		sGlobalsSet;
	static ZLThreadLocalPtr < ZLContexts >		sInstance;

	//----------------------------------------------------------------//
							ZLContextsMgr			();
							~ZLContextsMgr			();

public:

	//----------------------------------------------------------------//
	static bool				Check					( ZLContexts* globals );
	static ZLContexts*		Create					();
	static void				Delete					( ZLContexts* globals );
	static void				Finalize				();
	static ZLContexts*		Get						();
	static ZLContexts*		Set						( ZLContexts* globals );
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
		assert ( ZLContextsMgr::Get ());
		TYPE* global = ZLContextsMgr::Get ()->AffirmGlobal < TYPE >();
		assert ( global );
		return *global;
	}
	
	//----------------------------------------------------------------//
	inline static TYPE& Get () {
		assert ( ZLContextsMgr::Get ());
		TYPE* global = ZLContextsMgr::Get ()->GetGlobal < TYPE >();
		assert ( global );
		return *global;
	}
	
	//----------------------------------------------------------------//
	void InvalidateContext () {
		assert ( ZLContextsMgr::Get ());
		ZLContextsMgr::Get ()->Invalidate < TYPE >();
	}
	
	//----------------------------------------------------------------//
	inline static bool IsValid () {
		assert ( ZLContextsMgr::Get ());
		return ZLContextsMgr::Get ()->IsValid < TYPE >();
	}
	
	//----------------------------------------------------------------//
	inline static void Proxy ( TYPE& proxy ) {
		assert ( ZLContextsMgr::Get ());
		ZLContextsMgr::Get ()->ProxyGlobal < TYPE >( proxy );
	}
};

//================================================================//
// ZLScopedContext
//================================================================//
class ZLScopedContext {
private:

	ZLContexts*		mOriginalContext;

public:
	
	//----------------------------------------------------------------//
	void				Clear						();
						ZLScopedContext				();
						~ZLScopedContext			();
};

#endif
