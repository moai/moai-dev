// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSPROPBASE_H
#define	MOAIGRAPHICSPROPBASE_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAIMaterialBatchHolder.h>
#include <moai-sim/MOAIRenderable.h>
#include <moai-sim/MOAITransform.h>

class MOAICellCoord;
class MOAIDeck;
class MOAIGrid;
class MOAILayoutFrame;
class MOAIMaterialBatch;
class MOAIOverlapPrim2D;
class MOAIPartition;
class MOAIPartitionCell;
class MOAIPartitionLevel;
class MOAIPartitionResultBuffer;
class MOAIScissorRect;
class MOAIShader;
class MOAISurfaceSampler2D;
class MOAISingleTexture;

//================================================================//
// MOAIAbstractDrawable
//================================================================//
class MOAIAbstractDrawable :
	public virtual RTTIBase {
private:

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractDrawable_Draw				( int subPrimID ) = 0;
	virtual void		MOAIAbstractDrawable_DrawDebug			( int subPrimID ) = 0;

public:

	//----------------------------------------------------------------//
	void		Draw			( int subPrimID ) { MOAIAbstractDrawable_Draw ( subPrimID = MOAIPartitionHull::NO_SUBPRIM_ID ); }
	void		DrawDebug		( int subPrimID ) { MOAIAbstractDrawable_DrawDebug ( subPrimID = MOAIPartitionHull::NO_SUBPRIM_ID ); }
};

//================================================================//
// MOAIGraphicsPropBase
//================================================================//
/**	@lua	MOAIGraphicsPropBase
	@text	Base class for graphics props.
	
	@const	FRAME_FROM_DECK
	@const	FRAME_FROM_PARENT
	@const	FRAME_FROM_SELF
	
	@const	BLEND_NORMAL
	@const	BLEND_ADD
	@const	BLEND_MULTIPLY

	@const  GL_FUNC_ADD
	@const  GL_FUNC_SUBTRACT
	@const  GL_FUNC_REVERSE_SUBTRACT
	
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
	
	@const	DEPTH_TEST_DISABLE
	@const	DEPTH_TEST_NEVER
	@const	DEPTH_TEST_LESS
	@const	DEPTH_TEST_EQUAL
	@const	DEPTH_TEST_LESS_EQUAL
	@const	DEPTH_TEST_GREATER
	@const	DEPTH_TEST_NOTEQUAL
	@const	DEPTH_TEST_GREATER_EQUAL
	@const	DEPTH_TEST_ALWAYS
	
	@const	CULL_NONE
	@const	CULL_ALL
	@const	CULL_BACK
	@const	CULL_FRONT
*/
class MOAIGraphicsPropBase :
	public virtual MOAIPartitionHull,
	public virtual MOAIColor,
	public virtual MOAIRenderable,
	public virtual MOAIAbstractDrawable,
	public virtual MOAIMaterialBatchHolder {
private:
	
	//----------------------------------------------------------------//
	static int		_draw					( lua_State* L );
	static int		_getBillboard			( lua_State* L );
	static int		_getScissorRect			( lua_State* L );
	static int		_isVisible				( lua_State* L );
	static int		_setBillboard			( lua_State* L );
	static int		_setParent				( lua_State* L );
	static int		_setScissorRect			( lua_State* L );
	static int		_setUVTransform			( lua_State* L );
	static int		_setVisible				( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAIAbstractDrawable_DrawDebug				( int subPrimID );
	void			MOAIPartitionHull_AddToSortBuffer			( MOAIPartitionResultBuffer& buffer, u32 key );
	u32				MOAIPartitionHull_AffirmInterfaceMask		( MOAIPartition& partition );

protected:

	// TODO: Most of these members should be moved into a latch or
	// a subclass. 2D apps will rarely use these so they make
	// MOAIGraphicsPropBase heavier than it needs to be.

	u32										mBillboard;
	u32										mDisplayFlags;
	
	// TODO: these should all be attributes
	MOAILuaSharedPtr < MOAITransformBase >	mUVTransform;
	MOAILuaSharedPtr < MOAIScissorRect >	mScissorRect;

	//----------------------------------------------------------------//
	virtual ZLMatrix4x4		GetWorldDrawingMtx			(); // factors in billboard flags
	void					LoadUVTransform				();
	void					LoadVertexTransform			();
	void					PopGfxState					();
	void					PushGfxState				();

	//----------------------------------------------------------------//
	bool					MOAINode_ApplyAttrOp		( u32 attrID, MOAIAttribute& attr, u32 op );
	void					MOAINode_Update				();

public:

	DECL_ATTR_HELPER ( MOAIGraphicsPropBase )
	DECL_LUA_ABSTRACT ( MOAIGraphicsPropBase )

	enum {
		BILLBOARD_NONE,
		BILLBOARD_NORMAL,
		BILLBOARD_ORTHO,
		BILLBOARD_COMPASS,
		BILLBOARD_SCREEN,
	};

	enum {
		ATTR_SCISSOR_RECT,
		
		ATTR_LOCAL_VISIBLE,		// direct access to the prop's 'local' visbility setting
		ATTR_VISIBLE,			// read only - reflects the composite state of visibility
		INHERIT_VISIBLE,		// used to *pull* parent visibility via inheritance
		
		INHERIT_FRAME,
		FRAME_TRAIT,
		
		TOTAL_ATTR,
	};

	enum {
		FLAGS_LOCAL_VISIBLE			= 0x01,
		FLAGS_VISIBLE				= 0x02, // this is a composite of FLAGS_LOCAL_VISIBLE plus the parent's ATTR_VISIBLE
	};

	static const u32 DEFAULT_FLAGS = FLAGS_LOCAL_VISIBLE | FLAGS_VISIBLE;;

	//----------------------------------------------------------------//
	MOAIMaterialBatch*		AffirmMaterialBatch			();
	bool					IsVisible					();
							MOAIGraphicsPropBase		();
	virtual					~MOAIGraphicsPropBase		();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
	void					Render						();
	void					SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void					SetVisible					( bool visible );
};

#endif
