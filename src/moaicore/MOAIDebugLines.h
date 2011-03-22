// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDEBUGLINES_H
#define	MOAIDEBUGLINES_H

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
// MOAIDebugLines
//================================================================//
/**	@brief Singleton for managing rendering of world space debug vectors.
*/
class MOAIDebugLines :
	public USGlobalClass < MOAIDebugLines, USLuaData >,
	public USDebugLines {
public:

	enum {
		COLLISION_PRIMS,
		OVERLAPS,
		PARTITION_CELLS,
		PARTITION_PADDED_CELLS,
		SPRITE_MODEL_BOUNDS,
		SPRITE_WORLD_BOUNDS,
		SURFACES,
		TEXT_BOX,
		TOTAL_STYLES,
	};

private:

	MOAIDebugLineStyle mStyles [ TOTAL_STYLES ];

	//----------------------------------------------------------------//
	static int		_setStyle				( lua_State* L );
	static int		_showStyle				( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIDebugLines )
	
	//----------------------------------------------------------------//
	bool			Bind					( u32 styleID );
	bool			IsVisible				( u32 styleID );
					MOAIDebugLines			();
					~MOAIDebugLines			();
	void			RegisterLuaClass		( USLuaState& state );
	void			SetStyle				( u32 styleID, u32 size, u32 color );
	void			ShowStyle				( u32 styleID, bool show );
};

#endif
