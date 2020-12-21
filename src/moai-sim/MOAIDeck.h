// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK_H
#define	MOAIDECK_H

class MOAIAbstractPickable;
class MOAIBoundsDeck;
class MOAICellCoord;
class MOAICollisionShape;
class MOAIColor;
class MOAIGrid;
class MOAIImage;
class MOAIGfxScriptBatch;
class MOAIQuadBrush;
class MOAIShader;
class MOAISurfaceSampler2D;

//================================================================//
// MOAIDeck
//================================================================//
/**	@lua	MOAIDeck
	@text	Base class for all decks.
*/
class MOAIDeck :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	friend class MOAIDeckProxy;

	enum {
		NO_CONTENT = 0xffffffff,
	};
	
	//----------------------------------------------------------------//
	static int				_draw							( lua_State* L );
	static int				_getBounds						( lua_State* L );

	//----------------------------------------------------------------//
	void							_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void							_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual ZLBounds				MOAIDeck_GetBounds					();
	virtual ZLBounds				MOAIDeck_GetBounds					( ZLIndex idx );
	virtual MOAICollisionShape*		MOAIDeck_GetCollisionShape			( ZLIndex idx );
	virtual MOAIPickResult			MOAIDeck_PickByPoint				( ZLIndex idx, ZLVec3D loc );
	virtual MOAIPickResult			MOAIDeck_PickByRay					( ZLIndex idx, ZLVec3D loc, ZLVec3D normal );
	virtual void					MOAIDeck_Render						( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase );

public:
	
	//----------------------------------------------------------------//
	ZLBounds				GetBounds						();
	ZLBounds				GetBounds						( ZLIndex idx );
	MOAICollisionShape*		GetCollisionShape				( ZLIndex idx );
							MOAIDeck						();
							~MOAIDeck						();
	MOAIPickResult			PickByPoint						( ZLIndex idx, ZLVec3D loc );
	MOAIPickResult			PickByRay						( ZLIndex idx, ZLVec3D loc, ZLVec3D normal );
	void					Render							( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase );
};

#endif
