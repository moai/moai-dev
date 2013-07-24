// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLOR_H
#define	MOAICOLOR_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAINode.h>

class MOAIDeck;
class MOAILayer;

//================================================================//
// MOAIColor
//================================================================//
/**	@name	MOAIColor
	@text	Color vector with animation helper methods.
	
	@attr	ATTR_R_COL
	@attr	ATTR_G_COL
	@attr	ATTR_B_COL
	@attr	ATTR_OPACITY
	
	@attr	INHERIT_COLOR
	@attr	COLOR_TRAIT
*/
class MOAIColor :
	public virtual MOAINode,
	public USColorVec {
protected:
	
	USColorVec	mColor;
	
	//----------------------------------------------------------------//
	static int		_getColor			( lua_State* L );
	static int		_getOpacity			( lua_State* L );
	static int		_setColor			( lua_State* L );
	static int		_setOpacity			( lua_State* L );
	static int		_setParent			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIColor )
	DECL_ATTR_HELPER ( MOAIColor )
	
	enum {
		ATTR_R_COL,
		ATTR_G_COL,
		ATTR_B_COL,
		ATTR_OPACITY,
		
		ADD_COLOR,
		INHERIT_COLOR,
		INHERIT_COLOR_RAW,
		COLOR_TRAIT,
		
		TOTAL_ATTR,
	};
	
	//----------------------------------------------------------------//
	bool			ApplyAttrOp			( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	USColorVec		GetColorTrait		();
					MOAIColor			();
					~MOAIColor			();
	void			OnDepNodeUpdate		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
