// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITILEDECK2D_H
#define	MOAITILEDECK2D_H

#include <moai-sim/MOAIDeck.h>

//================================================================//
// MOAITileDeck2DCallable
//================================================================//
class MOAITileDeck2DCallable :
	public MOAIAbstractGfxScriptCallback {
public:

	friend class MOAISpriteDeck2D;

	MOAIGfxMgr&		mGfxMgr;

	float			mUScale;
	float			mVScale;

	float			mUOff;
	float			mVOff;

	MOAIQuadBrush	mBrush;

	//----------------------------------------------------------------//
	void 		MOAIAbstractGfxScriptCallback_Call 		();
				MOAITileDeck2DCallable					( MOAIGfxMgr& gfxMgr );
};

//================================================================//
// MOAITileDeck2D
//================================================================//
/**	@lua	MOAITileDeck2D
	@text	Subdivides a single texture into uniform tiles enumerated
			from the texture's left top to right bottom.
*/
class MOAITileDeck2D :
	public virtual MOAIDeck,
	public virtual MOAIHasGfxScriptBatchesForPhases,
	public virtual MOAIGridSpace {
private:
	
	MOAIQuadBrush 	mQuad;
	
	//----------------------------------------------------------------//
	static int		_setQuad				( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setUVQuad				( lua_State* L );
	static int		_setUVRect				( lua_State* L );
	static int		_setSize				( lua_State* L );
	static int		_transform				( lua_State* L );
	static int		_transformUV			( lua_State* L );
	
	//----------------------------------------------------------------//
	void			_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_SerializeIn							( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void			_SerializeOut							( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	void			MOAIAbstractGfxScriptCallback_Call 		();
	ZLBounds		MOAIDeck_GetBounds						();
	ZLBounds		MOAIDeck_GetBounds						( ZLIndex idx );
	void			MOAIDeck_Render							( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase );
	
public:
	
	DECL_LUA_FACTORY ( MOAITileDeck2D )
	
	//----------------------------------------------------------------//
					MOAITileDeck2D			( ZLContext& context );
					~MOAITileDeck2D			();
	void			Transform				( const ZLAffine3D& mtx );
	void			TransformUV				( const ZLAffine3D& mtx );
};

#endif
