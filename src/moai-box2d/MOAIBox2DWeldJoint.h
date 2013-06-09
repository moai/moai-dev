// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DWELDJOINT_H
#define	MOAIBOX2DWELDJOINT_H

#include <moai-box2d/MOAIBox2DJoint.h>

//================================================================//
// MOAIBox2DWeldJoint
//================================================================//
/**	@name MOAIBox2DWeldJoint
	@text Box2D weld joint.
*/
class MOAIBox2DWeldJoint :
	public MOAIBox2DJoint {
private:

	//----------------------------------------------------------------//

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DWeldJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DWeldJoint		();
					~MOAIBox2DWeldJoint		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
