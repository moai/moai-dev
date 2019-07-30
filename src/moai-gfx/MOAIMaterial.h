// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIAL_H
#define	MOAIMATERIAL_H

#include <moai-gfx/MOAIMaterialBase.h>
#include <moai-gfx/MOAIMaterialInterface.h>

class MOAILight;
class MOAITextureBaseGL;

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
// MOAIMaterial
//================================================================//
// TODO: doxygen
class MOAIMaterial :
	public virtual MOAIMaterialBase,
	public virtual MOAIMaterialInterface {
private:

	friend class MOAIMaterialMgr;
	friend class MOAIMaterialInterface;

	MOAIMaterialNamedGlobal < MOAILight >*		mLights;
	MOAIMaterialNamedGlobal < MOAITexture >*	mTextures;
	
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

	//----------------------------------------------------------------//
	static u32			GetNamedGlobalID			( MOAILuaState& state, int idx );

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
	MOAIMaterial&		MOAIMaterialInterface_AffirmMaterial		();
	MOAIMaterial*		MOAIMaterialInterface_GetMaterial			();

public:

	DECL_LUA_FACTORY ( MOAIMaterial )

	//----------------------------------------------------------------//
	void				Clear					();
	MOAILight*			GetLight				( u32 name );
	MOAITexture*		GetTexture				();
	MOAITexture*		GetTexture				( u32 name );
						MOAIMaterial			();
	virtual				~MOAIMaterial			();
	void				SetLight				( u32 name );
	void				SetLight				( u32 name, MOAILight* light );
	void				SetTexture				();
	void				SetTexture				( MOAITexture* texture );
	void				SetTexture				( u32 name );
	void				SetTexture				( u32 name, MOAITexture* texture );
};

#endif
