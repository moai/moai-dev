// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSPROPBASE_H
#define	MOAIGRAPHICSPROPBASE_H

#include <moai-sim/MOAIAbstractProp.h>
#include <moai-gfx/MOAITransform.h>

class MOAIScissorRect;

//================================================================//
// MOAIGraphicsPropBase
//================================================================//
/**	@lua	MOAIGraphicsPropBase
	@text	Base class for graphics props.
	
	@const	FRAME_FROM_DECK
	@const	FRAME_FROM_PARENT
	@const	FRAME_FROM_SELF

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
	public virtual MOAIAbstractProp,
	public virtual MOAIColor,
	public virtual MOAIAbstractDrawable,
	public virtual MOAIHasGfxScript {
protected:

	friend class MOAIGraphicsPropBaseCallable;

	u32						mBillboard;
	u32						mDisplayFlags;
	
	// TODO: should be attributes?
	MOAILuaSharedPtr < MOAIAbstractChildTransform >		mUVTransform;
	MOAILuaSharedPtr < MOAIScissorRect >				mScissorRect;

	//----------------------------------------------------------------//
	static int				_draw						( lua_State* L );
	static int				_getBillboard				( lua_State* L );
	static int				_getScissorRect				( lua_State* L );
	static int				_isVisible					( lua_State* L );
	static int				_setBillboard				( lua_State* L );
	static int				_setParent					( lua_State* L );
	static int				_setScissorRect				( lua_State* L );
	static int				_setUVTransform				( lua_State* L );
	static int				_setVisible					( lua_State* L );

	//----------------------------------------------------------------//
	void					LoadUVTransform				();
	void					LoadVertexTransform			();

	//----------------------------------------------------------------//
	void					_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
		
	void					MOAIDrawable_Draw							( int subPrimID );
	void					MOAIDrawable_DrawDebug						( int subPrimID );
	
	virtual void			MOAIGraphicsPropBase_Draw					( int subPrimID );
	virtual void			MOAIGraphicsPropBase_DrawDebug				( int subPrimID );
	virtual ZLMatrix4x4		MOAIGraphicsPropBase_GetWorldDrawingMtx		() const; // factors in billboard flags
	virtual bool			MOAIGraphicsPropBase_LoadGfxState			();
	
	bool					MOAINode_ApplyAttrOp						( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void					MOAINode_Update								();

public:

	DECL_LUA_ABSTRACT ( MOAIGraphicsPropBase )
	DECL_ATTR_HELPER ( MOAIGraphicsPropBase );

	enum {
		DEBUG_DRAW_PARTITION_CELLS,
		DEBUG_DRAW_PARTITION_PADDED_CELLS,
		DEBUG_DRAW_AXIS,
		DEBUG_DRAW_DIAGONALS,
		DEBUG_DRAW_MODEL_BOUNDS,
		DEBUG_DRAW_WORLD_BOUNDS,
		TOTAL_DEBUG_LINE_STYLES,
	};

	enum {
		BILLBOARD_NONE,
		BILLBOARD_NORMAL,
		BILLBOARD_ORTHO,
		BILLBOARD_COMPASS,
		BILLBOARD_COMPASS_SCALE,
		BILLBOARD_SCREEN,
	};

	enum {
		FLAGS_LOCAL_VISIBLE			= 0x01,
		FLAGS_VISIBLE				= 0x02, // this is a composite of FLAGS_LOCAL_VISIBLE plus the parent's ATTR_VISIBLE
	};

	static const u32 DEFAULT_FLAGS = FLAGS_LOCAL_VISIBLE | FLAGS_VISIBLE;

	enum {
		ATTR_SCISSOR_RECT,
		
		ATTR_LOCAL_VISIBLE,		// direct access to the prop's 'local' visbility setting
		ATTR_VISIBLE,			// read only - reflects the composite state of visibility
		INHERIT_VISIBLE,		// used to *pull* parent visibility via inheritance
		
		INHERIT_FRAME,
		FRAME_TRAIT,
		
		TOTAL_ATTR,
	};

	DECL_ATTR_ID ( ATTR_SCISSOR_RECT )
	
	DECL_ATTR_ID ( ATTR_LOCAL_VISIBLE )
	DECL_ATTR_ID ( ATTR_VISIBLE )
	DECL_ATTR_ID ( INHERIT_VISIBLE )
	
	DECL_ATTR_ID ( INHERIT_FRAME )
	DECL_ATTR_ID ( FRAME_TRAIT )

	//----------------------------------------------------------------//
	ZLMatrix4x4				GetWorldDrawingMtx			() const;
	bool					IsVisible					();
							MOAIGraphicsPropBase		();
	virtual					~MOAIGraphicsPropBase		();
	void					Render						();
	void					SetVisible					( bool visible );
};

#endif
