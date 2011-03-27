// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISIMPLESHADER_H
#define	MOAISIMPLESHADER_H

#include <moaicore/MOAIColor.h>

//================================================================//
// MOAISimpleShader
//================================================================//
/**	@brief Color modulation shader. Can inherit colors from other shaders.
*/
class MOAISimpleShader :
	public virtual MOAIColor {
protected:
	
	USRef < MOAISimpleShader > mParent;
	USBlendMode mBlendMode;
	USColorVec mColor;
	
	//----------------------------------------------------------------//
	static int		_setBlendMode		( lua_State* L );
	static int		_setParent			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAISimpleShader )
	
	enum {
		ATTR_PARENT,
		TOTAL_ATTR,
	};
	
	//----------------------------------------------------------------//
	void			ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
	void			Bind					();
	u32				CountAttributes			();
					MOAISimpleShader		();
					~MOAISimpleShader		();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			SetParent				( MOAISimpleShader* parent );
	STLString		ToString				();
};

#endif
