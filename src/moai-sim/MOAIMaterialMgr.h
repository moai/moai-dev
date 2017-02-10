// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALMGR_H
#define	MOAIMATERIALMGR_H

#include <moai-sim/MOAIMaterial.h>

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
// MOAIGlobalLight
//================================================================//
class MOAIGlobalLight {
private:

	friend MOAIMaterialMgr;
	
	MOAILight*		mLight;
	u32				mStackDepth;
};

//================================================================//
// MOAIGlobalTexture
//================================================================//
class MOAIGlobalTexture {
private:

	friend MOAIMaterialMgr;
	
	MOAITextureBase*	mTexture;
	u32					mStackDepth;
};

//================================================================//
// MOAIMaterialStackClearCmd
//================================================================//
class MOAIMaterialStackClearCmd {
private:

	friend MOAIMaterialMgr;
	
	enum {
		CLEAR_LIGHT_GLOBAL,
		CLEAR_TEXTURE_GLOBAL,
	};
	
	u32								mGlobalID;
	u32								mType;
	MOAIMaterialStackClearCmd*		mNext;
};

//================================================================//
// MOAIMaterialStackFrame
//================================================================//
class MOAIMaterialStackFrame {
private:

	friend MOAIMaterialMgr;
	
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
	ZLLeanArray < MOAIGlobalLight >						mGlobalLights;
	ZLLeanArray < MOAIGlobalTexture >					mGlobalTextures;
	ZLLeanStack < MOAIMaterialStackFrame, 8 >			mStack;

	//----------------------------------------------------------------//
	void				Compose						( const MOAIMaterial& material );

public:

	static const u32 MAX_GLOBAL_LIGHTS = 256;

	//----------------------------------------------------------------//
	//void				Clear						();
	const MOAILight*	GetLight					( u32 lightID );
	void				LoadGfxState				();
						MOAIMaterialMgr				();
						~MOAIMaterialMgr			();
	void				Pop							();
	void				Push						( const MOAIMaterial* material = 0 );
	void				SetBlendMode				( const MOAIBlendMode& blendMode );
	void				SetCullMode					( int cullMode );
	void				SetDepthMask				( bool depthMask );
	void				SetDepthTest				( int depthTest );
	void				SetLight					( u32 lightID, MOAILight* light );
	void				SetShader					( u32 shaderID );
	void				SetShader					( MOAIShader* shader );
	void				SetTexture					( MOAITextureBase* texture );
};

#endif
