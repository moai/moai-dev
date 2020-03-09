// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITILEDECK2D_H
#define	MOAITILEDECK2D_H

#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIQuadBrush.h>

//================================================================//
// MOAITileDeck2D
//================================================================//
/**	@lua	MOAITileDeck2D
	@text	Subdivides a single texture into uniform tiles enumerated
			from the texture's left top to right bottom.
*/
class MOAITileDeck2D :
	public MOAIDeck,
	public MOAIHasMaterialBatch,
	public MOAIGridSpace {
private:
	
	MOAIQuadBrush mQuad;
	
	//----------------------------------------------------------------//
	static int		_setQuad				( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setUVQuad				( lua_State* L );
	static int		_setUVRect				( lua_State* L );
	static int		_setSize				( lua_State* L );
	static int		_transform				( lua_State* L );
	static int		_transformUV			( lua_State* L );
	
	//----------------------------------------------------------------//
	ZLBounds				MOAIDeck_ComputeMaxAABB				();
	void					MOAIDeck_Draw						( ZLIndex idx );
	ZLBounds				MOAIDeck_GetBounds					( ZLIndex idx );
	MOAICollisionShape*		MOAIDeck_GetCollisionShape			( ZLIndex idx );
	bool					MOAIDeck_Overlap					( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	bool					MOAIDeck_Overlap					( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );
	void					MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void					MOAILuaObject_SerializeIn			( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void					MOAILuaObject_SerializeOut			( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	
public:
	
	DECL_LUA_FACTORY ( MOAITileDeck2D )
	
	//----------------------------------------------------------------//
					MOAITileDeck2D			();
					~MOAITileDeck2D			();
	void			Transform				( const ZLAffine3D& mtx );
	void			TransformUV				( const ZLAffine3D& mtx );
};

#endif
