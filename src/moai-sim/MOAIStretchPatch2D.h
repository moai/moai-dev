// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISTRETCHPATCH2D_H
#define	MOAISTRETCHPATCH2D_H

#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIMaterialBatchHolder.h>

//================================================================//
// MOAIStretchPatchSpan
//================================================================//
class MOAIStretchPatchSpan {
private:

	friend class MOAIStretchPatch2D;

	float	mPercent;		// total percent of native width of height covered by this span
	bool	mCanStretch;	// can this span stretch?
};

//================================================================//
// MOAIStretchPatch2D
//================================================================//
/**	@lua	MOAIStretchPatch2D
	@text	Moai implementation of a 9-patch. Textured quad with any number of
			stretchable and non-stretchable 'bands.' Grid drawing not supported.
*/
class MOAIStretchPatch2D :
	public MOAIDeck,
	public MOAIMaterialBatchHolder {
private:

	ZLLeanArray < MOAIStretchPatchSpan >	mRows;
	ZLLeanArray < MOAIStretchPatchSpan >	mCols;

	// native dimensions of image; will stretch to fill model rect
	ZLRect					mRect;
	ZLLeanArray < ZLRect >	mUVRects;

	float	mXFix;
	float	mXFlex;
	
	float	mYFix;
	float	mYFlex;
	
	bool	mNeedsUpdate;

	//----------------------------------------------------------------//
	static int		_ninePatch				( lua_State* L );
	static int		_reserveColumns			( lua_State* L );
	static int		_reserveRows			( lua_State* L );
	static int		_reserveUVRects			( lua_State* L );
	static int		_setColumn				( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setRow					( lua_State* L );
	static int		_setUVRect				( lua_State* L );

	//----------------------------------------------------------------//
	void			DrawStretch				( ZLIndex idx, float xStretch, float yStretch );
	void			UpdateParams			();

	//----------------------------------------------------------------//
	virtual ZLBounds				MOAIDeck_ComputeMaxBounds		();
	virtual void					MOAIDeck_Draw					( ZLIndex idx );
	virtual ZLBounds				MOAIDeck_GetBounds				( ZLIndex idx );
	virtual MOAICollisionShape*		MOAIDeck_GetCollisionShape		( ZLIndex idx );
	virtual bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	virtual bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );

public:
	
	DECL_LUA_FACTORY ( MOAIStretchPatch2D )
	
	//----------------------------------------------------------------//
					MOAIStretchPatch2D		();
					~MOAIStretchPatch2D		();
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetColumn				( u32 idx, float percent, bool canStretch );
	void			SetRow					( u32 idx, float percent, bool canStretch );
};

#endif
