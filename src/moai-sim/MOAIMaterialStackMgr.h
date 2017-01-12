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
	
	// TODO: this could be a lot more efficient; look into
	// just keeping a stack of flags. there is too much copying
	// of MOAIMaterial in this model.
	
	friend class MOAIScopedMaterialStack;
	
	ZLLeanStack < MOAIMaterial, 8 >		mStack;

public:

	//----------------------------------------------------------------//
	void					Clear							();
	void					LoadGfxState					( u32 defaultShader );
	void					LoadGfxState					( MOAIShader* defaultShader = 0 );
							MOAIMaterialStackMgr			();
							~MOAIMaterialStackMgr			();
	void					Pop								();
	void					Push							( const MOAIMaterial* material );
	const MOAIMaterial&		Top								();
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
	void					Clear							();
	void					LoadGfxState					( u32 defaultShader );
	void					LoadGfxState					( MOAIShader* defaultShader = 0 );
							MOAIScopedMaterialStack			();
							~MOAIScopedMaterialStack		();
	void					Pop								();
	void					Push							( const MOAIMaterial* material );
	const MOAIMaterial&		Top								();
};

#endif
