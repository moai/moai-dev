// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISTRETCHPATCH2D_H
#define	MOAISTRETCHPATCH2D_H

#include <moai-sim/MOAIStretchDeck.h>

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
	public MOAIStretchDeck,
	public MOAIHasMaterialBatch {
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
	ZLBounds				MOAIDeck_ComputeMaxAABB				();
	void					MOAIDeck_Draw						( ZLIndex idx );
	ZLBounds				MOAIDeck_GetBounds					( ZLIndex idx );
	MOAICollisionShape*		MOAIDeck_GetCollisionShape			( ZLIndex idx );
	bool					MOAIDeck_Overlap					( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	bool					MOAIDeck_Overlap					( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );
	void					MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void					MOAILuaObject_SerializeIn			( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void					MOAILuaObject_SerializeOut			( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	DECL_LUA_FACTORY ( MOAIStretchPatch2D )
	
	//----------------------------------------------------------------//
					MOAIStretchPatch2D		();
					~MOAIStretchPatch2D		();
	void			SetColumn				( ZLIndex idx, float percent, bool canStretch );
	void			SetRow					( ZLIndex idx, float percent, bool canStretch );
};

#endif
