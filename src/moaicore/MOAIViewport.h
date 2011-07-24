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
	public virtual USLuaObject,
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
	
	//----------------------------------------------------------------//
	float			GetAspect				() const;
	float			GetInvAspect			() const;
	void			GetNormToWndMtx			( USAffine2D& normToWnd ) const;
	void			GetProjMtx				( USAffine2D& proj ) const;
	USRect			GetRect					() const;
	USVec2D			GetScale				() const;
	USVec2D			GetUnits				() const;
	void			GetViewProjMtx			( const USAffine2D& camera, USAffine2D& viewProj ) const;
	void			GetViewProjMtxInv		( const USAffine2D& camera, USAffine2D& viewProjInv ) const;
	void			GetWndToNormMtx			( USAffine2D& wndToNorm ) const;
	void			GetWndToWorldMtx		( const USAffine2D& camera, USAffine2D& wndToWorld ) const;
	void			GetWorldToWndMtx		( const USAffine2D& camera, USAffine2D& worldToWnd ) const;
					MOAIViewport			();
					~MOAIViewport			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			SetOffset				( float xOffset, float yOffset );
	void			SetRotation				( float degrees );
	void			SetScale				( float xScale, float yScale );
	STLString		ToString				();
};

#endif
