// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISTRETCHPATCH2D_H
#define	MOAISTRETCHPATCH2D_H

#include <moai-sim/MOAIDeck.h>

class MOAITextureBase;

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
	public MOAIDeck {
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
	static int		_reserveColumns			( lua_State* L );
	static int		_reserveRows			( lua_State* L );
	static int		_reserveUVRects			( lua_State* L );
	static int		_setColumn				( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setRow					( lua_State* L );
	static int		_setUVRect				( lua_State* L );

	//----------------------------------------------------------------//
	ZLBox			ComputeMaxBounds		();
	void			DrawStretch				( u32 idx, float xStretch, float yStretch );
	ZLBox			GetItemBounds			( u32 idx );
	void			UpdateParams			();

public:
	
	DECL_LUA_FACTORY ( MOAIStretchPatch2D )
	
	//----------------------------------------------------------------//
	using MOAIDeck::DrawIndex;
	void			DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
					MOAIStretchPatch2D		();
					~MOAIStretchPatch2D		();
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
