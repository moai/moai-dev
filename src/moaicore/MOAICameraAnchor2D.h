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
	USRect							mRect;
	USVec2D							mLoc;
	
	//----------------------------------------------------------------//
	static int		_setParent				( lua_State* L );
	static int		_setRect				( lua_State* L );

	//----------------------------------------------------------------//
	USRect			GetRect					();
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
