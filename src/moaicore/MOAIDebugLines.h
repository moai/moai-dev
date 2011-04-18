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
/**	@name	MOAIDebugLines
	@text	Singleton for managing rendering of world space debug vectors.

	@const	PARTITION_CELLS
	@const	PARTITION_PADDED_CELLS
	@const	PROP_MODEL_BOUNDS
	@const	PROP_WORLD_BOUNDS
	@const	TEXT_BOX
*/
class MOAIDebugLines :
	public USGlobalClass < MOAIDebugLines, USLuaObject >,
	public USDebugLines {
public:

	enum {
		PARTITION_CELLS,
		PARTITION_PADDED_CELLS,
		PROP_MODEL_BOUNDS,
		PROP_WORLD_BOUNDS,
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
