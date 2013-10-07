// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORDRAWING_H
#define	MOAIVECTORDRAWING_H

#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIVectorStyle.h>
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
	ZLLeanStack < MOAIVectorShape*, 16 >	mShapeStack; // TODO: ditto
	ZLLeanStack < ZLVec2D, 32 >				mVertexStack;

	ZLMemStream			mIdxStream;
	ZLMemStream			mVtxStream;

	MOAIIndexBuffer		mIdxBuffer;
	MOAIVertexBuffer	mVtxBuffer;
	
	MOAIVectorStyle		mStyle;

	//----------------------------------------------------------------//
	static int		_finish					( lua_State* L );
	static int		_pushCombo				( lua_State* L );
	static int		_pushPolygon			( lua_State* L );
	static int		_pushStroke				( lua_State* L );
	static int		_pushVertex				( lua_State* L );
	static int		_setFillColor			( lua_State* L );
	static int		_setFillStyle			( lua_State* L );
	static int		_setLineColor			( lua_State* L );
	static int		_setLineOffset			( lua_State* L );
	static int		_setLineStyle			( lua_State* L );
	static int		_setLineWidth			( lua_State* L );
	static int		_setWindingRule			( lua_State* L );

	//----------------------------------------------------------------//
	u32				PushShape				( MOAIVectorShape* shape );
	void			Tessalate				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIVectorDrawing )
	
	GET_SET ( MOAIVectorStyle&, Style, mStyle );
	
	//----------------------------------------------------------------//
	void			Clear					();
	u32				CopyVertexStack			( ZLVec2D* vertices, u32 total );
	u32				CountVertices			();
	void			Draw					();
	void			Finish					();
					MOAIVectorDrawing		();
					~MOAIVectorDrawing		();
	void			PushCombo				();
	void			PushPolygon				( ZLVec2D* vertices, u32 total );
	void			PushVertex				( float x, float y );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			WriteContourIndices		( TESStesselator* tess, u32 base );
	void			WriteTriangleIndices	( TESStesselator* tess, u32 base );
	void			WriteVertices			( TESStesselator* tess, u32 color );
};

#endif
