// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDEBUGLINES_H
#define	MOAIDEBUGLINES_H

#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAILua.h>

//================================================================//
// MOAIDebugLineStyle
//================================================================//
class MOAIDebugLineStyle {
private:

	friend class MOAIDebugLines;

	bool	mVisible;
	u32		mColor;
	float	mSize;
	
	//----------------------------------------------------------------//
			MOAIDebugLineStyle		();
			~MOAIDebugLineStyle		();
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

	MOAIDebugLineStyle				mStyles [ TOTAL_STYLES ];

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
	void			RegisterLuaClass		( MOAILuaState& state );
	void			SetStyle				( u32 styleID, float size, u32 color );
	void			ShowStyle				( u32 styleID, bool show );
};

#endif
