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
	void				MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIPath )
	
	GET_SET ( float, Flatness, mFlatness )
	GET_SET ( float, Angle, mAngle )
	
	GET ( float, Length, mLength )
	
	//----------------------------------------------------------------//
	void				Bless					();
	size_t				CountSegments			();
	ZLVec2D				Evaluate				( float t );
	ZLCubicBezier2D		GetSegment				( ZLIndex idx );
	ZLCubicBezier2D		GetSegmentForTime		( float t, float* st = 0 );
						MOAIPath				();
						~MOAIPath				();
	void				Reserve					( ZLSize size );
	void				SetPoint				( ZLIndex idx, float x, float y );
};

#endif
