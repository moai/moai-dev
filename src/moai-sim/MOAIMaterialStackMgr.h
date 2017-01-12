// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALSTACKMGR_H
#define	MOAIMATERIALSTACKMGR_H

#include <moai-sim/MOAIMaterial.h>

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
							MOAIMaterialStackScope			();
							~MOAIMaterialStackScope			();
};

//================================================================//
// MOAIMaterialStackMgr
//================================================================//
class MOAIMaterialStackMgr :
	public ZLContextClass < MOAIMaterialStackMgr >,
	public MOAIMaterial {
private:
	
	friend class MOAIMaterialStackScope;
	
	ZLLeanStack < u32, 8 >	mStack;

public:

	//----------------------------------------------------------------//
	void					Clear							();
							MOAIMaterialStackMgr			();
							~MOAIMaterialStackMgr			();
	void					Pop								();
	void					Push							( const MOAIMaterial* material = 0 );
};

#endif
