// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLOBALS_H
#define	MOAIGLOBALS_H

#include <moai-core/MOAIRtti.h>

//================================================================//
// MOAIGlobalIDBase
//================================================================//
class MOAIGlobalIDBase {
protected:

	//----------------------------------------------------------------//
	static u32 GetUniqueID ()	{
		static u32 counter = 0;	
		return counter++;
	};
};

//================================================================//
// MOAIGlobalID
//================================================================//
template < typename	TYPE >
class MOAIGlobalID :
	public MOAIGlobalIDBase {
public:
	
	//----------------------------------------------------------------//
	static u32 GetID ( void	) {
	
		static u32 type	= GetUniqueID ();
		return type;
	};
};

//================================================================//
// MOAIGlobalClassBase
//================================================================//
class MOAIGlobalClassBase {
protected:

	friend class MOAIGlobals;

	//----------------------------------------------------------------//
	virtual void			OnGlobalsFinalize			();
							MOAIGlobalClassBase			();
	virtual					~MOAIGlobalClassBase		();
};

//================================================================//
// MOAIGlobalPairBase
//================================================================//
class MOAIGlobalPair {
protected:
	friend class MOAIGlobals;

	MOAIGlobalClassBase*		mGlobalBase;
	void*						mGlobal;
	void*						mProxy;
	bool						mIsValid;
};

//================================================================//
// MOAIGlobals
//================================================================//
class MOAIGlobals {
private:

	friend class MOAIGlobalsMgr;

	enum {
		CHUNK_SIZE = 32,
	};

	ZLLeanArray < MOAIGlobalPair >	mGlobals;

	//----------------------------------------------------------------//
				MOAIGlobals			();
				~MOAIGlobals		();

public:
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* AffirmGlobal () {
		
		u32 id = MOAIGlobalID < TYPE >::GetID ();
		
		if ( this->mGlobals.Size () <= id ) {

			MOAIGlobalPair pair;
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
			
			MOAIGlobalPair& pair = this->mGlobals [ id ];
			
			pair.mGlobalBase	= global;
			pair.mGlobal		= global;
			pair.mProxy			= 0;
			pair.mIsValid		= true;
		}
		
		if ( !this->mGlobals [ id ].mIsValid ) {
			return 0;
		}
		
		return ( TYPE* )this->mGlobals [ id ].mGlobal;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* GetGlobal () {
		
		u32 id = MOAIGlobalID < TYPE >::GetID ();
		if ( id < this->mGlobals.Size ()) {
			MOAIGlobalPair& pair = this->mGlobals [ id ];
			if ( pair.mIsValid ) {
				return ( TYPE* )( pair.mProxy ? pair.mProxy : pair.mGlobal );
			}
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool IsValid () {
		
		u32 id = MOAIGlobalID < TYPE >::GetID ();
		
		if ( id < this->mGlobals.Size ()) {
			return this->mGlobals [ id ].mIsValid;
		}
		return false;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void ProxyGlobal ( TYPE& proxy ) {
		
		u32 id = MOAIGlobalID < TYPE >::GetID ();
		if ( id < this->mGlobals.Size ()) {
			this->mGlobals [ id ].mProxy	= &proxy;
		}
	}
};

//================================================================//
// MOAIGlobalsMgr
//================================================================//
class MOAIGlobalsMgr {
private:

	friend class MOAIGlobalClassBase;

	typedef STLSet < MOAIGlobals* >::iterator GlobalsSetIt;
	typedef STLSet < MOAIGlobals* > GlobalsSet;

	static GlobalsSet* sGlobalsSet;
	static MOAIGlobals* sInstance;

	//----------------------------------------------------------------//
							MOAIGlobalsMgr			();
							~MOAIGlobalsMgr			();

public:

	//----------------------------------------------------------------//
	static bool				Check					( MOAIGlobals* globals );
	static MOAIGlobals*		Create					();
	static void				Delete					( MOAIGlobals* globals );
	static void				Finalize				();
	static MOAIGlobals*		Get						();
	static MOAIGlobals*		Set						( MOAIGlobals* globals );
};

//================================================================//
// MOAIGlobalClass
//================================================================//
/**	@lua	MOAIGlobalClass
	@text	Base class for Moai singletons.
*/
template < typename TYPE, typename SUPER = RTTIBase >
class MOAIGlobalClass :
	public virtual MOAIGlobalClassBase,
	public virtual SUPER {
public:
	
	//----------------------------------------------------------------//
	inline static TYPE& Affirm () {
		assert ( MOAIGlobalsMgr::Get ());
		TYPE* global = MOAIGlobalsMgr::Get ()->AffirmGlobal < TYPE >();
		assert ( global );
		return *global;
	}
	
	//----------------------------------------------------------------//
	inline static TYPE& Get () {
		assert ( MOAIGlobalsMgr::Get ());
		TYPE* global = MOAIGlobalsMgr::Get ()->GetGlobal < TYPE >();
		assert ( global );
		return *global;
	}
	
	//----------------------------------------------------------------//
	inline static bool IsValid () {
		assert ( MOAIGlobalsMgr::Get ());
		return MOAIGlobalsMgr::Get ()->IsValid < TYPE >();
	}
	
	//----------------------------------------------------------------//
	inline static void Proxy ( TYPE& proxy ) {
		assert ( MOAIGlobalsMgr::Get ());
		MOAIGlobalsMgr::Get ()->ProxyGlobal < TYPE >( proxy );
	}
};

//================================================================//
// MOAIScopedContext
//================================================================//
class MOAIScopedContext {
private:

	MOAIGlobals*	mOriginalContext;

public:
	
	//----------------------------------------------------------------//
						MOAIScopedContext			();
						~MOAIScopedContext			();
};

#endif
