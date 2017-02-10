// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALSTACKMGR_H
#define	MOAIMATERIALSTACKMGR_H

#include <moai-sim/MOAIMaterial.h>

class MOAILight;
class MOAIMaterialStackMgr;

//================================================================//
// MOAIMaterialStackScope
//================================================================//
class MOAIMaterialStackScope {
private:

	MOAIMaterialStackMgr&	mMaterialStack;
	u32						mRestoreTop;

public:

	GET ( MOAIMaterialStackMgr&, MaterialStack, mMaterialStack )

	//----------------------------------------------------------------//
	operator MOAIMaterialStackMgr& () {
	
		return this->mMaterialStack;
	}

	//----------------------------------------------------------------//
					MOAIMaterialStackScope		();
					~MOAIMaterialStackScope		();
};

//================================================================//
// MOAIMaterialStackLightState
//================================================================//
class MOAIMaterialStackLightState {
private:

	friend MOAIMaterialStackMgr;
	
	//u32				mFlags;
	MOAILight*		mLight;
	u32				mStackDepth;
};

//================================================================//
// MOAIMaterialStackRestoreCmd
//================================================================//
class MOAIMaterialStackRestoreCmd :
	MOAIMaterialStackLightState {
private:

	friend MOAIMaterialStackMgr;
	
	u32									mLightID;
	MOAIMaterialStackRestoreCmd*		mNext;
};

//================================================================//
// MOAIMaterialStackFrame
//================================================================//
class MOAIMaterialStackFrame {
private:

	friend MOAIMaterialStackMgr;
	
	u32								mFlags;
	MOAIMaterialStackRestoreCmd*	mRestoreList;
};

//================================================================//
// MOAIMaterialStackMgr
//================================================================//
class MOAIMaterialStackMgr :
	public ZLContextClass < MOAIMaterialStackMgr >,
	public MOAIMaterialBase {
private:
	
	friend class MOAIMaterialStackScope;

	ZLLeanPool < MOAIMaterialStackRestoreCmd, 32 >		mRestoreCmdPool;
	ZLLeanArray < MOAIMaterialStackLightState >			mLightStateArray;
	ZLLeanStack < MOAIMaterialStackFrame, 8 >			mStack;

	//----------------------------------------------------------------//
	void				Compose						( const MOAIMaterial& material );

public:

	static const u32 MAX_GLOBAL_LIGHTS = 256;

	//----------------------------------------------------------------//
	//void				Clear						();
	const MOAILight*	GetLight					( u32 lightID );
	void				LoadGfxState				();
						MOAIMaterialStackMgr		();
						~MOAIMaterialStackMgr		();
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
