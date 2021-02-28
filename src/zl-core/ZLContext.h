// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLCONTEXT_H
#define	ZLCONTEXT_H

#include <zl-core/STLSet.h>
#include <zl-core/ZLLeanArray.h>
#include <zl-core/ZLRtti.h>
#include <zl-core/ZLThreadLocalPtr.h>

class ZLContext;

//================================================================//
// ZLHasContext
//================================================================//
class ZLHasContext {
protected:

	ZLContext&		mContext;

public:

	//----------------------------------------------------------------//
	template < typename TYPE > TYPE& 	Get 		();
	template < typename TYPE > TYPE& 	Get 		() const;
	template < typename TYPE > bool 	IsValid 	();
	
	//----------------------------------------------------------------//
	ZLContext&		GetContext				();
	ZLContext&		GetContext				() const;
					ZLHasContext			( ZLContext& context );
					~ZLHasContext			();
};

//================================================================//
// ZLContextClass
//================================================================//
class ZLContextClass :
	public virtual ZLHasContext {
protected:

	friend class ZLContext;
	
	ZLIndex				mContextID;
	
	//----------------------------------------------------------------//
	virtual void		ZLContextClass_Finalize		();
	virtual void		ZLContextClass_Initialize	();

public:

	//----------------------------------------------------------------//
	void				Finalize					();
	void				Initialize					();
	bool				IsValid						();
						ZLContextClass				( ZLContext& context );
	virtual				~ZLContextClass				();
};

//================================================================//
// ZLContextPair
//================================================================//
class ZLContextPair {
public:

	ZLContextClass*				mGlobalBase;
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
		mAliasID ( 0 ) {
	}
};

//================================================================//
// ZLContext
//================================================================//
class ZLContext {
private:

	enum {
		CHUNK_SIZE = 32,
	};

	ZLLeanArray < ZLContextPair >	mGlobals;

	//----------------------------------------------------------------//
	static ZLIndex GetUniqueID () {
	
		static ZLIndex counter = 0;
		return counter++;
	};

	//----------------------------------------------------------------//
	template < typename TYPE >
	static ZLIndex GetClassID () {
	
		static ZLIndex type	= ZLContext::GetUniqueID ();
		return type;
	};

	//----------------------------------------------------------------//
	void		AffirmSize			( ZLSize size );

public:
	
	//----------------------------------------------------------------//
				ZLContext			();
				~ZLContext			();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE& Affirm () {
		
		ZLIndex globalID = this->GetClassID < TYPE >();
		this->AffirmSize (( ZLSize )globalID + 1 );
		
		if ( !this->mGlobals [ globalID ].mGlobal ) {
			
			TYPE* global = new TYPE ( *this );
			ZLContextClass* contextClass = global;
//			contextClass->mContext 		= this;
			contextClass->mContextID 	= globalID;
			
			ZLContextPair& pair = this->mGlobals [ globalID ];
			
			pair.mGlobalBase	= contextClass;
			pair.mGlobal		= global;
			pair.mAliasOf		= NULL;
			pair.mIsValid		= true;
			
			pair.mGlobalBase->Initialize ();
		}
		return *( TYPE* )this->mGlobals [ globalID ].mGlobal;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE& Get () {
		
		TYPE* type = NULL;
		
		ZLIndex globalID = this->GetClassID < TYPE >();
		if ( globalID < this->mGlobals.Size ()) {
			ZLContextPair& pair = this->mGlobals [ globalID ];
			if ((( pair.mAliasOf == NULL ) && pair.mIsValid ) || ( pair.mAliasOf && pair.mAliasOf->mIsValid )) {
				type = ( TYPE* )pair.mGlobal;
			}
		}
		assert ( type );
		return *type;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool IsValid () {
		
		ZLIndex globalID = this->GetClassID < TYPE >();
		
		if ( globalID < this->mGlobals.Size ()) {
			ZLContextPair& pair = this->mGlobals [ globalID ];
			return ((( pair.mAliasOf == NULL ) && pair.mIsValid ) || ( pair.mAliasOf && pair.mAliasOf->mIsValid ));
		}
		return false;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE, typename ALIAS_TYPE >
	ALIAS_TYPE* RegisterAlias () {
		
		ZLIndex globalID = this->GetClassID < TYPE >();
		assert ( globalID < this->mGlobals.Size ());
		ZLContextPair& globalPair = this->mGlobals [ globalID ];
		TYPE* global = ( TYPE* )globalPair.mGlobal;
		assert ( global );

		ZLIndex aliasID = this->GetClassID < ALIAS_TYPE >();
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

//----------------------------------------------------------------//
template < typename TYPE >
TYPE& ZLHasContext::Get () {

	return this->mContext.Get < TYPE >();
}

//----------------------------------------------------------------//
template < typename TYPE >
TYPE& ZLHasContext::Get () const {

	return this->mContext.Get < TYPE >();
}

//----------------------------------------------------------------//
template < typename TYPE >
bool ZLHasContext::IsValid () {
	
	return this->mContext.IsValid < TYPE >();
}

#endif
