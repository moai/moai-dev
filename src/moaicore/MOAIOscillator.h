// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIOSCILLATOR_H
#define	MOAIOSCILLATOR_H

#include <moaicore/MOAINode.h>

//================================================================//
// MOAIOscillator
//================================================================//
/**	@brief Oscillator.
*/
class MOAIOscillator :
	public virtual MOAINode {
private:

	float	mTime;
	float	mValue;

	float	mDuration;
	float	mTargetAmplitude;
	float	mTargetFrequency;

	float	mPower;
	float	mAmplitude;
	float	mPeriod;
	float	mFrequency;
	float	mPhase;

	bool	mBounce;

	//----------------------------------------------------------------//
	static int	_setAmplitude	( lua_State* L );
	static int	_setBounce		( lua_State* L );
	static int	_setFrequency	( lua_State* L );
	static int	_setPeriod		( lua_State* L );
	static int	_setPhase		( lua_State* L );
	static int	_setPower		( lua_State* L );
	static int	_setSlide		( lua_State* L );

public:
	
	enum {
		ATTR_TIME,
		ATTR_VALUE,
		TOTAL_ATTR,
	};
	
	DECL_LUA_DATA ( MOAIOscillator )
	
	//----------------------------------------------------------------//
	void		ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
	u32			CountAttributes			();
				MOAIOscillator			();
				~MOAIOscillator			();
	void		OnDepNodeUpdate			();
	void		RegisterLuaClass		( USLuaState& state );
	void		RegisterLuaFuncs		( USLuaState& state );
	STLString	ToString				();
};

#endif
