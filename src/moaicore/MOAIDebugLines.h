// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDEBUGLINES_H
#define	MOAIDEBUGLINES_H

#include <moaicore/MOAILineBrush.h>
#include <moaicore/MOAILua.h>

//================================================================//
// MOAIDebugLineStyle
//================================================================//
class MOAIDebugLineStyle {
private:

	friend class MOAIDebugLines;

	bool	mVisible;
	u32		mColor;
	u32		mSize;
	
	//----------------------------------------------------------------//
			MOAIDebugLineStyle		();
			~MOAIDebugLineStyle		();
};

//================================================================//
// MOAIDebugLine
//================================================================//
class MOAIDebugLine :
	public MOAILineBrush {
private:

	friend class MOAIDebugLines;

	USVec3D		mVtx [ 2 ];
	u32			mColor;
	float		mWidth;
	
	//----------------------------------------------------------------//
	void	Draw		();
	void	SetVerts	( float x0, float y0, float x1, float y1 );
};

//================================================================//
// MOAIDebugLines
//================================================================//
/**	@name	MOAIDebugLines
	@text	Singleton for managing rendering of world space debug vectors.

	@const	PARTITION_CELLS
	@const	PARTITION_PADDED_CELLS
	@const	PROP_MODEL_BOUNDS
	@const	PROP_WORLD_BOUNDS
	@const	TEXT_BOX
	@const	TEXT_BOX_BASELINES
	@const	TEXT_BOX_LAYOUT
*/
class MOAIDebugLines :
	public MOAIGlobalClass < MOAIDebugLines, MOAILuaObject > {
public:

	enum {
		PARTITION_CELLS,
		PARTITION_PADDED_CELLS,
		PROP_MODEL_BOUNDS,
		PROP_WORLD_BOUNDS,
		TEXT_BOX,
		TEXT_BOX_BASELINES,
		TEXT_BOX_LAYOUT,
		TOTAL_STYLES,
	};

private:

	static const u32				MAX_LINES = 1024;

	MOAIDebugLineStyle				mStyles [ TOTAL_STYLES ];

	u32								mPenColor;
	u32								mPenWidth;
	u32								mPenSpace;

	u32								mTop;
	USLeanArray < MOAIDebugLine >	mLineBuffer;
	USAffine3D						mModelToWorldMtx;

	//----------------------------------------------------------------//
	static int		_setStyle				( lua_State* L );
	static int		_showStyle				( lua_State* L );
	
	//----------------------------------------------------------------//
	MOAIDebugLine*	NextLine				();

public:
	
	DECL_LUA_SINGLETON ( MOAIDebugLines )
	
	enum {
		MODEL_SPACE,
		WORLD_SPACE,
	};

	GET_SET ( u32, PenColor, mPenColor )
	GET_SET ( u32, PenWidth, mPenWidth )
	GET_SET ( u32, PenSpace, mPenSpace )
	
	//----------------------------------------------------------------//
	bool			Bind					( u32 styleID );
	void			Draw					();
	void			DrawEllipse				( USVec2D& loc, float xRad, float yRad, u32 steps );
	void			DrawLine				( const USVec2D& v0, const USVec2D& v1 );
	void			DrawLine				( float x0, float y0, float x1, float y1 );
	void			DrawRect				( const USRect& rect );
	void			DrawQuad				( const USQuad& quad );
	bool			IsVisible				( u32 styleID );
					MOAIDebugLines			();
					~MOAIDebugLines			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			Reset					();
	void			SetPen					( u32 penColor, float penWidth, u32 penSpace = MODEL_SPACE );
	void			SetStyle				( u32 styleID, u32 size, u32 color );
	void			SetWorldMtx				();
	void			SetWorldMtx				( const USAffine3D& mtx );
	void			ShowStyle				( u32 styleID, bool show );
};

#endif
