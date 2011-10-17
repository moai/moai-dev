// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLOR_H
#define	MOAICOLOR_H

#include <moaicore/MOAITraits.h>

class MOAIDeck;
class MOAILayer2D;

//================================================================//
// MOAIColor
//================================================================//
/**	@name	MOAIColor
	@text	Color vector with animation helper methods.
*/
class MOAIColor :
	public virtual MOAITraits,
	public USColorVec {
protected:
	
	USColorVec	mColor;
	
	//----------------------------------------------------------------//
	static int		_moveColor			( lua_State* L );
	static int		_seekColor			( lua_State* L );
	static int		_setColor			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIColor )
	DECL_ATTR_HELPER ( MOAIColor )
	
	enum {
		ATTR_R_COL,
		ATTR_G_COL,
		ATTR_B_COL,
		ATTR_A_COL,
		TOTAL_ATTR,
	};
	
	//----------------------------------------------------------------//
	bool			ApplyAttrOp			( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	USColorVec		GetColorTrait		();
					MOAIColor			();
					~MOAIColor			();
	void			OnDepNodeUpdate		();
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
};

#endif
