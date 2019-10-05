// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALMGR_H
#define	MOAIMATERIALMGR_H

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIMaterial.h>

class MOAILight;
class MOAIMaterialMgr;

//================================================================//
// MOAIMaterialStackScope
//================================================================//
class MOAIMaterialStackScope {
private:

	MOAIMaterialMgr&	mMaterialStack;
	u32					mRestoreTop;

public:

	GET ( MOAIMaterialMgr&, MaterialStack, mMaterialStack )

	//----------------------------------------------------------------//
	operator MOAIMaterialMgr& () {
	
		return this->mMaterialStack;
	}

	//----------------------------------------------------------------//
					MOAIMaterialStackScope		();
					~MOAIMaterialStackScope		();
};

//================================================================//
// MOAIMaterialGlobal
//================================================================//
class MOAIMaterialGlobal {
private:

	friend class MOAIMaterialMgr;
	
	union {
		MOAITexture*	mTexture;
		MOAILight*		mLight;
		void*			mPtr;
	};
	
	u32					mStackDepth;

public:

	//----------------------------------------------------------------//
					MOAIMaterialGlobal			();
					~MOAIMaterialGlobal			();
};

//================================================================//
// MOAIMaterialStackClearCmd
//================================================================//
class MOAIMaterialStackClearCmd {
private:

	friend class MOAIMaterialMgr;
	
	MOAIMaterialGlobal*				mGlobal;
	MOAIMaterialStackClearCmd*		mNext;
};

//================================================================//
// MOAIMaterialStackFrame
//================================================================//
class MOAIMaterialStackFrame {
private:

	friend class MOAIMaterialMgr;
	
	u32								mFlags;
	MOAIMaterialStackClearCmd*		mClearList;
};

//================================================================//
// MOAIMaterialMgr
//================================================================//
class MOAIMaterialMgr :
	public ZLContextClass < MOAIMaterialMgr >,
	public virtual MOAIAbstractMaterial {
private:
	
	friend class MOAIMaterialStackScope;

	ZLLeanPool < MOAIMaterialStackClearCmd, 32 >	mRestoreCmdPool;
	ZLLeanArray < MOAIMaterialGlobal >				mNamedLights;		// TODO: semantics
	ZLLeanArray < MOAIMaterialGlobal >				mNamedTextures;		// TODO: semantics
	ZLLeanStack < MOAIMaterialStackFrame, 8 >		mStack;

	//----------------------------------------------------------------//
	void				SetGlobal					( MOAIMaterialGlobal& global, void* ptr );

	//----------------------------------------------------------------//
	void						MOAIAbstractMaterial_ApplyGlobals					( MOAIAbstractMaterialInterface& dest );
	MOAILight*					MOAIAbstractMaterial_ClearGlobals					();
	MOAILight*					MOAIAbstractMaterial_GetLight						( u32 name );
	MOAITexture*				MOAIAbstractMaterial_GetTexture						( u32 name );
	void						MOAIAbstractMaterial_SetLight						( u32 name, MOAILight* light );
	void						MOAIAbstractMaterial_SetTexture						( u32 name, MOAITexture* texture );
	MOAIAbstractMaterial&		MOAIAbstractMaterialInterface_AffirmMaterial		();
	MOAIAbstractMaterial*		MOAIAbstractMaterialInterface_GetMaterial			();

public:

	IMPLEMENT_ABSTRACT_FINALIZABLE ( MOAIMaterialMgr );

	//----------------------------------------------------------------//
						MOAIMaterialMgr				();
						~MOAIMaterialMgr			();
	void				Pop							();
	void				Push						( MOAIAbstractMaterial* material = 0 );
};

#endif
