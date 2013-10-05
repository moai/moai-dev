// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRIDFANCY_H
#define	MOAIGRIDFANCY_H

#include <moai-sim/MOAIGrid.h>

//================================================================//
// MOAIGridFancy
//================================================================//
/**	@name	MOAIGridFancy
	@text	Fancier grid which supports color/scale alterations
*/
class MOAIGridFancy :
	public MOAIGrid {
private:

	ZLLeanArray < u32 > mColors;
	ZLLeanArray < ZLColorVec > mColorPalette;
	ZLLeanArray < float > mAlphas;
	ZLLeanArray < float > mScales;

	static int		_fillAlpha			( lua_State* L );
	static int		_fillColor			( lua_State* L );
	static int		_fillScale			( lua_State* L );
	static int		_getAlpha			( lua_State* L );
	static int		_getColor			( lua_State* L );
	static int		_getPalette			( lua_State* L );
	static int		_getScale			( lua_State* L );
	static int		_setAlpha			( lua_State* L );
	static int		_setColor			( lua_State* L );
	static int		_setPalette			( lua_State* L );
	static int		_setScale			( lua_State* L );
	static int		_setRowAlpha			( lua_State* L );
	static int		_setRowColor			( lua_State* L );
	static int		_setRowScale			( lua_State* L );

protected:
	void			OnResize			();

public:
	
	DECL_LUA_FACTORY ( MOAIGridFancy )
	
	//----------------------------------------------------------------//
	float			GetAlpha			( int xTile, int yTile );
	u32			GetColor			( int xTile, int yTile );
	float			GetScale			( int xTile, int yTile );
	ZLColorVec		GetPalette			( u32 idx );
				MOAIGridFancy			();
				~MOAIGridFancy			();
	void			Draw			( MOAIDeck *deck, MOAIDeckRemapper *remapper, const MOAICellCoord &c0, const MOAICellCoord &c1 );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			FillAlpha			( float value );
	void			FillColor			( u32 value );
	void			FillScale			( float value );
	void			SetAlpha			( u32 addr, float value );
	void			SetColor			( u32 addr, u32 value );
	void			SetScale			( u32 addr, float value );
	void			SetAlpha			( int xTile, int yTile, float value );
	void			SetColor			( int xTile, int yTile, u32 value );
	void			SetScale			( int xTile, int yTile, float value );
	void			SetPalette			( u32 addr, const ZLColorVec &value );
};

#endif
