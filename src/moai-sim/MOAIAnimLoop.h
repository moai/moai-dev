// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMLOOP_H
#define	MOAIANIMLOOP_H

class MOAIAnimCurveBase;
class MOAIAnimMixer;

//================================================================//
// MOAIAnimLoopChannel
//================================================================//
class MOAIAnimLoopChannel {
public:

	ZLStrongPtr < MOAIAnimCurveBase >	mCurve;
	u32									mMapping; // which channel
	bool								mDelta;
};

//================================================================//
// MOAIAnimLoop
//================================================================//
// TODO: doxygen
class MOAIAnimLoop :
	public virtual MOAILuaObject {
private:

	ZLLeanArray < MOAIAnimLoopChannel >	mCurves;
	float								mLength;

	//----------------------------------------------------------------//
	static int		_getLength			( lua_State* L );
	static int		_reserveCurves		( lua_State* L );
	static int		_setCurve			( lua_State* L );
	static int		_setLength			( lua_State* L );
	
public:
	
	DECL_LUA_FACTORY ( MOAIAnimLoop )
	
	GET ( float, Length, mLength )
	
	//----------------------------------------------------------------//
	void			Apply				( MOAIAnimMixer& mixer, float t, float weight );
//	void			Apply				( float t0, float t1 );
					MOAIAnimLoop		();
					~MOAIAnimLoop		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			ReserveCurves		( u32 nCurves );
	void			SetCurve			( u32 curveID, MOAIAnimCurveBase* curve, u32 mapping, bool delta );
};

#endif
