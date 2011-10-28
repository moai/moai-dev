// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USGLOBALS_H
#define	USGLOBALS_H

#include <uslscore/STLList.h>
#include <uslscore/STLSet.h>
#include <uslscore/USAccessors.h>
#include <uslscore/USObject.h>
#include <uslscore/USLeanArray.h>

//================================================================//
// USGloablIDBase
//================================================================//
class USGloablIDBase {
protected:

	//----------------------------------------------------------------//
	static u32 GetUniqueID ()	{
		static u32 counter = 0;	
		return counter++;
	};
};

//================================================================//
// USGlobalID
//================================================================//
template < typename	TYPE >
class USGlobalID :
	public USGloablIDBase {
public:
	
	//----------------------------------------------------------------//
	static u32 GetID ( void	) {
	
		static u32 type	= GetUniqueID ();
		return type;
	};
};

//================================================================//
// USGlobalClassFinalizer
//================================================================//
class USGlobalClassFinalizer {
private:

	friend class USGlobals;
	
	USGlobalClassFinalizer* mNext;

protected:

	//----------------------------------------------------------------//
	virtual void	OnGlobalsFinalize			();
	virtual void	OnGlobalsRestore			();
	virtual void	OnGlobalsRetire				();
					USGlobalClassFinalizer		();
	virtual			~USGlobalClassFinalizer		();
};

//================================================================//
// USGlobalPair
//================================================================//
class USGlobalPair {
private:
	friend class USGlobals;

	USObject*					mObject;
	void*						mPtr;
};

//================================================================//
// USGlobals
//================================================================//
class USGlobals {
private:

	friend class USGlobalsMgr;
	friend class USGlobalClassFinalizer;

	enum {
		CHUNK_SIZE = 32,
	};

	USLeanArray < USGlobalPair >	mGlobals;
	USGlobalClassFinalizer*			mFinalizers;

	//----------------------------------------------------------------//
	void		Restore			();
	void		Retire			();
				USGlobals		();
				~USGlobals		();

public:
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* AffirmGlobal () {
		
		u32 id = USGlobalID < TYPE >::GetID ();
		
		if ( !this->IsValid < TYPE >()) {
			
			TYPE* global = new TYPE;
			global->Retain ();
			
			USGlobalPair pair;
			pair.mObject	= 0;
			pair.mPtr		= 0;
			
			this->mGlobals.Grow ( id + 1, CHUNK_SIZE, pair );
			this->mGlobals [ id ].mObject = global;
			this->mGlobals [ id ].mPtr = global;
		}
		return ( TYPE* )this->mGlobals [ id ].mPtr;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* GetGlobal () {
		
		u32 id = USGlobalID < TYPE >::GetID ();
		if ( id < this->mGlobals.Size ()) {
			return ( TYPE* )this->mGlobals [ id ].mPtr;
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool IsValid () {
		
		u32 id = USGlobalID < TYPE >::GetID ();
		
		if ( id < this->mGlobals.Size ()) {
			if ( this->mGlobals [ id ].mPtr ) {
				return true;
			}
		}
		return false;
	}
};

//================================================================//
// USGlobalsMgr
//================================================================//
class USGlobalsMgr {
private:

	typedef STLSet < USGlobals* >::iterator GlobalsSetIt;
	typedef STLSet < USGlobals* > GlobalsSet;

	static GlobalsSet* sGlobalsSet;
	static USGlobals* sInstance;

	//----------------------------------------------------------------//
						USGlobalsMgr			();
						~USGlobalsMgr			();

public:

	//----------------------------------------------------------------//
	static USGlobals*		Create			();	
	static void				Delete			( USGlobals* globals );	
	static void				Finalize		();
	static USGlobals*		Get				();
	static void				Set				( USGlobals* globals );
};

//================================================================//
// USGlobalClass
//================================================================//
template < typename TYPE, typename SUPER = USObject >
class USGlobalClass :
public virtual SUPER {
public:
	
	//----------------------------------------------------------------//
	inline static TYPE& Affirm () {
		TYPE* global = USGlobalsMgr::Get ()->AffirmGlobal < TYPE >();
		assert ( global );
		return *global;
	}
	
	//----------------------------------------------------------------//
	inline static TYPE& Get () {
		TYPE* global = USGlobalsMgr::Get ()->GetGlobal < TYPE >();
		assert ( global );
		return *global;
	}
	
	//----------------------------------------------------------------//
	inline static bool IsValid () {
		return USGlobalsMgr::Get ()->IsValid < TYPE >();
	}
};

#endif
