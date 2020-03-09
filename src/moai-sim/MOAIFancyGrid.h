// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFANCYGRID_H
#define	MOAIFANCYGRID_H

#include <moai-sim/MOAIGrid.h>

//================================================================//
// MOAIFancyGrid
//================================================================//
/**	@lua	MOAIFancyGrid
	@text	"Fancy" grid which supports data for color alterations when drawing.
*/
class MOAIFancyGrid :
	public MOAIGrid {
private:

	// PCM: I removed support for tile scaling for now. The only use case I could think
	// of is wipe/transition effects that might be more easily handled by implementing
	// a way to convert a grid to a particle system. That said, scale (and rotation)
	// are easy enough to add back if people need them.

	static const u32 COLOR_SIZE_8	= 8;
	static const u32 COLOR_SIZE_16	= 16;
	static const u32 COLOR_SIZE_32	= 32;
	
	static const u32 NO_COLOR		= 0xffffffff;

	u32							mColorSize;

	void*						mColorSet;
	ZLLeanArray < MOAIColor* >	mPalette;

	//----------------------------------------------------------------//
	static int		_fillColor			( lua_State* L );
	static int		_getColor			( lua_State* L );
	static int		_getPaletteColor	( lua_State* L );
	static int		_getTileColor		( lua_State* L );
	static int		_reservePalette		( lua_State* L );
	static int		_setColor			( lua_State* L );
	static int		_setColorRow		( lua_State* L );
	static int		_setPaletteColor	( lua_State* L );

protected:

	//----------------------------------------------------------------//
	void			AffirmColorSet		();
	void			AffirmRotationSet	();
	void			AffirmScaleSet		();

	void			DiscardColorSet		();
	void			DiscardPalette		();

	void			OnResize			();

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass	( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs	( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_SerializeIn		( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void			MOAILuaObject_SerializeOut		( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	DECL_LUA_FACTORY ( MOAIFancyGrid )
	
	//----------------------------------------------------------------//	
	void			FillColor			( u32 value );
	
	u32				GetColor			( int addr ) const;
	u32				GetColor			( int xTile, int yTile ) const;
	MOAIColor*		GetPaletteColor		( ZLIndex idx ) const;
	
	ZLColorVec		GetTileColor		( int addr ) const;
	ZLColorVec		GetTileColor		( int xTile, int yTile ) const;
	
					MOAIFancyGrid		();
					~MOAIFancyGrid		();
	
	void			ReservePalette		( ZLSize size );
	
	void			SetColor			( int addr, u32 value );
	void			SetColor			( int xTile, int yTile, u32 value );
	void			SetPaletteColor		( ZLIndex idx, MOAIColor* color );
};

#endif
