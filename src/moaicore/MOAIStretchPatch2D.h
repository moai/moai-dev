// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISTRETCHPATCH2D_H
#define	MOAISTRETCHPATCH2D_H

#include <moaicore/MOAIDeck.h>

class MOAITexture;

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
/**	@name	MOAIStretchPatch2D
	@text	Moai implementation of a 9-patch. Textured quad with any number of
			stretchable and non-stretchable 'bands.' Grid drawing not supported.
*/
class MOAIStretchPatch2D :
	virtual public MOAIDeck {
private:

	USRef < MOAITexture >	mTexture; // source texture for patch

	USLeanArray < MOAIStretchPatchSpan >	mRows;
	USLeanArray < MOAIStretchPatchSpan >	mCols;

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
	void				Draw					( u32 idx, MOAIDeckRemapper* remapper, float xStretch, float yStretch );
	void				UpdateParams			();

public:
	
	DECL_LUA_FACTORY ( MOAIStretchPatch2D )
	
	//----------------------------------------------------------------//
	bool				Bind					();
	void				Draw					( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper );
	void				Draw					( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1 );
	USRect				GetBounds				( u32 idx, MOAIDeckRemapper* remapper );
						MOAIStretchPatch2D		();
						~MOAIStretchPatch2D		();
	void				SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void				SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	void				RegisterLuaClass		( USLuaState& state );
	void				RegisterLuaFuncs		( USLuaState& state );
	STLString			ToString				();
};

#endif
