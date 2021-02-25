// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESHDECK_H
#define	MOAIMESHDECK_H

#include <moai-sim/MOAIDeck.h>

class MOAIIndexBuffer;
class MOAIMeshPartition;
class MOAISelectionSpan;
class MOAITexture;
class MOAIVertexAttribute;

//================================================================//
// MOAIMeshSpan
//================================================================//
class MOAIMeshSpan {
public:
	
	ZLIndex				mBase;
	ZLIndex				mTop;
	
	MOAIMeshSpan*		mPrev;
	MOAIMeshSpan*		mNext;
};

//================================================================//
// MOAIMeshDeck
//================================================================//
// TODO: doxygen
class MOAIMeshDeck :
	public virtual MOAIDeck,
	public virtual MOAIHasGfxScript {
protected:

	friend class MOAIMeshPrimReader;

	ZLStrongPtr < MOAIMesh >	mMesh;

//	u32									mTotalElements;
//	ZLBounds							mBounds;
//	MOAIGfxTopologyEnum::_				mPrimType;
//	float								mPenWidth;
//	MOAIMeshPartition*					mPartition;

	//----------------------------------------------------------------//
	static int			_setMesh					( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	ZLBounds			MOAIDeck_GetBounds			();
	ZLBounds			MOAIDeck_GetBounds			( ZLIndex idx );
	void				MOAIDeck_Render				( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase );

public:

	DECL_LUA_FACTORY ( MOAIMeshDeck )
	
	//----------------------------------------------------------------//
	void				DrawIndex					( ZLIndex idx, MOAIMeshSpan* span );
						MOAIMeshDeck				();
						~MOAIMeshDeck				();
};

#endif
