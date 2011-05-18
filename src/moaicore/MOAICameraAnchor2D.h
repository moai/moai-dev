// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICAMERAANCHOR2D_H
#define	MOAICAMERAANCHOR2D_H

#include <moaicore/MOAINode.h>

class MOAITransformBase;

//================================================================//
// MOAIProp
//================================================================//
/**	@name	MOAICameraAnchor2D
	@text	Used by MOAICameraFitter
*/
class MOAICameraAnchor2D :
	public MOAINode {
private:

	USWeak < MOAITransformBase >	mParent;
	USRect							mScreenRect;
	USRect							mWorldRect;
	USVec2D							mWorldLoc;
	
	//----------------------------------------------------------------//
	static int		_setParent				( lua_State* L );
	static int		_setScreenRect			( lua_State* L );
	static int		_setWorldRect			( lua_State* L );

	//----------------------------------------------------------------//
	USRect			GetScreenRect			( const USAffine2D& worldToWnd );
	USRect			GetWorldRect			();
	void			OnDepNodeUpdate			();

public:

	DECL_LUA_FACTORY ( MOAICameraAnchor2D )

	friend class MOAICameraFitter2D;

	//----------------------------------------------------------------//
					MOAICameraAnchor2D		();
	virtual			~MOAICameraAnchor2D		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			SetParent				( MOAITransformBase* parent );
	STLString		ToString				();
};

#endif
