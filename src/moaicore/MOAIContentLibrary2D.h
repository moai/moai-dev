// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICONTENTLIBRARY2D_H
#define	MOAICONTENTLIBRARY2D_H

class MOAIDrawingMtx2D;
class MOAIGrid;
class MOAIShader;
class MOAISurfaceSampler2D;

//================================================================//
// MOAIContentLibrary2D
//================================================================//
/**	@brief Base class for content objects.
*/
class MOAIContentLibrary2D :
	public virtual USLuaData {
private:

	USRef < MOAIShader >	mDefaultShader;
	u32						mContentMask;

	//----------------------------------------------------------------//
	static int			_setDefaultShader			( lua_State* L );

protected:

	enum {
		NO_CONTENT = 0xffffffff,
	};

	SET ( u32, ContentMask, mContentMask )

	//----------------------------------------------------------------//
	u32					GetContentAddr				( u32 idx, u32 total );
	
public:
	
	enum {
		CAN_DRAW				= 1 << 0x00,
		CAN_DRAW_DEBUG			= 1 << 0x01,
		CAN_GATHER_SURFACES		= 1 << 0x02,
		CAN_GET_OVERLAP_PRIM	= 1 << 0x03,
	};
	
	GET ( u32, ContentMask, mContentMask )
	
	//----------------------------------------------------------------//
	virtual bool		Bind						();
	virtual bool		Contains					( u32 idx, const USVec2D& vec );
	virtual void		Draw						( MOAIDrawingMtx2D& transform, u32 idx );
	virtual void		Draw						( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 );
	virtual void		DrawDebug					( MOAIDrawingMtx2D& transform, u32 idx );
	virtual void		DrawDebug					( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 );
	virtual void		GatherSurfaces				( u32 idx, MOAISurfaceSampler2D& sampler );
	virtual void		GatherSurfaces				( MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1, MOAISurfaceSampler2D& sampler );
	virtual USRect		GetBounds					( u32 idx );
						MOAIContentLibrary2D		();
						~MOAIContentLibrary2D		();
	void				RegisterLuaClass			( USLuaState& state );
	void				RegisterLuaFuncs			( USLuaState& state );
};

#endif
