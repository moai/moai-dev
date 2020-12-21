// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISTRETCHPATCH2D_H
#define	MOAISTRETCHPATCH2D_H

#include <moai-sim/MOAIStretchDeck.h>

class MOAIStretchPatch2D;

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
// MOAIStretchPatch2DCallable
//================================================================//
class MOAIStretchPatch2DCallable :
	public MOAIAbstractGfxScriptCallback {
public:

	friend class MOAIStretchPatch2D;

	ZLIndex					mIndex;
	MOAIStretchPatch2D*		mStretchPatch;

	//----------------------------------------------------------------//
	void 		MOAIAbstractGfxScriptCallback_Call 		();
};


//================================================================//
// MOAIStretchPatch2D
//================================================================//
/**	@lua	MOAIStretchPatch2D
	@text	Moai implementation of a 9-patch. Textured quad with any number of
			stretchable and non-stretchable 'bands.' Grid drawing not supported.
*/
class MOAIStretchPatch2D :
	public virtual MOAIDeck,
	public virtual MOAIHasGfxScriptBatchesForPhases,
	public virtual MOAIStretchDeck {
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
	void			UpdateParams			();

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_SerializeIn			( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void			_SerializeOut			( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	ZLBounds		MOAIDeck_GetBounds		();
	ZLBounds		MOAIDeck_GetBounds		( ZLIndex idx );
	void			MOAIDeck_Render			( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase );

public:
	
	DECL_LUA_FACTORY ( MOAIStretchPatch2D )
	
	//----------------------------------------------------------------//
	void			DrawStretch				( ZLIndex idx );
					MOAIStretchPatch2D		();
					~MOAIStretchPatch2D		();
	void			SetColumn				( ZLIndex idx, float percent, bool canStretch );
	void			SetRow					( ZLIndex idx, float percent, bool canStretch );
};

#endif
