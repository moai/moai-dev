// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICAMERAANCHOR2D_H
#define	MOAICAMERAANCHOR2D_H

//================================================================//
// MOAICameraAnchor2D
//================================================================//
/**	@lua	MOAICameraAnchor2D
	@text	Attaches fitting information to a transform. Used by
			MOAICameraFitter2D.
*/
class MOAICameraAnchor2D :
	public virtual MOAINode {
private:

	ZLRect			mRect;
	ZLVec3D			mLoc;
	
	//----------------------------------------------------------------//
	static int		_setParent				( lua_State* L );
	static int		_setRect				( lua_State* L );

	//----------------------------------------------------------------//
	ZLRect			GetRect					();
	
	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAINode_Update			();

public:

	DECL_LUA_FACTORY ( MOAICameraAnchor2D )
	DECL_ATTR_HELPER ( MOAICameraAnchor2D )

	friend class MOAICameraFitter2D;

	enum {
		INHERIT_LOC,
		TOTAL_ATTR,
	};
	
	DECL_ATTR_ID ( INHERIT_LOC );

	//----------------------------------------------------------------//
					MOAICameraAnchor2D		();
	virtual			~MOAICameraAnchor2D		();
};

#endif
