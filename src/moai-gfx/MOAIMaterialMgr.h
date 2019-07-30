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
	public MOAIAbstractMaterialInterface,
	public MOAIAbstractMaterialGlobalsContext {
private:
	
	friend class MOAIMaterialStackScope;

	ZLLeanPool < MOAIMaterialStackClearCmd, 32 >	mRestoreCmdPool;
	ZLLeanArray < MOAIMaterialGlobal >				mNamedLights;		// TODO: semantics
	ZLLeanArray < MOAIMaterialGlobal >				mNamedTextures;		// TODO: semantics
	ZLLeanStack < MOAIMaterialStackFrame, 8 >		mStack;

	ZLStrongPtr < MOAIMaterial >					mComposedMaterial;

	//----------------------------------------------------------------//
	void				SetGlobal					( MOAIMaterialGlobal& global, void* ptr );

	//----------------------------------------------------------------//
	void				MOAIAbstractMaterialGlobalsContext_Apply			( MOAIAbstractMaterialGlobalsContext& dest );
	MOAILight*			MOAIAbstractMaterialGlobalsContext_Clear			();
	MOAILight*			MOAIAbstractMaterialGlobalsContext_GetLight			( u32 name );
	MOAITexture*		MOAIAbstractMaterialGlobalsContext_GetTexture		( u32 name );
	void				MOAIAbstractMaterialGlobalsContext_SetLight			( u32 name, MOAILight* light );
	void				MOAIAbstractMaterialGlobalsContext_SetTexture		( u32 name, MOAITexture* texture );
	MOAIMaterial&		MOAIAbstractMaterialInterface_AffirmMaterial		();
	MOAIMaterial*		MOAIAbstractMaterialInterface_GetMaterial			();

public:

	//----------------------------------------------------------------//
	MOAILight*			GetLight					( u32 lightID );
	MOAITexture*		GetTexture					( u32 textureID );
						MOAIMaterialMgr				();
						~MOAIMaterialMgr			();
	void				Pop							();
	void				Push						( MOAIMaterial* material = 0 );
};

#endif
