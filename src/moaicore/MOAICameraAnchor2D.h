// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICAMERAANCHOR2D_H
#define	MOAICAMERAANCHOR2D_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAINode.h>

class MOAITransformBase;

//================================================================//
// MOAIProp
//================================================================//
/**	@name	MOAICameraAnchor2D
	@text	Attaches fitting information to a transform. Used by
			MOAICameraFitter2D.
*/
class MOAICameraAnchor2D :
	public MOAINode {
private:

	USRect							mRect;
	USVec3D							mLoc;
	
	//----------------------------------------------------------------//
	static int		_setParent				( lua_State* L );
	static int		_setRect				( lua_State* L );

	//----------------------------------------------------------------//
	USRect			GetRect					();
	void			OnDepNodeUpdate			();

public:

	DECL_LUA_FACTORY ( MOAICameraAnchor2D )
	DECL_ATTR_HELPER ( MOAICameraAnchor2D )

	friend class MOAICameraFitter2D;

	enum {
		INHERIT_LOC,
		TOTAL_ATTR,
	};

	//----------------------------------------------------------------//
					MOAICameraAnchor2D		();
	virtual			~MOAICameraAnchor2D		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
