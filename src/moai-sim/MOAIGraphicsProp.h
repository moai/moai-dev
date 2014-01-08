// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSPROP_H
#define	MOAIGRAPHICSPROP_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIRenderable.h>
#include <moai-sim/MOAITransform.h>

class MOAICellCoord;
class MOAICollisionShape;
class MOAIDeck;
class MOAIDeckRemapper;
class MOAIGfxState;
class MOAIGrid;
class MOAILayoutFrame;
class MOAIOverlapPrim2D;
class MOAIPartition;
class MOAIPartitionCell;
class MOAIPartitionLevel;
class MOAIPartitionResultBuffer;
class MOAIScissorRect;
class MOAIShader;
class MOAISurfaceSampler2D;
class MOAITextureBase;

//================================================================//
// MOAIGraphicsProp
//================================================================//
/**	@name	MOAIGraphicsProp
	@text	Base class for props.
	
	@attr	ATTR_INDEX
	
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
class MOAIGraphicsProp :
	public MOAIProp,
	public MOAIColor,
	public MOAIRenderable {
private:
	
	//----------------------------------------------------------------//
	static int		_getBounds			( lua_State* L );
	static int		_getDims			( lua_State* L );
	static int		_getGrid			( lua_State* L );
	static int		_getIndex			( lua_State* L );
	static int		_getPriority		( lua_State* L );
	static int		_getWorldBounds		( lua_State* L );
	static int		_isVisible			( lua_State* L );
	static int		_inside				( lua_State* L );
	static int		_setBillboard		( lua_State* L );
	static int		_setBlendEquation	( lua_State* L );
	static int		_setBlendMode		( lua_State* L );
	static int		_setBounds			( lua_State* L );
	static int		_setCullMode		( lua_State* L );
	static int		_setDeck			( lua_State* L );
	static int		_setDepthMask		( lua_State* L );
	static int		_setDepthTest		( lua_State* L );
	static int		_setExpandForSort	( lua_State* L );
	static int		_setGrid			( lua_State* L );
	static int		_setGridScale		( lua_State* L );
	static int		_setIndex			( lua_State* L );
	static int		_setParent			( lua_State* L );
	static int		_setPriority		( lua_State* L );
	static int		_setRemapper		( lua_State* L );
	static int		_setScissorRect		( lua_State* L );
	static int		_setShader			( lua_State* L );
	static int		_setTexture			( lua_State* L );
	static int		_setUVTransform		( lua_State* L );
	static int		_setVisible			( lua_State* L );

	//----------------------------------------------------------------//
	void			DrawGrid			( int subPrimID );

protected:

	u32										mFlags;
	u32										mBillboard;
	
	// TODO: these should all be attributes
	MOAILuaSharedPtr < MOAIShader >			mShader;
	MOAILuaSharedPtr < MOAIGfxState >		mTexture;
	MOAILuaSharedPtr < MOAITransformBase >	mUVTransform;
	MOAILuaSharedPtr < MOAIScissorRect >	mScissorRect;
	
	int										mCullMode;
	int										mDepthTest;
	bool									mDepthMask;
	MOAIBlendMode							mBlendMode;

	ZLBox									mBoundsOverride;

	//----------------------------------------------------------------//
	u32				GetFrameFitting			( ZLBox& bounds, ZLVec3D& offset, ZLVec3D& scale );
	void			LoadGfxState			();
	void			LoadTransforms			();

public:

	DECL_LUA_FACTORY ( MOAIGraphicsProp )
	DECL_ATTR_HELPER ( MOAIGraphicsProp )

	static const s32 UNKNOWN_PRIORITY	= 0x80000000;
	static const int NO_SUBPRIM_ID		= 0xffffffff;

	enum {
		BILLBOARD_NONE,
		BILLBOARD_NORMAL,
		BILLBOARD_ORTHO,
	};

	enum {
		ATTR_INDEX,
		ATTR_PARTITION,
		ATTR_SHADER,
		ATTR_BLEND_MODE,
		
		ATTR_LOCAL_VISIBLE,		// direct access to the prop's 'local' visbility setting
		ATTR_VISIBLE,			// read only - reflects the composite state of visibility
		INHERIT_VISIBLE,		// used to *pull* parent visibility via inheritance
		
		INHERIT_FRAME,
		FRAME_TRAIT,
		
		TOTAL_ATTR,
	};

	enum {
		FLAGS_OVERRIDE_BOUNDS		= 0x01,
		FLAGS_EXPAND_FOR_SORT		= 0x02,
		FLAGS_LOCAL_VISIBLE			= 0x04,
		FLAGS_VISIBLE				= 0x08, // this is a composite of FLAGS_LOCAL_VISIBLE plus the parent's ATTR_VISIBLE
	};

	static const u32 DEFAULT_FLAGS	= FLAGS_LOCAL_VISIBLE | FLAGS_VISIBLE;

	GET_SET ( int, CullMode, mCullMode )
	GET_SET ( int, DepthTest, mDepthTest )
	GET_SET ( bool, DepthMask, mDepthMask )
	GET_SET ( const MOAIBlendMode&, BlendMode, mBlendMode )

	//----------------------------------------------------------------//
	bool				ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	void				Draw					( int subPrimID );
	void				DrawDebug				( int subPrimID );
	bool				IsVisible				();
						MOAIGraphicsProp		();
	virtual				~MOAIGraphicsProp		();
	void				OnDepNodeUpdate			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				Render					();
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void				SetVisible				( bool visible );
};

#endif
