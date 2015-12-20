// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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

	//================================================================//
	// MOAIPathSegmant
	//================================================================//
	// TODO: doxygen
	class MOAIPathSegment {
	public:

		size_t	mBase;
		size_t	mSize;
		float	mLength;
	};

	ZLLeanArray < ZLVec2D >				mControlPoints;
	ZLLeanArray < MOAIPathSegment >		mSegments;
	ZLLeanArray < ZLVec2D >				mVertices;

	//----------------------------------------------------------------//
	static int		_evaluate			( lua_State* L );
	static int		_reserve			( lua_State* L );
	static int		_setPoint			( lua_State* L );

	//----------------------------------------------------------------//

public:
	
	DECL_LUA_FACTORY ( MOAILuaObject )
	
	//----------------------------------------------------------------//
	ZLVec2D			Evaluate			( float t );
	float			GetCubicBezier		( float t, ZLCubicBezier2D& curve );
					MOAIPath			();
					~MOAIPath			();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			Reserve				( size_t size );
	void			SetPoint			( size_t idx, float x, float y );
};

#endif
