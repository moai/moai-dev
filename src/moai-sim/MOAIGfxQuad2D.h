// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXQUAD2D_H
#define	MOAIGFXQUAD2D_H

#include <moai-sim/MOAIGraphicsDeckBase.h>
#include <moai-sim/MOAIQuadBrush.h>

//================================================================//
// MOAIGfxQuad2D
//================================================================//
/**	@lua	MOAIGfxQuad2D
	@text	Single textured quad.
*/
class MOAIGfxQuad2D :
	public MOAIGraphicsDeckBase {
private:

	MOAIQuadBrush mQuad;

	//----------------------------------------------------------------//
	static int		_setQuad			( lua_State* L );
	static int		_setRect			( lua_State* L );
	static int		_setUVQuad			( lua_State* L );
	static int		_setUVRect			( lua_State* L );
	static int		_transform			( lua_State* L );
	static int		_transformUV		( lua_State* L );

	//----------------------------------------------------------------//
	virtual ZLBounds				MOAIDeck_ComputeMaxBounds		();
	virtual void					MOAIDeck_Draw					( u32 idx );
	virtual ZLBounds				MOAIDeck_GetBounds				( u32 idx );
	virtual MOAICollisionShape*		MOAIDeck_GetCollisionShape		( u32 idx );
	virtual bool					MOAIDeck_Overlap				( u32 idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	virtual bool					MOAIDeck_Overlap				( u32 idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );

public:
	
	DECL_LUA_FACTORY ( MOAIGfxQuad2D )
	
	//----------------------------------------------------------------//
					MOAIGfxQuad2D			();
					~MOAIGfxQuad2D			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			Transform				( const ZLAffine3D& mtx );
	void			TransformUV				( const ZLAffine3D& mtx );
};

#endif
