// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIAL_H
#define	MOAIMATERIAL_H

#include <moai-sim/MOAIMaterialBase.h>

class MOAILight;
class MOAITextureBase;

#define MOAI_UNKNOWN_MATERIAL_GLOBAL 0x80000000

//================================================================//
// MOAIMaterialNamedGlobal
//================================================================//
template < typename TYPE >
class MOAIMaterialNamedGlobal {
private:

	friend class MOAIMaterial;
	friend class MOAIMaterialMgr;
	
	u32							mName;
	ZLStrongPtr < TYPE >		mValue;
	MOAIMaterialNamedGlobal*	mNext;

public:

	//----------------------------------------------------------------//
	MOAIMaterialNamedGlobal () :
		mName ( MOAI_UNKNOWN_MATERIAL_GLOBAL ),
		mNext ( 0 ) {
	}
};

//================================================================//
// MOAILightSet
//================================================================//
// TODO: doxygen
class MOAIMaterial :
	public MOAIMaterialBase {
private:

	friend class MOAIMaterialMgr;

	MOAIMaterialNamedGlobal < MOAILight >*			mLights;
	MOAIMaterialNamedGlobal < MOAITextureBase >*	mTextures;
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void ClearNamedGlobalList ( MOAIMaterialNamedGlobal < TYPE >*& list ) {

		while ( list ) {
		
			MOAIMaterialNamedGlobal < TYPE >* global = list;
			list = list->mNext;
			delete global;
		}
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	MOAIMaterialNamedGlobal < TYPE >* FindNamedGlobal ( MOAIMaterialNamedGlobal < TYPE >* list, u32 name ) {
	
		for ( ; list; list = list->mNext ) {
			if ( list->mName == name ) return list;
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	MOAIMaterialNamedGlobal < TYPE >* SetNamedGlobal ( MOAIMaterialNamedGlobal < TYPE >*& list, u32 name, TYPE* value ) {
	
		if ( value ) {
	
			MOAIMaterialNamedGlobal < TYPE >* global = this->FindNamedGlobal < TYPE >( list, name );
			
			if ( !global ) {
			
				global = new MOAIMaterialNamedGlobal < TYPE >();
				
				global->mName = name;
				global->mNext = list;
				list = global;
			}
			global->mValue = value;
			return global;
		}
		else {
		
			MOAIMaterialNamedGlobal < TYPE >* cursor = list;
			list = 0;
			
			while ( cursor ) {
			
				MOAIMaterialNamedGlobal < TYPE >* global = cursor;
				cursor = cursor->mNext;
				
				if ( global->mName == name ) {
					delete global;
				}
				else {
					global->mNext = list;
					list = global;
				}
			}
		}
		return 0;
	}

public:

	//----------------------------------------------------------------//
	void				Clear					();
	MOAILight*			GetLight				( u32 name );
	MOAITextureBase*	GetTexture				();
	MOAITextureBase*	GetTexture				( u32 name );
						MOAIMaterial			();
	virtual				~MOAIMaterial			();
	void				SetLight				( u32 name );
	void				SetLight				( u32 name, MOAILight* light );
	void				SetTexture				();
	void				SetTexture				( MOAITextureBase* texture );
	void				SetTexture				( u32 name );
	void				SetTexture				( u32 name, MOAITextureBase* texture );
};

#endif
