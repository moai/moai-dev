// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTPICKABLE_H
#define	MOAIABSTRACTPICKABLE_H

//================================================================//
// MOAIPickResult
//================================================================//
class MOAIPickResult {
public:

	bool		mPicked;
	ZLReal		mTime;
	ZLVec3D 	mLoc;
	ZLVec3D		mNormal;
	
	//----------------------------------------------------------------//
			MOAIPickResult		();
			MOAIPickResult		( bool picked );
			MOAIPickResult		( bool picked, ZLReal time, ZLVec3D loc, ZLVec3D normal );
};

//================================================================//
// MOAIAbstractPickable
//================================================================//
class MOAIAbstractPickable :
	public virtual MOAILuaObject {
protected:
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
	static int					_pickByPoint				( lua_State* L );
	static int					_pickByRay					( lua_State* L );

	//----------------------------------------------------------------//
	void						_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );

	//----------------------------------------------------------------//
	virtual MOAIPickResult		MOAIAbstractPickable_PickByPoint	( ZLVec3D loc ) = 0;
	virtual MOAIPickResult		MOAIAbstractPickable_PickByRay		( ZLVec3D loc, ZLVec3D normal ) = 0;

public:

	//----------------------------------------------------------------//
								MOAIAbstractPickable		( ZLContext& context );
								~MOAIAbstractPickable		();
	MOAIPickResult				PickByPoint					( ZLVec3D loc );
	static MOAIPickResult		PickByPointHelper			( ZLBounds bounds, ZLVec3D loc );
	MOAIPickResult				PickByRay					( ZLVec3D loc, ZLVec3D normal );
	static MOAIPickResult		PickByRayHelper				( ZLBounds bounds, ZLVec3D loc, ZLVec3D normal );
};

#endif
