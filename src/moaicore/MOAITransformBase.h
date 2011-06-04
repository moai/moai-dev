// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRANSFORMBASE_H
#define	MOAITRANSFORMBASE_H

#include <moaicore/MOAITraits.h>

//================================================================//
// MOAITransformBase
//================================================================//
/**	@name	MOAITransformBase
	@text	Base class for 2D affine transforms.
*/
class MOAITransformBase :
	public virtual MOAITraits {
protected:
	
	USAffine2D		mLocalToWorldMtx;
	USAffine2D		mWorldToLocalMtx;

	//----------------------------------------------------------------//
	static int	_getWorldDir	( lua_State* L );
	static int	_getWorldLoc	( lua_State* L );
	static int	_getWorldRot	( lua_State* L );
	static int	_getWorldScl	( lua_State* L );

public:
	
	//----------------------------------------------------------------//
	const USAffine2D&		GetLocalToWorldMtx			();
	const USAffine2D*		GetTransformTrait			();
	const USAffine2D&		GetWorldToLocalMtx			();
							MOAITransformBase			();
							~MOAITransformBase			();
	void					RegisterLuaClass			( USLuaState& state );
	void					RegisterLuaFuncs			( USLuaState& state );
	STLString				ToString					();
};

#endif
