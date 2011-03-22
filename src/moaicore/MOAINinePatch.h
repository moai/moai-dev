// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAININEPATCH_H
#define	MOAININEPATCH_H

#include <moaicore/MOAIContentLibrary2D.h>

class MOAITexture;

//================================================================//
// MOAINinePatchSpan
//================================================================//
class MOAINinePatchSpan {
private:

	friend class MOAINinePatch;

	float	mPercent;		// total percent of native width of height covered by this span
	bool	mCanStretch;	// can this span stretch?
};

//================================================================//
// MOAINinePatch
//================================================================//
/**	@brief Stretchable image provider.
*/
class MOAINinePatch :
	virtual public MOAIContentLibrary2D {
private:

	USRef < MOAITexture >	mTexture; // source texture for patch

	USLeanArray < MOAINinePatchSpan >	mRows;
	USLeanArray < MOAINinePatchSpan >	mCols;

	// native dimensions of image; will stretch to fill model rect
	USRect					mRect;
	USLeanArray < USRect >	mUVRects;

	float	mXFix;
	float	mXFlex;
	
	float	mYFix;
	float	mYFlex;
	
	bool	mNeedsUpdate;

	//----------------------------------------------------------------//
	static int			_reserveColumns			( lua_State* L );
	static int			_reserveRows			( lua_State* L );
	static int			_reserveUVRects			( lua_State* L );
	static int			_setColumn				( lua_State* L );
	static int			_setRect				( lua_State* L );
	static int			_setRow					( lua_State* L );
	static int			_setTexture				( lua_State* L );
	static int			_setUVRect				( lua_State* L );

	//----------------------------------------------------------------//
	void				Draw					( u32 idx, float xStretch, float yStretch );
	void				UpdateParams			();

public:
	
	DECL_LUA_DATA ( MOAINinePatch )
	
	//----------------------------------------------------------------//
	bool				Bind					();
	void				Draw					( MOAIDrawingMtx2D& transform, u32 idx );
	void				Draw					( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 );
	USRect				GetBounds				( u32 idx );
						MOAINinePatch			();
						~MOAINinePatch			();
	void				SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void				SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	void				RegisterLuaClass		( USLuaState& state );
	void				RegisterLuaFuncs		( USLuaState& state );
	STLString			ToString				();
};

#endif
