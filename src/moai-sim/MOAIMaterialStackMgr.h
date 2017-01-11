// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALSTACKMGR_H
#define	MOAIMATERIALSTACKMGR_H

#include <moai-sim/MOAIMaterial.h>

//================================================================//
// MOAIMaterialStackMgr
//================================================================//
class MOAIMaterialStackMgr :
	public ZLContextClass < MOAIMaterialStackMgr > {
private:
	
	friend class MOAIScopedMaterialStack;
	
	ZLLeanStack < MOAIMaterial, 8 >		mStack;

public:

	//----------------------------------------------------------------//
	void				Clear							();
	void				LoadGfxState					( u32 defaultShader );
	void				LoadGfxState					( MOAIShader* defaultShader = 0 );
						MOAIMaterialStackMgr			();
						~MOAIMaterialStackMgr			();
	void				Pop								();
	void				Push							( MOAIMaterial* material );
};

//================================================================//
// MOAIScopedMaterialStack
//================================================================//
class MOAIScopedMaterialStack {
private:

	MOAIMaterialStackMgr&	mMaterialStack;
	u32						mRestoreTop;

public:

	//----------------------------------------------------------------//
	void				Clear							();
	void				LoadGfxState					( u32 defaultShader );
	void				LoadGfxState					( MOAIShader* defaultShader = 0 );
						MOAIScopedMaterialStack			();
						~MOAIScopedMaterialStack		();
	void				Pop								();
	void				Push							( MOAIMaterial* material );
};

#endif
