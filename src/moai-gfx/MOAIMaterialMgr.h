// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALMGR_H
#define	MOAIMATERIALMGR_H

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
		MOAITextureBase*	mTexture;
		MOAILight*			mLight;
		void*				mPtr;
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
	public MOAIMaterialBase {
private:
	
	friend class MOAIMaterialStackScope;

	ZLLeanPool < MOAIMaterialStackClearCmd, 32 >		mRestoreCmdPool;
	ZLLeanArray < MOAIMaterialGlobal >					mNamedLights;		// TODO: semantics
	ZLLeanArray < MOAIMaterialGlobal >					mNamedTextures;		// TODO: semantics
	ZLLeanStack < MOAIMaterialStackFrame, 8 >			mStack;

	//----------------------------------------------------------------//
	void				Compose						( const MOAIMaterial& material );
	void				SetGlobal					( MOAIMaterialGlobal& global, void* ptr );

public:

	static const u32 MAX_GLOBAL_LIGHTS		= 256;
	static const u32 MAX_GLOBAL_TEXTURES	= 32;

	//----------------------------------------------------------------//
	//void				Clear						();
	MOAILight*			GetLight					( u32 lightID );
	MOAITextureBase*	GetTexture					( u32 textureID );
	void				LoadGfxState				();
						MOAIMaterialMgr				();
						~MOAIMaterialMgr			();
	void				Pop							();
	void				Push						( const MOAIMaterial* material = 0 );
	void				SetBlendMode				( const ZLBlendMode& blendMode );
	void				SetCullMode					( int cullMode );
	void				SetDepthMask				( bool depthMask );
	void				SetDepthTest				( int depthTest );
	void				SetLight					( u32 lightID, MOAILight* light );
	void				SetShader					( u32 shaderID );
	void				SetShader					( MOAIShader* shader );
	void				SetTexture					( MOAITextureBase* texture );
	void				SetTexture					( u32 textureID, MOAITextureBase* texture );
};

#endif
