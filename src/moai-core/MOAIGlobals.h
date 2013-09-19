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
private:

	friend class MOAIGlobals;

protected:

	//----------------------------------------------------------------//
	virtual void	OnGlobalsFinalize			();
	virtual void	OnGlobalsRestore			();
	virtual void	OnGlobalsRetire				();
					MOAIGlobalClassBase			();
	virtual			~MOAIGlobalClassBase		();
};

//================================================================//
// MOAIGlobalPair
//================================================================//
class MOAIGlobalPair {
private:
	friend class MOAIGlobals;

	MOAIGlobalClassBase*		mGlobal;
	void*						mPtr;
	bool						mIsValid;
};

//================================================================//
// MOAIGlobals
//================================================================//
class MOAIGlobals {
private:

	friend class MOAIGlobalsMgr;
	friend class MOAIGlobalClassFinalizer;

	enum {
		CHUNK_SIZE = 32,
	};

	ZLLeanArray < MOAIGlobalPair >	mGlobals;

	//----------------------------------------------------------------//
	void		Restore				();
	void		Retire				();
				MOAIGlobals			();
				~MOAIGlobals		();

public:
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* AffirmGlobal () {
		
		u32 id = MOAIGlobalID < TYPE >::GetID ();
		
		if ( this->mGlobals.Size () <= id ) {

			MOAIGlobalPair pair;
			pair.mGlobal	= 0;
			pair.mPtr		= 0;
			pair.mIsValid	= true;
			
			this->mGlobals.Grow ( id, CHUNK_SIZE, pair );
		}
		
		if ( !this->mGlobals [ id ].mIsValid ) {
			return 0;
		}
		
		if ( !this->mGlobals [ id ].mPtr ) {
			TYPE* global = new TYPE;
			this->mGlobals [ id ].mGlobal	= global;
			this->mGlobals [ id ].mPtr		= global;
		}
		
		return ( TYPE* )this->mGlobals [ id ].mPtr;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* GetGlobal () {
		
		u32 id = MOAIGlobalID < TYPE >::GetID ();
		if ( id < this->mGlobals.Size ()) {
			if ( this->mGlobals [ id ].mIsValid ) {
				return ( TYPE* )this->mGlobals [ id ].mPtr;
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
};

//================================================================//
// MOAIGlobalsMgr
//================================================================//
class MOAIGlobalsMgr {
private:

	typedef STLSet < MOAIGlobals* >::iterator GlobalsSetIt;
	typedef STLSet < MOAIGlobals* > GlobalsSet;

	static GlobalsSet* sGlobalsSet;
	static MOAIGlobals* sInstance;

	//----------------------------------------------------------------//
						MOAIGlobalsMgr			();
						~MOAIGlobalsMgr			();

public:

	//----------------------------------------------------------------//
	static MOAIGlobals*		Create			();	
	static void				Delete			( MOAIGlobals* globals );
	static void				Finalize		();
	static MOAIGlobals*		Get				();
	static void				Set				( MOAIGlobals* globals );
};

//================================================================//
// MOAIGlobalClass
//================================================================//
template < typename TYPE, typename SUPER = RTTIBase >
class MOAIGlobalClass :
	public MOAIGlobalClassBase,
	public virtual SUPER {
public:
	
	//----------------------------------------------------------------//
	inline static TYPE& Affirm () {
		TYPE* global = MOAIGlobalsMgr::Get ()->AffirmGlobal < TYPE >();
		assert ( global );
		return *global;
	}
	
	//----------------------------------------------------------------//
	inline static TYPE& Get () {
		TYPE* global = MOAIGlobalsMgr::Get ()->GetGlobal < TYPE >();
		assert ( global );
		return *global;
	}
	
	//----------------------------------------------------------------//
	inline static bool IsValid () {
		return MOAIGlobalsMgr::Get ()->IsValid < TYPE >();
	}
};

#endif
