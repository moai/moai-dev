// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESH_H
#define	MOAIMESH_H

#include <moai-sim/MOAIDeck.h>

class MOAIIndexBuffer;
class MOAIMesh;
class MOAIMeshPartition;
class MOAISelectionSpan;
class MOAITexture;
class MOAIVertexAttribute;

//================================================================//
// MOAIMeshSpan
//================================================================//
class MOAIMeshSpan {
public:
	
	ZLIndex				mBase;
	ZLIndex				mTop;
	
	MOAIMeshSpan*		mPrev;
	MOAIMeshSpan*		mNext;
};

//================================================================//
// MOAIMeshPrimCoords
//================================================================//
class MOAIMeshPrimCoords {
public:
	
	static const u32 PRIM_POINT			= 1;
	static const u32 PRIM_LINE			= 2;
	static const u32 PRIM_TRIANGLE		= 3;
	static const u32 MAX_COORDS			= 3;
	
	u32			mIndex;
	u32			mPrimSize;
	ZLVec3D		mCoords [ MAX_COORDS ];
	
	//----------------------------------------------------------------//
	ZLBox		GetAABB		();
};

//================================================================//
// MOAIMeshPrimReader
//================================================================//
class MOAIMeshPrimReader {
private:

	friend class MOAIMesh;

	MOAIVertexFormat*			mVertexFormat;
	MOAIGfxTopologyEnum::_		mPrimType;
	ZLSize						mTotalElements;
	ZLSize						mTotalPrims;
	
	const MOAIVertexAttribute*	mAttribute;
	const void*					mVertexBuffer;
	MOAIIndexBuffer*			mIndexBuffer;

	//----------------------------------------------------------------//
	bool		Init			( MOAIVertexFormat& vertexFormat, MOAIVertexBuffer& vertexBuffer, MOAIGfxTopologyEnum::_ primType, u32 totalElements, MOAIIndexBuffer* indexBuffer = NULL );
	ZLVec3D		ReadCoord		( u32 idx ) const;

public:

	GET_CONST ( ZLSize, TotalPrims, mTotalPrims )

	//----------------------------------------------------------------//
	bool		GetPrimCoords	( u32 idx, MOAIMeshPrimCoords& prim ) const;
};

//================================================================//
// MOAIMesh
//================================================================//
/**	@lua	MOAIMesh
	@text	Loads a texture and renders the contents of a vertex buffer.
			Grid drawing not supported.
	
	@const	GL_POINTS
	@const	GL_LINES
	@const	GL_TRIANGLES
	@const	GL_LINE_LOOP
	@const	GL_LINE_STRIP
	@const	GL_TRIANGLE_FAN
	@const	GL_TRIANGLE_STRIP
*/
class MOAIMesh :
	public virtual MOAIDeck,
	public virtual MOAIHasGfxScript {
protected:

	friend class MOAIMeshPrimReader;

	u32								mTotalElements;
	ZLBounds						mBounds;
	MOAIGfxTopologyEnum::_			mPrimType;
	float							mPenWidth;
	MOAIMeshPartition*				mPartition;

	//----------------------------------------------------------------//
	static int			_buildQuadTree				( lua_State* L );
	static int			_buildTernaryTree			( lua_State* L );
	static int			_getPrimsForPoint			( lua_State* L );
	static int			_getRegionForPrim			( lua_State* L );
	static int			_intersectRay				( lua_State* L );
	static int			_printPartition				( lua_State* L );
	static int			_readPrimCoords				( lua_State* L );
	static int			_setBounds					( lua_State* L );
	static int			_setPenWidth				( lua_State* L );
	static int			_setPrimType				( lua_State* L );
	static int			_setTotalElements			( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_SerializeIn				( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void				_SerializeOut				( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	ZLBounds			MOAIDeck_GetBounds			();
	ZLBounds			MOAIDeck_GetBounds			( ZLIndex idx );
	void				MOAIDeck_Render				( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase );

public:

	DECL_LUA_FACTORY ( MOAIMesh )
	
	GET_SET ( MOAIGfxTopologyEnum::_, PrimType, mPrimType )
	GET_SET ( u32, TotalElements, mTotalElements )
	
	//----------------------------------------------------------------//
	void				ClearBounds					();
	u32					CountPrims					() const;
	void				DrawIndex					( ZLIndex idx, MOAIMeshSpan* span );
						MOAIMesh					();
						~MOAIMesh					();
	void				SetBounds					( const ZLBox& aabb );
};

#endif
