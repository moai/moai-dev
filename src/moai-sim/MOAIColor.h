// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLOR_H
#define	MOAICOLOR_H

class MOAIDeck;
class MOAIPartitionViewLayer;

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

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	bool			MOAINode_ApplyAttrOp				( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void			MOAINode_Update						();

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
	
	DECL_ATTR_ID ( ATTR_R_COL )
	DECL_ATTR_ID ( ATTR_G_COL )
	DECL_ATTR_ID ( ATTR_B_COL )
	DECL_ATTR_ID ( ATTR_A_COL )
	
	DECL_ATTR_ID ( ADD_COLOR )
	DECL_ATTR_ID ( INHERIT_COLOR )
	DECL_ATTR_ID ( COLOR_TRAIT )
	
	//----------------------------------------------------------------//
	static MOAIColor*	AffirmColor			( MOAILuaState& state, int idx );
	ZLColorVec			GetColorTrait		() const;
	bool				IsClear				();
						MOAIColor			();
						~MOAIColor			();
};

#endif
