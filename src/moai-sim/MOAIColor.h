// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLOR_H
#define	MOAICOLOR_H

#include <moai-sim/MOAINode.h>

class MOAIDeck;
class MOAILayer;

//================================================================//
// MOAIColor
//================================================================//
/**	@lua	MOAIColor
	@text	Color vector with animation helper methods.
	
	@attr	ATTR_R_COL	The R (red) channel
	@attr	ATTR_G_COL	The G (green) channel
	@attr	ATTR_B_COL	The B (blue) channel
	@attr	ATTR_A_COL	The A (alpha) channel
	
	@attr	INHERIT_COLOR
	@attr	COLOR_TRAIT
*/
class MOAIColor :
	public virtual MOAINode,
	public ZLColorVec {
protected:
	
	ZLColorVec	mColor;
	
	//----------------------------------------------------------------//
	static int		_getColor			( lua_State* L );
	static int		_moveColor			( lua_State* L );
	static int		_packRGBA			( lua_State* L );
	static int		_seekColor			( lua_State* L );
	static int		_setColor			( lua_State* L );
	static int		_setParent			( lua_State* L );
	static int		_unpackRGBA			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIColor )
	DECL_ATTR_HELPER ( MOAIColor )
	
	enum {
		ATTR_R_COL,
		ATTR_G_COL,
		ATTR_B_COL,
		ATTR_A_COL,
		
		ADD_COLOR,
		INHERIT_COLOR,
		COLOR_TRAIT,
		
		TOTAL_ATTR,
	};
	
	//----------------------------------------------------------------//
	static MOAIColor*	AffirmColor			( MOAILuaState& state, int idx );
	bool				ApplyAttrOp			( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	ZLColorVec			GetColorTrait		();
	bool				IsClear				();
						MOAIColor			();
						~MOAIColor			();
	void				OnDepNodeUpdate		();
	void				RegisterLuaClass	( MOAILuaState& state );
	void				RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
