// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIPolar.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>getDirection (longcoord1, latcoord1, longcoord2, latcoord2)</tt>\n
\n
	Takes two coordinates and determines the direction angle from coordinate 1 to coordinate 2.
	@param longcoord1 The first coordinate's longitude.
	@param latcoord1 The first coordinate's latitude.
	@param longcoord2 The second coordinate's longitude.
	@param latcoord2 The second coordinate's latitude.
	@return Direction angle.
*/
int MOAIPolar::_getDirection ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "NNNN" )) return 0;
	
	USPolarCoord coord0;
	coord0.mLon = state.GetValue < double >( 1, 0.0f );
	coord0.mLat = state.GetValue < double >( 2, 0.0f );
	
	USPolarCoord coord1;
	coord1.mLon = state.GetValue < double >( 3, 0.0f );
	coord1.mLat = state.GetValue < double >( 4, 0.0f );
	
	lua_pushnumber ( state, USPolar::GreatCircleDirection ( coord0, coord1 ));
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>getDistance (longcoord1, latcoord1, longcoord2, latcoord2)</tt>\n
\n
	Takes two coordinates and determines the distance from coordinate 1 to coordinate 2.
	@param longcoord1 The first coordinate's longitude.
	@param latcoord1 The first coordinate's latitude.
	@param longcoord2 The second coordinate's longitude.
	@param latcoord2 The second coordinate's latitude.
	@return Distance in current units.
*/
int MOAIPolar::_getDistance ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "NNNN" )) return 0;
	
	USPolarCoord coord0;
	coord0.mLon = state.GetValue < double >( 1, 0.0f );
	coord0.mLat = state.GetValue < double >( 2, 0.0f );
	
	USPolarCoord coord1;
	coord1.mLon = state.GetValue < double >( 3, 0.0f );
	coord1.mLat = state.GetValue < double >( 4, 0.0f );
	
	lua_pushnumber ( state, USPolar::GreatCircleDistance ( coord0, coord1 ) * MOAIPolar::Get ().mMetersToUnits );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>getUnits ()</tt>\n
\n
	Returns the value of the metric system being used.
	@return The metric system value.  Either FEET or METERS.
*/
int MOAIPolar::_getUnits ( lua_State* L ) {

	USLuaState state ( L );
	
	lua_pushnumber ( state, MOAIPolar::Get ().mUnits );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIPolar::_metersToUnits ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	double meters = state.GetValue < double >( 1, 0.0f );
	lua_pushnumber ( state, MOAIPolar::Get ().MetersToUnits ( meters ));
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>setUnits (units)</tt>\n
\n
	Sets the value of the metric system to use.
	@param units The value of the metric system. Can be set to FEET or METERS.
*/
int MOAIPolar::_setUnits ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	MOAIPolar::Get ().SetUnits ( state.GetValue < u32 >( 1, METERS ));
	
	return 0;
}

//================================================================//
// MOAIPolar
//================================================================//

//----------------------------------------------------------------//
MOAIPolar::MOAIPolar () {

	this->SetUnits ( METERS );
}

//----------------------------------------------------------------//
MOAIPolar::~MOAIPolar () {
}

//----------------------------------------------------------------//
double MOAIPolar::MetersToUnits ( double meters ) {

	return meters * this->mMetersToUnits;
}

//----------------------------------------------------------------//
void MOAIPolar::RegisterLuaClass ( USLuaState& state ) {

	LuaReg regTable[] = {
		{ "getDirection",		_getDirection },
		{ "getDistance",		_getDistance },
		{ "getUnits",			_getUnits },
		{ "metersToUnits",		_metersToUnits },
		{ "setUnits",			_setUnits },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
	
	state.SetField ( -1, "FEET", ( u32 )FEET );
	state.SetField ( -1, "METERS", ( u32 )METERS );
}

//----------------------------------------------------------------//
void MOAIPolar::SetUnits ( u32 units ) {

	this->mUnits = units;

	switch ( units ) {
		case FEET: {
			this->mMetersToUnits = ( float )METERS_TO_FEET;
			break;
		}
		case METERS: {
			this->mMetersToUnits = 1.0f;
			break;
		}
	}
}
