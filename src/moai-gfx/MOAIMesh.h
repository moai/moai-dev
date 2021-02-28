// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESH_H
#define	MOAIMESH_H

#include <moai-gfx/MOAIGfxConsts.h>
#include <moai-gfx/MOAIGfxResource.h>

class MOAIIndexBuffer;
class MOAIMeshPartition;
class MOAIVertexBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIMesh
//================================================================//
class MOAIMesh :
	public virtual MOAIGfxResource {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	u32								mTotalElements;
	ZLBounds						mBounds;
	MOAIGfxTopologyEnum::_			mPrimType;
	float							mPenWidth;

	//----------------------------------------------------------------//
	static int				_getIndexBuffer				( lua_State* L );
	static int				_getVertexBuffer			( lua_State* L );
	static int				_setBounds					( lua_State* L );
	static int				_setIndexBuffer				( lua_State* L );
	static int				_setPenWidth				( lua_State* L );
	static int				_setPrimType				( lua_State* L );
	static int				_setTotalElements			( lua_State* L );
	static int				_setVertexBuffer			( lua_State* L );

	//----------------------------------------------------------------//
	void							_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void							_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	void 							_SerializeIn 					( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void							_SerializeOut					( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	virtual void					MOAIMesh_DrawPrims				( MOAIGfxTopologyEnum::_ primType, u32 base, u32 count ) = 0;
	virtual MOAIIndexBuffer*		MOAIMesh_GetIndexBuffer			() = 0;
	virtual MOAIVertexBuffer*		MOAIMesh_GetVertexBuffer		( ZLIndex idx ) = 0;
	virtual MOAIVertexFormat*		MOAIMesh_GetVertexFormat		( ZLIndex idx ) = 0;
	virtual void					MOAIMesh_ReserveVertexBuffers	( ZLSize size ) = 0;
	virtual void					MOAIMesh_SetIndexBuffer			( MOAIIndexBuffer* indexBuffer ) = 0;
	virtual void					MOAIMesh_SetVertexBuffer		( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) = 0;

public:
	
	GET_SET ( MOAIGfxTopologyEnum::_, PrimType, mPrimType )
	GET_SET ( u32, TotalElements, mTotalElements )
	GET_SET ( ZLBounds, Bounds, mBounds )

	//----------------------------------------------------------------//
	void					ClearBounds					();
	u32						CountPrims					() const;
	void					DrawPrims					( u32 base, u32 count );
	MOAIIndexBuffer*		GetIndexBuffer				();
	MOAIVertexBuffer*		GetVertexBuffer				( ZLIndex idx = 0 );
	MOAIVertexFormat*		GetVertexFormat				( ZLIndex idx = 0 );
							MOAIMesh					( ZLContext& context );
							~MOAIMesh					();
	void					ReserveVertexBuffers		( ZLSize size );
	void					SetIndexBuffer				( MOAIIndexBuffer* indexBuffer );
	void					SetVertexBuffer				( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat );
};

#endif
