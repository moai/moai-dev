// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIM_H
#define	MOAIANIM_H

class MOAIAnimCurve;

//================================================================//
// MOAIAnim
//================================================================//
/**	@brief Basic animation class.
*/
class MOAIAnim :
	public virtual USLuaData {
private:

	float mLength;

	USLeanArray < MOAIAnimCurve* > mCurves;

	//----------------------------------------------------------------//
	static int	_getLength			( lua_State* L );
	static int	_reserveCurves		( lua_State* L );
	static int	_setCurve			( lua_State* L );
	
public:
	
	friend class MOAIAnimPlayer;
	
	DECL_LUA_DATA ( MOAIAnim )
	
	GET ( float, Length, mLength )
	
	//----------------------------------------------------------------//
	void			Clear				();
	float			Eval				( u32 curveID, float time );
	float			EvalDelta			( u32 curveID, float t0, float t1 );
					MOAIAnim			();
					~MOAIAnim			();
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	void			ReserveCurves		( u32 totalCurves );
	void			SetCurve			( u32 curveID, MOAIAnimCurve* curve ); 
	STLString		ToString			();
};

#endif
