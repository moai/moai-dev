// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVIEWPORT_H
#define	MOAIVIEWPORT_H

//================================================================//
// MOAIViewport
//================================================================//
/**	@name	MOAIViewport
	@text	Viewport object.
*/
class MOAIViewport :
	public virtual MOAILuaObject,
	public ZLRect {
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
	void			GetNormToWndMtx			( ZLMatrix4x4& normToWnd ) const;
	ZLMatrix4x4		GetProjMtx				() const;
	ZLMatrix4x4		GetProjMtxInv			() const;
	ZLRect			GetRect					() const;
	USVec2D			GetScale				() const;
	USVec2D			GetUnits				() const;
	void			GetWndToNormMtx			( ZLMatrix4x4& wndToNorm ) const;
	ZLMatrix4x4		GetWndToWorldMtx		( const ZLMatrix4x4& view ) const;
	ZLMatrix4x4		GetWorldToWndMtx		( const ZLMatrix4x4& view ) const;
					MOAIViewport			();
					~MOAIViewport			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetOffset				( float xOffset, float yOffset );
	void			SetRotation				( float degrees );
	void			SetScale				( float xScale, float yScale );
};

#endif
