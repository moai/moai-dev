// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIAL_H
#define	MOAIMATERIAL_H

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIAbstractMaterial.h>

//================================================================//
// MOAIMaterialNamedGlobal
//================================================================//
template < typename TYPE >
class MOAIMaterialNamedGlobal {
private:

	friend class MOAIMaterial;
	
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
// MOAIMaterial
//================================================================//
// TODO: doxygen
class MOAIMaterial :
	public virtual MOAIAbstractMaterial {
private:

	MOAIMaterialNamedGlobal < MOAILight >*		mLights;
	MOAIMaterialNamedGlobal < MOAITexture >*	mTextures;
	
	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass						( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs						( MOAIComposer& composer, MOAILuaState& state );
	void				MOAIAbstractMaterial_ApplyGlobals					( MOAIAbstractMaterialInterface& dest );
	MOAILight*			MOAIAbstractMaterial_ClearGlobals					();
	MOAILight*			MOAIAbstractMaterial_GetLight						( u32 name );
	MOAITexture*		MOAIAbstractMaterial_GetTexture						( u32 name );
	void				MOAIAbstractMaterial_SetLight						( u32 name, MOAILight* light );
	void				MOAIAbstractMaterial_SetTexture						( u32 name, MOAITexture* texture );
	MOAIMaterial&		MOAIAbstractMaterialInterface_AffirmMaterial		();
	MOAIMaterial*		MOAIAbstractMaterialInterface_GetMaterial			();

public:

	DECL_LUA_FACTORY ( MOAIMaterial )

	//----------------------------------------------------------------//
						MOAIMaterial			();
	virtual				~MOAIMaterial			();
};

#endif
