// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTLAYER_H
#define	MOAIABSTRACTLAYER_H

class MOAIColor;
class MOAIFrameBuffer;
class MOAIPartition;
class MOAIFrameBuffer;

//================================================================//
// MOAIAbstractLayer
//================================================================//
class MOAIAbstractLayer :
	public virtual MOAIAbstractRenderNode,
	public virtual MOAISurfaceClearColor,
	public virtual MOAIColor {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	MOAILuaSharedPtr < MOAIFrameBuffer >	mFrameBuffer;

	//----------------------------------------------------------------//
	static int		_pushRenderPass				( lua_State* L );
	static int		_setFrameBuffer				( lua_State* L );

	//----------------------------------------------------------------//
	virtual void	MOAIAbstractLayer_Render				( u32 renderPhase ) = 0;
	void			MOAIAbstractRenderNode_RenderInner		( u32 renderPhase );
	void			MOAIAbstractRenderNode_RenderOuter		( u32 renderPhase );

protected:

	//----------------------------------------------------------------//
	void			_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	//----------------------------------------------------------------//
					MOAIAbstractLayer			();
					~MOAIAbstractLayer			();
};

#endif
