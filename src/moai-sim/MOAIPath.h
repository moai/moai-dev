// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPATH_H
#define	MOAIPATH_H

//================================================================//
// MOAIPath
//================================================================//
// TODO: doxygen
class MOAIPath :
	public virtual MOAILuaObject {
private:

	ZLLeanArray < ZLVec2D >				mControlPoints;
	ZLLeanArray < float >				mSegmentLengths;

	float			mFlatness;
	float			mAngle;

	float			mLength;

	//----------------------------------------------------------------//
	static int		_bless				( lua_State* L );
	static int		_evaluate			( lua_State* L );
	static int		_getLength			( lua_State* L );
	static int		_reserve			( lua_State* L );
	static int		_setPoint			( lua_State* L );
	static int		_setThresholds		( lua_State* L );

	//----------------------------------------------------------------//

public:
	
	DECL_LUA_FACTORY ( MOAIPath )
	
	GET_SET ( float, Flatness, mFlatness )
	GET_SET ( float, Angle, mAngle )
	
	GET ( float, Length, mLength )
	
	//----------------------------------------------------------------//
	void				Bless					();
	size_t				CountSegments			();
	ZLVec2D				Evaluate				( float t );
	ZLCubicBezier2D		GetSegment				( size_t idx );
	ZLCubicBezier2D		GetSegmentForTime		( float t, float* st = 0 );
						MOAIPath				();
						~MOAIPath				();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				Reserve					( size_t size );
	void				SetPoint				( size_t idx, float x, float y );
};

#endif
