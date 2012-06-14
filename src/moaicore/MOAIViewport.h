// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVIEWPORT_H
#define	MOAIVIEWPORT_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIViewport
//================================================================//
/**	@name	MOAIViewport
	@text	Viewport object.
*/
class MOAIViewport :
	public virtual MOAILuaObject,
	public USRect {
private:

	bool		mXScale;
	bool		mYScale;
	
	USVec2D		mScale;
	USVec2D		mOffset;
	float		mRotation;

	//----------------------------------------------------------------//
	static int		_setOffset		( lua_State* L );
	static int		_setRotation	( lua_State* L );
	static int		_setScale		( lua_State* L );
	static int		_setSize		( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIViewport )
	
	friend class MOAICamera;
	
	//----------------------------------------------------------------//
	float			GetAspect				() const;
	float			GetInvAspect			() const;
	void			GetNormToWndMtx			( USMatrix4x4& normToWnd ) const;
	USMatrix4x4		GetProjMtx				() const;
	USMatrix4x4		GetProjMtxInv			() const;
	USRect			GetRect					() const;
	USVec2D			GetScale				() const;
	USVec2D			GetUnits				() const;
	void			GetWndToNormMtx			( USMatrix4x4& wndToNorm ) const;
	USMatrix4x4		GetWndToWorldMtx		( const USMatrix4x4& view ) const;
	USMatrix4x4		GetWorldToWndMtx		( const USMatrix4x4& view ) const;
					MOAIViewport			();
					~MOAIViewport			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetOffset				( float xOffset, float yOffset );
	void			SetRotation				( float degrees );
	void			SetScale				( float xScale, float yScale );
};

#endif
