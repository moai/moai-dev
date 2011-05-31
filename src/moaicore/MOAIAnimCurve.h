// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVE_H
#define	MOAIANIMCURVE_H

#include <moaicore/MOAINode.h>

//================================================================//
// MOAIAnimCurve
//================================================================//
/**	@name	MOAIAnimCurve
	@text	Piecewise animation function with one input (time) and one output (value).
	
	@attr	ATTR_TIME
	@attr	ATTR_VALUE
*/
class MOAIAnimCurve :
	public virtual MOAINode,
	public USAnimCurve {
private:

	float	mTime;
	float	mValue;

	//----------------------------------------------------------------//
	static int	_getLength		( lua_State* L );
	static int	_reserveKeys	( lua_State* L );
	static int	_setKey			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurve )
	DECL_ATTR_HELPER ( MOAIAnimCurve )
	
	enum {
		ATTR_TIME,
		ATTR_VALUE,
		TOTAL_ATTR,
	};
	
	//----------------------------------------------------------------//
	bool			ApplyAttrOp			( u32 attrID, USAttrOp& attrOp );
					MOAIAnimCurve		();
					~MOAIAnimCurve		();
	void			OnDepNodeUpdate		();
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	STLString		ToString			();
};

#endif
