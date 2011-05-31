// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISIMPLESHADER_H
#define	MOAISIMPLESHADER_H

#include <moaicore/MOAIColor.h>
#include <moaicore/MOAIShader.h>

//================================================================//
// MOAISimpleShader
//================================================================//
/**	@name	MOAISimpleShader
	@text	Fixed function pipeline shader.

	@const	BLEND_NORMAL
	@const	BLEND_ADD
	@const	BLEND_MULTIPLY
	
	@const	GL_ONE
	@const	GL_ZERO
	@const	GL_DST_ALPHA
	@const	GL_DST_COLOR
	@const	GL_SRC_COLOR
	@const	GL_ONE_MINUS_DST_ALPHA
	@const	GL_ONE_MINUS_DST_COLOR
	@const	GL_ONE_MINUS_SRC_ALPHA
	@const	GL_ONE_MINUS_SRC_COLOR
	@const	GL_SRC_ALPHA
	@const	GL_SRC_ALPHA_SATURATE
*/
class MOAISimpleShader :
	public virtual MOAIShader,
	public virtual MOAIColor {
protected:
	
	USWeak < MOAISimpleShader > mParent;
	USBlendMode mBlendMode;
	USColorVec mColor;
	
	//----------------------------------------------------------------//
	static int		_setBlendMode		( lua_State* L );
	static int		_setParent			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAISimpleShader )
	
	//----------------------------------------------------------------//
	bool			ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
	void			Bind					();
					MOAISimpleShader		();
					~MOAISimpleShader		();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			SetParent				( MOAISimpleShader* parent );
	STLString		ToString				();
};

#endif
