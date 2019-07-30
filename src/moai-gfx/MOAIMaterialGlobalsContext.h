// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALGLOBALSCONTEXT_H
#define	MOAIMATERIALGLOBALSCONTEXT_H

#include <moai-gfx/MOAIAbstractMaterialGlobalsContext.h>

//================================================================//
// MOAIMaterialNamedGlobal
//================================================================//
template < typename TYPE >
class MOAIMaterialNamedGlobal {
private:

	friend class MOAIMaterialGlobalsContext;
	
	u32							mName;
	ZLStrongPtr < TYPE >		mValue;
	MOAIMaterialNamedGlobal*	mNext;

public:

	//----------------------------------------------------------------//
	static void Clear ( MOAIMaterialNamedGlobal < TYPE >*& list ) {

		while ( list ) {
			MOAIMaterialNamedGlobal < TYPE >* global = list;
			list = list->mNext;
			delete global;
		}
	}

	//----------------------------------------------------------------//
	MOAIMaterialNamedGlobal () :
		mName ( MOAIMaterialGlobals::MOAI_UNKNOWN_MATERIAL_GLOBAL ),
		mNext ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	static MOAIMaterialNamedGlobal < TYPE >* FindNamedGlobal ( MOAIMaterialNamedGlobal < TYPE >* list, u32 name ) {
	
		for ( ; list; list = list->mNext ) {
			if ( list->mName == name ) return list;
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	static MOAIMaterialNamedGlobal < TYPE >* SetNamedGlobal ( MOAIMaterialNamedGlobal < TYPE >*& list, u32 name, TYPE* value ) {
	
		if ( value ) {
	
			MOAIMaterialNamedGlobal < TYPE >* global = MOAIMaterialNamedGlobal < TYPE >::FindNamedGlobal ( list, name );
			
			if ( !global ) {
			
				global = new MOAIMaterialNamedGlobal < TYPE >();
				
				global->mName = name;
				global->mValue = value;
				global->mNext = list;
				list = global;
			}
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
};

//================================================================//
// MOAIMaterialGlobalsContext
//================================================================//
// TODO: doxygen
class MOAIMaterialGlobalsContext :
	public MOAIAbstractMaterialGlobalsContext {
protected:

	MOAIMaterialNamedGlobal < MOAILight >*		mLights;
	MOAIMaterialNamedGlobal < MOAITexture >*	mTextures;

	//----------------------------------------------------------------//
	void			MOAIAbstractMaterialGlobalsContext_Apply			( MOAIAbstractMaterialGlobalsContext& dest );
	MOAILight*		MOAIAbstractMaterialGlobalsContext_Clear			();
	MOAILight*		MOAIAbstractMaterialGlobalsContext_GetLight			( u32 name );
	MOAITexture*	MOAIAbstractMaterialGlobalsContext_GetTexture		( u32 name );
	void			MOAIAbstractMaterialGlobalsContext_SetLight			( u32 name, MOAILight* light );
	void			MOAIAbstractMaterialGlobalsContext_SetTexture		( u32 name, MOAITexture* texture );

public:

	//----------------------------------------------------------------//
					MOAIMaterialGlobalsContext		();
					~MOAIMaterialGlobalsContext		();
};

#endif
