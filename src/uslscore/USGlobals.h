// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USGLOBALS_H
#define	USGLOBALS_H

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
// USGlobalPair
//================================================================//
class USGlobalPair {
private:

	friend class USGlobals;

	USObject*		mObject;
	void*			mPtr;
};

//================================================================//
// USGlobals
//================================================================//
class USGlobals {
private:

	enum {
		CHUNK_SIZE = 32,
	};

	USLeanArray < USGlobalPair > mGlobals;

	//----------------------------------------------------------------//
						USGlobals			();
						~USGlobals			();

public:

	//----------------------------------------------------------------//
	static USGlobals*	Create				();
	static void			Delete				( USGlobals* globals );	
	static void			Finalize			();
	static USGlobals*	Get					();
	static void			Set					( USGlobals* globals );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* GetGlobal () {
		
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
// USGlobalClass
//================================================================//
template < typename TYPE, typename SUPER = USObject >
class USGlobalClass :
public virtual SUPER {
public:
	
	//----------------------------------------------------------------//
	inline static TYPE& Get () {
		TYPE* global = USGlobals::Get ()->GetGlobal < TYPE >();
		assert ( global );
		return *global;
	}
	
	//----------------------------------------------------------------//
	inline static bool IsValid () {
		return USGlobals::Get ()->IsValid < TYPE >();
	}
};

#endif
