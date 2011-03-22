// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPOLAR_H
#define	MOAIPOLAR_H

//================================================================//
// MOAIPolar
//================================================================//
/**	@brief Geospatial math.
*/
class MOAIPolar :
	public USGlobalClass < MOAIPolar, USLuaData > {
private:

	int mUnits;
	float mMetersToUnits;

	//----------------------------------------------------------------//
	static int	_getDirection			( lua_State* L );
	static int	_getDistance			( lua_State* L );
	static int	_getUnits				( lua_State* L );
	static int	_metersToUnits			( lua_State* L );
	static int	_setUnits				( lua_State* L );

public:
	
	enum {
		FEET,
		METERS,
	};
	
	DECL_LUA_SINGLETON ( MOAIPolar )
	
	//----------------------------------------------------------------//
				MOAIPolar				();
				~MOAIPolar				();
	double		MetersToUnits			( double meters );
	void		RegisterLuaClass		( USLuaState& state );
	void		SetUnits				( u32 units );
};

#endif
