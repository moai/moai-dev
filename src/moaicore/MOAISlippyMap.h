// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISLIPPYMAP_H
#define	MOAISLIPPYMAP_H

#include <moaicore/MOAIGfxPrim2D.h>

//================================================================//
// MOAISlippyMap
//================================================================//
/**	@brief Grapics primitive for rendering level of detail images.
*/
class MOAISlippyMap :
	public MOAIGfxPrim2D,
	public USSlipMapBrush {
private:

	u32		mMinZoom;
	u32		mMaxZoom;
	u32		mTileSize;

	USLuaRef	mGetTileURL;
	USLuaRef	mGetTileKey;

	//----------------------------------------------------------------//
	static int		_modelToPolar			( lua_State* L );
	static int		_polarToModel			( lua_State* L );
	static int		_polarToWorld			( lua_State* L );
	static int		_setFileCache			( lua_State* L );
	static int		_setFrame				( lua_State* L );
	static int		_setLoadingTiles		( lua_State* L );
	static int		_setTileKeyCallback		( lua_State* L );
	static int		_setTileSize			( lua_State* L );
	static int		_setTileURLCallback		( lua_State* L );
	static int		_setZoomRange			( lua_State* L );
	static int		_worldToPolar			( lua_State* L );

	//----------------------------------------------------------------//
	STLString		DoTileCallback			( USLuaRef& callback, USQuadCoord& quadID );

public:
	
	enum {
		TOTAL_ATTR = MOAIGfxPrim2D::TOTAL_ATTR,
	};
	
	DECL_LUA_DATA ( MOAISlippyMap )
	
	//----------------------------------------------------------------//
	void			ApplyAttrOp			( u32 attrID, USAttrOp& attrOp );
	u32				CountAttributes		();
	void			Draw				();
	u32				GetLocalFrame		( USRect& frame );
	u32				GetMaxZoom			();
	u32				GetMinZoom			();
	u32				GetTileSize			();
	STLString		GetTileStoreKey		( USQuadCoord& quadID );
	STLString		GetTileURL			( USQuadCoord& quadID );
					MOAISlippyMap		();
					~MOAISlippyMap		();
	void			LoadImage			( cc8* filename );
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	STLString		ToString			();
};

#endif
