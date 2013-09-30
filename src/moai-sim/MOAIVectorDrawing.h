// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORDRAWING_H
#define	MOAIVECTORDRAWING_H

#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIVertexBuffer.h>

class MOAIVectorShape;
struct TESStesselator;

//================================================================//
// MOAIVectorDrawing
//================================================================//
/**	@name	MOAIVectorImage
	@text	Display object made of vector shapes.
*/
class MOAIVectorDrawing :
	public MOAILuaObject {
private:

	ZLLeanStack < MOAIVectorShape*, 64 >	mDirectory; // TODO: should use a chunked array or something
	ZLLeanStack < u32, 16 >					mStack; // TODO: ditto
	ZLLeanStack < u32, 16 >					mCommands; // TODO: ditto

	ZLMemStream			mIdxStream;
	ZLMemStream			mVtxStream;

	MOAIIndexBuffer		mIdxBuffer;
	MOAIVertexBuffer	mVtxBuffer;
	
	ZLColorVec			mFillColor;
	ZLColorVec			mLineColor;
	
	u32					mFillStyle;
	u32					mLineStyle;
	
	u32					mWindingRule;

	//----------------------------------------------------------------//
	static int		_addDrawCommand		( lua_State* L );
	static int		_pushCombo			( lua_State* L );
	static int		_pushPolygon		( lua_State* L );
	static int		_pushStroke			( lua_State* L );
	static int		_pushTag			( lua_State* L );
	static int		_setFillColor		( lua_State* L );
	static int		_setFillStyle		( lua_State* L );
	static int		_setLineColor		( lua_State* L );
	static int		_setLineStyle		( lua_State* L );
	static int		_setWindingRule		( lua_State* L );
	static int		_tessalate			( lua_State* L );

	//----------------------------------------------------------------//
	void			AddCommand				();
	u32				PushShape				( MOAIVectorShape* shape );
	void			Tessalate				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIVectorDrawing )
	
	//----------------------------------------------------------------//
	void			Clear					();
	u32				CountVertices			();
	void			Draw					();
					MOAIVectorDrawing		();
					~MOAIVectorDrawing		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			WriteContourIndices		( TESStesselator* tess, u32 base );
	void			WriteTriangleIndices	( TESStesselator* tess, u32 base );
	void			WriteVertices			( TESStesselator* tess, u32 color );
};

#endif
